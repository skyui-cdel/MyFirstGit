/**
*  @brief Micro blog session.
*  @file  wbSession.cpp
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


#include "wbSession.h"
#include "wbRequest.h"
#include "wbURL.h"
#include "wbLog.h"
#include "wbCheck.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <libhttp/loHttp+.h>


/*下载大文件(内存缓冲区) 回调接口［边下载中 ，与OnHttpBody有所不同 ］ */
int Weibo_httpbuffer_callback(unsigned char* pBuffer , long lBuffer, void* pUsrData )
{
	wbPRINTF("Weibo_httpbuffer_callback = %s \n\n" , pBuffer );

	return 0;
}

/* 进度状态 */
int Weibo_httpprogress_callback(long lCurBytes , long lTotalBytes, void* pUsrData )
{
	wbPRINTF("Weibo_httpprogress_callback \n\n");

	return 0;
}

/** 下载状态
*
* @param state 状态(成功，失败）
* @param errCode 失败码
*/
void Weibo_httpcode_callback(int state , int errCode ,const lohttp::THttpTime* pTime , void* pUsrData )
{
	struct t_wb_Request* request = (struct t_wb_Request*)pUsrData;
	if( !request )
		return ;

	wbPRINTF("Weibo_httpcode_callback \n");
	switch(state)
	{
	case lohttp::E_HTTPSTATUS_START:
		wbPRINTF("   state = %d , errCode = %d , 开始下载 \n\n",state , errCode );
		break;

	case lohttp::E_HTTPSTATUS_STOP:
		wbPRINTF("   state = %d , errCode = %d , 停止下载 \n\n",state , errCode );
		break;

	case lohttp::E_HTTPSTATUS_CANCEL:
		wbPRINTF("   state = %d , errCode = %d 下载取消 \n\n",state , errCode );
		break;

	case lohttp::E_HTTPSTATUS_COMPLETE:
		wbPRINTF("   state = %d , errCode = %d 整个过程完成 \n\n",state , errCode );
		break;
	}

	if( request->sendata_ )
	{
		struct t_wb_base* base = (struct t_wb_base*)request->sendata_;
		if( base->wbauth_.fstatus_cb )
			base->wbauth_.fstatus_cb(state , errCode , request->puserdata_ );
	}
}

void Weibo_httprespone_callback(const lohttp::LOHttpResponse* resp , void* pUsrData)
{
	struct t_wb_Request* request = (struct t_wb_Request*)pUsrData;
	if( !request )
	{
		return ;
	}

	wbPRINTF("Weibo_httprespone_callback \n---------BEGIN go to parse respone packet---------\n\n");

	// to read body
	if( request->sendata_ )
	{
		struct t_wb_base* base = (struct t_wb_base*)request->sendata_;

		if( base->wbauth_.fheader_cb )
		{
			lohttp::Http_resp_readheader(resp , (lohttp::fun_http_resp_header_callback)base->wbauth_.fheader_cb , request->puserdata_);
		}

		if( base->wbauth_.fbody_cb )
		{
			lohttp::Http_resp_readbody( resp , (lohttp::fun_http_resp_body_callback)base->wbauth_.fbody_cb , request->puserdata_ );
		}
	}

	wbPRINTF("\n\n---------END complete parse respone packet---------\n\n");
}

/** CloHttp 资源释放掉 */
void Weibo_httprelease_callback(void* pUsrData)
{
	wbPRINTF("Weibo_httprelease_callback 资源释放掉 \n\n");
	struct t_wb_Request* request = (struct t_wb_Request*)pUsrData;
	if( !request )
	{
		return ;
	}
	if (request->freq_destroy_)
	{
		request->freq_destroy_(request->puserdata_);
		request->puserdata_ = 0;
	}
}


// 设置回调函数
static
void Weibo_session_callback(lohttp::LOHttp* pHttp , struct t_wb_Request* request )
{ 
	lohttp::Http_setopt(pHttp , lohttp::LOHTTP_OPT_callbackOfBuffer   , Weibo_httpbuffer_callback );
//	lohttp::Http_setopt(pHttp , lohttp::LOHTTP_OPT_callbackOfProgress , Weibo_httpprogress_callback );
	lohttp::Http_setopt(pHttp , lohttp::LOHTTP_OPT_callbackOfCode     , Weibo_httpcode_callback );
	lohttp::Http_setopt(pHttp , lohttp::LOHTTP_OPT_callbackOfRespone  , Weibo_httprespone_callback );
 	lohttp::Http_setopt(pHttp , lohttp::LOHTTP_OPT_callbackOfRelease  , Weibo_httprelease_callback );

	lohttp::Http_setopt(pHttp , lohttp::LOHTTP_OPT_userdata , request );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
struct t_wb_HttpSession* Weibo_httpsession_create(void)
{
	struct t_wb_HttpSession* psession = (struct t_wb_HttpSession*)malloc( sizeof( struct t_wb_HttpSession) );

	if( psession ) 
		memset(psession , 0 , sizeof( struct t_wb_HttpSession ) );

	return psession;
}

void Weibo_httpsession_destroy(struct t_wb_HttpSession* session)
{
	if( !session )
		return ;

	// free 
	if( session->phttp_ )
	{// libhttp.httptask
		lohttp::Http_destroyobject( session->phttp_ );
		session->phttp_ = 0;
	}
	//
	free(session);
}

int Weibo_session_start(struct t_wb_HttpSession* session, void* userdata )
{
	struct t_wb_Request* request = (struct t_wb_Request*)userdata;
	if( !session || !request )
	{
		wbASSERT(0);
		return ERR_Fail;
	}
	// free 
	if( !session->phttp_ )
	{// create libhttp.httptask
		session->phttp_ = lohttp::Http_createobject();
		if( !session->phttp_ )
		{
			wbASSERT(0);
			return ERR_createhttpobject_nil; // 内存分配失败
		}
	}

	lohttp::Http_setopt(session->phttp_ , lohttp::LOHTTP_OPT_clearparam , 0);
	lohttp::Http_setopt(session->phttp_ , lohttp::LOHTTP_OPT_clearheader , 0);
	lohttp::Http_setopt(session->phttp_ , lohttp::LOHTTP_OPT_clearbinarydata , 0);

	if( 0 != Weibo_check_valid(request->datatype_ , request->sendata_) )
	{// 内容条件不满足
		return ERR_checkvalid_fail;
	}

	if( 0 != Weibo_url_set( session->phttp_ , request->datatype_ , request->sendata_ , &request->exdata_ ) ){

		return ERR_url_set_fail; // 设置URL有问题
	}

	Weibo_session_callback( session->phttp_ , request );

	lohttp::Http_setopt( session->phttp_ , lohttp::LOHTTP_OPT_setuseragent ,HTTP_T("Weibo SDK/1.0") );

	if( session->bAsyn_ )
	{
		return lohttp::Http_start_asyn( session->phttp_ );
	}
	return lohttp::Http_start( session->phttp_ );
}

int Weibo_session_stop(struct t_wb_HttpSession* session)
{
	if( !session )
		return 0;

	// free 
	if( !session->phttp_ )
	{// destroy libhttp.httptask
		return 0;
	}

	lohttp::Http_cancel( session->phttp_ );

	return 0;
}

void Weibo_session_wait(struct t_wb_HttpSession* session)
{
	if( !session )
		return ;

	// free 
	if( !session->phttp_ )
	{// destroy libhttp.httptask
		return ;
	}
	lohttp::Http_wait( session->phttp_ );
}
