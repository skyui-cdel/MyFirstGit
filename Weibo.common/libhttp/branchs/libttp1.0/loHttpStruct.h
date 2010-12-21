#ifndef __lo_HTTPSTRUCT_H__
#define __lo_HTTPSTRUCT_H__

namespace lohttp
{
	//
#define Http_Char_New(p,size) \
	(p) = new TCHAR[size]; 

	//
#define Http_Char_Free(p) \
	{ if(p) delete[] (p); (p) = NULL ;}

	// add by loach �ͷŵ��ڴ� 2009-07-27
#define SAFE_DELETE_MEMORYBUFFER(mbuff)\
	{\
	   if( (mbuff)->buffer && (mbuff)->size > 0 ){\
	   delete[] (mbuff)->buffer;\
	   (mbuff)->buffer = NULL;\
	   (mbuff)->size = 0;}\
    }

	// new �ڴ�  2009-07-27
#define SAFE_NEW_MEMORYBUFFER(mbuff,buffsize,len)\
	{\
	   SAFE_DELETE_MEMORYBUFFER(mbuff);\
	   (mbuff)->buffer = new char[buffsize];\
	   (mbuff)->size = len;\
	}

	// copy memory buffer
#define COPY_MEMORYBUFFER(msrbuff,mdestbuff)\
	{\
	   if( (msrbuff)->buffer && (msrbuff)->size >= 0 && (mdestbuff)->buffer && (mdestbuff)->size >= (msrbuff)->size )\
	   memcpy( (mdestbuff)->buffer, (msrbuff)->buffer, (msrbuff)->size);\
	}

#define SAFE_FREE_BUFFER(p)    if( p ) { free(p); (p) = NULL; }

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)		   if( p ) { delete (p); (p)=NULL;}
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p)	if( p ) { delete[] (p); (p)=NULL;}
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)		   if( p ) { (p)->Release(); (p) = NULL; } 
#endif

	/**
	* �������Ͷ���
	*/
	enum EHttpTransferType
	{
		E_HTTPTYPE_GET,			// GET����
		E_HTTPTYPE_DOWNLOAD,		// DOWNLOAD�������ص��ļ��ĺ�����
		E_HTTPTYPE_POST,			// ��POST����POST������Ҫָ��Ϊ���ͷǱ���
		E_HTTPTYPE_POST_FILE,	// POST�����ļ���
		E_HTTPTYPE_POST_TEXT_FILE,//post���������ı�����
		E_HTTPTYPE_UPLOAD,		// UPLOAD�����ļ���HTTP1.1��method
		E_HTTPTYPE_DELETE,		// DELETE method,HTTP 1.1
		E_HTTPTYPE_PUT,			// PUT method,HTTP 1.1
	};

	/**
	* ����״̬����
	*/
	enum EHttpTransferState
	{
		E_HTTPSTATE_STOP,		// �����Ѿ�ֹͣ
		E_HTTPSTATE_RUNNING,		// ������������
	};

	/**
	* �������н��
	*/
	enum EHttpTransferResult
	{
		E_HTTPTRANS_RESULT_UNDONE,		// ����ոճ�ʼ������δ��ʼִ��
		E_HTTPTRANS_RESULT_COMPLETE,	// �����Ѿ�ִ����ϡ�
		E_HTTPTRANS_RESULT_FAILED,		// ���������⣬û�гɹ���ɡ�
		E_HTTPTRANS_RESULT_CANCEL,		// ������ֹ��
	};


	/**
	* �����һЩTaskError
	*/
	enum EHttpTaskErrorCode
	{
		E_HTTPTASK_ERR_INVALID_TASK,		// ��Ч����
		E_HTTPTASK_ERR_FAIL_TO_START,		// �����������
		E_HTTPTASK_ERR_CURL_EXCEPTION,		// CURL������
		E_HTTPTASK_ERR_CANNOT_GET_REMOTE_FILE_NAME,		// �޷���ȡԶ���ļ���
		E_HTTPTASK_ERR_FAIL_TO_OPEN_LOCAL_FILE,			// �򿪱����ļ�ʧ��
	};

