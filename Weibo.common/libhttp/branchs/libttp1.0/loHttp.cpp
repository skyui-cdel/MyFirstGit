#include "stdafx.h"
#include "loHttp.h"
#include "loHttpTask.h"
#include "loHttpStruct.h"
#include "common/lo/lolock.h"
#include "common/lo/lolist.h"
#include "common/lofile.h"
#include <assert.h>
#include <atltime.h>

USE_NAMESPACE(locom)
USE_NAMESPACE(lo)

namespace lohttp
{
	typedef CloListT<CloHttp*> HTTP_MAP;

	//
#define SAFE_CALLBACK( phttp, status, errCode )\
	if( phttp->m_pAssoc->m_cbFunc )\
	{ \
	   phttp->m_pAssoc->m_cbFunc->OnHttpCode(phttp,phttp->m_pAssoc->m_pUsrData, status, errCode );\
	   phttp->m_pAssoc->m_status = status;\
	}

	//
#define AUTO_LOCK() \
	lo::CloAutoLock<CRITICAL_SECTION, lo::CloCriticalSection> al(&m_pAssoc->m_cs)

	//
	const int REQ_OPEN_FILE    =  1;
	const int REQ_READ_DATA    =  2;
	const int REQ_CLOSE_HANDLE =  3;

	// �ͷŻص�����
	void call_free_downloader(void* element)
	{
		CloHttp* pHttp = reinterpret_cast<CloHttp*>( element );
		if ( pHttp )
		{
			pHttp->CancelHttp();
			pHttp->Release();
		}
	}

	int call_query_download_with_dlhandle(POSITION_l position ,  void* element , void* data)
	{
		CloHttp* p = reinterpret_cast<CloHttp*>(element);
		if( p->GetHandle() == (DLHANDLE)data )
			return 0;

		return -1;
	}

	//------------------------------------------------------------------------------------//
	// CloHttpAssoc

	class CloHttpAssoc : public IloTaskCallBack
	{
	public:
		CloHttpAssoc( CloHttp* pThis)
			: m_pThis( pThis )
			, m_hInetTh(NULL)
			, m_cbFunc(NULL)
			, m_pUsrData(NULL)
			, m_status(E_HTTPSTATUS_CLOSED)
			, m_bCompleted(false)
			, m_bCancel(false)
			, m_bAutoDestroy(true)
			, m_bStoped(false)
			, m_pHttpResponse(NULL)
			, m_DlMethod(E_HTTP_GET_METHOD)
			, m_uTimeout(30000)
			, m_uTimeout_count(0)
			, m_lRef(0)
			, m_pHttpTask( new CloHttpTask(this) )
		{
			m_strUrl[0]  = _T('\0');
		}

		~CloHttpAssoc()
		{
			delete m_pHttpTask;
			m_pHttpTask = NULL;

			if( m_cbFunc )
			{
				m_cbFunc->OnHttpRelease(m_pThis,m_pUsrData);
				m_pUsrData = NULL;
			}
		}

		// IloTaskCallBack
	private:
		void OnTaskState(long lState);

		int  OnTaskProgress(long lCurBytes,  long lTotalBytes);

		int  OnTaskBuffer(BYTE* pBuffer, long lBuffer);

		void OnTaskCode(long code, const char* errString);

		/* HttpTask �Խ� */
		void OnTaskTime(const THttpTime * pDownloadTime);

		// ����
		unsigned long Start(void);
	private:
		CloHttp* m_pThis;

		/// ���ط���,Ĭ��Ϊ E_HTTP_GET_METHOD
		enHttpMethod m_DlMethod;

		/// �����߳̾��
		HANDLE   m_hInetTh;

		CloHttpTask* m_pHttpTask;
		CloHttpResponse* m_pHttpResponse;

		/// �ص�����
		IHttpCallBack* m_cbFunc;

		/// �û�����
		void*  m_pUsrData;

		/// ��ʱʱ��(Ĭ��Ϊ 30000 30��)
		unsigned long   m_uTimeout;

		/// ��ʱ����(Ĭ��Ϊ0,��ʱ������loop)
		int  m_uTimeout_count;

		/// ��ǰ����״̬
		enHttpStatus  m_status;

