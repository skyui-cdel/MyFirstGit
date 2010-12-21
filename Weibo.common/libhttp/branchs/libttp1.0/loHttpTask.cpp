#include "StdAfx.h"
#include "loHttpTask.h"
#include "loHttpTaskAssoc.h"
#include "loHttpCurl.h"
#include "loUrlcode.h"
#include <time.h>

using namespace lohttp;

/*
** Convert an ansi string to microsoft unicode, based on the
** current codepage settings for file apis.
** 
** Space to hold the returned string is obtained
** from sqliteMalloc.
*/
static 
std::wstring MbcsToUnicode(const char *zFilename)
{
	wchar_t *zMbcsFilename;
	std::wstring strFileName;

	int codepage = AreFileApisANSI() ? CP_ACP : CP_OEMCP;

	int nByte = MultiByteToWideChar(codepage, 0, zFilename, -1, NULL,0);
	zMbcsFilename = new wchar_t[nByte+1];
	if( zMbcsFilename==0 ){
		return strFileName;
	}
	nByte = MultiByteToWideChar(codepage, 0, zFilename, -1, zMbcsFilename, nByte);
	if( nByte > 0 )
	{
		zMbcsFilename[nByte] = 0;
		strFileName = zMbcsFilename;
	}
	delete[] zMbcsFilename;
	return strFileName;

}

static
std::string UnicodeToUtf8(const wchar_t *zWideFilename)
{
	int nByte;
	char *zFilename;
	std::string strFileName;

	nByte = WideCharToMultiByte(CP_UTF8, 0, zWideFilename, -1, 0, 0, 0, 0);
	zFilename = new char[nByte +1];
	if( zFilename == 0 ){
		return strFileName;
	}
	nByte = WideCharToMultiByte(CP_UTF8, 0, zWideFilename, -1, zFilename, nByte+1, 	0, 0);
	if( nByte > 0 )
	{
		zFilename[nByte] = '\0';
	    strFileName = zFilename;
	}
	delete []zFilename;
	return strFileName;
}

/*
** Convert multibyte character string to UTF-8.  Space to hold the
** returned string is obtained from sqliteMalloc().
*/
static
std::string MbcsToUtf8(const char *zFilename)
{
  std::wstring zTmpWide = MbcsToUnicode(zFilename);
  if( zTmpWide.empty() ){
	  return std::string("");
  }
  return UnicodeToUtf8(zTmpWide.c_str());
}

#ifdef _UNICODE
#define STR_TO_UTF8(s) UnicodeToUtf8(s)
#else
#define STR_TO_UTF8(s) MbcsToUtf8(s)
#endif


CloHttpTask::CloHttpTask(IloTaskCallBack* pTaskCallBack)
: m_pAssoc( new CloHttpTaskAssoc(pTaskCallBack) )
{
}

CloHttpTask::~CloHttpTask(void)
{
	delete m_pAssoc;
	m_pAssoc = NULL;
}

void CloHttpTask::AddBinaryData(const char* szBinaryData,long lLen)
{
	if( szBinaryData && lLen > 0 )
	{
		m_pAssoc->m_BinData.WriteBytes( szBinaryData , lLen );
	}
}

void  CloHttpTask::SetFileName(LPCTSTR szPath,LPCTSTR szFile)
{
	if( szPath )
	{
		m_pAssoc->m_strPath = szPath;
	}

	if( szFile )
	{
		m_pAssoc->m_strFile = szFile;		
	}
}

void CloHttpTask::ClearParams()
{
	m_pAssoc->ClearParams();
}

void CloHttpTask::ClearHeader()
{
	m_pAssoc->ClearHeader();
}

#define REQUEST_CHECK() \
{\
	_tcscpy_s(m_pAssoc->m_szURL, 1024, szUrl);\
	if( !m_pAssoc->m_pHttpCurl ) return NULL;\
}

CloHttpResponse* CloHttpTask::RequestGet (LPCTSTR szUrl)
{
	REQUEST_CHECK();

	if( HttpClientGet() != 0 )
		return NULL;

	return CreateHttpResponse();
}


