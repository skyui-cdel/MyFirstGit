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

	// 回调函数结构体
	struct t_lo_httpCB
	{
		/*下载大文件(内存缓冲区) 回调接口［边下载中 ，与OnHttpBody有所不同 ］ */
		fun_httpbuffer_callback fbuffer_cb;

		/* 进度状态 */
		fun_httpprogress_callback fprogress_cb;

		/** 下载状态
		*
		* @param state 状态(成功，失败）
		* @param errCode 失败码
		*/
		fun_httpcode_callback fcode_cb;

		/** */
		fun_httprespone_callback frespone_cb;

		/** CloHttp 资源释放掉 */
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

		/// 下载线程
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
		// 远程请求的地址
		HTTPChar   m_szURL[ConstURLLength];

		// 代理信息
		HTTPChar   m_szProxyUserName[ConstProxyUserLength];
		HTTPChar   m_szProxyUserPwd[ConstProxyPwdLength];
		HTTPChar   m_szProxySrvAddr[ConstProxyServerLength];
		int     m_nProxyType,m_nProxySrvPort;

		bool    m_bFormData;

		//// POST文件时，表头的参数
		//HTTPChar	m_szUploadParam[ConstUploadParamLength];
		//// POST本地的文件
		//HTTPChar	m_szUploadFile[MAX_PATH+1];

		// 文件
		HTTPChar    m_szFile[MAX_PATH+1];
		HTTPChar    m_szPath[MAX_PATH+1];

		// 记录传输信息
		CloHttpCurl* m_pHttpCurl;

		// 线程句柄
		THREAD_HANDLE  m_hInetTh;

		// 请求方法
		int     m_nMethod;

		// 回应包(用在异步请求）
		CloHttpResponse* m_pRespone;

		void* m_pUserData;
		struct t_lo_httpCB m_cb;

		// for http form data
		struct THttpForm m_Httpform;

		// 错误码
		int m_ierrCode;

		THttpTime m_time;

		// 二进制数据
		locom::CloByteBuffer m_binData;

		// 请求参数
		std::list<THttpURLParam*> m_lstParam;

		// 请求的Http Header
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
		// 返回的具体数据
		THttpMemoryBuffer m_httpHeader;
		THttpMemoryBuffer m_httpBody;

		long m_lhttpcode;

		friend class CloHttpResponse;
	};

}//namespace lohttp