		/// ָ���Ƿ��Ѿ��������
		bool m_bCompleted;

		/// ָ���Ƿ���Ҫȡ������
		bool m_bCancel;

		/// �Զ�����,Ĭ��ΪTRUE
		bool m_bAutoDestroy;

		/// �߳�ֹͣ
		bool m_bStoped;

		/// ���ؾ��
		DLHANDLE    m_hDlHandle;

		/// ���ü���
		volatile long m_lRef;

		/// ��Ҫ�����ļ��ĵ�ַ
		TCHAR m_strUrl[HTTP_URL_LENGTH];

		/// ͬ����
		lo::CloSmartLock<CRITICAL_SECTION, lo::CloCriticalSection> m_cs;

		friend class CloHttpManagerAssoc;
		friend class CloHttpMagr;
		friend class CloHttp;
	};

	void CloHttpAssoc::OnTaskTime(const THttpTime * pDownloadTime)
	{
		if( pDownloadTime && m_cbFunc )
		{
			m_cbFunc->OnHttpTime(m_pThis, m_pUsrData, pDownloadTime);
		}
	}

	int CloHttpAssoc::OnTaskProgress(long lCurBytes,  long lTotalBytes)
	{
		if( m_cbFunc )
		{
			return m_cbFunc->OnHttpProgress(m_pThis , m_pUsrData,lCurBytes,lTotalBytes);
		}
		return 0;
	}

	int CloHttpAssoc::OnTaskBuffer(BYTE* pBuffer, long lBuffer)
	{
		if( m_cbFunc )
		{
			return m_cbFunc->OnHttpBuffer(m_pThis , m_pUsrData,pBuffer,lBuffer);
		}
		return 0;
	}

	void CloHttpAssoc::OnTaskState(long lState)
	{
	}


	void CloHttpAssoc::OnTaskCode(long code, const char* errString)
	{
	}

	unsigned long CloHttpAssoc::Start(void)
	{
		CloHttpResponse* pHttpResponse = NULL;
		if( !m_pHttpTask )
			return -1;

		switch( m_DlMethod )
		{
		case E_HTTP_GET_METHOD:
			pHttpResponse = m_pHttpTask->RequestGet(m_strUrl) ;
			break;
		case E_HTTP_POST_METHOD:
			pHttpResponse = m_pHttpTask->BeginPost(m_strUrl);
			break;
		case E_HTTP_POST_BINARY_METHOD:
			pHttpResponse = m_pHttpTask->BeginPostBinary(m_strUrl); 
			break;
		case E_HTTP_UPLOAD_BINARY_TEXT_METHOD:
			pHttpResponse = m_pHttpTask->BeginPostBinaryText(m_strUrl);
			break;
		case E_HTTP_UPLOAD_METHOD:
			pHttpResponse = m_pHttpTask->BeginUpload(m_strUrl);
			break;
		case E_HTTP_DELETE_METHOD:
			pHttpResponse = m_pHttpTask->BeginDelete(m_strUrl);
			break;
		case E_HTTP_PUT_METHOD:
			pHttpResponse = m_pHttpTask->BeginPut(m_strUrl);
			break;
		case E_HTTP_GET_BUFFER_METHOD:
			pHttpResponse = m_pHttpTask->RequestBuffer(m_strUrl);
			break;
		case E_HTTP_DOWNLOAD_FILE:
			pHttpResponse = m_pHttpTask->BeginDownloadFile(m_strUrl);
			break;
		}

		if( pHttpResponse )
		{
			m_pHttpResponse = pHttpResponse;		
			return 0;
		}
		return -1;
	}
	//------------------------------------------------------------------------------------//
	// CloHttpManagerAssoc

	class CloHttpManagerAssoc
	{
	public:
		CloHttpManagerAssoc()
			: m_nextId(0)
			, m_hMagrThr(NULL)
			, m_dlerMaxCounts(5)
			, m_bExit(false)
			, m_dwTimeout(1000*60*5)
		{

		}

		~CloHttpManagerAssoc()
		{
		}

		/** �ͷ����ض��� */
		static void Free(HTTP_MAP* dlmap , DLHANDLE dlHandle)
		{
			POSITION_l pos = dlmap->query( call_query_download_with_dlhandle , (void*)dlHandle );
			if( pos )
			{
				CloHttp* phttp = dlmap->remove(pos);
				phttp->CancelHttp();
				phttp->Release();
			}
		}

