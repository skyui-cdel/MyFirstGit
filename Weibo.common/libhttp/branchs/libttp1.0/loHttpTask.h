#ifndef __lo_HTTPTASK_H__
#define __lo_HTTPTASK_H__

#include <libhttp/loHttpDef.h>

namespace lohttp
{	
	class CloHttpResponse;
	class CloHttpTaskAssoc;
	class CloHttpResponseAssoc;

	/** һ���������� �߳�����ʽ��
	 * @author loach
	*/
	class loHTTP_API CloHttpTask
	{
	public:
		CloHttpTask(IloTaskCallBack* pTaskCallBack);
		~CloHttpTask(void);	

	public:
		// ��Ӳ���ֵ
		void AddParam(LPCTSTR szName , LPCTSTR szValue = NULL, unsigned long dwParamAttr = lohttp::ParamNormal);
		void AddParam(LPCTSTR szName , BOOL bUrlEncode, BOOL bUTF8, LPCTSTR szValue = NULL, unsigned long dwParamAttr = lohttp::ParamNormal);
		void ClearParams();

		// ���ͷ��Ϣ
		void AddHeader(LPCTSTR szName, LPCTSTR szValue = NULL);
		void ClearHeader();

		// ��Ӷ���������
		void AddBinaryData(const char* szBinaryData,long lLen);

		// �ļ��������ر��棬�����ϴ��ļ���
		void SetFileName(LPCTSTR szPath,LPCTSTR szFile);

		// ����
		void SetProxySrvAddr(int proxyType, LPCTSTR szProxySrv, unsigned short nProxySrvPort, LPCTSTR lpszDownloadUrl);
		void SetProxyAccount(LPCTSTR szUserName = NULL, LPCTSTR szPassword = NULL);
		void ClearProxy();

		//
		void SetPostFormat(BOOL bForm = FALSE);
		void SetUploadParam(LPCTSTR lpszParamName, LPCTSTR lpszFile);

	    // E_HTTP_GET_METHOD
		CloHttpResponse* RequestGet(LPCTSTR szUrl);

		//E_HTTP_GET_BUFFER_METHOD
		CloHttpResponse* RequestBuffer(LPCTSTR szUrl);

		//E_HTTP_DELETE_METHOD
		CloHttpResponse* BeginDelete(LPCTSTR szUrl);

		//E_HTTP_PUT_METHOD
		CloHttpResponse* BeginPut(LPCTSTR szUrl);

		// E_HTTP_POST_METHOD
		CloHttpResponse* BeginPost(LPCTSTR szUrl);

		//E_HTTP_POST_BINARY_METHOD
		CloHttpResponse* BeginPostBinary(LPCTSTR szUrl);

		CloHttpResponse* BeginUpload(LPCTSTR szUrl);

		//E_HTTP_UPLOAD_BINARY_TEXT_METHOD
		CloHttpResponse* BeginPostBinaryText(LPCTSTR szUrl);

		// E_HTTP_DOWNLOAD_FILE
		CloHttpResponse* BeginDownloadFile(LPCTSTR szUrl);

		// �Ͽ�
		BOOL Cancel (void);
	private:
		int	 HttpClientGet();

		int	 HttpClientGetDownload();
		int	 HttpClientDelete();
		int	 HttpClientPut();
		int	 HttpClientPost();
		int	 HttpClientUpload();
		int	 HttpClientPostBinary();
		int	 HttpClientPostBinaryText();

		/** ���ô�����Ϣ */
		void SetHttpProxyCfg();

		CloHttpResponse* CreateHttpResponse(void);
	protected:
		CloHttpTaskAssoc* m_pAssoc;
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

		/** http header information */
		int GetHeadercount(LPCTSTR szName);
		LPCTSTR	GetHeader(LPCTSTR szName, unsigned long nIdx = 0);

		/** ��ȡ������ " 200 OK ��������Ϣ "*/
		LPCTSTR	ReadResultHeader();

		/** "Content-Length" �ֶεĳ���
		 * @return ���� ��Content-Length��������true
		*/
		BOOL GetContentLength (unsigned long & cbContLen);

		/** body length */
		BOOL GetBodyLength(unsigned long & dwBodyLen);

		/** ��λ��ȡ������ƫ�� ��ֱ����������ȡ��� */
		unsigned long ReadBody(BYTE * pbyBuff, unsigned long cbBuff);

		/** ��λ����ͷ��ʼ */
		void ResetBody();

		/** һ�ζ�ȡ����body */
		unsigned long ReadAllBody(BYTE * pbyBuff, unsigned long cbBuff);		

	private:
		CloHttpResponseAssoc* m_pAssoc;
		friend class CloHttpTask;
	};

}//namespace lohttp

#endif //__lo_HTTPTASK_H__