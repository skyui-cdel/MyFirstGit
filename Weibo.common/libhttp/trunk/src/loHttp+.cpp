#include "stdafx.h"
#include "loHttp+.h"
#include "loHttp.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

BEGIN_EXTERN_C

using namespace lohttp;

struct t_lo_http
{
	/*下载大文件(内存缓冲区) 回调接口［边下载中 ，与OnHttpBody有所不同 ］ */
	fun_httpbuffer_callback fbuffer_cb;

	/* 进度状态 */
	fun_httpprogress_callback fprogress_cb;

	/** 下载状态
	*
	* @param state 状态(成功，失败）
	* @param errCode 失败码
	*/
	fun_httpcode_callback fcode_cb;

	fun_httprespone_callback frespone_cb;

	/** CloHttp 资源释放掉 */
	fun_httprelease_callback frelease_cb;

	// 用户自定义数据
	void* userdata;

	// 请求方法
	int   method;
	//
	CloHttp* pHttp;
};

/*下载大文件(内存缓冲区) 回调接口［边下载中 ，与OnHttpBody有所不同 ］ */
static 
int httpbuffer_callback(unsigned char* pBuffer , long lBuffer, void* pUsrData )
{
	struct t_lo_http* phttpstruct = (struct t_lo_http*)pUsrData;

	if( phttpstruct && phttpstruct->fbuffer_cb )
	{
		return phttpstruct->fbuffer_cb(pBuffer , lBuffer , phttpstruct->userdata );
	}
	return 0;
}

/* 进度状态 */
static 
int httpprogress_callback(long lCurBytes , long lTotalBytes, void* pUsrData )
{
	struct t_lo_http* phttpstruct = (struct t_lo_http*)pUsrData;

	if( phttpstruct && phttpstruct->fprogress_cb )
	{
		return phttpstruct->fprogress_cb(lCurBytes , lTotalBytes , phttpstruct->userdata );
	}
	return 0;
}

/** 下载状态
*
* @param state 状态(成功，失败）
* @param errCode 失败码
*/
static
void httpcode_callback(int state , int errCode ,const THttpTime* pTime , void* pUsrData )
{
	struct t_lo_http* phttpstruct = (struct t_lo_http*)pUsrData;

	if( phttpstruct && phttpstruct->fcode_cb )
	{
		phttpstruct->fcode_cb(state , errCode , pTime , phttpstruct->userdata );
	}
}

static
void httprespone_callback(const LOHttpResponse* resp , void* pUsrData)
{
	struct t_lo_http* phttpstruct = (struct t_lo_http*)pUsrData;

	if( phttpstruct && phttpstruct->frespone_cb )
	{
		phttpstruct->frespone_cb(resp , phttpstruct->userdata );
	}
}

/** CloHttp 资源释放掉 */
static
void httprelease_callback(void* pUsrData)
{
	struct t_lo_http* phttpstruct = (struct t_lo_http*)pUsrData;
}

// 创建
loHTTP_API LOHttp* Http_createobject(void)
{
	struct t_lo_http* phttpstruct = (struct t_lo_http*)malloc( sizeof( struct t_lo_http) );
	if( !phttpstruct )
		return 0;

	memset(phttpstruct , 0 , sizeof(struct t_lo_http));

	//
	phttpstruct->pHttp = new CloHttp();
	if( !phttpstruct->pHttp )
	{
		free( phttpstruct );
		return NULL;
	}

	// 设置回调函数
	phttpstruct->pHttp->SetCallbackOfBuffer  ( httpbuffer_callback );
	phttpstruct->pHttp->SetCallbackOfCode    ( httpcode_callback );
	phttpstruct->pHttp->SetCallbackOfProgress( httpprogress_callback );
	phttpstruct->pHttp->SetCallbackOfRespone ( httprespone_callback );
	phttpstruct->pHttp->SetCallbackOfRelease ( httprelease_callback );

	//自定义数据
	phttpstruct->pHttp->SetUserData( phttpstruct );

	return phttpstruct;
}