		/// �߳��˳���־
		BOOL m_bExit;

		/// �����߳�
		HANDLE m_hMagrThr;

		/// ͬ����
		lo::CloSmartLock<CRITICAL_SECTION, lo::CloCriticalSection> m_cs;

		/// ������������ӳ���(��������еĳ�Ա�����꣬����ȡ��ʱ���Ŵ�m_dlWaitMapȡ����)
		HTTP_MAP m_dlMap;

		/// ���ڵȴ�����������ӳ���(һ��ʼ�����ģ�����������)
		HTTP_MAP m_dlWaitMap;

		/// �����б�
		HTTP_MAP m_dlTaskli;

		/// ��һ�������������ľ��ֵ
		int m_nextId;

		/// ÿ��ͬʱ��༸����������������,Ĭ��Ϊ 10
		int m_dlerMaxCounts;

		/// ��ʱ���� ,��������ʱ���ڣ�û���������񣬽��Զ��˳������߳� Ĭ��Ϊ 2����
		unsigned long m_dwTimeout;
	};


	CloHttp::CloHttp()
		: m_pAssoc( new CloHttpAssoc(this) )
	{
	}

	CloHttp::~CloHttp(void)
	{
		Free();

		delete m_pAssoc;
		m_pAssoc = NULL;
	}

	long CloHttp::AddRef( void )
	{
		InterlockedIncrement( &m_pAssoc->m_lRef );
		return m_pAssoc->m_lRef;
	}

	long CloHttp::Release(void)
	{
		assert(m_pAssoc->m_lRef>0);
		if( 0 == InterlockedDecrement( &m_pAssoc->m_lRef ) )
		{
			delete this;
			return( 0 );
		}
		return( m_pAssoc->m_lRef );
	}

	void CloHttp::SetCallbackFunc(IHttpCallBack* cbFunc, void * pUsrData)
	{
		m_pAssoc->m_cbFunc = cbFunc;
		m_pAssoc->m_pUsrData = pUsrData;
	}

	bool CloHttp::StartHttp()
	{
		Free();

		if( m_pAssoc->m_strUrl[0] == _T('\0') )
		{
			return false;
		}

		m_pAssoc->m_bStoped = false;
		m_pAssoc->m_hInetTh = ::CreateThread( NULL, 0, CloHttp::HttpClientThread, this, 0, NULL );
		if(  NULL == m_pAssoc->m_hInetTh )
			m_pAssoc->m_bStoped = true;

		return !m_pAssoc->m_bStoped;
	}

	void CloHttp::SetHttpUrl(LPCTSTR szUrl)
	{
		_tcsncpy_s( m_pAssoc->m_strUrl , szUrl , HTTP_URL_LENGTH);
		m_pAssoc->m_strUrl[HTTP_URL_LENGTH-1] = '\0';
	}

	bool CloHttp::AddParam (LPCTSTR szName , LPCTSTR szValue, unsigned long dwParamAttr)
	{
		if( !m_pAssoc->m_pHttpTask )
			return false;

		m_pAssoc->m_pHttpTask->AddParam(szName , szValue, dwParamAttr);

		return true;
	}

	bool CloHttp::AddParam (LPCTSTR szName , BOOL bUrlEncode, BOOL bUTF8, LPCTSTR szValue, unsigned long dwParamAttr)
	{
		if( !m_pAssoc->m_pHttpTask )
			return false;

		m_pAssoc->m_pHttpTask->AddParam(szName ,  bUrlEncode,  bUTF8,  szValue,  dwParamAttr);
		return true;
	}

	/** ���ͷ��Ϣ */
	bool CloHttp::AddHeader (LPCTSTR szName, LPCTSTR szValue)
	{
		if( !m_pAssoc->m_pHttpTask )
			return false;

		m_pAssoc->m_pHttpTask->AddHeader(szName ,  szValue );
		return true;
	}

	/** �ϴ����������� */
	bool CloHttp::AddBinaryData(char* szBinaryData,long lLen)
	{
		if( !m_pAssoc->m_pHttpTask )
			return false;

		m_pAssoc->m_pHttpTask->AddBinaryData(szBinaryData ,  lLen );
		return true;
	}

