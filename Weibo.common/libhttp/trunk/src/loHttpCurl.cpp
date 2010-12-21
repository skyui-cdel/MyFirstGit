#include "stdafx.h"
#include <assert.h>
#include "loHttpCurl.h"
#include "loFile.h"
#include "curl/curl.h"
#include "lostrconv.h"
#include "loHttpThread.h"
#include "common/lo/lolock.h"
#include <string.h>


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

		m_lhttpCode = 0;
		m_errCode   = 0;
		m_nState      = E_HTTPSTATUS_CLOSED;
		m_bUserCancel = FALSE;

		strcpy(m_useAgent,"lohttp-loach/1.0");

		// ����ID
		m_lTaskID = iTaskId++;

		CURL_INIT();
	}

	~CloHttpCurlAssoc()
	{
		CURL_UNINIT();
	}

	void SetErrCode(int code)
	{
		m_errCode = code;
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
	int	 m_nState;

	/** error */
	int  m_errCode;

	// 200 . 503 , 302...
	long m_lhttpCode;
	//
	char  m_useAgent[256];
	///
	long  m_lCurBytes;
	long  m_lTotalBytes;

	BOOL  m_bUserCancel;
	///
	IloTaskCallBack* m_pHttpTask;

	CURL* m_curlHandle;

	locom::CloSmartLock<THREAD_CRITICAL_SECTION, locom::CloCriticalSection> m_cs;

	friend class CloHttpCurl;
};


static 
bool lohttp_formadd_callback(void** httpost , void** last , HTTPFormType type , void* value )
{
	struct curl_httppost **post_ = (struct curl_httppost **)httpost;
	struct curl_httppost **last_ = (struct curl_httppost **)last; 

	int formid = -1;
	switch(type)
	{
	case HTTP_FORMTYPE_COPYNAME:
		formid = CURLFORM_COPYNAME;
		break;
	case HTTP_FORMTYPE_PTRNAME:
		formid = CURLFORM_PTRNAME;
		break;
	case HTTP_FORMTYPE_NAMELENGTH:
		formid = CURLFORM_NAMELENGTH;
		break;
	case HTTP_FORMTYPE_COPYCONTENTS:
		formid = CURLFORM_COPYCONTENTS;
		break;
	case HTTP_FORMTYPE_PTRCONTENTS:
		formid = CURLFORM_PTRCONTENTS;
		break;
	case HTTP_FORMTYPE_CONTENTSLENGTH:
		formid = CURLFORM_CONTENTSLENGTH;
		break;
	case HTTP_FORMTYPE_FILECONTENT:
		formid = CURLFORM_FILECONTENT;
		break;
	case HTTP_FORMTYPE_ARRAY:
		formid = CURLFORM_ARRAY;
		break;
	case HTTP_FORMTYPE_OBSOLETE:
		formid = CURLFORM_OBSOLETE;
		break;
	case HTTP_FORMTYPE_FILE:
		formid = CURLFORM_FILE;
		break;
	case HTTP_FORMTYPE_BUFFER:
		formid = CURLFORM_BUFFER;
		break;
	case HTTP_FORMTYPE_BUFFERPTR:
		formid = CURLFORM_BUFFERPTR;
		break;
	case HTTP_FORMTYPE_BUFFERLENGTH:
		formid = CURLFORM_BUFFERLENGTH;
		break;
	case HTTP_FORMTYPE_CONTENTTYPE:
		formid = CURLFORM_CONTENTTYPE;
		break;
	case HTTP_FORMTYPE_CONTENTHEADER:
		formid = CURLFORM_CONTENTHEADER;
		break;
	case HTTP_FORMTYPE_FILENAME:
		formid = CURLFORM_FILENAME;
		break;
	case HTTP_FORMTYPE_OBSOLETE2:
		formid = CURLFORM_OBSOLETE2;
		break;
	case HTTP_FORMTYPE_STREAM:
		formid = CURLFORM_STREAM;
		break;
	}

	if( formid <= CURLFORM_NOTHING || formid >= CURLFORM_LASTENTRY || formid == CURLFORM_END )
		return false;

	return ( CURL_FORMADD_OK == curl_formadd(post_, last_ , formid , value , CURLFORM_END) )?true:false;
}
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
	if( m_pAssoc->m_pTransferParam )
	{
		delete m_pAssoc->m_pTransferParam;
		m_pAssoc->m_pTransferParam  = 0;
	}
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

	return TransferProc();
}

