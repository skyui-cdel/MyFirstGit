#include "Stdafx.h"
#include "loHttp.h"
#include "loHttpAssoc.h"
#include "loHttpCurl.h"
#include "loUrlcode.h"
#include "lostrconv.h"
#include "loHttpStruct.h"
#include <time.h>


using namespace lohttp;

//
CloHttp::CloHttp()
: m_pAssoc( new CloHttpAssoc() )
{

}

CloHttp::~CloHttp(void)
{
	Cancel();

	delete m_pAssoc;
	m_pAssoc = NULL;
}

void CloHttp::AddBinaryData(const char* szBinaryData,long lLen)
{
	if( szBinaryData && lLen > 0 )
	{
		m_pAssoc->m_binData.WriteBytes( szBinaryData , lLen );
	}
}

void CloHttp::ClearBinaryData(void)
{
	m_pAssoc->m_binData.Shift( 0 );
}

void  CloHttp::SetFileName(const HTTPChar* szPath,const HTTPChar* szFile)
{
	if( szPath )
	{
		
		HTTP_TCSCPY_S(m_pAssoc->m_szPath ,HTTP_TCSLEN(szPath) + 1,szPath );
	}

	if( szFile )
	{
		HTTP_TCSCPY_S(m_pAssoc->m_szFile ,HTTP_TCSLEN(szFile) + 1, szFile);
	}
}

void CloHttp::ClearParams()
{
	m_pAssoc->ClearParams();
}

void CloHttp::ClearHeader()
{
	m_pAssoc->ClearHeader();
}

#define REQUEST_CHECK( ret ) \
{\
	if( szUrl && *szUrl != '\0' ) \
	   HTTP_TCSCPY_S(m_pAssoc->m_szURL , 1024, szUrl);\
	if( !m_pAssoc->m_pHttpCurl ) return ret;\
	if( *m_pAssoc->m_szURL == '\0' ) return ret;\
}\

CloHttpResponse* CloHttp::CreateHttpResponse(void)
{
	THttpMemoryBuffer header, body;
	m_pAssoc->m_pHttpCurl->GetResponedHeader(&header);
	m_pAssoc->m_pHttpCurl->GetResponedBody(&body);

	CloHttpResponse * pResponse = new CloHttpResponse();
	pResponse->m_pAssoc->SetHttpResponse(&header, &body);
	pResponse->m_pAssoc->SetHttpCode( m_pAssoc->m_pHttpCurl->GetHttpCode() );

	SAFE_DELETE_MEMORYBUFFER(&header);
	SAFE_DELETE_MEMORYBUFFER(&body);

	return pResponse;
}

// 获取到回应包
CloHttpResponse* CloHttp::GetRespone(void) const
{
	return m_pAssoc->m_pRespone;
}

// 分离 回应包
CloHttpResponse* CloHttp::DetachRespone(void)
{
	CloHttpResponse* p = m_pAssoc->m_pRespone;
	m_pAssoc->m_pRespone = 0;
	return p;
}

THttpTime* CloHttp::GetHttpTime() const
{
	return &m_pAssoc->m_time;
}

int CloHttp::Request(const HTTPChar* szUrl,int method)
{
	REQUEST_CHECK( -1 );

	int iret = -1;
#define METHOD_CALLBACK( FUN ) \
	{\
    	iret = FUN();\
		break;\
	}

	m_pAssoc->OnStart();

	switch( method )
	{
	case E_HTTP_GET_METHOD:
		METHOD_CALLBACK( HttpMethodGet );

	case E_HTTP_POST_METHOD:
		METHOD_CALLBACK( HttpMethodPost );

//	case E_HTTP_POST_BINARY_METHOD:
//		METHOD_CALLBACK( HttpMethodPostBinary );

//	case E_HTTP_UPLOAD_BINARY_TEXT_METHOD:
//		METHOD_CALLBACK( HttpMethodPostBinaryText );

	case E_HTTP_UPLOAD_METHOD:
		METHOD_CALLBACK( HttpMethodUpload );

	case E_HTTP_DELETE_METHOD:
		METHOD_CALLBACK( HttpMethodDelete );

	case E_HTTP_PUT_METHOD:
		METHOD_CALLBACK( HttpMethodPut );
		break;
	case E_HTTP_DOWNLOAD_METHOD:
		METHOD_CALLBACK( HttpMethodDownload );
		break;
	default:
		break;
	}

	m_pAssoc->OnFinished( CreateHttpResponse() );
		
	m_pAssoc->OnTaskState( E_HTTPSTATUS_COMPLETE );

	return iret;
}

