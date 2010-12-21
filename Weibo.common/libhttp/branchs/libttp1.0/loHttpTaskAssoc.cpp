#include "StdAfx.h"
#include "loHttpTaskAssoc.h"
#include "loHttpCurl.h"
#include "loUrlcode.h"
#include <time.h>

using namespace lohttp;


CloHttpTaskAssoc::CloHttpTaskAssoc(IloTaskCallBack* pTaskCallback)
: m_pTaskCallback(pTaskCallback)
, m_pHttpCurl( new CloHttpCurl(this) )
{
	m_bPostForm = FALSE;

	memset(m_szURL, 0, sizeof(TCHAR)*1024 );
	memset(m_szUploadParam, 0, sizeof(TCHAR)*1024 );
	memset(m_szUploadFile, 0, sizeof(TCHAR)*1024 );

	ClearProxy();
}

CloHttpTaskAssoc::~CloHttpTaskAssoc(void)
{
	ClearParams();
	ClearHeader();

	SAFE_DELETE( m_pHttpCurl );
}

void CloHttpTaskAssoc::OnTaskState(long lState)
{
	if( !m_pTaskCallback || !m_pHttpCurl )
		return ;

	m_pTaskCallback->OnTaskState(lState);

	return ;
}

int CloHttpTaskAssoc::OnTaskProgress(long lCurBytes, long lTotalBytes)
{
	if( m_pTaskCallback )
	{
		return m_pTaskCallback->OnTaskProgress(lCurBytes,lTotalBytes);
	}
	return 0;
}

int CloHttpTaskAssoc::OnTaskBuffer(BYTE* pBuffer, long lBuffer)
{
	if( m_pTaskCallback )
		return m_pTaskCallback->OnTaskBuffer(pBuffer,lBuffer);

	return 0;
}

void CloHttpTaskAssoc::OnTaskCode(long code, const char* errString)
{
	if( m_pTaskCallback )
		m_pTaskCallback->OnTaskCode(code ,errString);

	return ;
}

void CloHttpTaskAssoc::OnTaskTime(const THttpTime * pDownloadTime)
{
	if( m_pTaskCallback )
		m_pTaskCallback->OnTaskTime(pDownloadTime);
}

void CloHttpTaskAssoc::ClearParams()
{
	std::list<THttpURLParam *>::iterator itemIter;
	for( itemIter = m_lstParam.begin(); itemIter != m_lstParam.end(); itemIter ++ )
	{
		THttpURLParam * param = *itemIter;
		SAFE_DELETE(param);
	}
	m_lstParam.clear();
}

void CloHttpTaskAssoc::ClearHeader()
{
	std::list<THttpHeader *>::iterator itemIter;
	for( itemIter = m_lstHeader.begin(); itemIter != m_lstHeader.end(); itemIter ++ )
	{
		THttpHeader * header = *itemIter;
		SAFE_DELETE(header);
	}
	m_lstHeader.clear();
}

void CloHttpTaskAssoc::ClearProxy()
{
	m_nProxyType = E_PROXYTYPE_NONE;

	memset(m_szProxySrvAddr, 0, sizeof(TCHAR)*1024);
	m_nProxySrvPort = 808;

	memset(m_szProxyUserName, 0, sizeof(TCHAR)*255);
	memset(m_szProxyUserPwd, 0, sizeof(TCHAR)*255);
}

CString CloHttpTaskAssoc::MakeGetURL()
{
	CString strURL = m_szURL;

	std::list<THttpURLParam *>::iterator itemIter;
	for( itemIter = m_lstParam.begin(); itemIter != m_lstParam.end(); itemIter ++ )
	{
		THttpURLParam * param = *itemIter;
		CString strName, strValue;
		strName = param->pszName;
		strValue = param->pszValue;
		if( (param->dwAttrib &  lohttp::ParamEncodedName) )
		{
#ifdef _UNICODE
			std::wstring ecout;
			std::string ecname = CT2A(param->pszName);
			CloURLCode::UrlEncode( ecout , ecname.c_str() , ecname.length() );
#else
			std::string ecout;
			std::string ecname = CT2A(param->pszName);
			CloURLCode::UrlEncodeA( ecout , ecname.c_str() , ecname.length() );
#endif
			strName = ecout.c_str();
		}

		if( (param->dwAttrib & lohttp::ParamEncodedValue) )
		{
#ifdef _UNICODE
			std::wstring ecout;
			std::string ecname = CT2A(param->pszValue);
			CloURLCode::UrlEncode( ecout , ecname.c_str() , ecname.length() );
#else
			std::string ecout;
			std::string ecname = CT2A(param->pszValue);
			CloURLCode::UrlEncodeA( ecout , ecname.c_str() , ecname.length() );
#endif
			strValue = ecout.c_str();
		}
		else if( (param->dwAttrib & lohttp::ParamFile) )
		{
			continue;
		}

		if( itemIter == m_lstParam.begin() && strURL.Find(_T("?")) < 0 )
		{
			strURL += _T("?");
		}
		else if( strURL.Right(strURL.GetLength() - 1) != '/' &&
			strURL.Right(strURL.GetLength() - 1) != '?' )
		{
			strURL += _T("&");	
		}

		strURL += strName;
		strURL += _T("=");
		strURL += strValue;
	}

	return strURL;
}

