#include "stdafx.h"
#include "loHttpCurl.h"
#include "common/loFile.h"
#include "common/lo/lolock.h"
#include "curl/curl.h"
#include <shlwapi.h>
#include <atlstr.h>
#include <shlobj.h>

DEFINE_NAMESPACE(lohttp)

#define USE_CLS_CURL_INIT
#ifdef  USE_CLS_CURL_INIT

class CurlInit
{
public:
	CurlInit()
	{
		curl_global_init(CURL_GLOBAL_ALL);		
	}
	~CurlInit()
	{
		curl_global_cleanup();
	}
	static CurlInit& Instance(){
		static CurlInit inst;
		return inst;
	}
	void Init()
	{
	}
	void UnInit()
	{
	}
};

#define CURL_INIT()  CurlInit::Instance().Init()
#define CURL_UNINIT()  CurlInit::Instance().UnInit()

#else
#define CURL_INIT()  curl_global_init(CURL_GLOBAL_ALL)
#define CURL_UNINIT()  curl_global_cleanup()
#endif


void* BufferRealloc(void *ptr, size_t size)
{
	/* There might be a realloc() out there that doesn't like reallocing
	NULL pointers, so we take care of it here */
	if(ptr)
		return realloc(ptr, size);
	else
		return malloc(size);
}

static long iTaskId = 1;

/** д���� */
static
size_t WriteBuffer(void *ptr, size_t size, size_t nmemb, void *data)
{
	size_t realsize = size * nmemb;
	THttpMemoryBuffer *mem = (THttpMemoryBuffer *)data;

	mem->buffer = (char *)BufferRealloc(mem->buffer, mem->size + realsize + 1);
	if (mem->buffer)
	{
		memcpy(&(mem->buffer[mem->size]), ptr, realsize);
		mem->size += realsize;
		mem->buffer[mem->size] = 0;
	}
	return realsize;
}

static 
std::string GetUrlFileName(std::string url)
{
	std::string::size_type pos = url.find_last_of("/");
	if( pos == std::string::npos )
	{
		return std::string("");
	}
	pos ++;

	std::string ret(url.substr(pos, url.length() - pos));

	return ret;
}

/** �����Զ���ͷ */
static 
void BuildCustomHeader(struct curl_slist**chunk, char * pHeaderData)
{
	std::string headerData = pHeaderData;
	int pos = 0;
	while( headerData.length() > 0 )
	{
		// �ҵ�\r\n����
		int posStart = (int)headerData.find("\r\n", pos);
		if( posStart < 0 )
		{
			// û�У�ȫ���ӽ�ȥ
			*chunk = curl_slist_append(*chunk, headerData.c_str());
			// ���
			break;
		}

		// ����ǰ���Ԫ��
		std::string header = headerData.substr(pos, posStart - pos);
		*chunk = curl_slist_append(*chunk , header.c_str() );

		// ���Ʋ���
		posStart += 2;
		if( posStart >= (int)headerData.length() )
		{
			// �������
			break;
		}

		// ��������Ԫ��
		headerData = headerData.substr(posStart);
	}
	*chunk = curl_slist_append(*chunk, "Expect:");
}

// convert to enHTTPCode
static 
int CURLcode2HttpCode(CURLcode code)
{
	return code;
}

class CloHttpCurlAssoc
{
public:
	CloHttpCurlAssoc(IloTaskCallBack* pTaskCallback)
		:m_pHttpTask(pTaskCallback)
	{
		m_curlHandle = NULL;

		m_bUseProxy = FALSE;

		m_nType = -1;
		m_pTransferParam = NULL;

		m_nState = E_HTTPSTATE_STOP;
		m_nResult = E_HTTPTRANS_RESULT_UNDONE;
		m_bUserCancel = FALSE;

		// ����ID
		m_lTaskID = iTaskId++;

		CURL_INIT();
	}

	~CloHttpCurlAssoc()
	{
		CURL_UNINIT();
	}

	void SetCURLCode(CURLcode code)
	{
		m_eCURLCode = code;
		memset(m_szCURLErrString, 0, 1024);
		strcpy_s(m_szCURLErrString , 1024 , curl_easy_strerror(code) );
	}

private:
	/* �Զ�����������ID */
	long  m_lTaskID;

	/* ��ȡ����HTTPͷ���� */
	THttpMemoryBuffer m_HeaderRecv;

	/* ��ȡ����BODY���ݣ�DOWNLOAD��DOWNLOAD_TO_BUFFERʱ���ò�����Ч */
	THttpMemoryBuffer m_BodyRecv;

	/* �����ļ�ʹ�õĲ��������ݽṹ�� */
	THttpFileBuffer	m_FileBufferRecv;

	/* ʹ�ô��� */
	BOOL m_bUseProxy;
	THttpProxyInfo m_ProxyInfo;

	/* �������ͺ�������� */
	int	 m_nType;
	TTaskParam*	m_pTransferParam;

	/* ����״̬����������Ϣ */
	int	m_nState;
	int	m_nResult;

	/** error */
	EHttpTaskErrorCode m_ErrorCode;
	CURLcode   m_eCURLCode;
	char       m_szCURLErrString[1024];

	///
	long  m_lCurBytes;
	long  m_lTotalBytes;

	BOOL  m_bUserCancel;
	///
	IloTaskCallBack* m_pHttpTask;

	CURL* m_curlHandle;

	lo::CloSmartLock<CRITICAL_SECTION, lo::CloCriticalSection> m_cs;

	friend class CloHttpCurl;
};

//----------------------------------------------------------------------------------------//
CloHttpCurl::CloHttpCurl(IloTaskCallBack* pHttpTask)
:m_pAssoc( new CloHttpCurlAssoc(pHttpTask) )
{
}

CloHttpCurl::~CloHttpCurl(void)
{
	StopTransfer();
	ClearBuffer();
	ClearParam();

	delete m_pAssoc;
	m_pAssoc = NULL;
}

void CloHttpCurl::ClearBuffer()
{
	// modified by loach 2009-07-27
	SAFE_DELETE_MEMORYBUFFER(&m_pAssoc->m_HeaderRecv);
	memset(&m_pAssoc->m_HeaderRecv, 0, sizeof(THttpMemoryBuffer));

	SAFE_DELETE_MEMORYBUFFER(&m_pAssoc->m_BodyRecv);
	memset(&m_pAssoc->m_BodyRecv, 0, sizeof(THttpMemoryBuffer));
}

void CloHttpCurl::ClearParam()
{
	SAFE_DELETE (m_pAssoc->m_pTransferParam);
}

/*
return value:
0: succeed;
-1: transfer already started
-2: task invalid
-3: start failed
*/
int CloHttpCurl::StartTransfer()
{
	if( m_pAssoc->m_curlHandle )
	{
		return -1;
	}

	if( !IsTaskValid() )
	{
		return -2;
	}

	m_pAssoc->m_bUserCancel = FALSE;

	m_pAssoc->m_nState = E_HTTPSTATE_RUNNING;
	TransferProc();

	return 0;
}

