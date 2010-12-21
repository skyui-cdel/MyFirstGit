/**
*  @brief Micro blog session.
*  @file  wbSession.h
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



#ifndef __wb_SESSION_H__
#define __wb_SESSION_H__

/** «Î«Ûsession ∂‘œÛ(http) */
struct t_wb_HttpSession
{
	void* phttp_;
	bool  bAsyn_;
};

/** create the object */
struct t_wb_HttpSession* Weibo_httpsession_create(void);

/** destroy this object */
void  Weibo_httpsession_destroy(struct t_wb_HttpSession* session);

/**
 * @param userdata == t_wb_Request*
*/
int  Weibo_session_start(struct t_wb_HttpSession* session , void* userdata);
int  Weibo_session_stop(struct t_wb_HttpSession* session);
void Weibo_session_wait(struct t_wb_HttpSession* session);

#endif /* __wb_SESSION_H__ */