void CloHttpCurl::StopTransfer()
{
	locom::CloAutoLock<THREAD_CRITICAL_SECTION, locom::CloCriticalSection> al(&m_pAssoc->m_cs);
    	if( E_HTTPSTATUS_START == m_pAssoc->m_nState )
	{// ֹͣ����		
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
		// ȡ��
		SetState( E_HTTPSTATUS_CANCEL );
	}
	else
	{//
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
	m_pAssoc->m_nType = nType;

	ClearParam();

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
	if( !m_pAssoc->m_pTransferParam || m_pAssoc->m_pTransferParam->cURL[0] == '\0'  )
	{
		return FALSE;
	}

//	switch(m_pAssoc->m_nType)
//	{
//	case E_HTTPTYPE_POST_FORMDATA:
//		{	
//#ifdef _UNICODE
//			if( !locom::CloFile::IsFileExists( CA2T(m_pAssoc->m_pTransferParam->cLocalFile) ) )
//#else
//			if( !locom::CloFile::IsFileExists( m_pAssoc->m_pTransferParam->cLocalFile) )
//#endif
//			{
//				return FALSE;
//			}
//		}
//		break;
//	default:
//		return TRUE;
//	}

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
	m_pAssoc->m_bUserCancel = FALSE;
	m_pAssoc->m_errCode = 0;

#define CALL_PROC(id , fun ) case id: return fun()
	switch(m_pAssoc->m_nType)
	{
		CALL_PROC( E_HTTPTYPE_GET           , TransferProcGET );
		CALL_PROC( E_HTTPTYPE_DOWNLOAD      , TransferProcDOWNLOAD );
		CALL_PROC( E_HTTPTYPE_POST          , TransferProcPOST );
		CALL_PROC( E_HTTPTYPE_POST_FORMDATA     , TransferProcPOSTFormData );
		//CALL_PROC( E_HTTPTYPE_POST_TEXT_FILE, TransferProcPOSTFormData2 );
		//CALL_PROC( E_HTTPTYPE_UPLOAD        , TransferProcUPLOAD );
		CALL_PROC( E_HTTPTYPE_PUT           , TransferProcPUT );
		CALL_PROC( E_HTTPTYPE_DELETE        , TransferProcDELETE );
	}
	return -2;
}

// 
#define GET_HTTP_CODE() \
	    m_pAssoc->m_lhttpCode = 0;\
		curl_easy_getinfo(m_pAssoc->m_curlHandle, CURLINFO_HTTP_CODE, &m_pAssoc->m_lhttpCode)

int CloHttpCurl::TransferProcGET()
{
	THttpMemoryBuffer headerBuffer;
	THttpMemoryBuffer bodyBuffer;

	SetState( E_HTTPSTATUS_START );

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
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_USERAGENT, GetUserAgent() );
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_FOLLOWLOCATION, 1);		// �Զ��ض���
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_AUTOREFERER, 1);


	// ������ȡ
	CURLcode code = curl_easy_perform(m_pAssoc->m_curlHandle);

	GET_HTTP_CODE();

	ClearBuffer();

	if( code == CURLE_OK )
	{
		// ��������������

		// modified by loach 2009-07-27
		SAFE_NEW_MEMORYBUFFER(&m_pAssoc->m_HeaderRecv,headerBuffer.size,headerBuffer.size);
		COPY_MEMORYBUFFER(&headerBuffer,&m_pAssoc->m_HeaderRecv);

		SAFE_NEW_MEMORYBUFFER(&m_pAssoc->m_BodyRecv,bodyBuffer.size,bodyBuffer.size);
		COPY_MEMORYBUFFER(&bodyBuffer,&m_pAssoc->m_BodyRecv);
	}

	// �����е��ر���Ϊ��ͨ�� BufferRealloc���� �����
	// ������
	SAFE_FREE_BUFFER(headerBuffer.buffer);
	SAFE_FREE_BUFFER(bodyBuffer.buffer);
	// �����ӵ�Header������Ϣ
	if( chunk != NULL )
		curl_slist_free_all(chunk);
	// �ͷ�curl
	curl_easy_cleanup(m_pAssoc->m_curlHandle);
	m_pAssoc->m_curlHandle = NULL;

	m_pAssoc->SetErrCode(code);
	TaskCode();

	// �������
	SetState( E_HTTPSTATUS_STOP );

	return (code != 0 )?-1:0;
}