	/** �ϴ��ļ� */
	bool CloHttp::SetFileName(LPCTSTR szPath , LPCTSTR szFile)
	{
		if( !m_pAssoc->m_pHttpTask )
			return false;

		m_pAssoc->m_pHttpTask->SetFileName(szPath , szFile);
		return true;
	}


	bool CloHttp::SetProxySrvAddr(int proxyType, LPCTSTR szProxySrv, unsigned short nProxySrvPort, LPCTSTR lpszDownloadUrl)
	{
		if( !m_pAssoc->m_pHttpTask )
			return false;

		m_pAssoc->m_pHttpTask->SetProxySrvAddr(proxyType,  szProxySrv,  nProxySrvPort,  lpszDownloadUrl);
		return true;
	}

	bool CloHttp::SetProxyAccount(LPCTSTR szUserName, LPCTSTR szPassword )
	{
		if( !m_pAssoc->m_pHttpTask )
			return false;

		m_pAssoc->m_pHttpTask->SetProxyAccount(szUserName,  szPassword);
		return true;
	}

	//�������ص�ַ
	LPCTSTR CloHttp::GetHttpUrl() const{
		return m_pAssoc->m_strUrl;
	}

	void* CloHttp::GetUserData() const {
		return m_pAssoc->m_pUsrData;
	}

	/**  �����Ƿ���������� */
	bool CloHttp::IsDownloadCompleted() const {
		return m_pAssoc->m_bCompleted;
	}

	/// ��ȡ��ǰ������״̬
	enHttpStatus CloHttp::GetStatus() const{
		return m_pAssoc->m_status;
	}

	DLHANDLE CloHttp::GetHandle(void) const{
		return m_pAssoc->m_hDlHandle;
	}
	bool CloHttp::IsAutoDestroy(void) const{
			return m_pAssoc->m_bAutoDestroy;
	}

	/** �Զ����ٿ���,���� DownloaderSetParam�ص�ʱ�������� */
	void CloHttp::AutoDestroy(bool bEnable){ 
		AUTO_LOCK(); 
		m_pAssoc->m_bAutoDestroy = bEnable; 
	}

	void CloHttp::SetMethod(enHttpMethod method) {
		m_pAssoc->m_DlMethod = method;
	}

	/** ��ȡ���� ��� CloHttpCurl ����ģʽ */
	int CloHttp::GetMethod(void) const{
		return m_pAssoc->m_DlMethod;
	}

	void CloHttp::SetHandle(DLHANDLE dlHandle){
		m_pAssoc->m_hDlHandle = dlHandle;
	}

	/** ���� EnterCriticalSection */
	void CloHttp::Lock()
	{
		m_pAssoc->m_cs.Lock();
	}

	/** LeaveCriticalSection */
	void CloHttp::Unlock()
	{
		m_pAssoc->m_cs.UnLock();
	}

	unsigned long CloHttp::HttpStart(void)
	{
		CloHttpTask * pHttpTask =  m_pAssoc->m_pHttpTask;
		if( !pHttpTask )
		{//
			assert(0);
			return -1;
		}

		// ��ʼ�����ļ�
		SAFE_CALLBACK( this , E_HTTPSTATUS_START, 0 );

		Lock();
		SAFE_DELETE(m_pAssoc->m_pHttpResponse);
		if( m_pAssoc->m_bCancel ) 
		{
			// It's being canceled!
			Unlock();
			SAFE_CALLBACK( this , E_HTTPSTATUS_CANCEL, 0 );
			return StopThread();
		}
		Unlock();

		try
		{
			//add by dongzhong 2009-05-26
			m_pAssoc->Start();

			// It's being canceled!
			if( m_pAssoc->m_bCancel ){
				pHttpTask->Cancel();
				SAFE_CALLBACK( this , E_HTTPSTATUS_CANCEL, 0 );
				return StopThread();
			}

			if(!m_pAssoc->m_pHttpResponse)
			{
				pHttpTask->Cancel();
				SAFE_CALLBACK( this , E_HTTPSTATUS_ERROR , GetLastError() );
				return StopThread();
			}

			// header 
			RecvHeader();

			// body
			RecvBody();

		}
		catch (...)
		{
			pHttpTask->Cancel() ;

			SAFE_CALLBACK( this , E_HTTPSTATUS_ERROR, GetLastError() );
			return StopThread();
		}	
		return StopThread();
	}