//--------------------------------------------------------------------------------------//
// option
// for declare function
#define LOHTTP_option_callback(name) \
	static int LOHTTP_option_##name##_cb(struct t_lo_http* phttpstruct, va_list param)

// for enum
#define LOHTTP_option_fun(name) LOHTTP_option_##name##_cb


LOHTTP_option_callback(LOHTTP_OPT_UNK)
{
	return -1;
}

LOHTTP_option_callback(LOHTTP_OPT_callbackOfBuffer)
{
	phttpstruct->fbuffer_cb = va_arg(param , fun_httpbuffer_callback );
	return 0;
}

LOHTTP_option_callback(LOHTTP_OPT_callbackOfProgress)///< 请求结果包回调函数
{
	phttpstruct->fprogress_cb = va_arg(param , fun_httpprogress_callback );
	return 0;
}

LOHTTP_option_callback(LOHTTP_OPT_callbackOfCode) ///< 当请求被消毁时，通过回调函数告开发者
{
	phttpstruct->fcode_cb = va_arg(param , fun_httpcode_callback );
	return 0;
}

LOHTTP_option_callback(LOHTTP_OPT_callbackOfRespone)
{
	phttpstruct->frespone_cb = va_arg(param ,fun_httprespone_callback);
	return 0;
}

LOHTTP_option_callback(LOHTTP_OPT_callbackOfRelease) ///< 当请求被消毁时，通过回调函数告开发者
{
	phttpstruct->frelease_cb = va_arg(param , fun_httprelease_callback );
	return 0;
}

LOHTTP_option_callback(LOHTTP_OPT_userdata) ///< 当请求被消毁时，通过回调函数告开发者
{
	phttpstruct->userdata = va_arg(param , void*);
	return 0;
}

LOHTTP_option_callback(LOHTTP_OPT_setmethod) ///< 当请求被消毁时，通过回调函数告开发者
{
	phttpstruct->method = va_arg(param , int);
	return 0;
}

LOHTTP_option_callback(LOHTTP_OPT_addparam)
{
	const HTTPChar* szName = 0, *szValue = 0;
	if( !phttpstruct->pHttp )
		return -1;

	szName = va_arg(param , const HTTPChar*);
	if( szName && *szName != '\0' )
	{
		szValue = va_arg(param , const HTTPChar*);
		phttpstruct->pHttp->AddParam(szName , szValue , va_arg(param , unsigned long) );
	}
	return 0;
}

LOHTTP_option_callback(LOHTTP_OPT_clearparam)
{
	if( !phttpstruct->pHttp )
		return -1;

	phttpstruct->pHttp->ClearParams();

	return 0;
}

LOHTTP_option_callback(LOHTTP_OPT_addheader)
{
	const HTTPChar* szName = 0, *szValue = 0;
	if( !phttpstruct->pHttp )
		return -1;

	szName = va_arg(param , const HTTPChar*);
	szValue = va_arg(param , const HTTPChar*);
	phttpstruct->pHttp->AddHeader(szName , szValue);

	return 0;
}

LOHTTP_option_callback(LOHTTP_OPT_clearheader)
{
	if( !phttpstruct->pHttp )
		return -1;
	phttpstruct->pHttp->ClearHeader();
	return 0;
}

LOHTTP_option_callback(LOHTTP_OPT_addbinarydata)
{
	const char* szdata = 0;
	if( !phttpstruct->pHttp )
		return -1;

	szdata = va_arg(param , const char*);
	phttpstruct->pHttp->AddBinaryData( szdata , va_arg(param , long) );

	return 0;
}

LOHTTP_option_callback(LOHTTP_OPT_clearbinarydata)
{
	if( !phttpstruct->pHttp )
		return -1;

	phttpstruct->pHttp->ClearBinaryData();
	return 0;
}