int CloHttpCurl::TransferProcDOWNLOAD()
{
	if( m_pAssoc->m_pTransferParam->bToBuffer )		// ���ص��ڴ�
	{
		return TransferProcDOWNLOAD_TOBUFFER();
	}

	///////////////////////////////////////////////////////////////////
	// ���ص��ļ�
	SetState( E_HTTPSTATUS_START );

	THttpMemoryBuffer headerBuffer;

	// �򿪱����ļ�
	THttpFileBuffer fileBuffer;
	memset(&fileBuffer, 0, sizeof(THttpFileBuffer));

	// �ر��ļ� 
#define FILE_CLOSE(p) if(p) { fclose(p);(p) = NULL;}

	// �����ļ����Ƿ���Ч
#ifdef _UNICODE
	if( !locom::CloFile::IsDirectoryExists( CA2T(m_pAssoc->m_pTransferParam->cSavePath) ) )
		locom::CloFile::CreateDirectory( CA2T(m_pAssoc->m_pTransferParam->cSavePath) );

#else
	if (!locom::CloFile::IsDirectoryExists(m_pAssoc->m_pTransferParam->cSavePath) )
		locom::CloFile::CreateDirectory(m_pAssoc->m_pTransferParam->cSavePath);

#endif

	// ����curl
	m_pAssoc->m_curlHandle = curl_easy_init();

	// ���÷�������
	CURLcode code = curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_URL, m_pAssoc->m_pTransferParam->cURL);

	// ���ø��ӵ�Header
	struct curl_slist *chunk = NULL;
