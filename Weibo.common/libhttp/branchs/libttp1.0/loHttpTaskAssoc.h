#pragma once

#include <list>
#include "loHttpDef.h"
#include "loHttpMap.h"
#include "loHttpStr.h"
#include "loHttpStruct.h"
#include "common/loByteBuffer.h"

namespace lohttp
{
#ifdef UNICODE
	typedef lohttp::CloHttpMapT<lohttp::CloHttpStrW> CHttpMap;
#else
	typedef lohttp::CloHttpMapT<lohttp::CloHttpStrA> CHttpMap;
#endif

	class CloHttpCurl;

	class CloHttpTaskAssoc : public IloTaskCallBack
	{
	public:
		CloHttpTaskAssoc(IloTaskCallBack* pTaskCallback);
		~CloHttpTaskAssoc();

		void ClearParams();
		void ClearHeader();
		void ClearProxy();

		CString	MakeGetURL();
		CString	MakeGetParam();
		CString	GetHttpHeader();
		/// IloTaskCallBack
	private:
		void  OnTaskState(long lState);
		int   OnTaskProgress(long lCurBytes, long lTotalBytes);
		int   OnTaskBuffer(BYTE* pBuffer, long lBuffer);
		void  OnTaskCode(long code, const char* errString);
		void  OnTaskTime(const THttpTime * pDownloadTime) ;
	private:
		// 远程请求的地址
		TCHAR   m_szURL[1024];

		// 代理信息
		TCHAR   m_szProxyUserName[256];
		TCHAR   m_szProxyUserPwd[256];
		TCHAR   m_szProxySrvAddr[256];
		int     m_nProxyType,m_nProxySrvPort;

		BOOL    m_bPostForm;
		// POST文件时，表头的参数
		TCHAR	m_szUploadParam[1024];
		// POST本地的文件
		TCHAR	m_szUploadFile[1024];

		// 二进制数据
		locom::CloByteBuffer m_BinData;

		// 文件
		CString	 m_strFile;
		CString  m_strPath;

		// 请求参数
		std::list<THttpURLParam*> m_lstParam;

		// 请求的Http Header
		std::list<THttpHeader* > m_lstHeader;

		// 记录传输信息
		IloTaskCallBack* m_pTaskCallback;
		CloHttpCurl* m_pHttpCurl;

		friend class CloHttpCurl;
		friend class CloHttpTask;
	};

	class CloHttpResponseAssoc
	{
	public:
		CloHttpResponseAssoc();
		~CloHttpResponseAssoc();

		void SetHttpResponse(THttpMemoryBuffer * pHeader, THttpMemoryBuffer * pBody);

		void ClearHttpResponse();

		void ClearHeader();

		void BuildHeader();
	private:
		// 返回的具体数据
		THttpMemoryBuffer m_httpHeader;
		THttpMemoryBuffer m_httpBody;

		CHttpMap m_mapHeader; ///< 返回的Http Header
		CString  m_httpState; ///< 返回的http 状态

		CString	m_strResultHeader; ///< 返回结果	

		friend class CloHttpResponse;
	};

}//namespace lohttp