// 异步请求（启动线程）
int CloHttp::RequestAsyn(const HTTPChar* szUrl , int method)
{
	REQUEST_CHECK( -1 );
	
	if( m_pAssoc->m_hInetTh )
	{// 线程名柄存在，但可能已失效
		if( WAIT_OBJECT_0 == WaitForSingleObject( m_pAssoc->m_hInetTh , 0  ) )
		{
			CloseHandle( m_pAssoc->m_hInetTh );
			m_pAssoc->m_hInetTh = NULL;
		}
		else
		{
			return 0;
		}
	}
	
	m_pAssoc->m_nMethod = method;
	m_pAssoc->m_hInetTh = ::CreateThread( NULL , 0 , CloHttpAssoc::HttpThread , this , 0 , NULL );
	return (  NULL == m_pAssoc->m_hInetTh ) ? -1 : 0;
	
}

void CloHttp::Wait()
{
	if( !m_pAssoc->m_hInetTh )
		return ;

	WaitForSingleObject( m_pAssoc->m_hInetTh , INFINITE );
}


unsigned long CloHttp::StartAsyn()
{
	return Request(NULL , m_pAssoc->m_nMethod );
}

bool CloHttp::Cancel (unsigned long msTimeout)
{

	// 取消HTTP下载
	if( m_pAssoc->m_pHttpCurl )
	{
		m_pAssoc->m_pHttpCurl->CancelRequest();
	}

	// 把线程kill
	if( m_pAssoc->m_hInetTh )
	{	
		if( WAIT_OBJECT_0 != WaitForSingleObject( m_pAssoc->m_hInetTh , msTimeout ) )
		{
			// wait time out
			::TerminateThread(m_pAssoc->m_hInetTh,-1);
		}
		
		CloseHandle( m_pAssoc->m_hInetTh );
		m_pAssoc->m_hInetTh = NULL;
	}

	// 其他数据清掉
	ClearParams();
	ClearHeader();
	ClearBinaryData();

	return true;
}


/*下载大文件(内存缓冲区) 回调接口［边下载中］ */
void CloHttp::SetCallbackOfBuffer(fun_httpbuffer_callback fbuffer_cb)
{
	m_pAssoc->m_cb.fbuffer_cb = fbuffer_cb;
}

/* 进度状态 */
void CloHttp::SetCallbackOfProgress(fun_httpprogress_callback fprogress_cb)
{
	m_pAssoc->m_cb.fprogress_cb = fprogress_cb;
}

/** 下载状态
*
* @param state 状态(成功，失败）
* @param errCode 失败码
*/
void CloHttp::SetCallbackOfCode(fun_httpcode_callback fcode_cb)
{
	m_pAssoc->m_cb.fcode_cb = fcode_cb;
}

void CloHttp::SetCallbackOfRespone(fun_httprespone_callback fresp_cb)
{
	m_pAssoc->m_cb.frespone_cb = fresp_cb;
}

/** CloHttp 资源释放掉 */
void CloHttp::SetCallbackOfRelease(fun_httprelease_callback frelease_cb)
{
	m_pAssoc->m_cb.frelease_cb = frelease_cb;
}

/** 设置自定义数据 */
void CloHttp::SetUserData(void* puserdata)
{
	m_pAssoc->m_pUserData = puserdata;
}