//add by dongzhong 支持Http的delete方法
CloHttpResponse* CloHttpTask::BeginDelete (LPCTSTR szUrl)
{
	REQUEST_CHECK();

	if( HttpClientDelete() != 0 )
		return NULL;

	return CreateHttpResponse();
}


CloHttpResponse *	CloHttpTask::BeginPut (LPCTSTR szUrl)
{
	REQUEST_CHECK();

	if( HttpClientPut() != 0 )			// 这里用POST实现比较好
		return NULL;


	return CreateHttpResponse();
}


CloHttpResponse * CloHttpTask::BeginPost (LPCTSTR szUrl)
{
	REQUEST_CHECK();

	if( HttpClientPost() != 0 )			// 这里用POST实现比较好
		return NULL;

	return CreateHttpResponse();
}

CloHttpResponse *	CloHttpTask::RequestBuffer (LPCTSTR szUrl)
{
	REQUEST_CHECK();

	if( HttpClientGetDownload() != 0 )
		return NULL;

	return CreateHttpResponse();
}


CloHttpResponse *	CloHttpTask::BeginDownloadFile (LPCTSTR szUrl)
{
	REQUEST_CHECK();

	if( HttpClientGetDownload() != 0 )
		return NULL;


	return CreateHttpResponse();
}

CloHttpResponse * CloHttpTask::BeginPostBinary (LPCTSTR szUrl)
{
	REQUEST_CHECK();

	if( HttpClientPostBinary() != 0 )			// 这里用POST实现比较好
		return NULL;

	return CreateHttpResponse();
}

CloHttpResponse * CloHttpTask::BeginPostBinaryText(LPCTSTR szUrl)
{
	REQUEST_CHECK();

	if( HttpClientPostBinaryText() != 0 )			// 这里用POST实现比较好
		return NULL;

	return CreateHttpResponse();
}

CloHttpResponse * CloHttpTask::BeginUpload (LPCTSTR szUrl)
{
	REQUEST_CHECK();

	if( HttpClientUpload() != 0 )			// 使用POST传递文件
		return NULL;

	return CreateHttpResponse();
}


CloHttpResponse* CloHttpTask::CreateHttpResponse(void)
{
	THttpMemoryBuffer header, body;
	m_pAssoc->m_pHttpCurl->GetResponedHeader(&header);
	m_pAssoc->m_pHttpCurl->GetResponedBody(&body);

	CloHttpResponse * pResponse = new CloHttpResponse();

	pResponse->m_pAssoc->SetHttpResponse(&header, &body);

	SAFE_DELETE_MEMORYBUFFER(&header);
	SAFE_DELETE_MEMORYBUFFER(&body);

	return pResponse;
}


BOOL CloHttpTask::Cancel (void)
{

	// 取消HTTP下载
	if( m_pAssoc->m_pHttpCurl )
	{
		m_pAssoc->m_pHttpCurl->CancelRequest();
	}

	return TRUE;

}

void CloHttpTask::AddParam(LPCTSTR szName,
						   LPCTSTR szValue /* = NULL */,
						   unsigned long dwParamAttr /* = lohttp::ParamNormal */)
{
	if( szValue == NULL )
		return;

	if( dwParamAttr & lohttp::ParamFile )	// 如果是上传文件
	{
		SetUploadParam(szName, szValue);
	}
	else	// 一般情况
	{
		THttpURLParam * param = new THttpURLParam;
		if( szName != NULL )
		{
			size_t len = _tcslen(szName);
			Http_Char_New(param->pszName,len+1);
			_tcscpy_s(param->pszName, len + 1, szName);
			param->pszName[len] = '\0';
		}
		if( szValue != NULL  )
		{
			size_t len = _tcslen(szValue);
			Http_Char_New(param->pszValue,len+1);
			_tcscpy_s(param->pszValue, len + 1, szValue);
			param->pszValue[len] = '\0';
		}
		param->dwAttrib = dwParamAttr;
		m_pAssoc->m_lstParam.push_back(param);
	}

}