LOHTTP_option_callback(LOHTTP_OPT_setfile)
{
	const HTTPChar* szPath = 0, *szFile = 0;
	if( !phttpstruct->pHttp )
		return -1;

	szPath = va_arg(param , const HTTPChar*);
	szFile = va_arg(param , const HTTPChar*);
	phttpstruct->pHttp->SetFileName(szPath , szFile);

	return 0;
}

LOHTTP_option_callback(LOHTTP_OPT_setproxysrvaddr)
{
	int proxyType = 0;
	const HTTPChar* szProxySrv = 0;	
	unsigned short nProxySrvPort = 80;

	if( !phttpstruct->pHttp )
		return -1;

	proxyType = va_arg(param , int);
	szProxySrv = va_arg(param , const HTTPChar*);
	nProxySrvPort = va_arg(param , unsigned short);

	phttpstruct->pHttp->SetProxySrvAddr(proxyType , szProxySrv , nProxySrvPort);

	return 0;
}

LOHTTP_option_callback(LOHTTP_OPT_setproxyaccount)
{
	const HTTPChar* szUserName = 0, *szPassword = 0;
	if( !phttpstruct->pHttp )
		return -1;

	szUserName = va_arg(param , const HTTPChar*);
	szPassword = va_arg(param , const HTTPChar*);
	phttpstruct->pHttp->SetProxyAccount(szUserName , szPassword);

	return 0;
}

LOHTTP_option_callback(LOHTTP_OPT_setuseragent)
{
	if( !phttpstruct->pHttp )
		return -1;

	phttpstruct->pHttp->SetUserAgent(va_arg(param , const HTTPChar*) );

	return 0;
}

LOHTTP_option_callback(LOHTTP_OPT_setform)
{
	phttpstruct->pHttp->SetHttpForm(va_arg(param , const struct THttpForm*) );
	return 0;
}


//
//
typedef int (*fun_http_option_callback)(struct t_lo_http* phttpstruct , va_list param);
static
fun_http_option_callback vector_http_option_cb[ LOHTTP_OPT_LAST ] = {
	LOHTTP_option_fun(LOHTTP_OPT_UNK),
	LOHTTP_option_fun(LOHTTP_OPT_callbackOfBuffer),
	LOHTTP_option_fun(LOHTTP_OPT_callbackOfProgress),
	LOHTTP_option_fun(LOHTTP_OPT_callbackOfCode),
	LOHTTP_option_fun(LOHTTP_OPT_callbackOfRespone),
	LOHTTP_option_fun(LOHTTP_OPT_callbackOfRelease),
	LOHTTP_option_fun(LOHTTP_OPT_userdata),
	LOHTTP_option_fun(LOHTTP_OPT_setmethod),
	LOHTTP_option_fun(LOHTTP_OPT_addparam),
	LOHTTP_option_fun(LOHTTP_OPT_clearparam),
	LOHTTP_option_fun(LOHTTP_OPT_addheader),
	LOHTTP_option_fun(LOHTTP_OPT_clearheader),
	LOHTTP_option_fun(LOHTTP_OPT_addbinarydata),
	LOHTTP_option_fun(LOHTTP_OPT_clearbinarydata),
	LOHTTP_option_fun(LOHTTP_OPT_setfile),
	LOHTTP_option_fun(LOHTTP_OPT_setproxysrvaddr),
	LOHTTP_option_fun(LOHTTP_OPT_setproxyaccount),
	LOHTTP_option_fun(LOHTTP_OPT_setuseragent),
	LOHTTP_option_fun(LOHTTP_OPT_setform),
};


loHTTP_API int Http_setopt(LOHttp* phttp , LOHttpoption option , ...)
{
	int iRet = -1;
	va_list arg;

	struct t_lo_http* phttpstruct = (struct t_lo_http*)phttp;
	if( !phttpstruct )
	{
		return -1;
	}

	if( LOHTTP_OPT_UNK > option ||
        LOHTTP_OPT_LAST <= option )
	{
		return -1;
	}

	va_start(arg, option);

	iRet = vector_http_option_cb[option](phttpstruct , arg );

	va_end(arg);

	return iRet;	
}

