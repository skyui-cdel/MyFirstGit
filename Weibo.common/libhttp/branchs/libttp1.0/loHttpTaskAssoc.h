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
		// Զ������ĵ�ַ
		TCHAR   m_szURL[1024];

		// ������Ϣ
		TCHAR   m_szProxyUserName[256];
		TCHAR   m_szProxyUserPwd[256];
		TCHAR   m_szProxySrvAddr[256];
		int     m_nProxyType,m_nProxySrvPort;

		BOOL    m_bPostForm;
		// POST�ļ�ʱ����ͷ�Ĳ���
		TCHAR	m_szUploadParam[1024];
		// POST���ص��ļ�
		TCHAR	m_szUploadFile[1024];

		// ����������
		locom::CloByteBuffer m_BinData;

		// �ļ�
		CString	 m_strFile;
		CString  m_strPath;

		// �������
		std::list<THttpURLParam*> m_lstParam;

		// �����Http Header
		std::list<THttpHeader* > m_lstHeader;

		// ��¼������Ϣ
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
		// ���صľ�������
		THttpMemoryBuffer m_httpHeader;
		THttpMemoryBuffer m_httpBody;

		CHttpMap m_mapHeader; ///< ���ص�Http Header
		CString  m_httpState; ///< ���ص�http ״̬

		CString	m_strResultHeader; ///< ���ؽ��	

		friend class CloHttpResponse;
	};

}//namespace lohttp