void CloHttpTask::AddParam(LPCTSTR szName, BOOL bUrlEncode, BOOL bUTF8, LPCTSTR szValue/* = NULL*/, unsigned long dwParamAttr/* = lohttp::ParamNormal*/)
{
	std::string	strUTF8Value;
	CString	strUrlEncodeValue;

	if( bUTF8 )
	{
		strUTF8Value = STR_TO_UTF8(szValue);
	}

	if( bUrlEncode )
	{
		if( bUTF8 )
		{
#ifdef UNICODE
			std::wstring out;
			CloURLCode::UrlEncode(out, strUTF8Value.c_str() , strUTF8Value.length() );
#else
			std::string out;
			CloURLCode::UrlEncodeA(out, strUTF8Value.c_str(), strUTF8Value.length() );
#endif
			strUrlEncodeValue = out.c_str();
		}
		else
		{
			CStringA temp = (CT2A)szValue;
#ifdef UNICODE
			std::wstring out;
			CloURLCode::UrlEncode(out, temp, temp.GetLength());
#else
			std::string out;
			CloURLCode::UrlEncodeA(out, temp, temp.GetLength());
#endif
			strUrlEncodeValue = out.c_str();
		}
	}
	else
	{
		strUrlEncodeValue = szValue;
	}

	AddParam(szName, (LPCTSTR)strUrlEncodeValue, dwParamAttr);
}

void CloHttpTask::AddHeader(LPCTSTR szName, LPCTSTR szValue/* = NULL*/)
{
	THttpHeader * header = new THttpHeader;
	if( szName != NULL )
	{
		size_t len = _tcslen(szName);
		Http_Char_New(header->pszName,len+1);
		_tcscpy_s(header->pszName, len + 1, szName);
		header->pszName[len] = '\0';
	}
	if( szValue != NULL  )
	{
		size_t len = _tcslen(szValue);
		Http_Char_New(header->pszValue,len+1);
		_tcscpy_s(header->pszValue, len + 1, szValue);
		header->pszValue[len] = '\0';
	}
	m_pAssoc->m_lstHeader.push_back(header);
}

void CloHttpTask::SetProxySrvAddr(int proxyType,
								LPCTSTR szProxySrv,
								unsigned short nProxySrvPort,
								LPCTSTR lpszDownloadUrl)
{
	m_pAssoc->m_nProxyType = proxyType;
	m_pAssoc->m_nProxySrvPort = nProxySrvPort;

	_tcscpy_s(m_pAssoc->m_szProxySrvAddr, szProxySrv);
}

void CloHttpTask::SetProxyAccount(LPCTSTR szUserName /* = NULL */,
								LPCTSTR szPassword /* = NULL */)
{
	_tcscpy_s(m_pAssoc->m_szProxyUserName, szUserName);
	_tcscpy_s(m_pAssoc->m_szProxyUserPwd , szPassword);
}

void CloHttpTask::ClearProxy()
{
	m_pAssoc->ClearProxy();
}

void  CloHttpTask::SetPostFormat(BOOL bForm)
{
	m_pAssoc->m_bPostForm = bForm;
}

void CloHttpTask::SetUploadParam(LPCTSTR lpszParamName, LPCTSTR lpszFile)
{
	_tcscpy_s(m_pAssoc->m_szUploadParam, 1024, lpszParamName);
	_tcscpy_s(m_pAssoc->m_szUploadFile, 1024, lpszFile);
}

int	CloHttpTask::HttpClientPut()
{
	if( !m_pAssoc->m_pHttpCurl )
		return -1;

	USES_CONVERSION;

	if(m_pAssoc->m_szURL == NULL || _tcslen(m_pAssoc->m_szURL) == 0 )
		return -1;



	TTaskParam param;
	param.nTransType = E_HTTPTYPE_PUT;

	strcpy_s(param.cURL, 1024, CT2A(m_pAssoc->m_szURL));
	CString strParam = m_pAssoc->MakeGetParam();
	CStringA strParamA = CT2A(strParam);

	param.uDataLength = strParamA.GetLength();
	param.pData = new char[param.uDataLength + 1];
	strcpy_s(param.pData, param.uDataLength + 1, strParamA.GetBuffer());
	param.pData[param.uDataLength] = '\0';

	CString strHeader = m_pAssoc->GetHttpHeader();
	strcpy_s(param.cHead, 4096, CT2A(strHeader));
	m_pAssoc->m_pHttpCurl->SetTransferParam(E_HTTPTYPE_PUT, (LPVOID)&param);
	strParam.ReleaseBuffer();
	strParamA.ReleaseBuffer();

	// 设置代理信息
	SetHttpProxyCfg();

	// 开始下载
	if( m_pAssoc->m_pHttpCurl->Request() != 0 )
		return -1;

	long lResult = m_pAssoc->m_pHttpCurl->GetResult();
	if( lResult == E_HTTPTRANS_RESULT_COMPLETE )
	{
		// 成功
		return 0;
	}
	else
	{
		return -1;
	}

	return 0;
}