_recvHeader:

	int state = GetState();
	if( state != E_HTTPSTATUS_START ) // ��ֹ ��loop , 
	{
		// �д�����

		FILE_CLOSE( fileBuffer.fp );

		// ������
		// �����е��ر���Ϊ��ͨ�� BufferRealloc���� �����
		SAFE_FREE_BUFFER( headerBuffer.buffer );
		// �����ӵ�Header������Ϣ
		if( chunk != NULL ){
			curl_slist_free_all(chunk);
			chunk = 0;
		}
		// �ͷ�curl
		curl_easy_cleanup(m_pAssoc->m_curlHandle);
		m_pAssoc->m_curlHandle = NULL;

		m_pAssoc->SetErrCode(code);
		TaskCode();

		// �������
		SetState( E_HTTPSTATUS_STOP );
		return -1;
	}

	if( strlen(m_pAssoc->m_pTransferParam->cHead) > 0 )
	{
		BuildCustomHeader(&chunk, m_pAssoc->m_pTransferParam->cHead);
		curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_HTTPHEADER, chunk);
	}

	// ���ô�����Ϣ
	UpdateProxy(m_pAssoc->m_pTransferParam->cURL);

	// ���÷���Agent
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_USERAGENT, GetUserAgent() );
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_AUTOREFERER, 1);

	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_NOBODY, 1);
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_HEADER, 1);

	// �����û�ȡHeader�ĺ���
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_HEADERFUNCTION, ToHeaderCallback);
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_HEADERDATA, (void *)&headerBuffer);

	curl_easy_perform(m_pAssoc->m_curlHandle);	

	// ��ȡhttp code
	m_pAssoc->m_lhttpCode = 0;
	code = curl_easy_getinfo(m_pAssoc->m_curlHandle, CURLINFO_HTTP_CODE, &m_pAssoc->m_lhttpCode);

	// �ɹ�
	if( m_pAssoc->m_lhttpCode == 200  && CURLE_OK == code )
	{
		double contentLength = 0;
		char * url = NULL;

		curl_easy_getinfo(m_pAssoc->m_curlHandle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &contentLength);

		curl_easy_getinfo(m_pAssoc->m_curlHandle, CURLINFO_EFFECTIVE_URL, (void *)&url);

		// ���û��ָ���ļ���
		if( !m_pAssoc->m_pTransferParam->bRename || strlen(m_pAssoc->m_pTransferParam->cNewFileName) == 0 )
		{
			// ��ȡ��Ч���ļ���
			std::string filename = GetUrlFileName(url);
			if( filename.length() > 0 )
			{
				strcpy_s(m_pAssoc->m_pTransferParam->cNewFileName, 1024, filename.c_str());
			}
		}

		if( strlen(m_pAssoc->m_pTransferParam->cNewFileName) > 0 )
		{
			// ���½���ʽ�����������ļ�
			char cNewFile[ConstLocalFileLength];
			strcpy(cNewFile , m_pAssoc->m_pTransferParam->cSavePath );
			strcat( cNewFile  , "\\");
			strcat( cNewFile  , m_pAssoc->m_pTransferParam->cNewFileName );

			FILE * fp = fopen(cNewFile , "w+b");
			if( fp == 0 )
			{
				// ������
				// �����е��ر���Ϊ��ͨ�� BufferRealloc���� �����
				SAFE_FREE_BUFFER(headerBuffer.buffer);
				// �����ӵ�Header������Ϣ
				if( chunk != NULL ){
					curl_slist_free_all(chunk);
					chunk = 0;
				}
				// �ͷ�curl
				curl_easy_cleanup(m_pAssoc->m_curlHandle);
				m_pAssoc->m_curlHandle = NULL;

				// ���������ļ�ʧ��
				m_pAssoc->SetErrCode( HTTPE_CREATE_FILE );
				TaskCode();

				// �������
				SetState( E_HTTPSTATUS_STOP );

				return -1;
			}

			// ׼��д�ļ���
			fileBuffer.fp = fp;
		}
		else 
		{
			//
			FILE_CLOSE( fileBuffer.fp );

			// ������
			// �����е��ر���Ϊ��ͨ�� BufferRealloc���� �����
			SAFE_FREE_BUFFER( headerBuffer.buffer );
			// �����ӵ�Header������Ϣ
			if( chunk != NULL ){
				curl_slist_free_all(chunk);
				chunk = 0;
			}
			// �ͷ�curl
			curl_easy_cleanup(m_pAssoc->m_curlHandle);
			m_pAssoc->m_curlHandle = NULL;

			m_pAssoc->SetErrCode( HTTPE_FILE_NAME_NULL );
			TaskCode();

			// �������
			SetState( E_HTTPSTATUS_STOP );

			return -1;
		}

		// �ȱ���һ��Header
		SAFE_NEW_MEMORYBUFFER(&m_pAssoc->m_HeaderRecv,headerBuffer.size,headerBuffer.size);
		COPY_MEMORYBUFFER(&headerBuffer,&m_pAssoc->m_HeaderRecv);

		goto _recvBody;
	}
	else if( m_pAssoc->m_lhttpCode == 302  && CURLE_OK == code )
	{// �ض���

		char * pUrl = NULL;
		curl_easy_getinfo(m_pAssoc->m_curlHandle, CURLINFO_REDIRECT_URL, (void *)&pUrl);

		curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_URL, pUrl);
		// �����ӵ�Header������Ϣ
		if( chunk != NULL )
		{
			curl_slist_free_all(chunk);
			chunk = 0;
		}
		goto _recvHeader;
	}
	else
	{// �д�����

		FILE_CLOSE( fileBuffer.fp );

		// ������
		// �����е��ر���Ϊ��ͨ�� BufferRealloc���� �����
		SAFE_FREE_BUFFER( headerBuffer.buffer );
		// �����ӵ�Header������Ϣ
		if( chunk != NULL )
			curl_slist_free_all(chunk);
		// �ͷ�curl
		curl_easy_cleanup(m_pAssoc->m_curlHandle);
		m_pAssoc->m_curlHandle = NULL;

		m_pAssoc->SetErrCode(code);
		TaskCode();

		// �������
		SetState( E_HTTPSTATUS_STOP );

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
	code = curl_easy_perform(m_pAssoc->m_curlHandle);	


	FILE_CLOSE( fileBuffer.fp ); 

	// ������
	// �����е��ر���Ϊ��ͨ�� BufferRealloc���� �����
	SAFE_FREE_BUFFER( headerBuffer.buffer );
	// �����ӵ�Header������Ϣ
	if( chunk != NULL )
		curl_slist_free_all(chunk);
	// �ͷ�curl
	curl_easy_cleanup(m_pAssoc->m_curlHandle);
	m_pAssoc->m_curlHandle = NULL;

	m_pAssoc->SetErrCode( code );
	TaskCode();

	// �������
	SetState( E_HTTPSTATUS_STOP );

	return (code != CURLE_OK)?-1:0;
}

