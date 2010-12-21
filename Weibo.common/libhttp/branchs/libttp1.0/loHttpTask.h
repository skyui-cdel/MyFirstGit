#ifndef __lo_HTTPTASK_H__
#define __lo_HTTPTASK_H__

#include <libhttp/loHttpDef.h>

namespace lohttp
{	
	class CloHttpResponse;
	class CloHttpTaskAssoc;
	class CloHttpResponseAssoc;

	/** 一个下载任务 线程阻塞式的
	 * @author loach
	*/
	class loHTTP_API CloHttpTask
	{
	public:
		CloHttpTask(IloTaskCallBack* pTaskCallBack);
		~CloHttpTask(void);	

	public:
		// 添加参数值
		void AddParam(LPCTSTR szName , LPCTSTR szValue = NULL, unsigned long dwParamAttr = lohttp::ParamNormal);
		void AddParam(LPCTSTR szName , BOOL bUrlEncode, BOOL bUTF8, LPCTSTR szValue = NULL, unsigned long dwParamAttr = lohttp::ParamNormal);
		void ClearParams();

		// 添加头信息
		void AddHeader(LPCTSTR szName, LPCTSTR szValue = NULL);
		void ClearHeader();

		// 添加二进制数据
		void AddBinaryData(const char* szBinaryData,long lLen);

		// 文件名（下载保存，或者上传文件）
		void SetFileName(LPCTSTR szPath,LPCTSTR szFile);

		// 代理
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

		// 断开
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

		/** 设置代理信息 */
		void SetHttpProxyCfg();

		CloHttpResponse* CreateHttpResponse(void);
	protected:
		CloHttpTaskAssoc* m_pAssoc;
	};

	/** 请求后，应答包
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

		/** 读取类似于 " 200 OK 这样的信息 "*/
		LPCTSTR	ReadResultHeader();

		/** "Content-Length" 字段的长度
		 * @return 存在 “Content-Length”，返回true
		*/
		BOOL GetContentLength (unsigned long & cbContLen);

		/** body length */
		BOOL GetBodyLength(unsigned long & dwBodyLen);

		/** 移位读取，它会偏移 ，直到把整个读取完成 */
		unsigned long ReadBody(BYTE * pbyBuff, unsigned long cbBuff);

		/** 复位，从头开始 */
		void ResetBody();

		/** 一次读取所有body */
		unsigned long ReadAllBody(BYTE * pbyBuff, unsigned long cbBuff);		

	private:
		CloHttpResponseAssoc* m_pAssoc;
		friend class CloHttpTask;
	};

}//namespace lohttp

#endif //__lo_HTTPTASK_H__