int	CloHttpTask::HttpClientDelete()
{
	if( !m_pAssoc->m_pHttpCurl )
		return -1;

	USES_CONVERSION;

	if(m_pAssoc->m_szURL == NULL || _tcslen(m_pAssoc->m_szURL) == 0 )
		return -1;


	TTaskParam param;
	param.nTransType = E_HTTPTYPE_DELETE;

	strncpy_s(param.cURL, 1024, CT2A(m_pAssoc->m_szURL), 1024);

	CString strParam = m_pAssoc->MakeGetParam();
	CStringA strParamA = CT2A(strParam);

	param.uDataLength = strParamA.GetLength();
	param.pData = new char[param.uDataLength + 1];
	strcpy_s(param.pData, param.uDataLength + 1, strParamA.GetBuffer());
	param.pData[param.uDataLength] = '\0';

	CString strHeader = m_pAssoc->GetHttpHeader();
	strcpy_s(param.cHead, 4096, CT2A(strHeader));
	m_pAssoc->m_pHttpCurl->SetTransferParam(E_HTTPTYPE_DELETE, (LPVOID)&param);
	strParam.ReleaseBuffer();
	strParamA.ReleaseBuffer();

	// 设置代理信息
	SetHttpProxyCfg();

	// 开始下载
	if( m_pAssoc->m_pHttpCurl->Request() != 0 )
		return -1;

	// 获取结果
	long lResult = m_pAssoc->m_pHttpCurl->GetResult();
	if( lResult == E_HTTPTRANS_RESULT_COMPLETE )
	{
		// 成功
		return 0;
	}
	else
	{
		return -1;
	}

	return 0;
}

int	CloHttpTask::HttpClientGetDownload()
{
	if( !m_pAssoc->m_pHttpCurl )
		return -1;

	USES_CONVERSION;

	if(m_pAssoc->m_szURL == NULL || _tcslen(m_pAssoc->m_szURL) == 0 )
		return -1;

	// 初始化HttpClient
	TTaskParam param;
	param.nTransType = E_HTTPTYPE_DOWNLOAD;

	CString strURL = m_pAssoc->MakeGetURL();
	CString strHeader = m_pAssoc->GetHttpHeader();
	strcpy_s(param.cURL, 1024, CT2A(strURL));
	strcpy_s(param.cHead, 4096, CT2A(strHeader));

	if( m_pAssoc->m_strPath.IsEmpty() || m_pAssoc->m_strFile.IsEmpty() )
	{// 进行缓冲区下载
		param.bToBuffer = TRUE;
	}
	else
	{
		param.bToBuffer = FALSE;
		strcpy_s(param.cSavePath,1024,CT2A(m_pAssoc->m_strPath));
		strcpy_s(param.cNewName,1024,CT2A(m_pAssoc->m_strFile));
		param.bRename = TRUE;
	}
	m_pAssoc->m_pHttpCurl->SetTransferParam(E_HTTPTYPE_DOWNLOAD, (LPVOID)&param);

	// 设置代理信息
	SetHttpProxyCfg();

	// 记录时间
	THttpTime dTime;
	_tcscpy_s(dTime.szURL, 1024, strURL);

	// 记录开始时间
	time_t now;
	time(&now);
	dTime.tRequest = now;

	// 开始下载
	if( m_pAssoc->m_pHttpCurl->Request() != 0 )
		return -1;	

	// 记录完成时间
	time(&now);
	dTime.tResponse = now;

	long lResult = m_pAssoc->m_pHttpCurl->GetResult();
	if( lResult == E_HTTPTRANS_RESULT_COMPLETE )
	{
		dTime.bSucceed = TRUE;
	}
	else
	{
		dTime.bSucceed = FALSE;
	}
	m_pAssoc->OnTaskTime(&dTime);
	return 0;
}