void CloHttpCurl::StopTransfer()
{
	lo::CloAutoLock<CRITICAL_SECTION, lo::CloCriticalSection> al(&m_pAssoc->m_cs);

    if( E_HTTPSTATE_RUNNING == m_pAssoc->m_nState)
	{		
		// ֹͣ����	
		int count = 0;
		m_pAssoc->m_bUserCancel = TRUE;
		while( m_pAssoc->m_curlHandle )
		{
			Sleep(200);
			count ++;

			if( count >= 15 )
			{
				break;
			}
		}
		if( m_pAssoc->m_nResult == E_HTTPTRANS_RESULT_UNDONE )
		{
			m_pAssoc->m_nResult = E_HTTPTRANS_RESULT_CANCEL;
		}

		// ����״̬
		SetState( E_HTTPSTATE_STOP );
	}
	else if( m_pAssoc->m_nState == E_HTTPSTATE_STOP )
	{
		// ʲô������
	}
}

int CloHttpCurl::Request()
{
	return StartTransfer();
}

void CloHttpCurl::CancelRequest()
{
	return StopTransfer();
}

void CloHttpCurl::SetProxyInfo(THttpProxyInfo &ProxyInfo)
{
	memset(&m_pAssoc->m_ProxyInfo, 0, sizeof(ProxyInfo));
	memcpy(&m_pAssoc->m_ProxyInfo, &ProxyInfo, sizeof(ProxyInfo));
}

/*
return value:
0:	Succeed;
-1: Task invalid;
-2: Task already set;
*/
int CloHttpCurl::SetTransferParam(int nType, LPVOID lpParam)
{
	if( m_pAssoc->m_nType >= 0 )
	{
		return -2;
	}

	m_pAssoc->m_nType = nType;

	if( m_pAssoc->m_pTransferParam )
	{
		SAFE_DELETE (m_pAssoc->m_pTransferParam);
	}

	m_pAssoc->m_pTransferParam = new TTaskParam();
	TTaskParam * pParam = (TTaskParam *)lpParam;
	*(m_pAssoc->m_pTransferParam) = *pParam;	// ��ֵ

	if( !IsTaskValid() )
	{
		return -1;
	}

	return 0;
}

BOOL CloHttpCurl::IsTaskValid()
{
	if( !m_pAssoc->m_pTransferParam )
	{
		return FALSE;
	}

	if( strlen(m_pAssoc->m_pTransferParam->cURL) <= 0 )
	{
		return FALSE;
	}

	switch(m_pAssoc->m_nType)
	{
	case E_HTTPTYPE_POST_FILE:
		{	
#ifdef _UNICODE
			if( !locom::CloFile::IsFileExists( (CA2T)m_pAssoc->m_pTransferParam->cLocalFile) )
#else
			if( !locom::CloFile::IsFileExists( m_pAssoc->m_pTransferParam->cLocalFile) )
#endif
			{
				return FALSE;
			}
		}
		break;
	default:
		return TRUE;
	}

	return TRUE;
}

/*
return value:
0: succeed;
-1: failed;
-2: invalid task
*/
int CloHttpCurl::TransferProc()
{
#define CALL_PROC(id , fun ) case id: return fun()
	switch(m_pAssoc->m_nType)
	{
		CALL_PROC( E_HTTPTYPE_GET           , TransferProcGET );
		CALL_PROC( E_HTTPTYPE_DELETE        , TransferProcDELETE );
		CALL_PROC( E_HTTPTYPE_DOWNLOAD      , TransferProcDOWNLOAD );
		CALL_PROC( E_HTTPTYPE_POST          , TransferProcPOST );
		CALL_PROC( E_HTTPTYPE_POST_FILE     , TransferProcPOSTFile );
		CALL_PROC( E_HTTPTYPE_POST_TEXT_FILE, TransferProcPOSTFile2 );
		CALL_PROC( E_HTTPTYPE_UPLOAD        , TransferProcUPLOAD );
		CALL_PROC( E_HTTPTYPE_PUT           , TransferProcPUT );
	}
	return -2;
}


int CloHttpCurl::TransferProcGET()
{
	SetState( E_HTTPSTATE_RUNNING );

	int ret = 0;
	THttpMemoryBuffer headerBuffer;
	THttpMemoryBuffer bodyBuffer;

	// ����curl
	m_pAssoc->m_curlHandle = curl_easy_init();


	// ���÷�������
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_URL, m_pAssoc->m_pTransferParam->cURL);


	// ���ô�����Ϣ
	UpdateProxy(m_pAssoc->m_pTransferParam->cURL);

	// ���ø��ӵ�Header
	struct curl_slist *chunk = NULL;
	if( strlen(m_pAssoc->m_pTransferParam->cHead) > 0 )
	{
		BuildCustomHeader(&chunk, m_pAssoc->m_pTransferParam->cHead);
		curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_HTTPHEADER, chunk);
	}

	// ���ô���״̬�ص�
	SetTransferCallback((void *)&headerBuffer, (void *)&bodyBuffer);

	// ���÷���Agent
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_USERAGENT, "httpclient-vika/1.0");
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_FOLLOWLOCATION, 1);		// �Զ��ض���
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_AUTOREFERER, 1);


	// ������ȡ
	CURLcode code = curl_easy_perform(m_pAssoc->m_curlHandle);

	// �����ӵ�Header������Ϣ
	if( chunk != NULL )
		curl_slist_free_all(chunk);

	// �������
	if( code == CURLE_OK )
	{
		ClearBuffer();

		// ��������������

		// modified by loach 2009-07-27
		SAFE_NEW_MEMORYBUFFER(&m_pAssoc->m_HeaderRecv,headerBuffer.size,headerBuffer.size);
		COPY_MEMORYBUFFER(&headerBuffer,&m_pAssoc->m_HeaderRecv);

		SAFE_NEW_MEMORYBUFFER(&m_pAssoc->m_BodyRecv,bodyBuffer.size,bodyBuffer.size);
		COPY_MEMORYBUFFER(&bodyBuffer,&m_pAssoc->m_BodyRecv);

		// ����������
		m_pAssoc->m_nResult = E_HTTPTRANS_RESULT_COMPLETE;

		ret = 0;
	}
	else
	{
		m_pAssoc->m_nResult = E_HTTPTRANS_RESULT_FAILED;
		m_pAssoc->m_ErrorCode = E_HTTPTASK_ERR_CURL_EXCEPTION;
		ret = -1;
	}

	// �����е��ر���Ϊ��ͨ�� BufferRealloc���� �����
	// ������
	SAFE_FREE_BUFFER(headerBuffer.buffer);
	SAFE_FREE_BUFFER(bodyBuffer.buffer);

	m_pAssoc->SetCURLCode(code);
	TaskCode();

	// �ͷ�curl
	curl_easy_cleanup(m_pAssoc->m_curlHandle);
	m_pAssoc->m_curlHandle = NULL;

	// �������
	SetState( E_HTTPSTATE_STOP );

	return ret;
}