int CloHttpCurl::TransferProcDOWNLOAD_TOBUFFER()
{
	THttpMemoryBuffer headerBuffer;

	SetState( E_HTTPSTATUS_START );

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
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_USERAGENT, GetUserAgent() );
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
	CURLcode code = curl_easy_perform(m_pAssoc->m_curlHandle);	

	GET_HTTP_CODE();

	ClearBuffer();
	// �������
	if( code == CURLE_OK )
	{
		// ��������������
		SAFE_NEW_MEMORYBUFFER(&m_pAssoc->m_HeaderRecv,headerBuffer.size,headerBuffer.size);
		COPY_MEMORYBUFFER(&headerBuffer,&m_pAssoc->m_HeaderRecv);
	}

	// ������
	// �����е��ر���Ϊ��ͨ�� BufferRealloc���� �����
	SAFE_FREE_BUFFER( headerBuffer.buffer );

	// �����ӵ�Header������Ϣ
	if( chunk != NULL ){
		curl_slist_free_all(chunk);
		chunk = 0;
	}
	// �ͷ�curl
	curl_easy_cleanup(m_pAssoc->m_curlHandle);
	m_pAssoc->m_curlHandle = NULL;

	m_pAssoc->SetErrCode( code );
	TaskCode();

	// �������
	SetState( E_HTTPSTATUS_STOP );

	return (code != CURLE_OK)?-1:0;
}