	unsigned long CloHttp::HttpClientThread( void * pParam )
	{
		CloHttp* pHttp = ( CloHttp * )pParam;
		return pHttp->HttpStart();
	}

	int CloHttp::StopThread()
	{
		m_pAssoc->m_bStoped = true;
		return 0;
	}

	void CloHttp::Free()
	{
		CancelHttp( );
		m_pAssoc->m_bCompleted = false;
		m_pAssoc->m_bCancel    = false;
		m_pAssoc->m_bStoped    = true;
	}

	int CloHttp::CancelHttp( unsigned long msTimeout )
	{
		int nResult = 0;

		Lock();
		// CloHttpCurl ����ģʽ
		if( m_pAssoc->m_pHttpTask )
			m_pAssoc->m_pHttpTask->Cancel();

		// Set cancel flag to cancel downloading progress.
		m_pAssoc->m_bCancel = true;

		Unlock();

		if( m_pAssoc->m_hInetTh )
		{	
			if( WAIT_OBJECT_0 == WaitForSingleObject( m_pAssoc->m_hInetTh , msTimeout ) ) {

				nResult = 1; // It's canceled.
			} else {
				// wait time out
				::TerminateThread(m_pAssoc->m_hInetTh,-1);
				nResult = -1;
			}
			//modify by dongzhong 2009-06-23
			CloseHandle( m_pAssoc->m_hInetTh );
			m_pAssoc->m_hInetTh = NULL;
		}

		Lock();		
		SAFE_DELETE(m_pAssoc->m_pHttpResponse);
		Unlock();

		return nResult; //( 0== Has never started.)
	}


	bool CloHttp::IsStopped() const
	{
		return m_pAssoc->m_bStoped;
	}

	LPCTSTR CloHttp::GetHttpResultHeader(LPCTSTR szName)	
	{
		if( m_pAssoc->m_pHttpResponse )
		{
			return m_pAssoc->m_pHttpResponse->GetHeader(szName);
		}
		return NULL;
	}

	LPCTSTR CloHttp::GetResult()
	{
		if( m_pAssoc->m_pHttpResponse )
		{
			return m_pAssoc->m_pHttpResponse->ReadResultHeader();
		}	
		return NULL;
	}

	/** ��ȡ ͷ��Ϣ */
	void CloHttp::RecvHeader(void)
	{
		if( m_pAssoc->m_cbFunc )
			m_pAssoc->m_cbFunc->OnHttpHeader(this , m_pAssoc->m_pUsrData );
	}

	/** ��ȡ body��Ϣ */
	int CloHttp::RecvBody(void)
	{
		if( !m_pAssoc->m_cbFunc )
			return -1 ;

		// It's being canceled!
		if( m_pAssoc->m_bCancel )
		{					
			SAFE_CALLBACK( this , E_HTTPSTATUS_CANCEL , 0  );
			return StopThread();
		}

		// ��ʼ�����ļ�
		SAFE_CALLBACK( this , E_HTTPSTATUS_BODY , 0 );

		BOOL bComplete = false;
		THttpBuffer DownloadBuffer;
		while( !bComplete && !m_pAssoc->m_bCancel )
		{			
			DownloadBuffer.dwLength =
				m_pAssoc->m_pHttpResponse->ReadBody(DownloadBuffer.byBuffer, HTTP_BUFFER_STREAM_LENGTH-1);

			if( DownloadBuffer.dwLength > 0 )
			{
				// Notify there is new data available now.
				m_pAssoc->m_cbFunc->OnHttpBody(this , m_pAssoc->m_pUsrData , DownloadBuffer.byBuffer , DownloadBuffer.dwLength );
			}
			else
			{
				bComplete = true; //���
				m_pAssoc->m_pHttpResponse->ResetBody();
			}
		}

		if( m_pAssoc->m_bCancel )
		{
			SAFE_CALLBACK( this, E_HTTPSTATUS_CANCEL, 0 );
		} 
		else if( bComplete)
		{
			SAFE_CALLBACK( this , E_HTTPSTATUS_COMPLETE, 0 );			
			m_pAssoc->m_bCompleted = true;
		}
		return 0;
	}

