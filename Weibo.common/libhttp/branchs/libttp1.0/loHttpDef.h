#ifndef __lo_HTTPDEF_H__
#define __lo_HTTPDEF_H__

#ifdef LIBHTTP_DLL

#ifdef LIBHTTP_EXPORTS
#define loHTTP_API __declspec(dllexport)
#else
#define loHTTP_API __declspec(dllimport)
#endif

#else

#define loHTTP_API 

#endif // LIBHTTP_DLL

#include <common/lo/lodefine.h>

namespace lohttp
{
	enum enHttpStatus
	{
		E_HTTPSTATUS_CLOSED,      ///< �������ر�(���·ŵ���������У��ȴ����أ�
		E_HTTPSTATUS_WAIT,        ///< ���������ڵȴ������У��ȴ�����
		E_HTTPSTATUS_START,       ///< ��������ʼ����(�����߳�����)
		E_HTTPSTATUS_BODY,     ///< ��������ȡĿ¼��Ϣ���磬�ļ���С,���ر�����ļ���...)
		E_HTTPSTATUS_COMPLETE,    ///< �������������	
		E_HTTPSTATUS_CANCEL,      ///< ��������ȡ�� 
		E_HTTPSTATUS_ERROR        ///< ����������
	};

	/** ���ط��� */
	enum enHttpMethod
	{
		E_HTTP_GET_METHOD, ///< GET 
		E_HTTP_GET_BUFFER_METHOD,///<GET
		E_HTTP_POST_METHOD, ///< POST
		E_HTTP_POST_BINARY_METHOD,//POST����������
		E_HTTP_UPLOAD_METHOD, ///< ����
		E_HTTP_UPLOAD_BINARY_TEXT_METHOD,//POST�������������ı�����
		E_HTTP_DELETE_METHOD,///<DELETE
		E_HTTP_PUT_METHOD,///PUT
		E_HTTP_DOWNLOAD_FILE,//GET
	};

	enum ParamAttr
	{
		ParamNormal			= 0x00000000,		//!< The parameter is a normal parameter.
		ParamFile			= 0x00000001,		//!< The parameter is a file parameter.
		//!  This means that the parameter's value contains the file path
		//!  and the file specified by the file path is uploaded when the HTTP UPLOAD is started.
		ParamEncodedName	= 0x00000002,		//!< The parameter's name is a URL-Encoded string.
		//!  This means that the parameter's name is not encoded using URL-encoding
		//!  before sending to the web server when the HTTP GET or POST is started.
		ParamEncodedValue	= 0x00000004,		//!< The parameter's value is a URL-Encoded string.
		//!  This means that the parameter's value is not encoded using URL-encoding
		//!  before sending to the web server when the HTTP GET or POST is started.
		ParamEncoded		= (ParamEncodedName| ParamEncodedValue)	//!< The parameter's name and value are URL-Encoded strings.
		//!  This means that the parameter's name and value are not encoded using URL-encoding
		//!  before sending to the web server when the HTTP GET or POST is started.
	} ;

