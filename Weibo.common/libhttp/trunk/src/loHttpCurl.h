#ifndef __lo_HTTP_CURL_H__
#define __lo_HTTP_CURL_H__

#include <string>
#include "loHttpStruct.h"
#include "loHttpDef.h"

namespace lohttp
{
	/** �ص����� */
	struct loNovtableM IloTaskCallBack
	{
		/** ����״̬ */
		virtual void  OnTaskState(long lState) = 0;

		/** ���ؽ���
		 *
		 * @return 1,��֮�ײ� ��
		*/
		virtual int  OnTaskProgress(long lCurBytes, long lTotalBytes) = 0;

		/** ���ػ����������㿪���߱����ļ���
		 *
		 * @return !=0 ��ʾ����Ҫ���������buffer ���ǿ�������Ҫ�ģ�
		*/
		virtual int  OnTaskBuffer(unsigned char* pBuffer, long lBuffer) = 0;

		/** ������
		 * @param code = enHTTPCode
		*/
		virtual void  OnTaskCode(long code, const char* errString) = 0;

		/** �������ִ��ʱ�� */
		virtual void OnTaskTime(const THttpTime * pDownloadTime) = 0 ;
	};

	/** ǰ���������� */
	class CloHttpCurlAssoc;

	/** ���� libcurl */
	class CloHttpCurl 
	{
	public:
		CloHttpCurl(IloTaskCallBack* pTaskCallback);
		~CloHttpCurl(void);

	public:
		/**
		* @ ��ȡ����ID
		*/
		long GetTaskID() const;

		/**
		* @ ��ʼ��ȡ����
		*/
		int	 Request();		// ��ȡ���ݣ�ͬ�������Ĺ���

		/**
		* @ ��ֹ����
		*/
		void CancelRequest();

		/**
		* @ ���ô�����Ϣ
		*/
		void SetProxyInfo(THttpProxyInfo &ProxyInfo);

		/**
		* @ ���ô�����Ϣ
		*/
		int	SetTransferParam(int nType, LPVOID lpParam);

		/**
		* @ �ж������Ƿ���Ч
		*/
		BOOL IsTaskValid();

		/**
		* @ ��ȡ����
		*/
		int	GetType() const;
		int	GetState() const;
		int	GetErrCode() const;

		long GetHttpCode() const;
		/**
		* @ ��ȡ���
		*/
		int	GetResponedHeader(THttpMemoryBuffer * pHeaderBuf);
		int	GetResponedBody(THttpMemoryBuffer * pBodyBuf);

		int	Step();		// Managerר�ú���������

		void SetUserAgent(const char* agent);
	protected:
		BOOL IsUserCancel() const;

		/** ����http */
		int	 TransferProc();


		int	 TransferProcGET();

		/** ���ص��ļ��� */
		int	 TransferProcDOWNLOAD();

		/** ���ص� Buffer; */
		int	 TransferProcDOWNLOAD_TOBUFFER();

		/** post һ��post���� */
		int	 TransferProcPOST();

		/** post �ļ� */
		int	 TransferProcPOSTFormData();
//		int  TransferProcPOSTFormData2();

//		/** �ϴ��ļ�*/
//		int	 TransferProcUPLOAD();

		/** delete operator */
		int	 TransferProcDELETE();

		/** put operator */
		int	 TransferProcPUT();

		//
		void SetTransferCallback(LPVOID headerData, LPVOID bodyData);

		// ֪ͨ httptask
	private:
		/** ״̬֪ͨ��EHttpTransferState�� */
		void TaskState(int nState);

		/** �������� */
		int TaskProgress(long lCurBytes, long lTotalBytes);

		/** ������ */
		int TaskBuffer(unsigned char* pBuffer, long lLength);

		/** ���� */
        void TaskCode(void);
	private:
		void ClearBuffer();
		void ClearParam();

		/** �����ϴ� */
		int	 StartTransfer();

		/** ֹͣ���� */
		void StopTransfer();

		void SetState(int nNewState);

		/** �������� */
		void UpdateProxy(char* lpszRequestURL);

		// user agent
		const char* GetUserAgent() const;
	private:
		/** body ���ݻص� */
		static size_t ToBodyCallback(void *ptr, size_t size, size_t nmemb, void *data);

		/** header ���ݻص� */
		static size_t ToHeaderCallback(void *ptr, size_t size, size_t nmemb, void *data);

		/** ������д���ļ���*/
		static size_t ToFileCallback(void *ptr, size_t size, size_t nmemb, void *data);

		/** �����ݻ��������ͨ�� TaskBuffer() */
		static size_t ToBufferCallback(void *ptr, size_t size, size_t nmemb, void *data);

		/** ���ݽ��� */
		static size_t ProgressCallback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);
	protected:
		friend class CloHttpCurlAssoc;
		CloHttpCurlAssoc* m_pAssoc;
	};

} // namespace lohttp

#endif /*__lo_HTTP_CURL_H__ */
