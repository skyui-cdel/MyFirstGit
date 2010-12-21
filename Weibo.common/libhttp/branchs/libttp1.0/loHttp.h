#ifndef __lo__HTTP_H__
#define __lo__HTTP_H__

#include <libhttp/loHttpDef.h>

namespace lohttp
{
	// 下载句柄定义
	typedef int DLHANDLE;

	// 引用
	class CloHttp;
	class CloHttpMagr;
	class CloHttpTask;
	class CloHttpResponse;
	class CloHttpAssoc;

	/** 下载的回调接口
	 *@author loach
	*/
	struct loNovtableM IHttpCallBack
	{
		/*下载大文件(内存缓冲区) 回调接口［边下载中 ，与OnHttpBody有所不同 ］ */
		virtual  int OnHttpBuffer(CloHttp *phttp , void* pUsrData , BYTE* pBuffer , long lBuffer) = 0;

		/* 进度状态 */
		virtual	 int OnHttpProgress(CloHttp *phttp , void* pUsrData ,long lCurBytes,  long lTotalBytes) = 0;

		/** 从网络获取的请求的HTTP头信息(数据已经从网络下载到本机的内存中了，只是通知用户去取数据 ) */
		virtual void OnHttpHeader(CloHttp * phttp , void* pUsrData) = 0;

		/** 从网络获取的请求的HTTP头信息 body 数据 , 这些数据中，可能是网络返回的一些结果 , 少量数据（数据已经从网络下载到本机的内存中了，只是通知用户去取数据 ） */
		virtual void OnHttpBody(CloHttp *phttp , void* pUsrData , BYTE* pBuffer, long lBuffer) = 0;

		/** 下载状态 */
		virtual void OnHttpCode(CloHttp * phttp, void* pUsrData , enHttpStatus status, int errCode ) = 0;

		/** 下载完毕后，记录下载信息 */
		virtual void OnHttpTime(CloHttp *phttp , void* pUsrData , const THttpTime* pDownloadTime) = 0;

		/** CloHttp 资源释放掉 */
		virtual void OnHttpRelease(CloHttp * phttp,void* pUsrData) = 0;
	};

	/**
	* @ingroup lohttp
	*
	* @version 
	*
	* @date 2010-06-04
	*
	* @author loach
	*
	* 简单的 http 协议的文件下载类。该类不支持断点续传。
	* 
	*
	*/
	class loHTTP_API CloHttp
	{
	public:
		CloHttp();
		virtual ~CloHttp(void);

	public:
		/**
		* @brief 设置回调函数
		* @param cbFunc 
		* @param pUsrData 
		*/
		void SetCallbackFunc(IHttpCallBack* cbFunc, void * pUsrData = NULL  );

		/**
		* @brief 设置下载文件URL
		* @param szUrl 要下载的文件Url
		* @param szSavePath 另存为的本地路径，可以是目录或者具体文件路径。
		* @param lpszExt 扩展名
		* @return true表示成功初始化下载工作，false表示错误
		*/
		void SetHttpUrl( LPCTSTR szUrl);

		/** 添加参数 */
		bool AddParam (LPCTSTR szName , LPCTSTR szValue = NULL, unsigned long dwParamAttr = lohttp::ParamNormal);
		bool AddParam (LPCTSTR szName , BOOL bUrlEncode , BOOL bUTF8, LPCTSTR szValue = NULL, unsigned long dwParamAttr = lohttp::ParamNormal);

		/** 添加头信息 */
		bool AddHeader (LPCTSTR szName, LPCTSTR szValue = NULL);

		/** 上传二进制数据 */
		bool AddBinaryData(char* szBinaryData,long lLen);

		/** 上传文件(或下载文件保存） */
		bool SetFileName(LPCTSTR szPath , LPCTSTR szFile);

		/** 代理 */
		bool SetProxySrvAddr(int proxyType, LPCTSTR szProxySrv, unsigned short nProxySrvPort, LPCTSTR lpszDownloadUrl);
		bool SetProxyAccount(LPCTSTR szUserName = NULL, LPCTSTR szPassword = NULL);

		/**
		* @brief 开始下载
		*/
		bool StartHttp();

		/**
		* @brief 取消下载操作
		* @param msTimeout 等待的超时时间
		* @return 1 表示已经成功取消， 0 表示下载未开始。-1 表示等待超时。
		* @remark 当返回 1 或 -1 时，下载器在成功取消后都会
		* 以 E_HTTPSTATUS_CANCEL 状态回调 CallBack 函数，但如果中途
		* 出现错误，则只会以 E_HTTPSTATUS_ERROR 回调
		*/
		int CancelHttp( unsigned long msTimeout = 1000/*INFINITE*/ );

		//返回下载地址
		LPCTSTR GetHttpUrl() const;