int CloHttpCurl::TransferProcDOWNLOAD()
{
	if( m_pAssoc->m_pTransferParam->bToBuffer )		// ���ص��ڴ�
	{
		return TransferProcDOWNLOAD2();
	}

	///////////////////////////////////////////////////////////////////
	// ���ص��ļ�
	SetState( E_HTTPSTATE_RUNNING );

	THttpMemoryBuffer headerBuffer;

	// �򿪱����ļ�
	THttpFileBuffer fileBuffer;
	memset(&fileBuffer, 0, sizeof(THttpFileBuffer));

	// �ر��ļ� 
#define FILE_CLOSE(p) if(p) { fclose(p);(p) = NULL;}

	// �����ļ����Ƿ���Ч
#ifdef _UNICODE
	if( !locom::CloFile::IsDirectoryExists( (CA2T)m_pAssoc->m_pTransferParam->cSavePath) )
	{
		locom::CloFile::CreateDirectory( (CA2T)m_pAssoc->m_pTransferParam->cSavePath );
	}
#else
	if (!locom::CloFile::IsDirectoryExists(m_pAssoc->m_pTransferParam->cSavePath) )
	{
		locom::CloFile::CreateDirectory(m_pAssoc->m_pTransferParam->cSavePath);
	}
#endif

	// ����curl
	//CURL_INIT();
	m_pAssoc->m_curlHandle = curl_easy_init();

	// ���÷�������
	CURLcode res = curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_URL, m_pAssoc->m_pTransferParam->cURL);


_recvHeader:

	if( GetState() == E_HTTPSTATE_STOP )   //genobili add on 2010 - 01 - 22
	{
		// �д�����
		m_pAssoc->m_nResult = E_HTTPTRANS_RESULT_FAILED;
		m_pAssoc->m_ErrorCode = E_HTTPTASK_ERR_CURL_EXCEPTION;
		FILE_CLOSE( fileBuffer.fp );
		// ������
		// �����е��ر���Ϊ��ͨ�� BufferRealloc���� �����
		SAFE_FREE_BUFFER( headerBuffer.buffer );

		m_pAssoc->SetCURLCode(res);
		TaskCode();

		// �ͷ�curl
		curl_easy_cleanup(m_pAssoc->m_curlHandle);
		m_pAssoc->m_curlHandle = NULL;

		// �������
		SetState( E_HTTPSTATE_STOP );
		return -1;
	}

	// ���ø��ӵ�Header
	struct curl_slist *chunk = NULL;
	if( strlen(m_pAssoc->m_pTransferParam->cHead) > 0 )
	{
		BuildCustomHeader(&chunk, m_pAssoc->m_pTransferParam->cHead);
		curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_HTTPHEADER, chunk);
	}

	// ���ô�����Ϣ
	UpdateProxy(m_pAssoc->m_pTransferParam->cURL);

	// ���÷���Agent
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_USERAGENT, "httpclient-vika/1.0");
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_AUTOREFERER, 1);

	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_NOBODY, 1);
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_HEADER, 1);

	// �����û�ȡHeader�ĺ���
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_HEADERFUNCTION, ToHeaderCallback);
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_HEADERDATA, (void *)&headerBuffer);

	curl_easy_perform(m_pAssoc->m_curlHandle);	

	// ��ȡhttp code
	long httpCode = 0;
	res = curl_easy_getinfo(m_pAssoc->m_curlHandle, CURLINFO_HTTP_CODE, &httpCode);

	// �ɹ�
	if( httpCode == 200  && CURLE_OK == res )
	{
		double contentLength = 0;
		curl_easy_getinfo(m_pAssoc->m_curlHandle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &contentLength);

		char * url = NULL;
		curl_easy_getinfo(m_pAssoc->m_curlHandle, CURLINFO_EFFECTIVE_URL, (void *)&url);

		// ���û��ָ���ļ���
		if( !m_pAssoc->m_pTransferParam->bRename || strlen(m_pAssoc->m_pTransferParam->cNewName) == 0 )
		{
			// ��ȡ��Ч���ļ���
			std::string filename = GetUrlFileName(url);
			if( filename.length() > 0 )
			{
				strcpy_s(m_pAssoc->m_pTransferParam->cNewName, 1024, filename.c_str());
			}
		}

		if( strlen(m_pAssoc->m_pTransferParam->cNewName) > 0 )
		{
			// ���½���ʽ�����������ļ�
			std::string filePath = m_pAssoc->m_pTransferParam->cSavePath;
			filePath += "\\";
			filePath += m_pAssoc->m_pTransferParam->cNewName;

			FILE * fp;
			errno_t err = fopen_s(&fp, filePath.c_str(), "w+b");
			if( err != 0 )
			{
				// ���������ļ�ʧ��
				m_pAssoc->m_nResult = E_HTTPTRANS_RESULT_FAILED;

				m_pAssoc->m_ErrorCode = E_HTTPTASK_ERR_FAIL_TO_OPEN_LOCAL_FILE;

				// ������
				// �����е��ر���Ϊ��ͨ�� BufferRealloc���� �����
				SAFE_FREE_BUFFER(headerBuffer.buffer);

				// �ͷ�curl
				curl_easy_cleanup(m_pAssoc->m_curlHandle);
				m_pAssoc->m_curlHandle = NULL;

				// �������
				SetState( E_HTTPSTATE_STOP );

				return -1;
			}

			// ׼��д�ļ���
			fileBuffer.fp = fp;
		}
		else 
		{
			// ����û�л�ȡ���ļ���
			m_pAssoc->m_nResult = E_HTTPTRANS_RESULT_FAILED;

			m_pAssoc->m_ErrorCode = E_HTTPTASK_ERR_CANNOT_GET_REMOTE_FILE_NAME;

			//
			FILE_CLOSE( fileBuffer.fp );

			// ������
			// �����е��ر���Ϊ��ͨ�� BufferRealloc���� �����
			SAFE_FREE_BUFFER( headerBuffer.buffer );

			// �ͷ�curl
			curl_easy_cleanup(m_pAssoc->m_curlHandle);
			m_pAssoc->m_curlHandle = NULL;

			// �������
			SetState( E_HTTPSTATE_STOP );

			return -1;
		}

		// �ȱ���һ��Header
		SAFE_NEW_MEMORYBUFFER(&m_pAssoc->m_HeaderRecv,headerBuffer.size,headerBuffer.size);
		COPY_MEMORYBUFFER(&headerBuffer,&m_pAssoc->m_HeaderRecv);

		goto _recvBody;
	}
	else if( httpCode == 302  && CURLE_OK == res )
	{
		char * pUrl = NULL;
		curl_easy_getinfo(m_pAssoc->m_curlHandle, CURLINFO_REDIRECT_URL, (void *)&pUrl);

		curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_URL, pUrl);

		goto _recvHeader;
	}
	else
	{
		// �д�����

		m_pAssoc->m_nResult = E_HTTPTRANS_RESULT_FAILED;

		m_pAssoc->m_ErrorCode = E_HTTPTASK_ERR_CURL_EXCEPTION;

		FILE_CLOSE( fileBuffer.fp );

		// ������
		// �����е��ر���Ϊ��ͨ�� BufferRealloc���� �����
		SAFE_FREE_BUFFER( headerBuffer.buffer );

		m_pAssoc->SetCURLCode(res);
		TaskCode();

		// �ͷ�curl
		curl_easy_cleanup(m_pAssoc->m_curlHandle);
		m_pAssoc->m_curlHandle = NULL;

		// �������
		SetState( E_HTTPSTATE_STOP );

		return -1;
	}