int CloHttpTask::HttpClientGet()
{
	if( !m_pAssoc->m_pHttpCurl )
		return -1;

	USES_CONVERSION;

	if(m_pAssoc->m_szURL == NULL || _tcslen(m_pAssoc->m_szURL) == 0 )
		return -1;

	// 初始化HttpClient
	TTaskParam param;
	param.nTransType = E_HTTPTYPE_GET;

	CString strURL = m_pAssoc->MakeGetURL();
	CString strHeader = m_pAssoc->GetHttpHeader();
	strcpy_s(param.cURL, 1024, CT2A(strURL));
	strcpy_s(param.cHead, 4096, CT2A(strHeader));

	m_pAssoc->m_pHttpCurl->SetTransferParam(E_HTTPTYPE_GET, (LPVOID)&param);

	// 设置代理信息
	SetHttpProxyCfg();

	// 记录时间
	THttpTime dTime;
	_tcscpy_s(dTime.szURL, 1024, strURL);

	// 记录开始时间
	time_t now;
	time(&now);
	dTime.tRequest = now;

	// 开始下载
	if( m_pAssoc->m_pHttpCurl->Request() != 0 )
		return -1;	

	// 记录完成时间
	time(&now);
	dTime.tResponse = now;

	long lResult = m_pAssoc->m_pHttpCurl->GetResult();
	if( lResult == E_HTTPTRANS_RESULT_COMPLETE )
	{
		dTime.bSucceed = TRUE;
	}
	else
	{
		dTime.bSucceed = FALSE;
	}
	m_pAssoc->OnTaskTime(&dTime);

	return 0;
}

int CloHttpTask::HttpClientPostBinaryText()
{
	if( !m_pAssoc->m_pHttpCurl )
		return -1;

	USES_CONVERSION;

	if(m_pAssoc->m_szURL == NULL || _tcslen(m_pAssoc->m_szURL) == 0 )
		return -1;

	TTaskParam param;
	param.nTransType = E_HTTPTYPE_POST;

	strcpy_s(param.cURL, 1024, CT2A(m_pAssoc->m_szURL));

	// 二进
	param.uDataLength = m_pAssoc->m_BinData.Length();
	param.pData = new char[ m_pAssoc->m_BinData.Length() ];
	memcpy(param.pData , m_pAssoc->m_BinData.Data() , m_pAssoc->m_BinData.Length() );

	// head
	strcpy_s(param.cLocalFile,1024,CT2A(m_pAssoc->m_strFile));
	CString strHeader = m_pAssoc->GetHttpHeader();
	strcpy_s(param.cHead, 4096, CT2A(strHeader));

	// param 
	param.bFormFormat = m_pAssoc->m_bPostForm;
	CString strParam = m_pAssoc->MakeGetParam();
	strcpy_s(param.cText,1024,CT2A(strParam));

	m_pAssoc->m_pHttpCurl->SetTransferParam(E_HTTPTYPE_POST_TEXT_FILE, (LPVOID)&param);

	// 设置代理信息
	SetHttpProxyCfg();

	// 开始下载
	if( m_pAssoc->m_pHttpCurl->Request() != 0 )
		return -1;

	// 获取结果
	long lResult = m_pAssoc->m_pHttpCurl->GetResult();
	if( lResult == E_HTTPTRANS_RESULT_COMPLETE )
	{
		// 成功
		return 0;
	}
	else
	{
		return -1;
	}

	return 0;
}