	/** ���ش���(��ҪΪ����Чʱ��Ĵ���) */
	enum enHTTPCode
	{
		HTTPE_OK = 0,
		HTTPE_UNSUPPORTED_PROTOCOL,    /* 1 */
		HTTPE_FAILED_INIT,             /* 2 */
		HTTPE_URL_MALFORMAT,           /* 3 */
		HTTPE_OBSOLETE4,               /* 4 - NOT USED */
		HTTPE_COULDNT_RESOLVE_PROXY,   /* 5 */
		HTTPE_COULDNT_RESOLVE_HOST,    /* 6 */
		HTTPE_COULDNT_CONNECT,         /* 7 */
		HTTPE_FTP_WEIRD_SERVER_REPLY,  /* 8 */
		HTTPE_REMOTE_ACCESS_DENIED,    /* 9 a service was denied by the server due to lack of access - when login fails this is not returned. */
		HTTPE_OBSOLETE10,              /* 10 - NOT USED */
		HTTPE_FTP_WEIRD_PASS_REPLY,    /* 11 */
		HTTPE_OBSOLETE12,              /* 12 - NOT USED */
		HTTPE_FTP_WEIRD_PASV_REPLY,    /* 13 */
		HTTPE_FTP_WEIRD_227_FORMAT,    /* 14 */
		HTTPE_FTP_CANT_GET_HOST,       /* 15 */
		HTTPE_OBSOLETE16,              /* 16 - NOT USED */
		HTTPE_FTP_COULDNT_SET_TYPE,    /* 17 */
		HTTPE_PARTIAL_FILE,            /* 18 */
		HTTPE_FTP_COULDNT_RETR_FILE,   /* 19 */
		HTTPE_OBSOLETE20,              /* 20 - NOT USED */
		HTTPE_QUOTE_ERROR,             /* 21 - quote command failure */
		HTTPE_HTTP_RETURNED_ERROR,     /* 22 */
		HTTPE_WRITE_ERROR,             /* 23 */
		HTTPE_OBSOLETE24,              /* 24 - NOT USED */
		HTTPE_UPLOAD_FAILED,           /* 25 - failed upload "command" */
		HTTPE_READ_ERROR,              /* 26 - couldn't open/read from file */
		HTTPE_OUT_OF_MEMORY,           /* 27  Note: HTTPE_OUT_OF_MEMORY may sometimes indicate a conversion error instead of a memory allocation error if CURL_DOES_CONVERSIONS is defined */
		HTTPE_OPERATION_TIMEDOUT,      /* 28 - the timeout time was reached */
		HTTPE_OBSOLETE29,              /* 29 - NOT USED */
		HTTPE_FTP_PORT_FAILED,         /* 30 - FTP PORT operation failed */
		HTTPE_FTP_COULDNT_USE_REST,    /* 31 - the REST command failed */
		HTTPE_OBSOLETE32,              /* 32 - NOT USED */
		HTTPE_RANGE_ERROR,             /* 33 - RANGE "command" didn't work */
		HTTPE_HTTP_POST_ERROR,         /* 34 */
		HTTPE_SSL_CONNECT_ERROR,       /* 35 - wrong when connecting with SSL */
		HTTPE_BAD_DOWNLOAD_RESUME,     /* 36 - couldn't resume download */
		HTTPE_FILE_COULDNT_READ_FILE,  /* 37 */
		HTTPE_LDAP_CANNOT_BIND,        /* 38 */
		HTTPE_LDAP_SEARCH_FAILED,      /* 39 */
		HTTPE_OBSOLETE40,              /* 40 - NOT USED */
		HTTPE_FUNCTION_NOT_FOUND,      /* 41 */
		HTTPE_ABORTED_BY_CALLBACK,     /* 42 */
		HTTPE_BAD_FUNCTION_ARGUMENT,   /* 43 */
		HTTPE_OBSOLETE44,              /* 44 - NOT USED */
		HTTPE_INTERFACE_FAILED,        /* 45 - CURLOPT_INTERFACE failed */
		HTTPE_OBSOLETE46,              /* 46 - NOT USED */
		HTTPE_TOO_MANY_REDIRECTS ,     /* 47 - catch endless re-direct loops */
		HTTPE_UNKNOWN_TELNET_OPTION,   /* 48 - User specified an unknown option */
		HTTPE_TELNET_OPTION_SYNTAX ,   /* 49 - Malformed telnet option */
		HTTPE_OBSOLETE50,              /* 50 - NOT USED */
		HTTPE_PEER_FAILED_VERIFICATION,/* 51 - peer's certificate or fingerprint wasn't verified fine */
		HTTPE_GOT_NOTHING,             /* 52 - when this is a specific error */
		HTTPE_SSL_ENGINE_NOTFOUND,     /* 53 - SSL crypto engine not found */
		HTTPE_SSL_ENGINE_SETFAILED,    /* 54 - can not set SSL crypto engine as default */
		HTTPE_SEND_ERROR,              /* 55 - failed sending network data */
		HTTPE_RECV_ERROR,              /* 56 - failure in receiving network data */
		HTTPE_OBSOLETE57,              /* 57 - NOT IN USE */
		HTTPE_SSL_CERTPROBLEM,         /* 58 - problem with the local certificate */
		HTTPE_SSL_CIPHER,              /* 59 - couldn't use specified cipher */
		HTTPE_SSL_CACERT,              /* 60 - problem with the CA cert (path?) */
		HTTPE_BAD_CONTENT_ENCODING,    /* 61 - Unrecognized transfer encoding */
		HTTPE_LDAP_INVALID_URL,        /* 62 - Invalid LDAP URL */
		HTTPE_FILESIZE_EXCEEDED,       /* 63 - Maximum file size exceeded */
		HTTPE_USE_SSL_FAILED,          /* 64 - Requested FTP SSL level failed */
		HTTPE_SEND_FAIL_REWIND,        /* 65 - Sending the data requires a rewind that failed */
		HTTPE_SSL_ENGINE_INITFAILED,   /* 66 - failed to initialise ENGINE */
		HTTPE_LOGIN_DENIED,            /* 67 - user, password or similar was not accepted and we failed to login */
		HTTPE_TFTP_NOTFOUND,           /* 68 - file not found on server */
		HTTPE_TFTP_PERM,               /* 69 - permission problem on server */
		HTTPE_REMOTE_DISK_FULL,        /* 70 - out of disk space on server */
		HTTPE_TFTP_ILLEGAL,            /* 71 - Illegal TFTP operation */
		HTTPE_TFTP_UNKNOWNID,          /* 72 - Unknown transfer ID */
		HTTPE_REMOTE_FILE_EXISTS,      /* 73 - File already exists */
		HTTPE_TFTP_NOSUCHUSER,         /* 74 - No such user */
		HTTPE_CONV_FAILED,             /* 75 - conversion failed */
		HTTPE_CONV_REQD,               /* 76 - caller must register conversion callbacks using curl_easy_setopt options CURLOPT_CONV_FROM_NETWORK_FUNCTION, CURLOPT_CONV_TO_NETWORK_FUNCTION, and CURLOPT_CONV_FROM_UTF8_FUNCTION */
		HTTPE_SSL_CACERT_BADFILE,      /* 77 - could not load CACERT file, missing or wrong format */
		HTTPE_REMOTE_FILE_NOT_FOUND,   /* 78 - remote file not found */
		HTTPE_SSH,                     /* 79 - error from the SSH layer, somewhat generic so the error message will be of interest when this has happened */
		HTTPE_SSL_SHUTDOWN_FAILED,     /* 80 - Failed to shut down the SSL connection */
		HTTPE_AGAIN,                   /* 81 - socket is not ready for send/recv,wait till it's ready and try again (Added in 7.18.2) */
		HTTPE_SSL_CRL_BADFILE,         /* 82 - could not load CRL file, missing or wrong format (Added in 7.19.0) */
		HTTPE_SSL_ISSUER_ERROR,        /* 83 - Issuer check failed.  (Added in 7.19.0) */