_recvBody:
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_NOBODY, 0);
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_HEADER, 0);

	// ���ö�ȡ���������ݵĺ���
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_WRITEFUNCTION, ToFileCallback);
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_WRITEDATA, (void *)&fileBuffer);

	// ����progress��д
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_NOPROGRESS, 0);
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_PROGRESSFUNCTION, ProgressCallback);
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_PROGRESSDATA, this);

	// ��ʼ��ȡ
	res = curl_easy_perform(m_pAssoc->m_curlHandle);	

	// �������
	int ret = 0;
	if( res == CURLE_OK )
	{
		// ����������
		m_pAssoc->m_nResult = E_HTTPTRANS_RESULT_COMPLETE;

		ret = 0;
	}
	else
	{
		m_pAssoc->m_nResult = E_HTTPTRANS_RESULT_FAILED;

		m_pAssoc->m_ErrorCode = E_HTTPTASK_ERR_CURL_EXCEPTION;

		ret = -1;
	}

	FILE_CLOSE( fileBuffer.fp ); 

	// ������
	// �����е��ر���Ϊ��ͨ�� BufferRealloc���� �����
	SAFE_FREE_BUFFER( headerBuffer.buffer );

	m_pAssoc->SetCURLCode( res );
	TaskCode();

	// �ͷ�curl
	curl_easy_cleanup(m_pAssoc->m_curlHandle);
	m_pAssoc->m_curlHandle = NULL;

	// �������
	SetState( E_HTTPSTATE_STOP );

	return ret;
}

int CloHttpCurl::TransferProcDOWNLOAD2()
{
	THttpMemoryBuffer headerBuffer;

	SetState( E_HTTPSTATE_RUNNING );

	// ����curl
	m_pAssoc->m_curlHandle = curl_easy_init();

	// ���÷�������
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_URL, m_pAssoc->m_pTransferParam->cURL);


	// ���ø��ӵ�Header
	struct curl_slist *chunk = NULL;
	if( strlen(m_pAssoc->m_pTransferParam->cHead) > 0 )
	{
		BuildCustomHeader(&chunk, m_pAssoc->m_pTransferParam->cHead);
		curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_HTTPHEADER, chunk);
	}

	// ���ô�����Ϣ
	UpdateProxy(m_pAssoc->m_pTransferParam->cURL);

	// ���÷���Agent
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_USERAGENT, "httpclient-vika/1.0");
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_AUTOREFERER, 1);

	// ���ö�ȡ���������ݵĺ���
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_WRITEFUNCTION, ToBufferCallback);
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_WRITEDATA, this);

	// �����û�ȡHeader�ĺ���
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_HEADERFUNCTION, ToHeaderCallback);
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_HEADERDATA, (void *)&headerBuffer);

	// ����progress��д
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_NOPROGRESS, 0);
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_PROGRESSFUNCTION, ProgressCallback);
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_PROGRESSDATA, this);

	// ��ʼ��ȡ
	CURLcode res = curl_easy_perform(m_pAssoc->m_curlHandle);	

	// �������
	int ret = 0;
	if( res == CURLE_OK )
	{
		// ��������������
		SAFE_NEW_MEMORYBUFFER(&m_pAssoc->m_HeaderRecv,headerBuffer.size,headerBuffer.size);
		COPY_MEMORYBUFFER(&headerBuffer,&m_pAssoc->m_HeaderRecv);

		// ����������
		m_pAssoc->m_nResult = E_HTTPTRANS_RESULT_COMPLETE;

		ret = 0;
	}
	else
	{
		m_pAssoc->m_nResult = E_HTTPTRANS_RESULT_FAILED;

		m_pAssoc->m_ErrorCode = E_HTTPTASK_ERR_CURL_EXCEPTION;

		ret = -1;
	}

	// ������
	// �����е��ر���Ϊ��ͨ�� BufferRealloc���� �����
	SAFE_FREE_BUFFER( headerBuffer.buffer );

	m_pAssoc->SetCURLCode( res );
	TaskCode();

	// �ͷ�curl
	curl_easy_cleanup(m_pAssoc->m_curlHandle);
	m_pAssoc->m_curlHandle = NULL;

	// �������
	SetState( E_HTTPSTATE_STOP );

	return ret;
}

int CloHttpCurl::TransferProcPOST()
{
	SetState( E_HTTPSTATE_RUNNING );

	THttpMemoryBuffer headerBuffer;
	THttpMemoryBuffer bodyBuffer;

	// ����curl
	m_pAssoc->m_curlHandle = curl_easy_init();

	// ���÷�������
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_URL, m_pAssoc->m_pTransferParam->cURL);

	// ���ø��ӵ�Header
	struct curl_slist *chunk = NULL;
	if( strlen(m_pAssoc->m_pTransferParam->cHead) > 0 )
	{
		BuildCustomHeader(&chunk, m_pAssoc->m_pTransferParam->cHead);
		curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_HTTPHEADER, chunk);
	}

	// ���ô�����Ϣ
	UpdateProxy(m_pAssoc->m_pTransferParam->cURL);

	// ����Post����
	if( m_pAssoc->m_pTransferParam->bFormFormat )
	{
		curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_POSTFIELDS, m_pAssoc->m_pTransferParam->pData);						// ����Ǳ�����Ҫ���峤��
	}
	else
	{	
		curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_POSTFIELDS, m_pAssoc->m_pTransferParam->pData);
		curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_POSTFIELDSIZE, m_pAssoc->m_pTransferParam->uDataLength);	
	}


	// ���ô���״̬�ص�
	SetTransferCallback((void *)&headerBuffer, (void *)&bodyBuffer);

	// ���÷���Agent
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_USERAGENT, "httpclient-vika/1.0");
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_FOLLOWLOCATION, 1);		// �Զ��ض���
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_AUTOREFERER, 1);

	// ������ȡ
	CURLcode code = curl_easy_perform(m_pAssoc->m_curlHandle);

	// �����ӵ�Header������Ϣ
	if( chunk != NULL )
		curl_slist_free_all(chunk);

	// �������
	int ret = 0;
	if( code == CURLE_OK )
	{
		ClearBuffer();

		// ��������������
		SAFE_NEW_MEMORYBUFFER(&m_pAssoc->m_HeaderRecv,headerBuffer.size,headerBuffer.size);
		COPY_MEMORYBUFFER(&headerBuffer,&m_pAssoc->m_HeaderRecv);

		SAFE_NEW_MEMORYBUFFER(&m_pAssoc->m_BodyRecv,bodyBuffer.size,bodyBuffer.size);
		COPY_MEMORYBUFFER(&bodyBuffer,&m_pAssoc->m_BodyRecv);

		// ����������
		m_pAssoc->m_nResult = E_HTTPTRANS_RESULT_COMPLETE;

		ret = 0;
	}
	else
	{
		m_pAssoc->m_nResult = E_HTTPTRANS_RESULT_FAILED;

		m_pAssoc->m_ErrorCode = E_HTTPTASK_ERR_CURL_EXCEPTION;

		ret = -1;
	}

	// ������
	// �����е��ر���Ϊ��ͨ�� BufferRealloc���� �����
	SAFE_FREE_BUFFER( headerBuffer.buffer );
	SAFE_FREE_BUFFER( bodyBuffer.buffer );

	m_pAssoc->SetCURLCode( code );
	TaskCode();

	// �ͷ�curl
	curl_easy_cleanup(m_pAssoc->m_curlHandle);
	m_pAssoc->m_curlHandle = NULL;

	// �������
	SetState( E_HTTPSTATE_STOP );

	return ret;
}