int	CloHttpTask::HttpClientPostBinary()
{
	if( !m_pAssoc->m_pHttpCurl )
		return -1;

	USES_CONVERSION;

	if(m_pAssoc->m_szURL == NULL || _tcslen(m_pAssoc->m_szURL) == 0 )
		return -1;

	TTaskParam param;
	param.nTransType = E_HTTPTYPE_POST;

	strcpy_s(param.cURL, 1024, CT2A(m_pAssoc->m_szURL));

	// 
	param.uDataLength = m_pAssoc->m_BinData.Length();
	param.pData = new char[ m_pAssoc->m_BinData.Length() ];
	memcpy(param.pData,m_pAssoc->m_BinData.Data() , m_pAssoc->m_BinData.Length() );

	// head
	CString strHeader = m_pAssoc->GetHttpHeader();
	strcpy_s(param.cHead, 4096, CT2A(strHeader));

	// param
	param.bFormFormat = m_pAssoc->m_bPostForm;
	m_pAssoc->m_pHttpCurl->SetTransferParam(E_HTTPTYPE_POST, (LPVOID)&param);

	// 设置代理信息
	SetHttpProxyCfg();

	// 开始下载
	if( m_pAssoc->m_pHttpCurl->Request() != 0 )
		return -1;

	// 获取结果
	long lResult = m_pAssoc->m_pHttpCurl->GetResult();
	if( lResult == E_HTTPTRANS_RESULT_COMPLETE )
	{
		// 成功
		return 0;
	}
	else
	{
		return -1;
	}

	return 0;
}

int CloHttpTask::HttpClientPost()
{
	if( !m_pAssoc->m_pHttpCurl )
		return -1;

	USES_CONVERSION;

	if(m_pAssoc->m_szURL == NULL || _tcslen(m_pAssoc->m_szURL) == 0 )
		return -1;

	TTaskParam param;
	param.nTransType = E_HTTPTYPE_POST;

	strcpy_s(param.cURL, 1024, CT2A(m_pAssoc->m_szURL));
	CString strParam = m_pAssoc->MakeGetParam();
	CStringA strParamA = CT2A(strParam);

	param.uDataLength = strParamA.GetLength();
	param.pData = new char[param.uDataLength + 1];
	strcpy_s(param.pData, param.uDataLength + 1, strParamA.GetBuffer());

	// head
	CString strHeader = m_pAssoc->GetHttpHeader();
	strcpy_s(param.cHead, 4096, CT2A(strHeader));

	// param
	param.bFormFormat = m_pAssoc->m_bPostForm;
	m_pAssoc->m_pHttpCurl->SetTransferParam(E_HTTPTYPE_POST, (LPVOID)&param);
	strParam.ReleaseBuffer();
	strParamA.ReleaseBuffer();

	// 设置代理信息
	SetHttpProxyCfg();

	// 开始下载
	if( m_pAssoc->m_pHttpCurl->Request() != 0 )
		return -1;

	// 获取结果
	long lResult = m_pAssoc->m_pHttpCurl->GetResult();
	if( lResult == E_HTTPTRANS_RESULT_COMPLETE )
	{
		// 成功
		return 0;
	}
	else
	{
		return -1;
	}

	return 0;
}

int CloHttpTask::HttpClientUpload()
{
	if( !m_pAssoc->m_pHttpCurl )
		return -1;

	USES_CONVERSION;

	if(m_pAssoc->m_szURL == NULL || _tcslen(m_pAssoc->m_szURL) == 0 )
		return -1;

	TTaskParam param;
	param.nTransType = E_HTTPTYPE_UPLOAD;

	strcpy_s(param.cURL, 1024, CT2A(m_pAssoc->m_szURL));
	CString strHeader = m_pAssoc->GetHttpHeader();
	strcpy_s(param.cHead, 4096, CT2A(strHeader));

	int nDataLength = strlen(CT2A(m_pAssoc->m_szUploadParam));
	param.pData = new char[nDataLength + 1];
	param.uDataLength = nDataLength;
	strcpy_s(param.pData, nDataLength, CT2A(m_pAssoc->m_szUploadParam));
	strcpy_s(param.cLocalFile, 1024, CT2A(m_pAssoc->m_szUploadFile));
	m_pAssoc->m_pHttpCurl->SetTransferParam(E_HTTPTYPE_POST_FILE, (LPVOID)&param);

	// 设置代理信息
	SetHttpProxyCfg();

	// 开始下载
	if( m_pAssoc->m_pHttpCurl->Request() != 0 )
		return -1;

	// 获取结果
	long lResult = m_pAssoc->m_pHttpCurl->GetResult();
	if( lResult == E_HTTPTRANS_RESULT_COMPLETE )
	{
		// 成功
		return 0;
	}
	else
	{
		return -1;
	}

	return 0;
}