// 设置url
void CloHttp::SetURL( const HTTPChar* szURL )
{
	if( szURL )	HTTP_TCSCPY_S(m_pAssoc->m_szURL , ConstURLLength, szURL);
}

void CloHttp::AddParam(const HTTPChar* szName, const HTTPChar* szValue , unsigned long dwParamAttr /* = lohttp::ParamNormal */)
{
	THttpURLParam * param = new THttpURLParam;
	if( szName != NULL )
	{
		size_t len = HTTP_TCSLEN(szName);
		Http_Char_New(param->pszName,len+1);
		HTTP_TCSCPY_S(param->pszName, len + 1, szName);
		param->pszName[len] = '\0';
	}
	if( szValue != NULL  )
	{
		size_t len = HTTP_TCSLEN(szValue);
		Http_Char_New(param->pszValue,len+1);
		HTTP_TCSCPY_S(param->pszValue, len + 1, szValue);
		param->pszValue[len] = '\0';
	}
	param->dwAttrib = dwParamAttr;
	m_pAssoc->m_lstParam.push_back(param);
}

void CloHttp::AddHeader(const HTTPChar* szName, const HTTPChar* szValue/* = NULL*/)
{
	THttpHeader * header = new THttpHeader;
	if( szName != NULL )
	{
		size_t len = HTTP_TCSLEN(szName);
		Http_Char_New(header->pszName,len+1);
		HTTP_TCSCPY_S(header->pszName, len + 1, szName);
		header->pszName[len] = '\0';
	}
	if( szValue != NULL  )
	{
		size_t len = HTTP_TCSLEN(szValue);
		Http_Char_New(header->pszValue,len+1);
		HTTP_TCSCPY_S(header->pszValue, len + 1, szValue);
		header->pszValue[len] = '\0';
	}
	m_pAssoc->m_lstHeader.push_back(header);
}

void CloHttp::SetProxySrvAddr(int proxyType,
								  const HTTPChar* szProxySrv,
								  unsigned short nProxySrvPort)
{
	m_pAssoc->m_nProxyType = proxyType;
	m_pAssoc->m_nProxySrvPort = nProxySrvPort;

	HTTP_TCSCPY_S(m_pAssoc->m_szProxySrvAddr,HTTP_TCSLEN(szProxySrv) + 1 ,szProxySrv);
}

void CloHttp::SetProxyAccount(const HTTPChar* szUserName /* = NULL */,
								  const HTTPChar* szPassword /* = NULL */)
{
	HTTP_TCSCPY_S(m_pAssoc->m_szProxyUserName,HTTP_TCSLEN(szUserName) + 1, szUserName);
	HTTP_TCSCPY_S(m_pAssoc->m_szProxyUserPwd ,HTTP_TCSLEN(szPassword) + 1, szPassword);
}

void CloHttp::ClearProxy()
{
	m_pAssoc->ClearProxy();
}

void CloHttp::EnableFormData(bool bForm)
{
	m_pAssoc->m_bFormData = bForm;
}

void CloHttp::SetHttpForm(const struct THttpForm* pform)
{
	if( pform )
	{
		// old 
		if( m_pAssoc->m_Httpform.ffromdata_free_cb && m_pAssoc->m_Httpform.pformdata )
		{
			m_pAssoc->m_Httpform.ffromdata_free_cb( m_pAssoc->m_Httpform.pformdata );
			m_pAssoc->m_Httpform.pformdata = 0;
		}
		//
		memcpy(& m_pAssoc->m_Httpform , pform , sizeof(struct THttpForm ));
	}
}

//void CloHttp::SetUploadParam(const HTTPChar* lpszParamName, const HTTPChar* lpszFile)
//{
//	if(lpszParamName)
//	{
//		_TCSCPY_S(m_pAssoc->m_szUploadParam, ConstURLLength, lpszParamName);
//	}
//
//	if( lpszFile )
//	{
//		_TCSCPY_S(m_pAssoc->m_szUploadFile, MAX_PATH+1, lpszFile);
//	}
//}

