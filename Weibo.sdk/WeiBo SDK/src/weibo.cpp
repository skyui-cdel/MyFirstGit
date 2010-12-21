/**
*  @brief  Interface function implements
*  @file   weibo.cpp
*  @author loach 
*  @Email <loachmr@sina.com>
*
* Copyright (C) 1996-2010 SINA Corporation, All Rights Reserved
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
*/

#include "weibo.h"
#include "wbOption.h"
#include "wbRequest.h"
#include "wbURL.h"
#include <stdlib.h>
#include <string.h>


#ifdef  __cplusplus
extern "C" {
#endif

	WEIBO_API void loWeibo_oauth_copy(struct t_wb_oauth* pdest , const struct t_wb_oauth* psrc)
	{
		if( psrc && pdest )
		{
			fun_wb_status_callback fstatus_cb = pdest->fstatus_cb;
			fun_wb_body_callback   fbody_cb = pdest->fbody_cb;
			fun_wb_header_callback fheader_cb = pdest->fheader_cb;

			memcpy(pdest , psrc , sizeof(struct t_wb_oauth) );

			pdest->fbody_cb = fbody_cb;
			pdest->fstatus_cb = fstatus_cb;
			pdest->fheader_cb = fheader_cb;
		}
	}

	/** 创建一个请求，每个事件都需要 创建一个请求（一定要通过 weibo_destroy_request（） 进行消毁 */
	WEIBO_API WEIBORequest* loWeibo_request_create(void)
	{
		WEIBORequest* request = malloc( sizeof( struct t_wb_Request ) );
		if( !request )
			return 0;

		memset( request , 0 , sizeof( struct t_wb_Request ) );

		return request;
	}

	/** 进行一些参数操作填写，然后执行(回调函数的设置...) */
	WEIBO_API int loWeibo_request_setopt(WEIBORequest* request , WEIBOReqoption option , ...)
	{
		int ret = -1;
		va_list arg;

		struct t_wb_Request* reqSession = (struct t_wb_Request*)request;
		if( !reqSession )
			return -1;

		va_start(arg, option);

		ret = loWeibo_request_setopt(reqSession, option, arg);

		va_end(arg);

		return ret;
	}

	/** 执行,等待回调数据的执行，其执行结果，会通过 weibo_request_setopt（）设置的方法，告之开发者  */
	WEIBO_API int loWeibo_request_start(WEIBORequest* request , bool bAsyn)
	{
		struct t_wb_Request* reqSession = (struct t_wb_Request*)request;
		if( !reqSession )
			return ERR_requestParam_nil;

		if( bAsyn )
		{// 异步处理
			return Weibo_start_request_asyn(reqSession);
		}
		else
		{//同步处理
			return Weibo_start_request(reqSession);
		}
		return ERR_Fail;
	}

	WEIBO_API void loWeibo_request_wait(WEIBORequest* request)
	{
		struct t_wb_Request* reqSession = (struct t_wb_Request*)request;
		if( !reqSession )
			return ;
		Weibo_wait_request(reqSession);
	}

	WEIBO_API int loWeibo_request_stop(WEIBORequest* request)
	{
		struct t_wb_Request* reqSession = (struct t_wb_Request*)request;

		//停止			
		return Weibo_stop_request( reqSession );

	}


	/** 消毁这个请求 */
	WEIBO_API int loWeibo_request_destroy(WEIBORequest* request)
	{
		struct t_wb_Request* reqSession = (struct t_wb_Request*)request;

		if( !reqSession  )
			return -1;

		if( reqSession->sendata_ )
		{
			Weibo_senddata_free( reqSession->sendata_ );
			reqSession->sendata_ = 0;
		}

		Weibo_httpsession_destroy( reqSession->session_ );

		if( reqSession->freq_destroy_ && reqSession->puserdata_ )
			reqSession->freq_destroy_(reqSession->puserdata_);

		free( reqSession );

		return 0;
	}

	WEIBO_API int loWeibo_get_url(struct t_wb_URI* outURI , WEIBOoption option , const void* t_wb , const struct t_wb_REQ_EX* req_ex)
	{
		return Weibo_url_get(outURI , option , t_wb ,req_ex);
	}

#ifdef  __cplusplus
}
#endif

