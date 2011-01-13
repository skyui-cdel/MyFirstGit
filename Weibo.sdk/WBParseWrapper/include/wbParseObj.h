/**
*  @brief analysis object,
*  @file  WeiboParsObj.h
*  @author welbon 
*  @Email < libo.weng@gmail.com >
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

#pragma once
#ifndef __WEIBO_PARSER_OBJECT_H__
#define __WEIBO_PARSER_OBJECT_H__

#include "wbParseStruct.h"
#include "WBParser.h"

namespace wbParserNS
{
#if defined(_USE_JSON_PARSER)
	/**
	* @brief weibo parser json object
	* 
	* @author welbon
	*
	* @date 2010-11-02
	*/
	class CWBJsonParser
	{
	public:
		/** 解析错误 */
		static t_wbParse_Error  *parse_error(/*[in]*/wbParserNS::REQOBJ *obj);
		static void parse_error(wbParserNS::REQOBJ *obj , t_wbParse_Error * p);

		/** 解析状态 */
		static t_wbParse_Status  *parse_status(/*[in]*/wbParserNS::REQOBJ *obj);
		static void parse_status(wbParserNS::REQOBJ *obj , t_wbParse_Status * p);

		/** 解析用户 */
		static t_wbParse_User    *parse_user(/*[in]*/wbParserNS::REQOBJ *obj);
		static void parse_user(wbParserNS::REQOBJ *obj , t_wbParse_User * p);

		/** 解析地理位置 */
		static t_wbParse_Geo     *parse_geo(/*[in]*/wbParserNS::REQOBJ *obj);
		static void parse_geo(wbParserNS::REQOBJ *obj , t_wbParse_Geo * p);

		/** 解析评论 */
		static t_wbParse_Comment *parse_comment(/*[in]*/wbParserNS::REQOBJ *obj);
		static void parse_comment(wbParserNS::REQOBJ *obj , t_wbParse_Comment * p);

		/** 私信 */
		static t_wbParse_DirectMessage *parse_directmessage(/*[in]*/ wbParserNS::REQOBJ *obj);
		static void parse_directmessage(wbParserNS::REQOBJ *obj , t_wbParse_DirectMessage * p);

		/** 解析未读数 */
		static t_wbParse_Unread  *parse_unread(/*[in]*/wbParserNS::REQOBJ *obj);
        static void parse_unread(wbParserNS::REQOBJ *obj , t_wbParse_Unread * p);

		/** 解析评论计数 */
		static t_wbParse_CommentCounts *parse_commentcounts(/*[in]*/wbParserNS::REQOBJ *obj);
		static void parse_commentcounts(wbParserNS::REQOBJ *obj , t_wbParse_CommentCounts * p);

		/** 解析访问限制 */
		static t_wbParse_LimitStatus *parse_limite_status(/*[in]*/wbParserNS::REQOBJ *obj);
        static void parse_limite_status(wbParserNS::REQOBJ *obj , t_wbParse_LimitStatus * p);

		/** 解析表情 */
		static t_wbParse_Emotion *parse_emotion(/*[in]*/wbParserNS::REQOBJ *obj);
		static void parse_emotion(/*[in]*/wbParserNS::REQOBJ *obj,t_wbParse_Emotion *p);

		/** 两个用户的关系 */
		static t_wbParse_Shipshow *parse_shipshow(/*[in]*/wbParserNS::REQOBJ *obj);
		static void parse_shipshow(/*[in]*/wbParserNS::REQOBJ *obj,t_wbParse_Shipshow *p);
		static t_wbParse_Shipshow::Item* parse_ship_item(wbParserNS::REQOBJ *obj);
	};

#endif //#if defined(_USE_JSON_PARSER)


#if defined(_USE_XML_PARSER)
	/**
	* @brief weibo parser xml object
	* 
	* @author welbon
	*
	* @date 2010-11-02
	*/
	class CWBXmlParser
	{
	public:
		/** 解析错误 */
		static t_wbParse_Error  *parse_error(/*[in]*/wbParserNS::REQOBJ *obj);
		static void parse_error(wbParserNS::REQOBJ *obj , t_wbParse_Error * p);

		/** 解析状态 */
		static t_wbParse_Status  *parse_status(/*[in]*/wbParserNS::REQOBJ *obj);
		static void parse_status(wbParserNS::REQOBJ *obj , t_wbParse_Status * p);

		/** 解析用户 */
		static t_wbParse_User    *parse_user(/*[in]*/wbParserNS::REQOBJ *obj);
		static void parse_user(wbParserNS::REQOBJ *obj , t_wbParse_User * p);

		/** 解析地理位置 */
		static t_wbParse_Geo     *parse_geo(/*[in]*/wbParserNS::REQOBJ *obj);
		static void parse_geo(wbParserNS::REQOBJ *obj , t_wbParse_Geo * p);

		/** 解析评论 */
		static t_wbParse_Comment *parse_comment(/*[in]*/wbParserNS::REQOBJ *obj);
		static void parse_comment(wbParserNS::REQOBJ *obj , t_wbParse_Comment * p);

		/** 私信 */
		static t_wbParse_DirectMessage *parse_directmessage(/*[in]*/ wbParserNS::REQOBJ *obj);
		static void parse_directmessage(wbParserNS::REQOBJ *obj , t_wbParse_DirectMessage * p);

		/** 解析未读数 */
		static t_wbParse_Unread  *parse_unread(/*[in]*/wbParserNS::REQOBJ *obj);
        static void parse_unread(wbParserNS::REQOBJ *obj , t_wbParse_Unread * p);

		/** 解析评论计数 */
		static t_wbParse_CommentCounts *parse_commentcounts(/*[in]*/wbParserNS::REQOBJ *obj);
		static void parse_commentcounts(wbParserNS::REQOBJ *obj , t_wbParse_CommentCounts * p);

		/** 解析访问限制 */
		static t_wbParse_LimitStatus *parse_limite_status(/*[in]*/wbParserNS::REQOBJ *obj);
        static void parse_limite_status(wbParserNS::REQOBJ *obj , t_wbParse_LimitStatus * p);

		/** 解析表情 */
		static t_wbParse_Emotion *parse_emotion(/*[in]*/wbParserNS::REQOBJ *obj);
		static void parse_emotion(/*[in]*/wbParserNS::REQOBJ *obj,t_wbParse_Emotion *p);

		/** 两个用户的关系 */
		static t_wbParse_Shipshow *parse_shipshow(/*[in]*/wbParserNS::REQOBJ *obj);
		static void parse_shipshow(/*[in]*/wbParserNS::REQOBJ *obj,t_wbParse_Shipshow *p);
		static t_wbParse_Shipshow::Item* parse_ship_item(wbParserNS::REQOBJ *obj);
	};
#endif //#if defined(_USE_XML_PARSER)

	extern void CheckShortLink(struct t_wbParse_Status *pStatus ,const char* wbParentId , t_wbParse_Media** pMedia , int& size ,int cmd);

	extern void MMIdCompare(struct t_wbParser_MMId* pMMId ,t_wbParser_MMId::eType type, const char* id , const char* time);
}

#endif // __WEIBO_PARSER_OBJECT_H__