void CloHttp::SetUserAgent(const HTTPChar* szAgent)
{
	if( szAgent && m_pAssoc->m_pHttpCurl )
	{
		m_pAssoc->m_pHttpCurl->SetUserAgent( HTTP_CT2A(szAgent) );
	}
}

// 记录开始时间
#define START_HTTP_TIME() \
	THttpTime dTime;\
	time_t now;\
	HTTP_TCSCPY_S(dTime.szURL, 1024, m_pAssoc->m_szURL );\
	time(&now);\
	dTime.tRequest = now;\

// 记录完成时间
#define END_HTTP_TIME() \
	time(&now);\
	dTime.tResponse = now;\
	m_pAssoc->OnTaskTime(&dTime);

int	CloHttp::HttpMethodDownload()
{
	TTaskParam param;

	strcpy_s(param.cURL, ConstURLLength, m_pAssoc->MakeGetURL().c_str() );
	strcpy_s(param.cHead, ConstHeadLength,  m_pAssoc->GetHttpHeader().c_str() );

	if( *(m_pAssoc->m_szPath) == '\0' || *(m_pAssoc->m_szFile) == '\0' )
	{// 进行缓冲区下载
		param.bToBuffer = TRUE;
	}
	else
	{
		param.bToBuffer = FALSE;
		strcpy_s(param.cSavePath,ConstLocalFileLength,HTTP_CT2A(m_pAssoc->m_szPath));
		strcpy_s(param.cNewFileName,ConstLocalFileLength ,HTTP_CT2A(m_pAssoc->m_szFile));
		param.bRename = TRUE;
	}
	m_pAssoc->m_pHttpCurl->SetTransferParam(E_HTTPTYPE_DOWNLOAD, (LPVOID)&param);

	// 设置代理信息
	SetHttpProxyCfg();


	START_HTTP_TIME();

	// 开始下载
	if( m_pAssoc->m_pHttpCurl->Request() != 0 )
		return -1;	

	END_HTTP_TIME();

	return 0;
}

int CloHttp::HttpMethodGet()
{
	TTaskParam param;

	strcpy_s(param.cURL, ConstURLLength, m_pAssoc->MakeGetURL().c_str() );
	strcpy_s(param.cHead, ConstHeadLength,  m_pAssoc->GetHttpHeader().c_str() );

	m_pAssoc->m_pHttpCurl->SetTransferParam(E_HTTPTYPE_GET, (LPVOID)&param);

	// 设置代理信息
	SetHttpProxyCfg();


	START_HTTP_TIME();

	// 开始下载
	if( m_pAssoc->m_pHttpCurl->Request() != 0 )
		return -1;	

	END_HTTP_TIME();

	return 0;
}

//int CloHttp::HttpMethodPostBinaryText()
//{
//	return HttpMethod(E_HTTPTYPE_POST_FORMDATA);
//}

//	TTaskParam param;
//	param.bFormData = m_pAssoc->m_bFormData;
//
//	strcpy_s(param.cURL , ConstURLLength , HTTP_CT2A(m_pAssoc->m_szURL) );
//
//	// head
//	strcpy_s(param.cHead , ConstHeadLength , m_pAssoc->GetHttpHeader().c_str() );
//
//	// 二进
//	if( m_pAssoc->m_binData.Length() > 0 )
//	{
//		param.uDataLength = m_pAssoc->m_binData.Length();	
//		param.pData = new char[ m_pAssoc->m_binData.Length() ];
//		memcpy(param.pData , m_pAssoc->m_binData.Data() , m_pAssoc->m_binData.Length() );
//	}
//
//
//	strcpy_s(param.cLocalFile   , ConstLocalFileLength , HTTP_CT2A(m_pAssoc->m_szFile) );
//
//	// param 
//	std::string strParam = m_pAssoc->MakeGetParam();
//	strcpy_s(param.cFormContents ,ConstFormContentLength , strParam.c_str() );
//
//	m_pAssoc->m_pHttpCurl->SetTransferParam(E_HTTPTYPE_POST_TEXT_FILE, (LPVOID)&param);
//
//	// 设置代理信息
//	SetHttpProxyCfg();
//
//	START_HTTP_TIME();
//
//	// 开始下载
//	if( m_pAssoc->m_pHttpCurl->Request() != 0 )
//		return -1;
//
//	END_HTTP_TIME();
//
//	return 0;
//}

