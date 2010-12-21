#ifndef __lo__HTTP_H__
#define __lo__HTTP_H__

#include <libhttp/loHttpDef.h>

namespace lohttp
{
	// ���ؾ������
	typedef int DLHANDLE;

	// ����
	class CloHttp;
	class CloHttpMagr;
	class CloHttpTask;
	class CloHttpResponse;
	class CloHttpAssoc;

	/** ���صĻص��ӿ�
	 *@author loach
	*/
	struct loNovtableM IHttpCallBack
	{
		/*���ش��ļ�(�ڴ滺����) �ص��ӿڣ۱������� ����OnHttpBody������ͬ �� */
		virtual  int OnHttpBuffer(CloHttp *phttp , void* pUsrData , BYTE* pBuffer , long lBuffer) = 0;

		/* ����״̬ */
		virtual	 int OnHttpProgress(CloHttp *phttp , void* pUsrData ,long lCurBytes,  long lTotalBytes) = 0;

		/** �������ȡ�������HTTPͷ��Ϣ(�����Ѿ����������ص��������ڴ����ˣ�ֻ��֪ͨ�û�ȥȡ���� ) */
		virtual void OnHttpHeader(CloHttp * phttp , void* pUsrData) = 0;

		/** �������ȡ�������HTTPͷ��Ϣ body ���� , ��Щ�����У����������緵�ص�һЩ��� , �������ݣ������Ѿ����������ص��������ڴ����ˣ�ֻ��֪ͨ�û�ȥȡ���� �� */
		virtual void OnHttpBody(CloHttp *phttp , void* pUsrData , BYTE* pBuffer, long lBuffer) = 0;

		/** ����״̬ */
		virtual void OnHttpCode(CloHttp * phttp, void* pUsrData , enHttpStatus status, int errCode ) = 0;

		/** ������Ϻ󣬼�¼������Ϣ */
		virtual void OnHttpTime(CloHttp *phttp , void* pUsrData , const THttpTime* pDownloadTime) = 0;

		/** CloHttp ��Դ�ͷŵ� */
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
	* �򵥵� http Э����ļ������ࡣ���಻֧�ֶϵ�������
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
		* @brief ���ûص�����
		* @param cbFunc 
		* @param pUsrData 
		*/
		void SetCallbackFunc(IHttpCallBack* cbFunc, void * pUsrData = NULL  );

		/**
		* @brief ���������ļ�URL
		* @param szUrl Ҫ���ص��ļ�Url
		* @param szSavePath ���Ϊ�ı���·����������Ŀ¼���߾����ļ�·����
		* @param lpszExt ��չ��
		* @return true��ʾ�ɹ���ʼ�����ع�����false��ʾ����
		*/
		void SetHttpUrl( LPCTSTR szUrl);

		/** ��Ӳ��� */
		bool AddParam (LPCTSTR szName , LPCTSTR szValue = NULL, unsigned long dwParamAttr = lohttp::ParamNormal);
		bool AddParam (LPCTSTR szName , BOOL bUrlEncode , BOOL bUTF8, LPCTSTR szValue = NULL, unsigned long dwParamAttr = lohttp::ParamNormal);

		/** ���ͷ��Ϣ */
		bool AddHeader (LPCTSTR szName, LPCTSTR szValue = NULL);

		/** �ϴ����������� */
		bool AddBinaryData(char* szBinaryData,long lLen);

		/** �ϴ��ļ�(�������ļ����棩 */
		bool SetFileName(LPCTSTR szPath , LPCTSTR szFile);

		/** ���� */
		bool SetProxySrvAddr(int proxyType, LPCTSTR szProxySrv, unsigned short nProxySrvPort, LPCTSTR lpszDownloadUrl);
		bool SetProxyAccount(LPCTSTR szUserName = NULL, LPCTSTR szPassword = NULL);

		/**
		* @brief ��ʼ����
		*/
		bool StartHttp();

		/**
		* @brief ȡ�����ز���
		* @param msTimeout �ȴ��ĳ�ʱʱ��
		* @return 1 ��ʾ�Ѿ��ɹ�ȡ���� 0 ��ʾ����δ��ʼ��-1 ��ʾ�ȴ���ʱ��
		* @remark ������ 1 �� -1 ʱ���������ڳɹ�ȡ���󶼻�
		* �� E_HTTPSTATUS_CANCEL ״̬�ص� CallBack �������������;
		* ���ִ�����ֻ���� E_HTTPSTATUS_ERROR �ص�
		*/
		int CancelHttp( unsigned long msTimeout = 1000/*INFINITE*/ );

		//�������ص�ַ
		LPCTSTR GetHttpUrl() const;

		/** ��ȡ�û��Զ������� */
		void * GetUserData() const;

		/**  �����Ƿ���������� */
		bool IsDownloadCompleted() const;

