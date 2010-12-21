/**
*  @brief Micro blog request option.
*  @file  wbRequest.h
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

#ifndef __wb_Request_H__
#define __wb_Request_H__

#include "weibo.h"
#include "wbSession.h"


// 请求
struct t_wb_Request
{
	// 消毁掉 对象 request（内部有自已消毁机制） ， 回调 WEIBO_OPTION(REQ_DESTROY_FUNCTION)
	fun_req_destroy freq_destroy_;

	// 用户数据
	void* puserdata_;

	// 请求数据( delete ... )
	void* sendata_;
	WEIBOoption  datatype_;

	// 代理数据

	// http session
	struct t_wb_HttpSession* session_;

	// 其他数据
	struct t_wb_REQ_EX exdata_;
};


/**
 * @breif 根据 type 创建相应的结构对象
 * 
 * @param type 参考 WEIBOoption
 * @param *senddata 对应于 WEIBOoption值，相应的结构
*/
void* Weibo_senddata_create(int type , const void* senddata);
void Weibo_senddata_free(void* senddata);

/** 启动异步请求 */
int  Weibo_start_request_asyn(struct t_wb_Request* reqSession);

/** 启动同步请求 */
int  Weibo_start_request(struct t_wb_Request* reqSession);

/** 停止请求 */
int  Weibo_stop_request(struct t_wb_Request* reqSession);

/** 异步等待，如果 使用 Weibo_start_request_asyn， 结合这函数就是实现同步效果 */
void Weibo_wait_request(struct t_wb_Request* reqSession);

#endif // __wb_Request_H__