		/** 获取用户自定义数据 */
		void * GetUserData() const;

		/**  返回是否已完成下载 */
		bool IsDownloadCompleted() const;

		/// 获取当前的下载状态
		enHttpStatus GetStatus() const;

		/** 获取下载句柄 */
		DLHANDLE GetHandle(void) const;

		/** 当下载器的状态不为 E_HTTPSTATUS_WAIT && E_HTTPSTATUS_START  ,是否自动删除Release 这个下载器（释放资源）*/
		bool IsAutoDestroy(void) const;

		/** 线程是否停止 */
		bool IsStopped() const;

		/** 自动消毁开关,请在 DownloaderSetParam回调时进行设置 */
		void AutoDestroy(bool bEnable);

		/** 设置方法 */
		void SetMethod(enHttpMethod method);

		/*
		function:获取从http返回结果的header信息
		*/
		LPCTSTR GetHttpResultHeader(LPCTSTR szName);
		LPCTSTR GetResult();

	private:
		/** 接受 头信息 */
		void RecvHeader(void);

		/** 接受 body信息 */
		int  RecvBody(void);

		/** 设置下载句柄 */
		void SetHandle(DLHANDLE dlHandle);

		/** 获取方法 针对 CloHttpCurl 下载模式 */
		int GetMethod(void) const;

		/// 释放所有资源
		void Free();

		/// HttpClient模式下载线程
		static unsigned long __stdcall HttpClientThread( void * pParam );

		/// HttpClient模式请求线程(超时机制)
		unsigned long HttpStart(void);

		/** 线程退出 */
		int StopThread();

		/** 锁定 EnterCriticalSection */
		void Lock();

		/** LeaveCriticalSection */
		void Unlock();

		/** */
		void SetStatus(enHttpStatus status);

		/** 引用计数(主要用于 CloHttpMagr 管理) */
		long AddRef(void);
		long Release(void);

		friend void call_free_downloader(void* element);
	private:
		CloHttpAssoc* m_pAssoc;
		friend class CloHttpMagr;
		friend class CloHttpManagerAssoc;
	};

	/** 下载管理,不支持多线程下载 */
	class loHTTP_API CloHttpMagr
	{
	public:
		/// constructor
		CloHttpMagr();
		/// destructor
		~CloHttpMagr();
	public:
		/** 设置可以同时下载几个任务 */
		void SetMaxHttpCounts(int Counts);

		/// 开始下载
		bool StartHttp(unsigned long dwTimeout = -1);

		/// 停止下载
		bool StopHttp(void);

		/**
		* @brief 添加一个下载任务
		* @param szUrl 要下载的文件Url
		* @param bFront 是否插入到任务最前面
		* @return CloHttp*
		*/
		CloHttp* AddHttp(LPCTSTR szUrl,bool bFront = false);		

		/**
		* @brief 取消某个 http 下载
		* @param dlHandle 下载器句柄
		* @return dlHandle 指定的下载器存在并成功取消下载时返回 true，否则返回 false
		*/
		bool CancelHttp( DLHANDLE dlHandle );

		/**
		* @brief 释放指定的下载器
		* @param dlHandle 下载器句柄
		* @return dlHandle 指定的下载器存在时返回true，否则返回false
		*/
		bool FreeHttp( DLHANDLE dlHandle );

		/** 重新下载 */
		bool ResetHttp( DLHANDLE dlHandle);

		/** 获取一个下载器	*/
		CloHttp* GetTaskHttp( DLHANDLE dlHandle );

		/** 从任务队列中 获取一个下载器 */
		CloHttp* GetTaskHttp(POSITION_l pos);

		/** 从任务队列中 获取一个下载器 */
		POSITION_l  GetFirstTaskPosition();

		/** 从任务队列中 获取一个下载器 */
		POSITION_l  GetNextTaskPosition(POSITION_l pos);

		/** 从任务队列中 删除一个下载器 */
		bool FreeTaskHttp(POSITION_l pos);
		/**
		* @brief 释放所有的下载器
		*/
		void FreeAll();	
	private:
		/** 从等待队列中 popup 一个下载器,
		* @return 返回 使用完后,Release()
		*/
		CloHttp* PopupWaitHttp(void);

		/** 获取有效下载器的数量 */
		int GetWaitHttpCounts(void) const;

		/** 获取正在下载器的数量 */
		int GetHttpingCounts(void) const;

		/** 锁定 EnterCriticalSection */
		void Lock();

		/** LeaveCriticalSection */
		void Unlock();

		/** 管理线程 */
		static unsigned long WINAPI MagrThread( void * pParam );
	private:
		friend class CloHttpManagerAssoc;
		CloHttpManagerAssoc* m_pAssoc;
	};
} // namespace lohttp

#endif /* __lo__HTTP_H__ */