/**
* @brief 设置下载文件URL
* @param szUrl 要下载的文件Url
* @param szSavePath 另存为的本地路径，可以是目录或者具体文件路径。
* @param lpszExt 扩展名
* @return true表示成功初始化下载工作，false表示错误
*/
loHTTP_API void Http_seturl(LOHttp* phttp, const HTTPChar* szUrl)
{
	struct t_lo_http* phttpstruct = (struct t_lo_http*)phttp;
	if( !phttpstruct || !phttpstruct->pHttp )
		return ;

	return phttpstruct->pHttp->SetURL(szUrl);
}

/**
* @brief 开始下载(异步下载,内部有线程）
* @param method 参考 enHttpMethod
*/
loHTTP_API int Http_start_asyn(LOHttp* phttp )
{
	struct t_lo_http* phttpstruct = (struct t_lo_http*)phttp;
	if( !phttpstruct || !phttpstruct->pHttp )
		return -1;

	return phttpstruct->pHttp->RequestAsyn( NULL , phttpstruct->method );
}

loHTTP_API void Http_wait(LOHttp* phttp)
{
	struct t_lo_http* phttpstruct = (struct t_lo_http*)phttp;
	if( !phttpstruct || !phttpstruct->pHttp )
		return ;

	phttpstruct->pHttp->Wait();
	return ;
}

/** 同步执行 ，阻塞线程
*
* @param method 参考 enHttpMethod
*/
loHTTP_API int Http_start(LOHttp* phttp)
{
	struct t_lo_http* phttpstruct = (struct t_lo_http*)phttp;
	if( !phttpstruct || !phttpstruct->pHttp )
		return -1;

	return phttpstruct->pHttp->Request( NULL , phttpstruct->method );
}

/**
* @brief 取消下载操作
* @param msTimeout 等待的超时时间
* @return 1 表示已经成功取消， 0 表示下载未开始。-1 表示等待超时。
* @remark 当返回 1 或 -1 时，下载器在成功取消后都会
*/
loHTTP_API int Http_cancel(LOHttp* phttp ,  unsigned long msTimeout)
{
	struct t_lo_http* phttpstruct = (struct t_lo_http*)phttp;
	if( !phttpstruct || !phttpstruct->pHttp )
		return 0;

	if( phttpstruct->pHttp->Cancel() )
		return 0;

	return -1;
}

// 消毁
loHTTP_API void Http_destroyobject(LOHttp* phttp)
{
	struct t_lo_http* phttpstruct = (struct t_lo_http*)phttp;
	if( !phttpstruct )
		return ;

	if( phttpstruct->pHttp )
	{
		//
		phttpstruct->pHttp->Cancel();

		delete phttpstruct->pHttp;
		phttpstruct->pHttp = 0;
	}

	if( phttpstruct->frelease_cb && phttpstruct->userdata )
	{//
		phttpstruct->frelease_cb(phttpstruct->userdata);
		phttpstruct->userdata = 0;
	}
	//
	free( phttpstruct );

	return ;
}

//--------------------------------------------------------------------------------------------//
// LOHttpRespone


// 请求完后，可能获取 回应包
loHTTP_API LOHttpResponse* Http_getRespone(LOHttp* phttp)
{
	struct t_lo_http* phttpstruct = (struct t_lo_http*)phttp;
	if( !phttpstruct || !phttpstruct->pHttp )
		return 0;

	return (void*)phttpstruct->pHttp->GetRespone();
}

// 请求完后，分离 回应包
loHTTP_API LOHttpResponse* Http_detachRespone(LOHttp* phttp)
{
	struct t_lo_http* phttpstruct = (struct t_lo_http*)phttp;
	if( !phttpstruct || !phttpstruct->pHttp )
		return 0;

	return (void*)phttpstruct->pHttp->DetachRespone();
}


loHTTP_API long Http_resp_code(const LOHttpResponse* resp)
{
	const CloHttpResponse* phttpRespone = (const CloHttpResponse*)resp;
	if( !phttpRespone )
		return 0;

	return phttpRespone->GetHttpCode();
}