		/// ��ȡ��ǰ������״̬
		enHttpStatus GetStatus() const;

		/** ��ȡ���ؾ�� */
		DLHANDLE GetHandle(void) const;

		/** ����������״̬��Ϊ E_HTTPSTATUS_WAIT && E_HTTPSTATUS_START  ,�Ƿ��Զ�ɾ��Release ������������ͷ���Դ��*/
		bool IsAutoDestroy(void) const;

		/** �߳��Ƿ�ֹͣ */
		bool IsStopped() const;

		/** �Զ����ٿ���,���� DownloaderSetParam�ص�ʱ�������� */
		void AutoDestroy(bool bEnable);

		/** ���÷��� */
		void SetMethod(enHttpMethod method);

		/*
		function:��ȡ��http���ؽ����header��Ϣ
		*/
		LPCTSTR GetHttpResultHeader(LPCTSTR szName);
		LPCTSTR GetResult();

	private:
		/** ���� ͷ��Ϣ */
		void RecvHeader(void);

		/** ���� body��Ϣ */
		int  RecvBody(void);

		/** �������ؾ�� */
		void SetHandle(DLHANDLE dlHandle);

		/** ��ȡ���� ��� CloHttpCurl ����ģʽ */
		int GetMethod(void) const;

		/// �ͷ�������Դ
		void Free();

		/// HttpClientģʽ�����߳�
		static unsigned long __stdcall HttpClientThread( void * pParam );

		/// HttpClientģʽ�����߳�(��ʱ����)
		unsigned long HttpStart(void);

		/** �߳��˳� */
		int StopThread();

		/** ���� EnterCriticalSection */
		void Lock();

		/** LeaveCriticalSection */
		void Unlock();

		/** */
		void SetStatus(enHttpStatus status);

		/** ���ü���(��Ҫ���� CloHttpMagr ����) */
		long AddRef(void);
		long Release(void);

		friend void call_free_downloader(void* element);
	private:
		CloHttpAssoc* m_pAssoc;
		friend class CloHttpMagr;
		friend class CloHttpManagerAssoc;
	};

	/** ���ع���,��֧�ֶ��߳����� */
	class loHTTP_API CloHttpMagr
	{
	public:
		/// constructor
		CloHttpMagr();
		/// destructor
		~CloHttpMagr();
	public:
		/** ���ÿ���ͬʱ���ؼ������� */
		void SetMaxHttpCounts(int Counts);

		/// ��ʼ����
		bool StartHttp(unsigned long dwTimeout = -1);

		/// ֹͣ����
		bool StopHttp(void);

		/**
		* @brief ���һ����������
		* @param szUrl Ҫ���ص��ļ�Url
		* @param bFront �Ƿ���뵽������ǰ��
		* @return CloHttp*
		*/
		CloHttp* AddHttp(LPCTSTR szUrl,bool bFront = false);		

		/**
		* @brief ȡ��ĳ�� http ����
		* @param dlHandle ���������
		* @return dlHandle ָ�������������ڲ��ɹ�ȡ������ʱ���� true�����򷵻� false
		*/
		bool CancelHttp( DLHANDLE dlHandle );

		/**
		* @brief �ͷ�ָ����������
		* @param dlHandle ���������
		* @return dlHandle ָ��������������ʱ����true�����򷵻�false
		*/
		bool FreeHttp( DLHANDLE dlHandle );

		/** �������� */
		bool ResetHttp( DLHANDLE dlHandle);

		/** ��ȡһ��������	*/
		CloHttp* GetTaskHttp( DLHANDLE dlHandle );

		/** ����������� ��ȡһ�������� */
		CloHttp* GetTaskHttp(POSITION_l pos);

		/** ����������� ��ȡһ�������� */
		POSITION_l  GetFirstTaskPosition();

		/** ����������� ��ȡһ�������� */
		POSITION_l  GetNextTaskPosition(POSITION_l pos);

		/** ����������� ɾ��һ�������� */
		bool FreeTaskHttp(POSITION_l pos);
		/**
		* @brief �ͷ����е�������
		*/
		void FreeAll();	
	private:
		/** �ӵȴ������� popup һ��������,
		* @return ���� ʹ�����,Release()
		*/
		CloHttp* PopupWaitHttp(void);

		/** ��ȡ��Ч������������ */
		int GetWaitHttpCounts(void) const;

		/** ��ȡ���������������� */
		int GetHttpingCounts(void) const;

		/** ���� EnterCriticalSection */
		void Lock();

		/** LeaveCriticalSection */
		void Unlock();

		/** �����߳� */
		static unsigned long WINAPI MagrThread( void * pParam );
	private:
		friend class CloHttpManagerAssoc;
		CloHttpManagerAssoc* m_pAssoc;
	};
} // namespace lohttp

#endif /* __lo__HTTP_H__ */