//int	CloHttp::HttpMethodPostBinary()
//{//
//	return HttpMethod( E_HTTPTYPE_POST );
//}

int CloHttp::HttpMethodPost()
{// 一般处理 

	return HttpMethod( E_HTTPTYPE_POST );
}


int CloHttp::HttpMethodUpload()
{
	return HttpMethod(E_HTTPTYPE_POST_FORMDATA);
}

int	CloHttp::HttpMethodPut()
{
	return HttpMethod(E_HTTPTYPE_PUT);
}

int	CloHttp::HttpMethodDelete()
{
	return HttpMethod(E_HTTPTYPE_DELETE);
}

int CloHttp::HttpMethod(int method)
{
	TTaskParam param;
	param.bFormData = m_pAssoc->m_bFormData;

	strcpy_s(param.cURL , ConstURLLength , HTTP_CT2A(m_pAssoc->m_szURL) );

	// head
	strcpy_s(param.cHead, ConstHeadLength, m_pAssoc->GetHttpHeader().c_str() );

	if( m_pAssoc->m_binData.Length() > 0 )
	{// 
		param.uDataLength = m_pAssoc->m_binData.Length();	
		param.pData = new char[ m_pAssoc->m_binData.Length() ];
		memcpy(param.pData , m_pAssoc->m_binData.Data() , m_pAssoc->m_binData.Length() );
	}
	else
	{//
		std::string postParam( m_pAssoc->MakeGetParam() );
		if( postParam.length() > 0 )
		{
			int dataLength = 0;
			param.uDataLength = dataLength = postParam.length();
			dataLength++;
			param.pData = new char[dataLength];
			strcpy_s(param.pData, dataLength, postParam.c_str() );
			param.pData[param.uDataLength] = '\0';
		}
	}

	// form data
	memcpy(&param.form , &m_pAssoc->m_Httpform, sizeof(THttpForm));


	m_pAssoc->m_pHttpCurl->SetTransferParam(method, (LPVOID)&param);

	// 设置代理信息
	SetHttpProxyCfg();

	START_HTTP_TIME();

	// 开始下载
	if( m_pAssoc->m_pHttpCurl->Request() != 0 )
		return -1;

	END_HTTP_TIME();

	return 0;
}

void CloHttp::SetHttpProxyCfg()
{
	if( !m_pAssoc->m_pHttpCurl )
		return;

	if( m_pAssoc->m_nProxyType == E_PROXYTYPE_SOCKS5 || m_pAssoc->m_nProxyType == E_PROXYTYPE_HTTP )
	{
		THttpProxyInfo proxyInfo;
		strcpy_s(proxyInfo.cServer, ConstProxyServerLength , HTTP_CT2A(m_pAssoc->m_szProxySrvAddr) );
		proxyInfo.wPort = m_pAssoc->m_nProxySrvPort;

		strcpy_s(proxyInfo.cUser    , ConstProxyUserLength, HTTP_CT2A(m_pAssoc->m_szProxyUserName) );
		strcpy_s(proxyInfo.cPassword, ConstProxyPwdLength, HTTP_CT2A(m_pAssoc->m_szProxyUserPwd) );
		proxyInfo.nProxyType = m_pAssoc->m_nProxyType;

		m_pAssoc->m_pHttpCurl->SetProxyInfo(proxyInfo);
	}
}

//------------------------------------------------------------------------------------//
// CloHttpResponse

CloHttpResponse::CloHttpResponse(void)
{
	m_pAssoc = new CloHttpResponseAssoc();
}