int CloHttpCurl::TransferProcPOSTFile()
{
	SetState( E_HTTPSTATE_RUNNING );
	THttpMemoryBuffer headerBuffer;
	THttpMemoryBuffer bodyBuffer;

	// ����curl
	m_pAssoc->m_curlHandle = curl_easy_init();

	// ���÷�������
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_URL, m_pAssoc->m_pTransferParam->cURL);
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_VERBOSE, 1L);



	// ���ø��ӵ�Header
	struct curl_slist *chunk = NULL;
	if( strlen(m_pAssoc->m_pTransferParam->cHead) > 0 )
	{
		BuildCustomHeader(&chunk, m_pAssoc->m_pTransferParam->cHead);
		curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_HTTPHEADER, chunk);
	}

	// ���POST�ļ���Ϣ
	struct curl_httppost *post=NULL;
	struct curl_httppost *last=NULL; 
#ifdef _UNICODE
	CString strLocalFile = (CA2T)m_pAssoc->m_pTransferParam->cLocalFile;
#else
	CString strLocalFile = m_pAssoc->m_pTransferParam->cLocalFile;
#endif

	if( locom::CloFile::IsFileExists(strLocalFile) )
	{
		curl_formadd(&post, &last,CURLFORM_COPYNAME, m_pAssoc->m_pTransferParam->pData,	CURLFORM_FILE, m_pAssoc->m_pTransferParam->cLocalFile, CURLFORM_END);

		/* Set the form info */  
		curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_HTTPPOST, post); 
	}

	// ���ô�����Ϣ
	UpdateProxy(m_pAssoc->m_pTransferParam->cURL);

	// ���ô���״̬�ص�
	SetTransferCallback((void *)&headerBuffer, (void *)&bodyBuffer);

	// ���÷���Agent
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_USERAGENT, "httpclient-vika/1.0");
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_FOLLOWLOCATION, 1);		// �Զ��ض���
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_AUTOREFERER, 1);

	// ������ȡ
	CURLcode code = curl_easy_perform(m_pAssoc->m_curlHandle);

	// �������
	int ret = 0;
	if( code == CURLE_OK )
	{
		ClearBuffer();

		// ��������������
		SAFE_NEW_MEMORYBUFFER(&m_pAssoc->m_HeaderRecv,headerBuffer.size,headerBuffer.size);
		COPY_MEMORYBUFFER(&headerBuffer,&m_pAssoc->m_HeaderRecv);

		SAFE_NEW_MEMORYBUFFER(&m_pAssoc->m_BodyRecv,bodyBuffer.size,bodyBuffer.size);
		COPY_MEMORYBUFFER(&bodyBuffer,&m_pAssoc->m_BodyRecv);

		// ����������
		m_pAssoc->m_nResult = E_HTTPTRANS_RESULT_COMPLETE;

		ret = 0;
	}
	else
	{
		m_pAssoc->m_nResult = E_HTTPTRANS_RESULT_FAILED;

		m_pAssoc->m_ErrorCode = E_HTTPTASK_ERR_CURL_EXCEPTION;

		ret = -1;
	}

	// ������
	// �����е��ر���Ϊ��ͨ�� BufferRealloc���� �����
	SAFE_FREE_BUFFER( headerBuffer.buffer );
	SAFE_FREE_BUFFER( bodyBuffer.buffer );


	m_pAssoc->SetCURLCode( code );
	TaskCode();

	// �������Ϣ
	if( post != NULL )
		curl_formfree(post);

	// ����Header������Ϣ
	if( chunk != NULL )
		curl_slist_free_all(chunk);

	// �ͷ�curl
	curl_easy_cleanup(m_pAssoc->m_curlHandle);
	m_pAssoc->m_curlHandle = NULL;

	// �������
	SetState( E_HTTPSTATE_STOP );

	return ret;
}

int CloHttpCurl::TransferProcPOSTFile2()
{
	SetState( E_HTTPSTATE_RUNNING );
	THttpMemoryBuffer headerBuffer;
	THttpMemoryBuffer bodyBuffer;

	// ����curl
	m_pAssoc->m_curlHandle = curl_easy_init();

	// ���÷�������
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_URL, m_pAssoc->m_pTransferParam->cURL);
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_VERBOSE, 1L);



	// ���ø��ӵ�Header
	struct curl_slist *chunk = NULL;
	if( strlen(m_pAssoc->m_pTransferParam->cHead) > 0 )
	{
		BuildCustomHeader(&chunk, m_pAssoc->m_pTransferParam->cHead);
		curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_HTTPHEADER, chunk);
	}

	// ���POST�ļ���Ϣ
	struct curl_httppost *post=NULL;
	struct curl_httppost *last=NULL; 
#ifdef _UNICODE
	CString strLocalFile = (CA2T)m_pAssoc->m_pTransferParam->cLocalFile;
#else
	CString strLocalFile = m_pAssoc->m_pTransferParam->cLocalFile;