int CloHttpCurl::TransferProcPOST()
{
	THttpMemoryBuffer headerBuffer;
	THttpMemoryBuffer bodyBuffer;

    SetState( E_HTTPSTATUS_START );

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
	if( m_pAssoc->m_pTransferParam->pData )
	{
		if( m_pAssoc->m_pTransferParam->bFormData )
		{// ����Ǳ�����Ҫ���峤��
			curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_POSTFIELDS, m_pAssoc->m_pTransferParam->pData);		
		}
		else
		{	
			curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_POSTFIELDS, m_pAssoc->m_pTransferParam->pData);
			curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_POSTFIELDSIZE, m_pAssoc->m_pTransferParam->uDataLength);	
		}
	}

	// ���ô���״̬�ص�
	SetTransferCallback((void *)&headerBuffer, (void *)&bodyBuffer);

	// ���÷���Agent
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_USERAGENT,GetUserAgent() );
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_FOLLOWLOCATION, 1);		// �Զ��ض���
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_AUTOREFERER, 1);

	// ������ȡ
	CURLcode code = curl_easy_perform(m_pAssoc->m_curlHandle);

	GET_HTTP_CODE();

	ClearBuffer();
	// �������
	if( code == CURLE_OK )
	{
		// ��������������
		SAFE_NEW_MEMORYBUFFER(&m_pAssoc->m_HeaderRecv,headerBuffer.size,headerBuffer.size);
		COPY_MEMORYBUFFER(&headerBuffer,&m_pAssoc->m_HeaderRecv);

		SAFE_NEW_MEMORYBUFFER(&m_pAssoc->m_BodyRecv,bodyBuffer.size,bodyBuffer.size);
		COPY_MEMORYBUFFER(&bodyBuffer,&m_pAssoc->m_BodyRecv);
	}

	// ������
	// �����е��ر���Ϊ��ͨ�� BufferRealloc���� �����
	SAFE_FREE_BUFFER( headerBuffer.buffer );
	SAFE_FREE_BUFFER( bodyBuffer.buffer );

	// �����ӵ�Header������Ϣ
	if( chunk != NULL )
	{
		curl_slist_free_all(chunk);
		chunk = 0;
	}
	// �ͷ�curl
	curl_easy_cleanup(m_pAssoc->m_curlHandle);
	m_pAssoc->m_curlHandle = NULL;

	m_pAssoc->SetErrCode( code );
	TaskCode();

	// �������
	SetState( E_HTTPSTATUS_STOP );

	return (code!=CURLE_OK)?-1:0 ;
}

int CloHttpCurl::TransferProcPOSTFormData()
{
	THttpMemoryBuffer headerBuffer;
	THttpMemoryBuffer bodyBuffer;

	SetState( E_HTTPSTATUS_START );

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

	// ����Post����
	if( m_pAssoc->m_pTransferParam->pData )
	{
		if( m_pAssoc->m_pTransferParam->bFormData )
		{// ����Ǳ�����Ҫ���峤��
			curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_POSTFIELDS, m_pAssoc->m_pTransferParam->pData);		
		}
		else
		{	
			curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_POSTFIELDS, m_pAssoc->m_pTransferParam->pData);
			curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_POSTFIELDSIZE, m_pAssoc->m_pTransferParam->uDataLength);	
		}
	}

	//
    if( m_pAssoc->m_pTransferParam->form.fformadd_cb )
	{
		m_pAssoc->m_pTransferParam->form.fformadd_cb( (void**)&post, (void**)&last , (fun_formadd_write_callback)curl_formadd , m_pAssoc->m_pTransferParam->form.pformdata );

		curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_HTTPPOST, post); 
	}

	// ���ô�����Ϣ
	UpdateProxy(m_pAssoc->m_pTransferParam->cURL);

	// ���ô���״̬�ص�
	SetTransferCallback((void *)&headerBuffer, (void *)&bodyBuffer);

	// ���÷���Agent
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_USERAGENT, GetUserAgent() );
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_FOLLOWLOCATION, 1);		// �Զ��ض���
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_AUTOREFERER, 1);

	// ������ȡ
	CURLcode code = curl_easy_perform(m_pAssoc->m_curlHandle);

	GET_HTTP_CODE();

	ClearBuffer();

	// �������
	if( code == CURLE_OK )
	{
		// ��������������
		SAFE_NEW_MEMORYBUFFER(&m_pAssoc->m_HeaderRecv,headerBuffer.size,headerBuffer.size);
		COPY_MEMORYBUFFER(&headerBuffer,&m_pAssoc->m_HeaderRecv);

		SAFE_NEW_MEMORYBUFFER(&m_pAssoc->m_BodyRecv,bodyBuffer.size,bodyBuffer.size);
		COPY_MEMORYBUFFER(&bodyBuffer,&m_pAssoc->m_BodyRecv);
	}

	// ������
	// �����е��ر���Ϊ��ͨ�� BufferRealloc���� �����
	SAFE_FREE_BUFFER( headerBuffer.buffer );
	SAFE_FREE_BUFFER( bodyBuffer.buffer );

	// �������Ϣ
	if( post != NULL )
		curl_formfree(post);

	// �����ӵ�Header������Ϣ
	if( chunk != NULL )
	{
		curl_slist_free_all(chunk);
		chunk = 0;
	}

	// �ͷ�curl
	curl_easy_cleanup(m_pAssoc->m_curlHandle);
	m_pAssoc->m_curlHandle = NULL;

	m_pAssoc->SetErrCode( code );
	TaskCode();

	// �������
	SetState( E_HTTPSTATUS_STOP );

	return (code != CURLE_OK)?-1:0;
}