	void CloHttp::SetStatus(enHttpStatus status)
	{
		m_pAssoc->m_status = status;
	}

	//----------------------- Implementation of CloHttpMagr Class -------------------------------

	CloHttpMagr::CloHttpMagr()
	{
		m_pAssoc = new CloHttpManagerAssoc();
	}

	CloHttpMagr::~CloHttpMagr()
	{
		FreeAll();
	}

	void CloHttpMagr::SetMaxHttpCounts(int Counts)
	{
		AUTO_LOCK();
		m_pAssoc->m_dlerMaxCounts = Counts;
	}

	void CloHttpMagr::Lock(){
		m_pAssoc->m_cs.Lock();
	}

	void CloHttpMagr::Unlock(){
		m_pAssoc->m_cs.UnLock();
	}


	bool CloHttpMagr::StartHttp(unsigned long dwTimeout)
	{
		BOOL bStop = true;
		// check if the thread is stopped.
		if(m_pAssoc-> m_hMagrThr && WAIT_OBJECT_0 != ::WaitForSingleObject( m_pAssoc->m_hMagrThr, 0 ) ) {
			bStop = false;
		}

		if( bStop && m_pAssoc->m_hMagrThr ){
			CloseHandle( m_pAssoc->m_hMagrThr );
			m_pAssoc->m_hMagrThr = NULL;
		}
		// ��ʱ����
		if ( dwTimeout != -1 )
			m_pAssoc->m_dwTimeout = dwTimeout;

		if(!m_pAssoc->m_hMagrThr)
		{
			m_pAssoc->m_hMagrThr = ::CreateThread( NULL, 0, MagrThread, this, 0, NULL );
			return ( NULL != m_pAssoc->m_hMagrThr );
		}
		while( ResumeThread(m_pAssoc->m_hMagrThr) > 0){
		}
		return true;
	}

	bool CloHttpMagr::StopHttp(void)
	{
		if( m_pAssoc->m_hMagrThr )
			SuspendThread( m_pAssoc->m_hMagrThr );

		return 0;
	}