CloHttpResponse::~CloHttpResponse(void)
{
	delete m_pAssoc;
}

static 
bool GetHttpMemoryBufferLength(const THttpMemoryBuffer* buffer , unsigned long& dwBodyLen)
{
	if( buffer->buffer != NULL && buffer->size > 0 )
	{
		dwBodyLen = buffer->size;
		return true;
	}
	return false;
}

static
unsigned long GetHttpMemoryBuffer(const THttpMemoryBuffer* buffer , char * pbyBuff, unsigned long cbBuff, unsigned long& offsize)
{
	if( buffer->buffer != NULL && buffer->size > 0 )
	{
		if( offsize >= buffer->size )
			return 0;

		unsigned long copyDataLength = (unsigned long)(buffer->size - offsize);
		if( cbBuff < copyDataLength )
			copyDataLength = cbBuff;

		memcpy(pbyBuff, buffer->buffer + offsize , copyDataLength);

		offsize += copyDataLength;

		return copyDataLength;
	}
	return 0;
}

// 头信息
bool CloHttpResponse::GetHeaderLength(unsigned long& len) const
{
	return GetHttpMemoryBufferLength(&m_pAssoc->m_httpHeader , len );
}

unsigned long CloHttpResponse::ReadHeader(char * pbyBuff, unsigned long cbBuff , unsigned long& offsize) const
{
	return GetHttpMemoryBuffer(&m_pAssoc->m_httpHeader , pbyBuff, cbBuff ,  offsize );
}

bool CloHttpResponse::ReadHeaderCallback(fun_http_resp_header_callback cb , void* pUsrData) const
{
	if( !cb )
		return false;

	cb( GetHttpCode() , m_pAssoc->m_httpHeader.buffer , m_pAssoc->m_httpHeader.size , pUsrData );

	return true;
}

bool CloHttpResponse::GetBodyLength(unsigned long & dwBodyLen) const
{
	return GetHttpMemoryBufferLength(&m_pAssoc->m_httpBody , dwBodyLen );
}

unsigned long CloHttpResponse::ReadBody(char * pbyBuff, unsigned long cbBuff, unsigned long& offsize) const
{
	return GetHttpMemoryBuffer(&m_pAssoc->m_httpBody , pbyBuff, cbBuff ,  offsize );
}

bool CloHttpResponse::ReadBodyCallback(fun_http_resp_body_callback cb , void* pUsrData) const
{
	if( !cb )
		return false;

	cb( GetHttpCode() , m_pAssoc->m_httpBody.buffer , m_pAssoc->m_httpBody.size , pUsrData );

	return true;
}

long CloHttpResponse::GetHttpCode(void) const
{
	return m_pAssoc->m_lhttpcode;
}

///////////////////////////////////////////////////////////////////////////////////////////////
/** http header 信息 */

CloHttpHeader::CloHttpHeader()
{
	m_pAssoc = new CHttpMap();
	m_httpState[0] = '\0';
}

CloHttpHeader::~CloHttpHeader()
{
	delete ( (CHttpMap*)m_pAssoc );
}

typedef struct tagHttpHeaderValue
{
	char* szVal;
	int   len;

	tagHttpHeaderValue()
	{
		szVal = NULL;
		len = 0;
	}
}THttpHeaderValue;

static
void gfun_str_trim(char* &sz1,char* &szEnd)
{
	// 除掉后面的空格符号
	while( (*szEnd) == 0x20 && szEnd >= sz1 ){
		szEnd--;
	}
	// 除掉前面的空格符号
	while( (*sz1) == 0x20 && sz1 <= szEnd ){
		sz1++;
	}
}

