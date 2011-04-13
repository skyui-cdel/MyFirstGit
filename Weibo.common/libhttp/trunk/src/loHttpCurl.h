#ifndef __lo_HTTP_CURL_H__
#define __lo_HTTP_CURL_H__

#include <string>
#include "loHttpStruct.h"
#include "loHttpDef.h"

namespace lohttp
{
	/** 回调函数 */
	struct loNovtableM IloTaskCallBack
	{
		/** 下载状态 */
		virtual void  OnTaskState(long lState) = 0;

		/** 下载进度
		 *
		 * @return 1,告之底层 退
		*/
		virtual int  OnTaskProgress(long lCurBytes, long lTotalBytes) = 0;

		/** 下载缓冲区（方便开发者保存文件）
		 *
		 * @return !=0 表示不需要（可能这个buffer 不是开发者需要的）
		*/
		virtual int  OnTaskBuffer(unsigned char* pBuffer, long lBuffer) = 0;

		/** 下载码
		 * @param code = enHTTPCode
		*/
		virtual void  OnTaskCode(long code, const char* errString) = 0;

		/** 下载完后，执行时间 */
		virtual void OnTaskTime(const THttpTime * pDownloadTime) = 0 ;
	};

	/** 前向引用声明 */
	class CloHttpCurlAssoc;

	/** 基于 libcurl */
	class CloHttpCurl 
	{
	public:
		CloHttpCurl(IloTaskCallBack* pTaskCallback);
		~CloHttpCurl(void);

	public:
		/**
		* @ 获取任务ID
		*/
		long GetTaskID() const;

		/**
		* @ 开始获取数据
		*/
		int	 Request();		// 获取数据，同步诸塞的过程

		/**
		* @ 终止传输
		*/
		void CancelRequest();

		/**
		* @ 设置代理信息
		*/
		void SetProxyInfo(THttpProxyInfo &ProxyInfo);

		/**
		* @ 设置代理信息
		*/
		int	SetTransferParam(int nType, LPVOID lpParam);

		/**
		* @ 判断任务是否有效
		*/
		BOOL IsTaskValid();

		/**
		* @ 获取属性
		*/
		int	GetType() const;
		int	GetState() const;
		int	GetErrCode() const;

		long GetHttpCode() const;
		/**
		* @ 获取结果
		*/
		int	GetResponedHeader(THttpMemoryBuffer * pHeaderBuf);
		int	GetResponedBody(THttpMemoryBuffer * pBodyBuf);

		int	Step();		// Manager专用函数，保留

		void SetUserAgent(const char* agent);
	protected:
		BOOL IsUserCancel() const;

		/** 启动http */
		int	 TransferProc();


		int	 TransferProcGET();

		/** 下载到文件中 */
		int	 TransferProcDOWNLOAD();

		/** 下载到 Buffer; */
		int	 TransferProcDOWNLOAD_TOBUFFER();

		/** post 一般post处理 */
		int	 TransferProcPOST();

		/** post 文件 */
		int	 TransferProcPOSTFormData();
//		int  TransferProcPOSTFormData2();

//		/** 上传文件*/
//		int	 TransferProcUPLOAD();

		/** delete operator */
		int	 TransferProcDELETE();

		/** put operator */
		int	 TransferProcPUT();

		//
		void SetTransferCallback(LPVOID headerData, LPVOID bodyData);

		// 通知 httptask
	private:
		/** 状态通知（EHttpTransferState） */
		void TaskState(int nState);

		/** 进度下载 */
		int TaskProgress(long lCurBytes, long lTotalBytes);

		/** 缓冲区 */
		int TaskBuffer(unsigned char* pBuffer, long lLength);

		/** 出错 */
        void TaskCode(void);
	private:
		void ClearBuffer();
		void ClearParam();

		/** 启动上传 */
		int	 StartTransfer();

		/** 停止下载 */
		void StopTransfer();

		void SetState(int nNewState);

		/** 代理设置 */
		void UpdateProxy(char* lpszRequestURL);

		// user agent
		const char* GetUserAgent() const;
	private:
		/** body 数据回调 */
		static size_t ToBodyCallback(void *ptr, size_t size, size_t nmemb, void *data);

		/** header 数据回调 */
		static size_t ToHeaderCallback(void *ptr, size_t size, size_t nmemb, void *data);

		/** 把数据写到文件中*/
		static size_t ToFileCallback(void *ptr, size_t size, size_t nmemb, void *data);

		/** 把数据缓冲输出，通过 TaskBuffer() */
		static size_t ToBufferCallback(void *ptr, size_t size, size_t nmemb, void *data);

		/** 数据进度 */
		static size_t ProgressCallback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);
	protected:
		friend class CloHttpCurlAssoc;
		CloHttpCurlAssoc* m_pAssoc;
	};

} // namespace lohttp

#endif /*__lo_HTTP_CURL_H__ */