loHTTP_API bool Http_resp_getheaderlength(const LOHttpResponse* resp, unsigned long& dwHeaderLen )
{
	const CloHttpResponse* phttpRespone = (const CloHttpResponse*)resp;
	if( !phttpRespone )
		return false;

	return phttpRespone->GetHeaderLength(dwHeaderLen);
}

loHTTP_API bool Http_resp_readheader(const LOHttpResponse* resp , fun_http_resp_header_callback cb , void* pUsrData )
{
	const CloHttpResponse* phttpRespone = (const CloHttpResponse*)resp;
	if( !phttpRespone )
		return false;

	return phttpRespone->ReadHeaderCallback( cb , pUsrData );
}

/** body length */
loHTTP_API bool Http_resp_getbodylength(const LOHttpResponse* resp , unsigned long& dwBodyLen)
{
	const CloHttpResponse* phttpRespone = (const CloHttpResponse*)resp;
	if( !phttpRespone )
		return false;

	return phttpRespone->GetBodyLength(dwBodyLen);
}

/** 移位读取，它会偏移 ，直到把整个读取完成 */
loHTTP_API bool Http_resp_readbody(const LOHttpResponse* resp ,fun_http_resp_body_callback cb , void* pUsrData )
{
	const CloHttpResponse* phttpRespone = (const CloHttpResponse*)resp;
	if( !phttpRespone )
		return false;

	return phttpRespone->ReadBodyCallback( cb , pUsrData );
}

/** 注：只有 Http_detachRespone 结合使用，否则会想不到的后果 */
loHTTP_API void Http_resp_destroy(LOHttpResponse* resp)
{
	CloHttpResponse* phttpRespone = (CloHttpResponse*)resp;

	if( phttpRespone )
	{
		delete phttpRespone;
		phttpRespone = 0;
	}
}

//--------------------------------------------------------------------------------------------//
// LOHttpHeader


loHTTP_API LOHttpHeader* Http_header_create(void)
{
	return new CloHttpHeader();
}

/** 分析 */
loHTTP_API void Http_header_parse(LOHttpHeader* header , const char* context , unsigned long len)
{
	CloHttpHeader* phttpheader = (CloHttpHeader*)header;
	if( !phttpheader )
		return ;

	phttpheader->Parse(context , len );
}

/** "Content-Length" 字段的长度
* @return 存在 “Content-Length”，返回true
*/
loHTTP_API bool Http_header_getcontentlength (const LOHttpHeader* resp,unsigned long & cbContLen)
{
	const CloHttpHeader* phttpheader = (const CloHttpHeader*)resp;
	if( !phttpheader )
		return false;

	return phttpheader->GetContentLength(cbContLen);
}

/** http header information */
loHTTP_API int Http_header_count(const LOHttpHeader* header , const HTTPChar* szName)
{
	const CloHttpHeader* phttpheader = (const CloHttpHeader*)header;
	if( !phttpheader )
		return 0;

	return phttpheader->GetHeadercount(szName);
}

loHTTP_API const HTTPChar*	Http_header_name(const LOHttpHeader* header , const HTTPChar* szName, unsigned long nIdx)
{
	const CloHttpHeader* phttpheader = (const CloHttpHeader*)header;
	if( !phttpheader )
		return 0;

	return phttpheader->GetHeader(szName , nIdx );
}

/** 读取类似于 " 200 OK 这样的信息 "*/
loHTTP_API const HTTPChar*	Http_header_state(const LOHttpHeader* header)
{
	const CloHttpHeader* phttpheader = (const CloHttpHeader*)header;
	if( !phttpheader )
		return 0;

	return phttpheader->GetHeaderState();
}

loHTTP_API void Http_header_destroy(LOHttpHeader* header )
{
	CloHttpHeader* phttpheader = (CloHttpHeader*)header;
	if( !phttpheader )
		return ;

	delete phttpheader;
}

END_EXTERN_C