#endif

	if( m_pAssoc->m_pTransferParam->cText )
	{
		curl_formadd(&post, &last, CURLFORM_COPYNAME ,"status" ,CURLFORM_COPYCONTENTS,m_pAssoc->m_pTransferParam->cText,CURLFORM_END);  
	}

	if( locom::CloFile::IsFileExists(strLocalFile) && m_pAssoc->m_pTransferParam->pData && m_pAssoc->m_pTransferParam->uDataLength > 0 )
	{
		curl_formadd(&post, &last, CURLFORM_COPYNAME , "pic", CURLFORM_BUFFER, m_pAssoc->m_pTransferParam->cLocalFile,CURLFORM_BUFFERPTR,m_pAssoc->m_pTransferParam->pData, CURLFORM_BUFFERLENGTH ,m_pAssoc->m_pTransferParam->uDataLength, CURLFORM_END);    


		/* Set the form info */  
		curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_HTTPPOST, post); 
	}

	// ���ô�����Ϣ
	UpdateProxy(m_pAssoc->m_pTransferParam->cURL);

	// ���ô���״̬�ص�
	SetTransferCallback((void *)&headerBuffer, (void *)&bodyBuffer);

	// ���÷���Agent
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_USERAGENT, "httpclient-vika/1.0");
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_FOLLOWLOCATION, 1);		// �Զ��ض���
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_AUTOREFERER, 1);

	// ������ȡ
	CURLcode code = curl_easy_perform(m_pAssoc->m_curlHandle);

	// �������
	int ret = 0;
	if( code == CURLE_OK )
	{
		ClearBuffer();

		// ��������������
		SAFE_NEW_MEMORYBUFFER(&m_pAssoc->m_HeaderRecv,headerBuffer.size,headerBuffer.size);
		COPY_MEMORYBUFFER(&headerBuffer,&m_pAssoc->m_HeaderRecv);

		SAFE_NEW_MEMORYBUFFER(&m_pAssoc->m_BodyRecv,bodyBuffer.size,bodyBuffer.size);
		COPY_MEMORYBUFFER(&bodyBuffer,&m_pAssoc->m_BodyRecv);

		// ����������
		m_pAssoc->m_nResult = E_HTTPTRANS_RESULT_COMPLETE;

		ret = 0;
	}
	else
	{
		m_pAssoc->m_nResult = E_HTTPTRANS_RESULT_FAILED;

		m_pAssoc->m_ErrorCode = E_HTTPTASK_ERR_CURL_EXCEPTION;

		ret = -1;
	}

	// ������
	// �����е��ر���Ϊ��ͨ�� BufferRealloc���� �����
	SAFE_FREE_BUFFER( headerBuffer.buffer );
	SAFE_FREE_BUFFER( bodyBuffer.buffer );


	m_pAssoc->SetCURLCode( code );
	TaskCode();

	// �������Ϣ
	if( post != NULL )
		curl_formfree(post);

	// ����Header������Ϣ
	if( chunk != NULL )
		curl_slist_free_all(chunk);

	// �ͷ�curl
	curl_easy_cleanup(m_pAssoc->m_curlHandle);
	m_pAssoc->m_curlHandle = NULL;

	// �������
	SetState( E_HTTPSTATE_STOP );

	return ret;
}

int CloHttpCurl::TransferProcUPLOAD()
{
	SetState( E_HTTPSTATE_RUNNING );
	THttpMemoryBuffer headerBuffer;
	THttpMemoryBuffer bodyBuffer;

	// ����curl
	m_pAssoc->m_curlHandle = curl_easy_init();

	// ���÷�������
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_URL, m_pAssoc->m_pTransferParam->cURL);



	// ���ø��ӵ�Header
	struct curl_slist *chunk = NULL;
	if( strlen(m_pAssoc->m_pTransferParam->cHead) > 0 )
	{
		BuildCustomHeader(&chunk, m_pAssoc->m_pTransferParam->cHead);
		curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_HTTPHEADER, chunk);
	}

	// ���ô�����Ϣ
	UpdateProxy(m_pAssoc->m_pTransferParam->cURL);

	// ����UPLOAD����
	FILE * fp = NULL;
	fopen_s(&fp, m_pAssoc->m_pTransferParam->cLocalFile, "rb");
	if( fp == NULL )
	{
		m_pAssoc->m_nResult = E_HTTPTRANS_RESULT_FAILED;

		m_pAssoc->m_ErrorCode = E_HTTPTASK_ERR_FAIL_TO_OPEN_LOCAL_FILE;

		return -1;
	}

	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_UPLOAD, 1);
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_PUT, 1);

	// ...

	// ���ô���״̬�ص�
	SetTransferCallback((void *)&headerBuffer, (void *)&bodyBuffer);

	// ���÷���Agent
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_USERAGENT, "httpclient-vika/1.0");
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_FOLLOWLOCATION, 1);		// �Զ��ض���
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_AUTOREFERER, 1);

	// ������ȡ
	CURLcode code = curl_easy_perform(m_pAssoc->m_curlHandle);

	// �����ӵ�Header������Ϣ
	if( chunk != NULL )
		curl_slist_free_all(chunk);

	// �������
	int ret = 0;
	if( code == CURLE_OK )
	{
		ClearBuffer();

		// ��������������
		SAFE_NEW_MEMORYBUFFER(&m_pAssoc->m_HeaderRecv,headerBuffer.size,headerBuffer.size);
		COPY_MEMORYBUFFER(&headerBuffer,&m_pAssoc->m_HeaderRecv);

		SAFE_NEW_MEMORYBUFFER(&m_pAssoc->m_BodyRecv,bodyBuffer.size,bodyBuffer.size);
		COPY_MEMORYBUFFER(&bodyBuffer,&m_pAssoc->m_BodyRecv);

		// ����������
		m_pAssoc->m_nResult = E_HTTPTRANS_RESULT_COMPLETE;

		ret = 0;
	}
	else
	{
		m_pAssoc->m_nResult = E_HTTPTRANS_RESULT_FAILED;

		m_pAssoc->m_ErrorCode = E_HTTPTASK_ERR_CURL_EXCEPTION;

		ret = -1;
	}

	// ������
	// �����е��ر���Ϊ��ͨ�� BufferRealloc���� �����
	SAFE_FREE_BUFFER( headerBuffer.buffer );
	SAFE_FREE_BUFFER( bodyBuffer.buffer );

	m_pAssoc->SetCURLCode( code );
	TaskCode();

	// �ͷ�curl
	curl_easy_cleanup(m_pAssoc->m_curlHandle);
	m_pAssoc->m_curlHandle = NULL;

	// �������
	SetState( E_HTTPSTATE_STOP );

	return ret;
}

