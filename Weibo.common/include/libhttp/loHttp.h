#ifndef __lo_HTTP_H__
#define __lo_HTTP_H__

#include <libhttp/loHttpDef.h>

namespace lohttp
{	
	class CloHttpResponse;
	class CloHttpAssoc;
	class CloHttpResponseAssoc;

	/** һ���������� �߳�����ʽ��
	 * @author loach
	*/
	class loHTTP_API CloHttp
	{
	public:
		CloHttp();
		~CloHttp(void);	
	public:		
		/*���ش��ļ�(�ڴ滺����) �ص��ӿڣ۱������У� */
		void SetCallbackOfBuffer(fun_httpbuffer_callback fbuffer_cb);
		
		/* ����״̬ */
		void SetCallbackOfProgress(fun_httpprogress_callback fprogress_cb);

		/** ����״̬
		*
		* @param state ״̬(�ɹ���ʧ�ܣ�
		* @param errCode ʧ����
		*/
		void SetCallbackOfCode(fun_httpcode_callback fcode_cb);

		void SetCallbackOfRespone(fun_httprespone_callback fresp_cb);

		/** CloHttp ��Դ�ͷŵ� */
		void SetCallbackOfRelease(fun_httprelease_callback frelease_cb);

		/** �����Զ������� */
		void SetUserData(void* puserdata);

		// ����url
		void SetURL( const HTTPChar* szURL );

		// ��Ӳ���ֵ
		void AddParam(const HTTPChar* szName , const HTTPChar* szValue , unsigned long dwParamAttr = lohttp::ParamNormal);
		void ClearParams();

		// ���ͷ��Ϣ
		void AddHeader(const HTTPChar* szName, const HTTPChar* szValue);
		void ClearHeader();

		// ��Ӷ���������
		void AddBinaryData(const char* szBinaryData,long lLen);
		void ClearBinaryData(void);

		// �ļ��������ر��棬�����ϴ��ļ���
		void SetFileName(const HTTPChar* szPath , const HTTPChar* szFile);

		// ����
		void SetProxySrvAddr(int proxyType, const HTTPChar* szProxySrv, unsigned short nProxySrvPort);
		void SetProxyAccount(const HTTPChar* szUserName = NULL, const HTTPChar* szPassword = NULL);
		void ClearProxy();

		//
		void EnableFormData(bool bForm = false);
//		void SetUploadParam(const HTTPChar* lpszParamName, const HTTPChar* lpszFile);

		void SetHttpForm(const struct THttpForm* pform);
		//
		void SetUserAgent(const HTTPChar* szAgent);

		// ����(ͬ��������һ���߳���ɣ��������̣߳�
		int  Request(const HTTPChar* szUrl , int method);

		// �첽���������̣߳�
		int  RequestAsyn(const HTTPChar* szUrl , int method);
		void Wait();

		// �Ͽ�
		bool Cancel(unsigned long msTimeout = 1000 );

		// ��ȡ����Ӧ��
		CloHttpResponse* GetRespone(void) const;

		// ���� ��Ӧ��
		CloHttpResponse* DetachRespone(void);

		// ����ʱ�䣨����ȷ )
		THttpTime* GetHttpTime() const;

		// http method
	private:
        /** init url */
		void InitURL( const HTTPChar* szUrl,int method );

		/** donwload to header AND body */
		int	 HttpMethodGet();

		/** ���ص�buffer �� �ļ��� */
		int	 HttpMethodDownload();

		/** delete http */
		int	 HttpMethodDelete();

		/** put http */
		int	 HttpMethodPut();

		/** post һ������� */
		int	 HttpMethodPost();

		/** �ϴ����ݣ����ԣ��ļ������� �� ���ֻ��� */
		int	 HttpMethodUpload();

		/** */
//		int	 HttpMethodPostBinary();
//		int	 HttpMethodPostBinaryText();

		int  HttpMethod(int method);

		/** ���ô�����Ϣ */
		void SetHttpProxyCfg();

		// ����һ�� respone��
		CloHttpResponse* CreateHttpResponse(void);

		// �첽���߳̿�ʼ)
		unsigned long StartAsyn();
	protected:
		CloHttpAssoc* m_pAssoc;
		friend class CloHttpAssoc;
	};

	/** �����Ӧ���
	 *
	 * @author loach
	*/
	class loHTTP_API CloHttpResponse
	{
	public:
		CloHttpResponse(void);
		~CloHttpResponse(void);
	public:
		// ͷ��Ϣ����
		bool GetHeaderLength(unsigned long& len) const;

		// ͷ��Ϣ����
		unsigned long ReadHeader(char * pbyBuff, unsigned long cbBuff , unsigned long& offsize) const;
		bool ReadHeaderCallback(fun_http_resp_header_callback cb , void* pUsrData) const;

		/** body length */
		bool GetBodyLength(unsigned long & dwBodyLen) const;

		/** ��λ��ȡ������ƫ�� ��ֱ����������ȡ��� 
		 *
		 * @offsize ��ȡ��ƫ����,offsize >= �����Сʱ ��return 0;
		 * @return ����ʵ�ʶ�ȡ�Ĵ�С�� = 0 ����ʾ��ȡ���
		*/
		unsigned long ReadBody(char * pbyBuff, unsigned long cbBuff , unsigned long& offsize) const;
	    bool ReadBodyCallback(fun_http_resp_body_callback cb , void* pUsrData) const;

		// 200 , 401 , 504 ...
		long GetHttpCode(void) const;

	private:
		CloHttpResponseAssoc* m_pAssoc;
		friend class CloHttp;
	};

	/** http header ��Ϣ */
	class loHTTP_API CloHttpHeader
	{
	public:
		CloHttpHeader();
		~CloHttpHeader();

		/** */
		void Parse(const char* header , unsigned long len);

		/** http header information */
		int GetHeadercount(const HTTPChar* szName) const;
		const HTTPChar*	GetHeader(const HTTPChar* szName, unsigned long nIdx = 0) const;

		/** ��ȡ������ " 200 OK ��������Ϣ "*/
		const HTTPChar*	GetHeaderState() const;

        bool GetContentLength (unsigned long & cbContLen) const;
	private:
		void* m_pAssoc;

		// 200 OK ,
		HTTPChar m_httpState[64];
	};

}//namespace lohttp

#endif //__lo_HTTP_H__