	unsigned long CloHttpMagr::MagrThread( void * pParam )
	{	
		enHttpStatus status;
		int size=0,i=0;
		unsigned long dwStartTime =0 , cTime = 0;
		POSITION_l    pos = NULL,oldpos=NULL;
		CloHttp   *pHttp = NULL,*pRemoveDownloader=NULL;
		CloHttpMagr *pDownloaderMagr = ( CloHttpMagr * )pParam;
		assert(pDownloaderMagr);
		dwStartTime = GetTickCount();

		while(1)
		{	
			// ��������б���û����Ҫ���صĵ�,�����, �Ͱѷŵ��ȴ�������ȥ
			pDownloaderMagr->Lock();		
			pos = pDownloaderMagr->GetFirstTaskPosition();
			while(pos)
			{
				if( pDownloaderMagr->m_pAssoc->m_bExit ){
					pDownloaderMagr->Unlock();
					return 0;
				}
				pHttp = pDownloaderMagr->GetTaskHttp(pos);
				if(pHttp)
				{
					status = pHttp->GetStatus();
					if( E_HTTPSTATUS_CLOSED == status )
					{
						if( -1 != pDownloaderMagr->m_pAssoc->m_dlWaitMap.add(pHttp) )
						{
							pHttp->AddRef();
							pHttp->SetStatus( E_HTTPSTATUS_WAIT );
						}
					}
					else if( (E_HTTPSTATUS_ERROR == status || pHttp->IsDownloadCompleted() ) && pHttp->IsAutoDestroy() && pHttp->IsStopped() )
					{ 
						oldpos = pDownloaderMagr->GetNextTaskPosition(pos);
						pDownloaderMagr->Unlock();
						pDownloaderMagr->FreeTaskHttp(pos);

						pDownloaderMagr->Lock();
						pos = oldpos;						
						continue;
					}
				}
				pos = pDownloaderMagr->GetNextTaskPosition(pos);
			}
			pDownloaderMagr->Unlock();

			Sleep(10);

			// ȥ�����������ص�������״̬,���ĳ����������ɣ�ȡ����
			pDownloaderMagr->Lock();	
			pos = pDownloaderMagr->m_pAssoc->m_dlMap.first();
			while( pos )
			{		
				if( pDownloaderMagr->m_pAssoc->m_bExit ){
					pDownloaderMagr->Unlock();
					return 0;
				}		
				pHttp = (CloHttp*)pDownloaderMagr->m_pAssoc->m_dlMap.get(pos);
				if(pHttp)
				{
					enHttpStatus status = pHttp->GetStatus();//״̬
					if ( status < E_HTTPSTATUS_WAIT || status > E_HTTPSTATUS_BODY )
					{
						oldpos = pDownloaderMagr->m_pAssoc->m_dlMap.next(pos);					
						pRemoveDownloader = (CloHttp*)pDownloaderMagr->m_pAssoc->m_dlMap.remove(pos);	// ��ɻ�ʧ��,Ҫ��m_dlMap����
						assert(pRemoveDownloader == pHttp );					
						SAFE_RELEASE(pRemoveDownloader);

						pos = oldpos;
						continue;
					}
				}
				pos = pDownloaderMagr->m_pAssoc->m_dlMap.next(pos);
			}
			pDownloaderMagr->Unlock();

			Sleep(10);

			// �ӵȴ��������������m_dlerMaxCounts����ִ�����ض�����
			BOOL bTrue = false;
			pDownloaderMagr->Lock();		
			while(1)
			{
				//�˳��߳�
				if( pDownloaderMagr->m_pAssoc->m_bExit ){
					pDownloaderMagr->Unlock();
					return 0;
				}
				if(pDownloaderMagr->m_pAssoc->m_dlMap.size() >= pDownloaderMagr->m_pAssoc->m_dlerMaxCounts )
					break;
				pHttp = pDownloaderMagr->PopupWaitHttp();
				if(!pHttp)
					break;			
				int idx = pDownloaderMagr->m_pAssoc->m_dlMap.add(pHttp);
				if( idx >= 0 )
				{
					if( !pHttp->StartHttp() )
					{
						pDownloaderMagr->m_pAssoc->m_dlMap.remove(idx);                    
						pHttp->m_pAssoc->m_cbFunc->OnHttpCode( pHttp , pHttp->GetUserData() ,E_HTTPSTATUS_ERROR,ERROR_OUTOFMEMORY);
						pHttp->Release();
					}
				}else SAFE_RELEASE(pHttp);
			}
			pDownloaderMagr->Unlock();

			// ��ʱ
			pDownloaderMagr->Lock();
			cTime = GetTickCount();
			if ( pDownloaderMagr->GetWaitHttpCounts() <= 0 && pDownloaderMagr->GetHttpingCounts() <= 0 ) 
			{
				//��ʼ��ʱ
				if(  cTime- dwStartTime >= pDownloaderMagr->m_pAssoc->m_dwTimeout )
				{
					pDownloaderMagr->Unlock();
					return 0;
				}
			}
			else
			{
				dwStartTime = cTime;
			}
			pDownloaderMagr->Unlock();

		} // while(1)
		return 0;
	}

	bool CloHttpMagr::FreeHttp( DLHANDLE dlHandle )
	{
		AUTO_LOCK();

		CloHttpManagerAssoc::Free(&m_pAssoc->m_dlTaskli,dlHandle);
		CloHttpManagerAssoc::Free(&m_pAssoc->m_dlWaitMap,dlHandle);
		CloHttpManagerAssoc::Free(&m_pAssoc->m_dlMap,dlHandle);

		return true;
	}


	bool CloHttpMagr::CancelHttp( DLHANDLE dlHandle )
	{
		CloHttp* phttp = NULL;
		phttp = GetTaskHttp(dlHandle);
		if( phttp ) {
			int nRet = phttp->CancelHttp();
			return (nRet == 1 || nRet == 0 )?true:false;
		}
		return true;
	}
	bool CloHttpMagr::ResetHttp( DLHANDLE dlHandle)
	{
		CloHttp* phttp = GetTaskHttp(dlHandle);	
		if( phttp)
		{
			Lock();
			enHttpStatus status;
			status = phttp->GetStatus();
			if( status == E_HTTPSTATUS_CANCEL || status == E_HTTPSTATUS_ERROR )
			{
				phttp->SetStatus( E_HTTPSTATUS_CLOSED );
			}
			Unlock();
			return true;
		}
		return false;
	}