int CloHttpCurl::TransferProcDELETE()
{
	SetState( E_HTTPSTATE_RUNNING );

	THttpMemoryBuffer headerBuffer;
	THttpMemoryBuffer bodyBuffer;

	// ����curl
	m_pAssoc->m_curlHandle = curl_easy_init();

	// ���÷�������
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_URL, m_pAssoc->m_pTransferParam->cURL);



	// ������ҪPOST����
	if( m_pAssoc->m_pTransferParam->pData != NULL && m_pAssoc->m_pTransferParam->uDataLength )
	{
		curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_POSTFIELDS, m_pAssoc->m_pTransferParam->pData);
		curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_POSTFIELDSIZE, m_pAssoc->m_pTransferParam->uDataLength);
	}

	// ���ø��Ӷ���
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_CUSTOMREQUEST, "DELETE");

	// ���ô�����Ϣ
	UpdateProxy(m_pAssoc->m_pTransferParam->cURL);

	// ���ø��ӵ�Header
	struct curl_slist *chunk = NULL;
	if( strlen(m_pAssoc->m_pTransferParam->cHead) > 0 )
	{
		BuildCustomHeader(&chunk, m_pAssoc->m_pTransferParam->cHead);
		curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_HTTPHEADER, chunk);
	}

	// ���ô���״̬�ص�
	SetTransferCallback(&headerBuffer, &bodyBuffer);

	// ���÷���Agent
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_USERAGENT, "httpclient-vika/1.0");
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_FOLLOWLOCATION, 1);		// �Զ��ض���
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_AUTOREFERER, 1);


	// ������ȡ
	CURLcode code = curl_easy_perform(m_pAssoc->m_curlHandle);

	// �����ӵ�Header������Ϣ
	if( chunk != NULL )
		curl_slist_free_all(chunk);

	// �������
	int ret = 0;
	if( code == CURLE_OK )
	{
		ClearBuffer();

		// ��������������
		SAFE_NEW_MEMORYBUFFER(&m_pAssoc->m_HeaderRecv,headerBuffer.size,headerBuffer.size);
		COPY_MEMORYBUFFER(&headerBuffer,&m_pAssoc->m_HeaderRecv);

		SAFE_NEW_MEMORYBUFFER(&m_pAssoc->m_BodyRecv,bodyBuffer.size,bodyBuffer.size);
		COPY_MEMORYBUFFER(&bodyBuffer,&m_pAssoc->m_BodyRecv);

		// ����������
		m_pAssoc->m_nResult = E_HTTPTRANS_RESULT_COMPLETE;

		ret = 0;
	}
	else
	{
		m_pAssoc->m_nResult = E_HTTPTRANS_RESULT_FAILED;

		m_pAssoc->m_ErrorCode = E_HTTPTASK_ERR_CURL_EXCEPTION;

		ret = -1;
	}

	// ������
	// �����е��ر���Ϊ��ͨ�� BufferRealloc���� �����
	SAFE_FREE_BUFFER( headerBuffer.buffer );
	SAFE_FREE_BUFFER( bodyBuffer.buffer );

	m_pAssoc->SetCURLCode( code );
	TaskCode();

	// �ͷ�curl
	curl_easy_cleanup(m_pAssoc->m_curlHandle);
	m_pAssoc->m_curlHandle = NULL;

	// �������
	SetState( E_HTTPSTATE_STOP );

	return ret;
}

int CloHttpCurl::TransferProcPUT()
{
	SetState( E_HTTPSTATE_RUNNING );

	THttpMemoryBuffer headerBuffer;
	THttpMemoryBuffer bodyBuffer;

	// ����curl
	m_pAssoc->m_curlHandle = curl_easy_init();

	// ���÷�������
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_URL, m_pAssoc->m_pTransferParam->cURL);


	// ���ô�����Ϣ
	UpdateProxy(m_pAssoc->m_pTransferParam->cURL);

	// ���ø��ӵ�Header
	struct curl_slist *chunk = NULL;
	if( strlen(m_pAssoc->m_pTransferParam->cHead) > 0 )
	{
		BuildCustomHeader(&chunk, m_pAssoc->m_pTransferParam->cHead);
		curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_HTTPHEADER, chunk);
	}

	// ����Post����
	if( m_pAssoc->m_pTransferParam->bFormFormat )
	{
		curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_POSTFIELDS, m_pAssoc->m_pTransferParam->pData);						// ����Ǳ�����Ҫ���峤��
	}
	else
	{	
		curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_POSTFIELDS, m_pAssoc->m_pTransferParam->pData);
		curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_POSTFIELDSIZE, m_pAssoc->m_pTransferParam->uDataLength);	
	}


	// ���ø��Ӷ���
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_CUSTOMREQUEST, "PUT");

	// ���÷���Agent
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_USERAGENT, "httpclient-vika/1.0");
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_FOLLOWLOCATION, 1);		// �Զ��ض���
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_AUTOREFERER, 1);

	// ���ô���״̬�ص�
	SetTransferCallback((void *)&headerBuffer, (void *)&bodyBuffer);

	// ������ȡ
	CURLcode code = curl_easy_perform(m_pAssoc->m_curlHandle);

	// �����ӵ�Header������Ϣ
	if( chunk != NULL )
		curl_slist_free_all(chunk);

	// �������
	int ret = 0;
	if( code == CURLE_OK )
	{
		ClearBuffer();

		// ��������������
		SAFE_NEW_MEMORYBUFFER(&m_pAssoc->m_HeaderRecv,headerBuffer.size,headerBuffer.size);
		COPY_MEMORYBUFFER(&headerBuffer,&m_pAssoc->m_HeaderRecv);

		SAFE_NEW_MEMORYBUFFER(&m_pAssoc->m_BodyRecv,bodyBuffer.size,bodyBuffer.size);
		COPY_MEMORYBUFFER(&bodyBuffer,&m_pAssoc->m_BodyRecv);


		// ����������
		m_pAssoc->m_nResult = E_HTTPTRANS_RESULT_COMPLETE;

		ret = 0;
	}
	else
	{
		m_pAssoc->m_nResult = E_HTTPTRANS_RESULT_FAILED;

		m_pAssoc->m_ErrorCode = E_HTTPTASK_ERR_CURL_EXCEPTION;

		ret = -1;
	}

	// ������
	// �����е��ر���Ϊ��ͨ�� BufferRealloc���� �����
	SAFE_FREE_BUFFER( headerBuffer.buffer );
	SAFE_FREE_BUFFER( bodyBuffer.buffer );


	m_pAssoc->SetCURLCode( code );
	TaskCode();

	// �ͷ�curl
	curl_easy_cleanup(m_pAssoc->m_curlHandle);
	m_pAssoc->m_curlHandle = NULL;

	// �������
	SetState( E_HTTPSTATE_STOP );

	return ret;
}

void CloHttpCurl::SetTransferCallback(LPVOID headerData, LPVOID bodyData)
{
	// �������ݻ�д
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_WRITEFUNCTION, ToBodyCallback);
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_WRITEDATA, bodyData);

	// ����httpͷ��д
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_HEADERFUNCTION, ToHeaderCallback);
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_HEADERDATA, headerData);

	// ����progress��д
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_NOPROGRESS, 0);
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_PROGRESSFUNCTION, ProgressCallback);
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_PROGRESSDATA, this);
}


void CloHttpCurl::SetState(int nNewState)
{
	m_pAssoc->m_nState = nNewState;

	TaskState(m_pAssoc->m_nState);
}