CString CloHttpTaskAssoc::GetHttpHeader()
{
	CString strRet;	
	std::list<THttpHeader *>::iterator itemIter;
	for( itemIter = m_lstHeader.begin(); itemIter != m_lstHeader.end(); itemIter ++ )
	{
		THttpHeader * header = *itemIter;
		strRet += header->pszName;
		strRet += _T(": ");
		strRet += header->pszValue;
		strRet += _T("\r\n");
	}
	return strRet;
}

CString	CloHttpTaskAssoc::MakeGetParam()
{
	CString strParam;
	int nIndex = 0;
	std::list<THttpURLParam *>::iterator itemIter;
	for( itemIter = m_lstParam.begin(); itemIter != m_lstParam.end(); itemIter ++,nIndex ++ )
	{
		THttpURLParam * param = *itemIter;
		CString strName, strValue;
		strName = param->pszName;
		strValue = param->pszValue;
		if( (param->dwAttrib &  lohttp::ParamEncodedName) )
		{
#ifdef _UNICODE
			std::wstring ecout;
			std::string ecname = CT2A(param->pszName);
			CloURLCode::UrlEncode( ecout , ecname.c_str() , ecname.length() );
#else
			std::string ecout;
			std::string ecname = CT2A(param->pszName);
			CloURLCode::UrlEncodeA( ecout , ecname.c_str() , ecname.length() );
#endif
			strName = ecout.c_str();
		}

		if( (param->dwAttrib & lohttp::ParamEncodedValue) )
		{
#ifdef _UNICODE
			std::wstring ecout;
			std::string ecname = CT2A(param->pszValue);
			CloURLCode::UrlEncode( ecout , ecname.c_str() , ecname.length() );
#else
			std::string ecout;
			std::string ecname = CT2A(param->pszValue);
			CloURLCode::UrlEncodeA( ecout , ecname.c_str() , ecname.length() );
#endif
			strValue = ecout.c_str();
		}
		else if( (param->dwAttrib & lohttp::ParamFile) )
		{
			continue;
		}

		if( strName.IsEmpty() )
		{
			strParam = strValue;
			break;
		}
		else
		{
			if( nIndex != 0 )
				strParam += "&";
			strParam += strName;
			strParam += "=";
			strParam +=  strValue;
		}
	}
	return strParam;
}
//-------------------------------------------------------------------------------//
// CloHttpResponseAssoc

CloHttpResponseAssoc::CloHttpResponseAssoc(void)
{
}

CloHttpResponseAssoc::~CloHttpResponseAssoc(void)
{
	ClearHeader();
	ClearHttpResponse();
}

void CloHttpResponseAssoc::SetHttpResponse(THttpMemoryBuffer * pHeader, THttpMemoryBuffer * pBody)
{
	// modified by loach 2009-07-27
	SAFE_NEW_MEMORYBUFFER(&m_httpHeader,pHeader->size+1,pHeader->size);
	COPY_MEMORYBUFFER(pHeader,&m_httpHeader);
	m_httpHeader.buffer[pHeader->size] = '\0';

	SAFE_NEW_MEMORYBUFFER(&m_httpBody,pBody->size+1,pBody->size);
	COPY_MEMORYBUFFER(pBody,&m_httpBody);
	m_httpBody.buffer[pBody->size] = '\0';

	BuildHeader();
}

void CloHttpResponseAssoc::ClearHttpResponse()
{
	SAFE_DELETE_MEMORYBUFFER(&m_httpHeader);
	SAFE_DELETE_MEMORYBUFFER(&m_httpBody);	
}

void CloHttpResponseAssoc::ClearHeader()
{
	m_mapHeader.Clear();
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

BOOL gfun_Header_line_parse(char* szLine, 
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
		return TRUE;
	}

	// 不对
	return FALSE;	
}

void CloHttpResponseAssoc::BuildHeader()
{
	ClearHeader();

	/*
	解释头如下格式,它们之间"\r\n" 分行
	Exmaple:

	HTTP/1.1 200 OK
	Server: Apache-Coyote/1.1
	Content-Type: text/html;charset=GBK
	Content-Length: 406
	Date: Tue, 28 Jul 2009 07:05:31 GMT

	*/
	BOOL bHttpState = TRUE;
	char* szBegin = m_httpHeader.buffer;
	char* szEnd   = m_httpHeader.buffer + m_httpHeader.size;
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
				m_httpState = CA2T( szxx );
				bHttpState = FALSE;				
			}
			else
			{
				THttpHeaderValue Key,Value;
				if( gfun_Header_line_parse(szLine , szBegin , Key , Value) ){
					// 加入
					m_mapHeader.Add( CA2T(Key.szVal), CA2T(Value.szVal) );
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
}