static
bool gfun_Header_line_parse(char* szLine, 
							char* szEnd ,
							THttpHeaderValue &Key , THttpHeaderValue& Value)
{
	char* szTmpEnd = szEnd;
	char* sz1 = strchr(szLine , ':');
	if( sz1 )
	{
		// 需要提出 key 非空格字符
		char* szkey = szLine;
		while( (*szkey) == 0x20 && szkey < sz1 ){
			szkey++;
		}
		Key.szVal = szkey;
		Key.len   = (int)(sz1 - szkey);
		(*sz1) = '\0';

		sz1++;
		if( sz1 < szEnd )
		{
			gfun_str_trim(sz1,szTmpEnd);
			( *szTmpEnd ) = '\0';
			// value
			Value.szVal = sz1;
			Value.len   = (int)(szTmpEnd - sz1);
		}
		return true;
	}

	// 不对
	return false;	
}


void CloHttpHeader::Parse(const char* c_httpHeader_buffer , unsigned long httpHeader_size)
{
	CHttpMap* pHeadMap = ( (CHttpMap*)m_pAssoc );
	pHeadMap->Clear();
	m_httpState[0] = '\0';

	if( !c_httpHeader_buffer || httpHeader_size <= 0 )
		return ;
	/*
	解释头如下格式,它们之间"\r\n" 分行
	Exmaple:

	HTTP/1.1 200 OK
	Server: Apache-Coyote/1.1
	Content-Type: text/html;charset=GBK
	Content-Length: 406
	Date: Tue, 28 Jul 2009 07:05:31 GMT

	*/
	BOOL bHttpState = true;

	char* httpHeader_buffer = new char[httpHeader_size+1];
	memcpy(httpHeader_buffer , c_httpHeader_buffer , httpHeader_size );
	httpHeader_buffer[ httpHeader_size ] = '\0';

	char* szBegin = httpHeader_buffer;
	char* szEnd   = httpHeader_buffer + httpHeader_size;
	char* szLine  = szBegin;
	while( szBegin < szEnd )
	{
		if( (*szBegin) == 0x0d && *(szBegin+1) == 0x0a )
		{
			if( bHttpState )
			{
				// 分析第一个，是状态
				char* szxx = szLine;
				char* szyy = szBegin;
				gfun_str_trim(szxx,szyy);
				*(szyy) = '\0';
				const HTTPChar *st = HTTP_CA2T( szxx );
				HTTP_TCSCPY_S( m_httpState,HTTP_TCSLEN(st) + 1, HTTP_CA2T( st ) );
				bHttpState = FALSE;				
			}
			else
			{
				THttpHeaderValue Key,Value;
				if( gfun_Header_line_parse(szLine , szBegin , Key , Value) ){
					// 加入
					pHeadMap->Add( HTTP_CA2T(Key.szVal), HTTP_CA2T(Value.szVal) );
				}
			}
			// 下一行
			szBegin++;
			szBegin++;
			szLine = szBegin;
		}
		else
			szBegin++;
	}
	delete[] httpHeader_buffer;
}

bool CloHttpHeader::GetContentLength (unsigned long & cbContLen) const
{
	const HTTPChar* szValue = GetHeader( HTTP_T("Content-Length"),0);
	if( szValue )
	{
		cbContLen = (unsigned long)( HTTP_TTOL(szValue) );
		return true;
	}
	return false;
}

int CloHttpHeader::GetHeadercount(const HTTPChar* szName) const
{
	CHttpMap* pHeadMap = ( (CHttpMap*)m_pAssoc );
	return pHeadMap->Count(szName);
}

const HTTPChar* CloHttpHeader::GetHeader(const HTTPChar* szName, unsigned long nIdx/* = 0*/) const
{
	CHttpMap* pHeadMap = ( (CHttpMap*)m_pAssoc );
	try
	{
		if( !pHeadMap->Exists(szName, nIdx) )
			return NULL;

		return pHeadMap->Get(szName,nIdx).szValue;
	}
	catch(...)
	{
		return NULL;
	}
	return NULL;
}

/** 读取类似于 " 200 OK 这样的信息 "*/
const HTTPChar*	CloHttpHeader::GetHeaderState() const
{
	return m_httpState;
}