void CloHttpTask::SetHttpProxyCfg()
{
	if( !m_pAssoc->m_pHttpCurl )
		return;

	if( m_pAssoc->m_nProxyType == E_PROXYTYPE_SOCKS5 || m_pAssoc->m_nProxyType == E_PROXYTYPE_HTTP )
	{
		THttpProxyInfo proxyInfo;
		strcpy_s(proxyInfo.cServer, 1024, CT2A(m_pAssoc->m_szProxySrvAddr) );
		proxyInfo.wPort = m_pAssoc->m_nProxySrvPort;

		strcpy_s(proxyInfo.cUser    , 1024, CT2A(m_pAssoc->m_szProxyUserName) );
		strcpy_s(proxyInfo.cPassword, 1024, CT2A(m_pAssoc->m_szProxyUserPwd) );
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

int CloHttpResponse::GetHeadercount(LPCTSTR szName){
	return m_pAssoc->m_mapHeader.Count(szName);
}

LPCTSTR CloHttpResponse::GetHeader(LPCTSTR szName, unsigned long nIdx/* = 0*/)
{
	try
	{
		if( !m_pAssoc->m_mapHeader.Exists(szName, nIdx) )
			return NULL;

		return m_pAssoc->m_mapHeader.Get(szName,nIdx).szValue;
	}
	catch(...)
	{
		return NULL;
	}
	return NULL;
}

BOOL CloHttpResponse::GetContentLength (unsigned long & cbContLen)
{
	LPCTSTR szValue = GetHeader(_T("Content-Length"),0);
	if( szValue )
	{
		cbContLen = (unsigned long)( _ttol(szValue) );
		return TRUE;
	}
	return FALSE;
}

unsigned long CloHttpResponse::ReadBody (BYTE * pbyBuff, unsigned long cbBuff)
{
	if( m_pAssoc->m_httpBody.buffer != NULL && m_pAssoc->m_httpBody.size > 0 )
	{
		if( m_pAssoc->m_httpBody.offset >= m_pAssoc->m_httpBody.size )
			return 0;

		unsigned long copyDataLength = (unsigned long)(m_pAssoc->m_httpBody.size - m_pAssoc->m_httpBody.offset);
		if( cbBuff < copyDataLength )
			copyDataLength = cbBuff;

		memcpy(pbyBuff, m_pAssoc->m_httpBody.buffer + m_pAssoc->m_httpBody.offset, copyDataLength);

		m_pAssoc->m_httpBody.offset += copyDataLength;

		return copyDataLength;
	}
	return 0;
}

unsigned long CloHttpResponse::ReadAllBody(BYTE * pbyBuff, unsigned long cbBuff)
{
	if( m_pAssoc->m_httpBody.buffer != NULL && m_pAssoc->m_httpBody.size > 0 )
	{
		if( cbBuff < m_pAssoc->m_httpBody.size )
			return 0;

		memcpy(pbyBuff, m_pAssoc->m_httpBody.buffer, m_pAssoc->m_httpBody.size);

		return m_pAssoc->m_httpBody.size;
	}

	return 0;
}

void CloHttpResponse::ResetBody(){
	m_pAssoc->m_httpBody.offset = 0;
}

BOOL CloHttpResponse::GetBodyLength(unsigned long & dwBodyLen)
{
	if( m_pAssoc->m_httpBody.buffer != NULL && m_pAssoc->m_httpBody.size > 0 )
	{
		dwBodyLen = m_pAssoc->m_httpBody.size;
		return TRUE;
	}
	return FALSE;
}

LPCTSTR	CloHttpResponse::ReadResultHeader()
{
	return (LPCTSTR)m_pAssoc->m_httpState;
}