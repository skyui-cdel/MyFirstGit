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

	// add by loach 释放掉内存 2009-07-27
#define SAFE_DELETE_MEMORYBUFFER(mbuff)\
	{\
	   if( (mbuff)->buffer && (mbuff)->size > 0 ){\
	   delete[] (mbuff)->buffer;\
	   (mbuff)->buffer = NULL;\
	   (mbuff)->size = 0;}\
    }

	// new 内存  2009-07-27
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
	* 任务类型定义
	*/
	enum EHttpTransferType
	{
		E_HTTPTYPE_GET,			// GET任务
		E_HTTPTYPE_DOWNLOAD,		// DOWNLOAD任务，下载到文件的函数。
		E_HTTPTYPE_POST,			// 简单POST任务，POST参数需要指定为表单和非表单。
		E_HTTPTYPE_POST_FILE,	// POST本地文件。
		E_HTTPTYPE_POST_TEXT_FILE,//post二进制与文本混排
		E_HTTPTYPE_UPLOAD,		// UPLOAD本地文件，HTTP1.1的method
		E_HTTPTYPE_DELETE,		// DELETE method,HTTP 1.1
		E_HTTPTYPE_PUT,			// PUT method,HTTP 1.1
	};

	/**
	* 传输状态定义
	*/
	enum EHttpTransferState
	{
		E_HTTPSTATE_STOP,		// 任务已经停止
		E_HTTPSTATE_RUNNING,		// 任务正在运行
	};

	/**
	* 任务运行结果
	*/
	enum EHttpTransferResult
	{
		E_HTTPTRANS_RESULT_UNDONE,		// 任务刚刚初始化，还未开始执行
		E_HTTPTRANS_RESULT_COMPLETE,	// 任务已经执行完毕。
		E_HTTPTRANS_RESULT_FAILED,		// 任务发生意外，没有成功完成。
		E_HTTPTRANS_RESULT_CANCEL,		// 任务被终止。
	};


	/**
	* 定义的一些TaskError
	*/
	enum EHttpTaskErrorCode
	{
		E_HTTPTASK_ERR_INVALID_TASK,		// 无效任务
		E_HTTPTASK_ERR_FAIL_TO_START,		// 启动任务错误
		E_HTTPTASK_ERR_CURL_EXCEPTION,		// CURL有误发生
		E_HTTPTASK_ERR_CANNOT_GET_REMOTE_FILE_NAME,		// 无法获取远程文件名
		E_HTTPTASK_ERR_FAIL_TO_OPEN_LOCAL_FILE,			// 打开本地文件失败
	};

#pragma pack(push)
#pragma pack(4)

	/**
	* @说明：自定的内存Buffer结构体。
	*/
	typedef struct _tagHttpMemoryBuffer
	{
		char *	buffer;				// Buffer内存地址
		size_t	size;				// Buffer大小
		size_t	offset;				// 预留，还未用到
		_tagHttpMemoryBuffer()
		{
			memset(this , 0 , sizeof(*this));
		}
	}THttpMemoryBuffer;

	/**
	* @下载文件时用到的参数结构体，外部程序不用关心。
	*/
	typedef struct _tagHttpFileBuffer
	{
		char	fileName[1024];		// 从远程获取的文件的名称
		size_t	fileSize;			// 从远程获取的文件大小
		FILE *	fp;					// File point
		size_t	offset;				// 保存文件的偏移量
		char *	buffer;				// 数据缓冲，不一定要用到
		size_t	size;				// 数据缓冲大小
		_tagHttpFileBuffer()
		{
			memset(this , 0 , sizeof(*this));
		}
	}THttpFileBuffer;


	/**
	* 代理类型
	*/
	enum EHttpProxyType
	{
		E_PROXYTYPE_NONE,				// 没有代理
		E_PROXYTYPE_HTTP,				// HTTP代理
		E_PROXYTYPE_SOCKS4,			// socks4代理
		E_PROXYTYPE_SOCKS5,			// socks5代理
	};

	typedef struct _tagHttpProxyInfo	// 代理信息结构体
	{
		int		nProxyType;
		char	cServer[1024];		// 代理服务器地址
		unsigned short	wPort;				// 代理服务器端口
		char	cUser[1024];			// 代理服务器用户
		char	cPassword[1024];		// 代理服务器用户密码

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
	* @说明：HttpClient，设置任务属性的结构体。
	* @备注：TTaskParam结构体有自身的构造函数和析构函数，外部程序不需要再手动
	* 		delete pData值。并且外部程序最好是不要使用memset，这样会使里面的初始参数无
	* 		效。
	*		TTaskParam具有”operator =”功能。
	*/
	typedef struct _tagTaskParam
	{
		int		nTransType;			// 任务类型
		char	cURL[1024];			// 访问地址
		char	cHead[4096];		// Http头

		// POST,DELETE,PUT用
		unsigned int uDataLength;		// POST数据长度
		char*	pData;				// POST数据
		BOOL	bFormFormat;		// 使用表单格式POST

		// 上传文件的参数
		char	cLocalFile[1024];	// 本地文件绝对地址，POST文件和UPLOAD文件用

		char    cText[1024];        //用于文件二进制数据与文本混排的post方法

		// 下载任务用的参数
		BOOL	bToBuffer;				// 只下载到Buffer中
		char	cSavePath[1024];		// 本地保存文件路径
		BOOL	bRename;				// 是否使用指定的新文件名
		char	cNewName[1024];		// 本地保存的文件名

		/**
		* @结构初始化
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
		* @赋值重载
		*/
		void operator = (const _tagTaskParam& value) throw()
		{
			nTransType = value.nTransType;
			strcpy_s(cURL, 1024, value.cURL);
			strcpy_s(cHead, 4096, value.cHead);
			strcpy_s(cText,1024,value.cText);

			// POST,DELETE,PUT用
			if( value.uDataLength > 0 && value.pData )
			{
				uDataLength = value.uDataLength;
				pData = new char[value.uDataLength+1];
				memcpy(pData, value.pData, value.uDataLength);
				pData[uDataLength] = '\0';
			}
			bFormFormat = value.bFormFormat;

			// 上传文件的参数
			strcpy_s(cLocalFile, 1024, value.cLocalFile);

			// 下载任务用的参数
			bToBuffer = value.bToBuffer;
			strcpy_s(cSavePath, 1024, value.cSavePath);
			bRename = value.bRename;
			strcpy_s(cNewName, 1024, value.cNewName);
		};
	}TTaskParam;

	// http 参数
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
