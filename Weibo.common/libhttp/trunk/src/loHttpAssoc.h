#pragma once

#include <list>
#include "loHttpDef.h"
#include "loHttpMap.h"
#include "loHttpStr.h"
#include "loHttpStruct.h"
#include "loHttpCurl.h"
#include "loBytebuffer.h"
#include "loHttpThread.h"


namespace lohttp
{
#ifndef BUILD_CHARACTER_SET

	typedef lohttp::CloHttpMapT<lohttp::CloHttpStrA> CHttpMap;

#else

#ifdef UNICODE
	typedef lohttp::CloHttpMapT<lohttp::CloHttpStrW> CHttpMap;
#else
	typedef lohttp::CloHttpMapT<lohttp::CloHttpStrA> CHttpMap;
#endif

#endif // BUILD_CHARACTER_SET

	class CloHttpResponse;

	// �ص������ṹ��
	struct t_lo_httpCB
	{
		/*���ش��ļ�(�ڴ滺����) �ص��ӿڣ۱������� ����OnHttpBody������ͬ �� */
		fun_httpbuffer_callback fbuffer_cb;

		/* ����״̬ */
		fun_httpprogress_callback fprogress_cb;

		/** ����״̬
		*
		* @param state ״̬(�ɹ���ʧ�ܣ�
		* @param errCode ʧ����
		*/
		fun_httpcode_callback fcode_cb;

		/** */
		fun_httprespone_callback frespone_cb;

		/** CloHttp ��Դ�ͷŵ� */
		fun_httprelease_callback frelease_cb;
	};

	class CloHttpAssoc : public IloTaskCallBack
	{
	public:
		CloHttpAssoc(void);
		~CloHttpAssoc();

		void ClearParams();
		void ClearHeader();
		void ClearProxy();

		std::string MakeGetParam();
		std::string MakeGetURL();
		std::string GetHttpHeader();

		/// �����߳�
		static THREAD_RETURN_TYPE STDCALLTYPE HttpThread(void* pParam);


		/// IloTaskCallBack
	private:
		void  OnTaskState(long lState);
		int   OnTaskProgress(long lCurBytes, long lTotalBytes);
		int   OnTaskBuffer(unsigned char* pBuffer, long lBuffer);
		void  OnTaskCode(long code, const char* errString);
		void  OnTaskTime(const THttpTime * pDownloadTime) ;

	private:
		void  OnStart();
		void  OnFinished(CloHttpResponse* resp);
	private:
		// Զ������ĵ�ַ
		HTTPChar   m_szURL[ConstURLLength];

		// ������Ϣ
		HTTPChar   m_szProxyUserName[ConstProxyUserLength];
		HTTPChar   m_szProxyUserPwd[ConstProxyPwdLength];
		HTTPChar   m_szProxySrvAddr[ConstProxyServerLength];
		int     m_nProxyType,m_nProxySrvPort;

		bool    m_bFormData;

		//// POST�ļ�ʱ����ͷ�Ĳ���
		//HTTPChar	m_szUploadParam[ConstUploadParamLength];
		//// POST���ص��ļ�
		//HTTPChar	m_szUploadFile[MAX_PATH+1];

		// �ļ�
		HTTPChar    m_szFile[MAX_PATH+1];
		HTTPChar    m_szPath[MAX_PATH+1];

		// ��¼������Ϣ
		CloHttpCurl* m_pHttpCurl;

		// �߳̾��
		THREAD_HANDLE  m_hInetTh;

		// ���󷽷�
		int     m_nMethod;

		// ��Ӧ��(�����첽����
		CloHttpResponse* m_pRespone;

		void* m_pUserData;
		struct t_lo_httpCB m_cb;

		// for http form data
		struct THttpForm m_Httpform;

		// ������
		int m_ierrCode;

		THttpTime m_time;

		// ����������
		locom::CloByteBuffer m_binData;

		// �������
		std::list<THttpURLParam*> m_lstParam;

		// �����Http Header
		std::list<THttpHeader* > m_lstHeader;

		friend class CloHttpCurl;
		friend class CloHttp;
	};

	class CloHttpResponseAssoc
	{
	public:
		CloHttpResponseAssoc();
		~CloHttpResponseAssoc();

		void SetHttpResponse(THttpMemoryBuffer * pHeader, THttpMemoryBuffer * pBody);

		void SetHttpCode( long httpcode ) {
			m_lhttpcode = httpcode;
		}
		void ClearHttpResponse();
	private:
		// ���صľ�������
		THttpMemoryBuffer m_httpHeader;
		THttpMemoryBuffer m_httpBody;

		long m_lhttpcode;

		friend class CloHttpResponse;
	};

}//namespace lohttp