int CloHttpCurl::TransferProcDELETE()
{
	THttpMemoryBuffer headerBuffer;
	THttpMemoryBuffer bodyBuffer;

	SetState( E_HTTPSTATUS_START );

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
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_USERAGENT, GetUserAgent() );
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_FOLLOWLOCATION, 1);		// �Զ��ض���
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_AUTOREFERER, 1);


	// ������ȡ
	CURLcode code = curl_easy_perform(m_pAssoc->m_curlHandle);

	GET_HTTP_CODE();

	ClearBuffer();
	// �������
	if( code == CURLE_OK )
	{
		// ��������������
		SAFE_NEW_MEMORYBUFFER(&m_pAssoc->m_HeaderRecv,headerBuffer.size,headerBuffer.size);
		COPY_MEMORYBUFFER(&headerBuffer,&m_pAssoc->m_HeaderRecv);

		SAFE_NEW_MEMORYBUFFER(&m_pAssoc->m_BodyRecv,bodyBuffer.size,bodyBuffer.size);
		COPY_MEMORYBUFFER(&bodyBuffer,&m_pAssoc->m_BodyRecv);
	}

	// ������
	// �����е��ر���Ϊ��ͨ�� BufferRealloc���� �����
	SAFE_FREE_BUFFER( headerBuffer.buffer );
	SAFE_FREE_BUFFER( bodyBuffer.buffer );

	// �����ӵ�Header������Ϣ
	if( chunk != NULL )
	{
		curl_slist_free_all(chunk);
		chunk = 0;
	}

	// �ͷ�curl
	curl_easy_cleanup(m_pAssoc->m_curlHandle);
	m_pAssoc->m_curlHandle = NULL;

	m_pAssoc->SetErrCode( code );
	TaskCode();

	// �������
	SetState( E_HTTPSTATUS_STOP );

	return (code != CURLE_OK)?-1:0;
}

int CloHttpCurl::TransferProcPUT()
{
	THttpMemoryBuffer headerBuffer;
	THttpMemoryBuffer bodyBuffer;

	SetState( E_HTTPSTATUS_START );
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

	// ���� post ����
	if( m_pAssoc->m_pTransferParam->pData )
	{
		if( m_pAssoc->m_pTransferParam->bFormData )
		{
			curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_POSTFIELDS, m_pAssoc->m_pTransferParam->pData);						// ����Ǳ�����Ҫ���峤��
		}
		else
		{	
			curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_POSTFIELDS, m_pAssoc->m_pTransferParam->pData);
			curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_POSTFIELDSIZE, m_pAssoc->m_pTransferParam->uDataLength);	
		}
	}

	// ���ø��Ӷ���
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_CUSTOMREQUEST, "PUT");

	// ���÷���Agent
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_USERAGENT, GetUserAgent() );
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_FOLLOWLOCATION, 1);		// �Զ��ض���
	curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_AUTOREFERER, 1);

	// ���ô���״̬�ص�
	SetTransferCallback((void *)&headerBuffer, (void *)&bodyBuffer);

	// ������ȡ
	CURLcode code = curl_easy_perform(m_pAssoc->m_curlHandle);

	GET_HTTP_CODE();

	ClearBuffer();
	// �������
	if( code == CURLE_OK )
	{
		// ��������������
		SAFE_NEW_MEMORYBUFFER(&m_pAssoc->m_HeaderRecv,headerBuffer.size,headerBuffer.size);
		COPY_MEMORYBUFFER(&headerBuffer,&m_pAssoc->m_HeaderRecv);

		SAFE_NEW_MEMORYBUFFER(&m_pAssoc->m_BodyRecv,bodyBuffer.size,bodyBuffer.size);
		COPY_MEMORYBUFFER(&bodyBuffer,&m_pAssoc->m_BodyRecv);
	}

	// ������
	// �����е��ر���Ϊ��ͨ�� BufferRealloc���� �����
	SAFE_FREE_BUFFER( headerBuffer.buffer );
	SAFE_FREE_BUFFER( bodyBuffer.buffer );

	// �����ӵ�Header������Ϣ
	if( chunk != NULL )
	{
		curl_slist_free_all(chunk);
		chunk = 0;
	}

	// �ͷ�curl
	curl_easy_cleanup(m_pAssoc->m_curlHandle);
	m_pAssoc->m_curlHandle = NULL;

	m_pAssoc->SetErrCode( code );
	TaskCode();

	// �������
	SetState( E_HTTPSTATUS_STOP );

	return (code != CURLE_OK)?-1:0;
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

