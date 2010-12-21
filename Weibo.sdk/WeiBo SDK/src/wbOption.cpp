/**
*  @brief Micro blog option fundation.
*  @file  wbOption.cpp
*  @author loach 
*  @Email < loachmr@sina.com >
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
#include "wbRequest.h"
#include "wbOption.h"
#include "wbString.h"
#include <string.h>
#include <assert.h>


//---------------------------------------------------------------------------------//
// for request option function 

// for declare function
#define WEIBO_request_callback(name) \
	static int Weibo_request_##name##_cb(struct t_wb_Request* reqSession, va_list param)

static
void WEIBO_modify_senddata_format(struct t_wb_Request* reqSession)
{
	if( reqSession->sendata_ )
	{
		struct t_wb_oauth* poauth = (struct t_wb_oauth*)reqSession->sendata_;
		poauth->format_ = reqSession->exdata_.format_;
	}
}

// for enum
#define WEIBO_request_fun(name) Weibo_request_##name##_cb


WEIBO_request_callback(REQ_UNK)
{
	return -1;
}

WEIBO_request_callback(REQ_DESTROY_FUNCTION) ///< 当请求被消毁时，通过回调函数告开发者
{
	if( reqSession->freq_destroy_ &&
		reqSession->puserdata_ )
	{
		reqSession->freq_destroy_(reqSession->puserdata_);
		reqSession->puserdata_ = 0;
	}
	reqSession->freq_destroy_ = va_arg(param , fun_req_destroy);
	return 0;
}

WEIBO_request_callback(REQ_SENDDATA) // 发送的数据
{
	// 1
	WEIBOoption type = (WEIBOoption)va_arg(param , int);
	if( WEIBO_OPTION(BASE) > type ||
		WEIBO_OPTION(LAST) <= type )
	{
		return -1;
	}

	//
	Weibo_senddata_free(reqSession->sendata_);

	// 2
	reqSession->sendata_ = Weibo_senddata_create(type , va_arg(param , void*) );
	reqSession->datatype_= type;

	WEIBO_modify_senddata_format(reqSession);
	return 0;
}

WEIBO_request_callback(REQ_USERDATA)
{ // 开发者自定义数据,如果设置了它，它会随着 回调函数返回给开发者使用
	if( reqSession->freq_destroy_ &&
		reqSession->puserdata_ )
	{
		reqSession->freq_destroy_(reqSession->puserdata_);
		reqSession->freq_destroy_ = 0;
	}
	reqSession->puserdata_ = va_arg(param , void*);
	return 0;
}

WEIBO_request_callback(REQ_PROXYDATA)
{// 请求的代理，目前不支持
	return 0;
}

WEIBO_request_callback(REQ_RESULT_DATAFORMAT)
{// 请求的代理，目前不支持
	reqSession->exdata_.format_ = va_arg(param , int);

	WEIBO_modify_senddata_format(reqSession);

	return 0;
}

WEIBO_request_callback(REQ_COOKIE)
{// 
	reqSession->exdata_.cookie_ = va_arg(param , int);
	return 0;
}

WEIBO_request_callback(REQ_APPKEY)
{
	const char *key = va_arg(param , const char*);
	if( key )
	{
		strncpy(reqSession->exdata_.appkey_ , key , 256  );
	}
	return 0;
}

WEIBO_request_callback(REQ_SECRETKEY)
{
	const char *key = va_arg(param , const char*);
	if( key )
	{
		strncpy(reqSession->exdata_.secret_ , key  , 256 );
	}
	return 0;
}

WEIBO_request_callback(REQ_IP)
{
	const char *key = va_arg(param , const char*);
	if( key )
	{
		strncpy(reqSession->exdata_.ip_ , key  , 64 );
	}
	return 0;
}

typedef int (*fun_request_format_callback)(struct t_wb_Request* reqSession , va_list param);
static fun_request_format_callback vector_request_format_cb[ WEIBO_OPTION(REQ_LAST) ] =
{
	WEIBO_request_fun(REQ_UNK),
	WEIBO_request_fun(REQ_DESTROY_FUNCTION),
	WEIBO_request_fun(REQ_SENDDATA),
	WEIBO_request_fun(REQ_USERDATA),
	WEIBO_request_fun(REQ_PROXYDATA),
	WEIBO_request_fun(REQ_RESULT_DATAFORMAT),
	WEIBO_request_fun(REQ_COOKIE),
	WEIBO_request_fun(REQ_APPKEY),
	WEIBO_request_fun(REQ_SECRETKEY),
	WEIBO_request_fun(REQ_IP),
};

int loWeibo_request_setopt(struct t_wb_Request* reqSession ,  WEIBOReqoption option , va_list param)
{
	if( !reqSession )
		return -1;

	if( WEIBO_OPTION(REQ_UNK) >= option ||
        WEIBO_OPTION(REQ_LAST) <= option )
	{
		return -1;
	}
	return vector_request_format_cb[option](reqSession , param);
}
