/**
*  @brief Check the option is validate
*  @file  wbCheck.h
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

#ifndef __wb_CHECK_H__
#define __wb_CHECK_H__


typedef enum
{
	Err_OK =0,
	Err_WB_ID,
	Err_USE_ID,
	Err_TEXT_ID,
	Err_IMAGE_ID,
	Err_WBCOM_ID,
	Err_PWD_ID,
	Err_OAUTHVERIFY_ID,
	Err_UIDTYPE_ID,

	// 
	Err_KEYWORD, // keyword null
	Err_TREND_ID,
	Err_TAGS_ID,

	// register
	Err_REG_NICK,	// nick name
	Err_REG_GENDER, // gender
	Err_REG_MAIL,	// email
	Err_REG_PWD,	// password
	Err_REG_IP,		// city id

	// tgt
	Err_UPDATETGT_SUE,
	Err_UPDATETGT_SUP,
	Err_UPDATETGT_VERIFY_TGT,

	// invite mail
	Err_MAIL_ID, // mail ID
	Err_MAIL_PWD,// mail password
	Err_MAIL_TYPE,// mail type
	Err_MAIL_LIST,// mail list
	Err_MAIL_NICKNAME,// mail nick name

	Err_AUTH_MODE,

}WBCheckerror;

/**
 * @brief 用来检测结构中变量参数值是否合法有效
 * 
 * @param option 参考 enum WEIBOoption in weibo.h
 * @param *t_wb 相对应 option的结构值，如  WEIBO_OPTION(GETSTATUSES_PUBLIC_TIMELINE)---> t_wb_get_statuses_public_timeline
 *
 * @return WBCheckerror值 ＝Err_OK 表示成功，其他值 失败
*/
int Weibo_check_valid(int option , const void* t_wb);

#endif