int CloHttpCurl::TaskBuffer(unsigned char* pBuffer, long lLength)
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
		m_pAssoc->m_pHttpTask->OnTaskCode( m_pAssoc->m_errCode , 0);
	}
}

int	CloHttpCurl::GetResponedHeader(THttpMemoryBuffer * pHeaderBuf)
{
	if( m_pAssoc->m_nState != E_HTTPSTATUS_STOP )
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
	if( m_pAssoc->m_nState != E_HTTPSTATUS_STOP )
	{
		return -1;
	}
	// modified by loach 2009-07-27
	SAFE_NEW_MEMORYBUFFER(pBodyBuf,(m_pAssoc->m_BodyRecv.size+1),m_pAssoc->m_BodyRecv.size);
	COPY_MEMORYBUFFER(&m_pAssoc->m_BodyRecv,pBodyBuf);

	return 0;
}

void CloHttpCurl::UpdateProxy(char* lpszRequestURL)
{
	//// a code snippet

	if( !m_pAssoc->m_curlHandle || !lpszRequestURL || *lpszRequestURL == '\0' )
		return ;

	char* szUrl = _strlwr(lpszRequestURL) ;
	
	if( 0 != strstr(szUrl , "https://" ) )
	{
		curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(m_pAssoc->m_curlHandle, CURLOPT_SSL_VERIFYHOST, 1L);
	}

	if( m_pAssoc->m_ProxyInfo.nProxyType != E_PROXYTYPE_NONE && strlen(m_pAssoc->m_ProxyInfo.cServer) > 0 )
	{
		char buf[1024]={0};
		sprintf( buf, "%s:%d", m_pAssoc->m_ProxyInfo.cServer, m_pAssoc->m_ProxyInfo.wPort );

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
		if( 0 != pClient->TaskBuffer( (unsigned char*)ptr, (long)realsize ) )
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
		case E_HTTPTYPE_POST_FORMDATA:
		//case E_HTTPTYPE_UPLOAD:
		case E_HTTPTYPE_PUT:
			return pClient->TaskProgress((long)ulnow, (long)ultotal);
		}
	}
	return 0;
}

long CloHttpCurl::GetTaskID() const {return m_pAssoc->m_lTaskID;}

int CloHttpCurl::GetType() const {return m_pAssoc->m_nType;}

int CloHttpCurl::GetState() const {return m_pAssoc->m_nState;}

int CloHttpCurl::GetErrCode() const {return m_pAssoc->m_errCode;}

BOOL CloHttpCurl::IsUserCancel() const { return m_pAssoc->m_bUserCancel;}

long CloHttpCurl::GetHttpCode() const { return m_pAssoc->m_lhttpCode;}

void CloHttpCurl::SetUserAgent(const char* agent)
{
	if(agent)
		strcpy(m_pAssoc->m_useAgent , agent);
}

const char* CloHttpCurl::GetUserAgent() const{
	return m_pAssoc->m_useAgent;
}

END_NAMESPACE(lohttp)