void CloHttpCurl::TaskState(int nState)
{
	if( m_pAssoc->m_pHttpTask )
	{
		m_pAssoc->m_pHttpTask->OnTaskState(nState);
	}
}

int CloHttpCurl::TaskProgress(long lCurBytes, long lTotalBytes)
{
	m_pAssoc->m_lCurBytes = lCurBytes;
	m_pAssoc->m_lTotalBytes = lTotalBytes;

	if( m_pAssoc->m_pHttpTask )
	{
		return m_pAssoc->m_pHttpTask->OnTaskProgress(lCurBytes, lTotalBytes);
	}
	return 0;
}

int CloHttpCurl::TaskBuffer(BYTE* pBuffer, long lLength)
{
	if( m_pAssoc->m_pHttpTask )
	{
		return m_pAssoc->m_pHttpTask->OnTaskBuffer( pBuffer, lLength);
	}
	return 0;
}

void CloHttpCurl::TaskCode(void)
{
	if( !m_pAssoc->m_bUserCancel && m_pAssoc->m_pHttpTask )
	{
		m_pAssoc->m_pHttpTask->OnTaskCode( CURLcode2HttpCode(m_pAssoc->m_eCURLCode) , m_pAssoc->m_szCURLErrString);
	}
}

int	CloHttpCurl::GetResponedHeader(THttpMemoryBuffer * pHeaderBuf)
{
	ATLASSERT(pHeaderBuf);

	if( m_pAssoc->m_nState != E_HTTPSTATE_STOP || 
		m_pAssoc->m_nResult != E_HTTPTRANS_RESULT_COMPLETE )
	{
		return -1;
	}
	// modified by loach 2009-07-27
	SAFE_NEW_MEMORYBUFFER(pHeaderBuf,m_pAssoc->m_HeaderRecv.size+1,m_pAssoc->m_HeaderRecv.size);
	COPY_MEMORYBUFFER(&m_pAssoc->m_HeaderRecv,pHeaderBuf);

	return 0;
}

int CloHttpCurl::GetResponedBody(THttpMemoryBuffer * pBodyBuf)
{
	ATLASSERT(pBodyBuf);

	if( m_pAssoc->m_nState != E_HTTPSTATE_STOP || 
		m_pAssoc->m_nResult != E_HTTPTRANS_RESULT_COMPLETE )
	{
		return -1;
	}
	// modified by loach 2009-07-27
	SAFE_NEW_MEMORYBUFFER(pBodyBuf,(m_pAssoc->m_BodyRecv.size+1),m_pAssoc->m_BodyRecv.size);
	COPY_MEMORYBUFFER(&m_pAssoc->m_BodyRecv,pBodyBuf);

	return 0;
}

void CloHttpCurl::UpdateProxy(LPCSTR lpszRequestURL)
{
	//// a code snippet ///
	CStringA strURL = CStringA(lpszRequestURL);
	strURL.MakeLower();

	if( !m_pAssoc->m_curlHandle )
		return ;
	
	if( strURL.Find("https://") == 0 )
	{
		curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_SSL_VERIFYHOST, 1L);
	}

	if( m_pAssoc->m_ProxyInfo.nProxyType != E_PROXYTYPE_NONE && strlen(m_pAssoc->m_ProxyInfo.cServer) > 0 )
	{
		char buf[1024];
		buf[0] = '\0';
		sprintf_s( buf, 1024, "%s:%d", m_pAssoc->m_ProxyInfo.cServer, m_pAssoc->m_ProxyInfo.wPort );

		curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_PROXY, buf);
		switch(m_pAssoc->m_ProxyInfo.nProxyType)
		{
		case E_PROXYTYPE_HTTP:
			curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_PROXYTYPE, CURLPROXY_HTTP);
			break;
		case E_PROXYTYPE_SOCKS4:
			curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS4);
			break;
		case E_PROXYTYPE_SOCKS5:
			curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS5);
			break;
		}

		if( strlen(m_pAssoc->m_ProxyInfo.cUser) > 0 )
		{
			curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_PROXYUSERNAME, m_pAssoc->m_ProxyInfo.cUser);
			curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_PROXYPASSWORD, m_pAssoc->m_ProxyInfo.cPassword);
		}
	}
}


size_t CloHttpCurl::ToBodyCallback(void *ptr, size_t size, size_t nmemb, void *data)
{
	size_t realsize = WriteBuffer(ptr, size, nmemb, data);

	return realsize;
}

size_t CloHttpCurl::ToHeaderCallback(void *ptr, size_t size, size_t nmemb, void *data)
{
	size_t realsize = WriteBuffer(ptr, size, nmemb, data);

	return realsize;
}

size_t CloHttpCurl::ToFileCallback(void *ptr, size_t size, size_t nmemb, void *data)
{
	size_t realsize = size * nmemb;
	if( realsize )
	{
		THttpFileBuffer * fileBuf = (THttpFileBuffer *)data;

		// ���������ݱ��浽�ļ���
		if( fileBuf->fp )
		{
			size_t write = fwrite(ptr, size, nmemb, fileBuf->fp);
		}
	}
	return realsize;
}

size_t CloHttpCurl::ToBufferCallback(void *ptr, size_t size, size_t nmemb, void *data)
{
	CloHttpCurl * pClient = (CloHttpCurl *)data;
	size_t realsize = size * nmemb;
	if( pClient )
	{
		if( 0 != pClient->TaskBuffer( (BYTE*)ptr, (long)realsize ) )
			return CURL_WRITEFUNC_PAUSE;
	}

	return realsize;
}

size_t CloHttpCurl::ProgressCallback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow)
{
	CloHttpCurl * pClient = (CloHttpCurl *)clientp;
	if( pClient )
	{
		// �Ƿ��û�ȡ��
		if( pClient->IsUserCancel() )
		{
			return 1;
		}

		int nType = pClient->GetType();
		switch(nType)
		{
		case E_HTTPTYPE_GET:
		case E_HTTPTYPE_DOWNLOAD:
		case E_HTTPTYPE_DELETE:
			return pClient->TaskProgress((long)dlnow, (long)dltotal);
		case E_HTTPTYPE_POST:
		case E_HTTPTYPE_POST_FILE:
		case E_HTTPTYPE_UPLOAD:
		case E_HTTPTYPE_PUT:
			return pClient->TaskProgress((long)ulnow, (long)ultotal);
		}
	}
	return 0;
}

long CloHttpCurl::GetTaskID() const {return m_pAssoc->m_lTaskID;}

int CloHttpCurl::GetType() const {return m_pAssoc->m_nType;}

int CloHttpCurl::GetState() const {return m_pAssoc->m_nState;}

int CloHttpCurl::GetResult() const {return m_pAssoc->m_nResult;}

BOOL CloHttpCurl::IsUserCancel() const { return m_pAssoc->m_bUserCancel;}

END_NAMESPACE(lohttp)