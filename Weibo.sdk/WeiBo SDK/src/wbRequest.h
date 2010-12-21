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


// ����
struct t_wb_Request
{
	// ���ٵ� ���� request���ڲ����������ٻ��ƣ� �� �ص� WEIBO_OPTION(REQ_DESTROY_FUNCTION)
	fun_req_destroy freq_destroy_;

	// �û�����
	void* puserdata_;

	// ��������( delete ... )
	void* sendata_;
	WEIBOoption  datatype_;

	// ��������

	// http session
	struct t_wb_HttpSession* session_;

	// ��������
	struct t_wb_REQ_EX exdata_;
};


/**
 * @breif ���� type ������Ӧ�Ľṹ����
 * 
 * @param type �ο� WEIBOoption
 * @param *senddata ��Ӧ�� WEIBOoptionֵ����Ӧ�Ľṹ
*/
void* Weibo_senddata_create(int type , const void* senddata);
void Weibo_senddata_free(void* senddata);

/** �����첽���� */
int  Weibo_start_request_asyn(struct t_wb_Request* reqSession);

/** ����ͬ������ */
int  Weibo_start_request(struct t_wb_Request* reqSession);

/** ֹͣ���� */
int  Weibo_stop_request(struct t_wb_Request* reqSession);

/** �첽�ȴ������ ʹ�� Weibo_start_request_asyn�� ����⺯������ʵ��ͬ��Ч�� */
void Weibo_wait_request(struct t_wb_Request* reqSession);

#endif // __wb_Request_H__