	CloHttp * CloHttpMagr::GetTaskHttp( DLHANDLE dlHandle )
	{
		if( dlHandle == -1 ) 
			return NULL;

		AUTO_LOCK();
		POSITION_l pos = m_pAssoc->m_dlTaskli.query( call_query_download_with_dlhandle ,(void*)dlHandle);
		if( pos  )
		{
			return  (CloHttp*)m_pAssoc->m_dlTaskli.get( pos );
		}
		return NULL;
	}

	/** �ӵȴ������� popup һ�������� */
	CloHttp* CloHttpMagr::PopupWaitHttp(void)
	{
		POSITION_l pos = m_pAssoc->m_dlWaitMap.first();
		if( pos )
		{
			return (CloHttp*)m_pAssoc->m_dlWaitMap.remove(pos);		
		}
		return NULL;
	}

	/** ����������� ��ȡһ�������� */
	CloHttp* CloHttpMagr::GetTaskHttp(POSITION_l pos)
	{
		return (CloHttp*)m_pAssoc->m_dlTaskli.get(pos);
	}

	/** ����������� ��ȡһ�������� */
	POSITION_l  CloHttpMagr::GetFirstTaskPosition()
	{
		return m_pAssoc->m_dlTaskli.first();
	}

	/** ����������� ��ȡһ�������� */
	POSITION_l  CloHttpMagr::GetNextTaskPosition(POSITION_l pos)
	{
		return m_pAssoc->m_dlTaskli.next(pos);
	}

	bool CloHttpMagr::FreeTaskHttp(POSITION_l pos)
	{
		AUTO_LOCK();

		void* element = m_pAssoc->m_dlTaskli.remove(pos);
		if( element )
		{
			call_free_downloader(element);	
			return true;
		}

		return false;
	}

	int CloHttpMagr::GetWaitHttpCounts(void) const
	{
		return m_pAssoc->m_dlWaitMap.size();
	}

	int CloHttpMagr::GetHttpingCounts(void) const
	{
		return m_pAssoc->m_dlMap.size();
	}

	void CloHttpMagr::FreeAll()
	{
		// check if the thread is stopped.
		if( m_pAssoc->m_hMagrThr &&
			WAIT_OBJECT_0 != ::WaitForSingleObject( m_pAssoc->m_hMagrThr, 0 ) )
		{
			while( ResumeThread(m_pAssoc->m_hMagrThr) > 0){};
		}

		m_pAssoc->m_bExit = true;

		if(m_pAssoc->m_hMagrThr )
		{// �˳��߳�
			WaitForSingleObject( m_pAssoc->m_hMagrThr, 1000 );
			CloseHandle( m_pAssoc->m_hMagrThr );
			m_pAssoc->m_hMagrThr = NULL;
		}

		//
		{
			AUTO_LOCK();

			// ����������
			m_pAssoc->m_dlMap.free(call_free_downloader);

			// �ȴ�������
			m_pAssoc->m_dlWaitMap.free(call_free_downloader);

			// ����������
			m_pAssoc->m_dlTaskli.free(call_free_downloader);

		}

		m_pAssoc->m_bExit = false;
	}

	CloHttp* CloHttpMagr::AddHttp(LPCTSTR szUrl ,bool bFront)
	{	
		if( ! szUrl )
			return NULL;

		AUTO_LOCK();

		CloHttp * phttp = new CloHttp();
		if( ! phttp )
		{	
			return phttp;
		}
		phttp->AddRef();
		phttp->SetHttpUrl( szUrl );	

		POSITION_l insert = NULL;
		DLHANDLE handle = -1;
		if( bFront )
		{
			insert = m_pAssoc->m_dlTaskli.first();
		}
		if( -1 != m_pAssoc->m_dlTaskli.add(phttp,insert) )
		{
			handle = m_pAssoc->m_nextId++;
			phttp->SetHandle(handle);
		}
		else
		{
			phttp->Release();
			phttp = NULL;
		}
		return phttp;
	}

} // namespace lohttp