		HTTPE_LAST /* never use! */
	};

    const int HTTP_BUFFER_STREAM_LENGTH  = 1024*10;
    const int HTTP_URL_LENGTH = 1024 ;

	/** �������ݻ����� */
	typedef struct  _tagHttpBuffer
	{
	public:
		_tagHttpBuffer(){
			dwLength = 0;
		}
		BYTE  byBuffer[HTTP_BUFFER_STREAM_LENGTH];
		unsigned long dwLength;
	}THttpBuffer;

	/** http ����ʱ���¼ */
	typedef struct _tagHttpTime
	{
		TCHAR	szURL[1024];
		time_t	tRequest;
		time_t	tResponse;
		BOOL	bSucceed;
		_tagHttpTime()
		{
			memset(this, 0, sizeof(_tagHttpTime) );
		}
	}THttpTime;


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
		virtual int  OnTaskBuffer(BYTE* pBuffer, long lBuffer) = 0;

		/** ������
		 * @param code = enHTTPCode
		*/
		virtual void  OnTaskCode(long code, const char* errString) = 0;

		/** �������ִ��ʱ�� */
		virtual void OnTaskTime(const THttpTime * pDownloadTime) = 0 ;
	};

} // namespace lohttp

#endif // __lo_HTTPDEF_H__