#pragma pack(push)
#pragma pack(4)

	/**
	* @˵�����Զ����ڴ�Buffer�ṹ�塣
	*/
	typedef struct _tagHttpMemoryBuffer
	{
		char *	buffer;				// Buffer�ڴ��ַ
		size_t	size;				// Buffer��С
		size_t	offset;				// Ԥ������δ�õ�
		_tagHttpMemoryBuffer()
		{
			memset(this , 0 , sizeof(*this));
		}
	}THttpMemoryBuffer;

	/**
	* @�����ļ�ʱ�õ��Ĳ����ṹ�壬�ⲿ�����ù��ġ�
	*/
	typedef struct _tagHttpFileBuffer
	{
		char	fileName[1024];		// ��Զ�̻�ȡ���ļ�������
		size_t	fileSize;			// ��Զ�̻�ȡ���ļ���С
		FILE *	fp;					// File point
		size_t	offset;				// �����ļ���ƫ����
		char *	buffer;				// ���ݻ��壬��һ��Ҫ�õ�
		size_t	size;				// ���ݻ����С
		_tagHttpFileBuffer()
		{
			memset(this , 0 , sizeof(*this));
		}
	}THttpFileBuffer;


	/**
	* ��������
	*/
	enum EHttpProxyType
	{
		E_PROXYTYPE_NONE,				// û�д���
		E_PROXYTYPE_HTTP,				// HTTP����
		E_PROXYTYPE_SOCKS4,			// socks4����
		E_PROXYTYPE_SOCKS5,			// socks5����
	};

	typedef struct _tagHttpProxyInfo	// ������Ϣ�ṹ��
	{
		int		nProxyType;
		char	cServer[1024];		// �����������ַ
		unsigned short	wPort;				// ����������˿�
		char	cUser[1024];			// ����������û�
		char	cPassword[1024];		// ����������û�����

		_tagHttpProxyInfo()
		{
			nProxyType = E_PROXYTYPE_NONE;
			wPort = 808;
			memset(cServer, 0, 1024);
			memset(cUser, 0, 1024);
			memset(cPassword, 0, 1024);
		}
	}THttpProxyInfo;

	/**
	* @˵����HttpClient�������������ԵĽṹ�塣
	* @��ע��TTaskParam�ṹ��������Ĺ��캯���������������ⲿ������Ҫ���ֶ�
	* 		delete pDataֵ�������ⲿ��������ǲ�Ҫʹ��memset��������ʹ����ĳ�ʼ������
	* 		Ч��
	*		TTaskParam���С�operator =�����ܡ�
	*/
	typedef struct _tagTaskParam
	{
		int		nTransType;			// ��������
		char	cURL[1024];			// ���ʵ�ַ
		char	cHead[4096];		// Httpͷ

		// POST,DELETE,PUT��
		unsigned int uDataLength;		// POST���ݳ���
		char*	pData;				// POST����
		BOOL	bFormFormat;		// ʹ�ñ���ʽPOST

		// �ϴ��ļ��Ĳ���
		char	cLocalFile[1024];	// �����ļ����Ե�ַ��POST�ļ���UPLOAD�ļ���

		char    cText[1024];        //�����ļ��������������ı����ŵ�post����

		// ���������õĲ���
		BOOL	bToBuffer;				// ֻ���ص�Buffer��
		char	cSavePath[1024];		// ���ر����ļ�·��
		BOOL	bRename;				// �Ƿ�ʹ��ָ�������ļ���
		char	cNewName[1024];		// ���ر�����ļ���

		/**
		* @�ṹ��ʼ��
		*/
		_tagTaskParam()
		{
			nTransType = E_HTTPTYPE_GET;

			memset(cURL, 0, 1024);
			memset(cHead, 0, 4096);

			//
			pData = NULL;
			uDataLength = 0;
			bFormFormat = TRUE;

			//
			memset(cLocalFile, 0, 1024);

			memset(cText,0,1024);

			//
			bToBuffer = FALSE;
			memset(cSavePath, 0, 1024);
			bRename = FALSE;
			memset(cNewName, 0, 1024);
		}

		~_tagTaskParam()
		{
			if( pData )
			{
				delete[] pData;
				pData = NULL;
			}
		}

		/**
		* @��ֵ����
		*/
		void operator = (const _tagTaskParam& value) throw()
		{
			nTransType = value.nTransType;
			strcpy_s(cURL, 1024, value.cURL);
			strcpy_s(cHead, 4096, value.cHead);
			strcpy_s(cText,1024,value.cText);

			// POST,DELETE,PUT��
			if( value.uDataLength > 0 && value.pData )
			{
				uDataLength = value.uDataLength;
				pData = new char[value.uDataLength+1];
				memcpy(pData, value.pData, value.uDataLength);
				pData[uDataLength] = '\0';
			}
			bFormFormat = value.bFormFormat;

			// �ϴ��ļ��Ĳ���
			strcpy_s(cLocalFile, 1024, value.cLocalFile);

			// ���������õĲ���
			bToBuffer = value.bToBuffer;
			strcpy_s(cSavePath, 1024, value.cSavePath);
			bRename = value.bRename;
			strcpy_s(cNewName, 1024, value.cNewName);
		};
	}TTaskParam;

	// http ����
	typedef struct _tagHttpURLParam
	{
		TCHAR*  pszName;
		TCHAR*  pszValue;
		unsigned long	dwAttrib;
		_tagHttpURLParam()
		{
			pszName = NULL;
			pszValue = NULL;
			dwAttrib = 0;
		};
		~_tagHttpURLParam()
		{
			Http_Char_Free( pszName );
			Http_Char_Free( pszValue );
		};
	}THttpURLParam;

	typedef struct _tagHttpHeader
	{
		TCHAR* pszName;
		TCHAR* pszValue;

		_tagHttpHeader()
		{
			pszName = NULL;
			pszValue = NULL;
		};
		~_tagHttpHeader()
		{
			Http_Char_Free( pszName );
			Http_Char_Free( pszValue );
		};
	}THttpHeader;
    
#pragma pack(pop)

} // namespace lohttp

#endif // __lo_HTTPSTRUCT_H__
