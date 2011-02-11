/**
*  @brief analysis object,
*  @file  WeiboParsObj.cpp
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



#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <json/json.h>
#include "wbParseObj.h"
#include "wbParseMalloc.h"
#include "splitstr.h"
#include "wbParseMacro.h"
#include "strconv.h"

using namespace wbParserNS ;

//  DATA KEY 
#define WBPARSER_KEY_IDX(name)        WBParser_##name##_KEY
#define WBPARSER_KEY_STATEIDX(name)   WBParser_state_##name##_KEY
#define WBPARSER_KEY_USERIDX(name)    WBParser_user_##name##_KEY
#define WBPARSER_KEY_COMMENTIDX(name) WBParser_comment_##name##_KEY
#define WBPARSER_KEY_DIRMSGIDX(name)  WBParser_directmsg_##name##_KEY
#define WBPARSER_KEY_GEO(name)        WBParser_geo_##name##_KEY
#define WBPARSER_KEY_UREADER(name)    WBParser_unreader_##name##_KEY
#define WBPARSER_KEY_COMCOUNT(name)    WBParser_comcount_##name##_KEY
#define WBPARSER_KEY_ACCESS_LMT(name)  WBParser_accesslmt_##name##_KEY
#define WBPARSER_KEY_EMOTION(name)     WBParser_emotion##name##_KEY
#define WBPARSER_KEY_XMLROOT(name)     WBParser_xmlroot_##name##KEY

/////////////////////////
//关键字定义


typedef enum 
{
	// ptr
	WBPARSER_KEY_STATEIDX(createtime),
	WBPARSER_KEY_STATEIDX(id),
	WBPARSER_KEY_STATEIDX(text),
	WBPARSER_KEY_STATEIDX(source),
	WBPARSER_KEY_STATEIDX(favorited),
	WBPARSER_KEY_STATEIDX(truncated),
	WBPARSER_KEY_STATEIDX(rsid),
	WBPARSER_KEY_STATEIDX(ruid),
	WBPARSER_KEY_STATEIDX(rtsname),
	WBPARSER_KEY_STATEIDX(thumbpic),
	WBPARSER_KEY_STATEIDX(midpic),
	WBPARSER_KEY_STATEIDX(originpic),
	WBPARSER_KEY_STATEIDX(geo),
	WBPARSER_KEY_STATEIDX(user),
	WBPARSER_KEY_STATEIDX(retstatus),


	// users
	WBPARSER_KEY_USERIDX(id),
	WBPARSER_KEY_USERIDX(scname),
	WBPARSER_KEY_USERIDX(name),
	WBPARSER_KEY_USERIDX(province),
	WBPARSER_KEY_USERIDX(city),
	WBPARSER_KEY_USERIDX(location),
	WBPARSER_KEY_USERIDX(description),
	WBPARSER_KEY_USERIDX(url),
	WBPARSER_KEY_USERIDX(prourl),
	WBPARSER_KEY_USERIDX(domain),
	WBPARSER_KEY_USERIDX(gender),
	WBPARSER_KEY_USERIDX(followcount),
	WBPARSER_KEY_USERIDX(frcount),
	WBPARSER_KEY_USERIDX(stacount),
	WBPARSER_KEY_USERIDX(favcount),
	WBPARSER_KEY_USERIDX(allowall),
	WBPARSER_KEY_USERIDX(geoenable),
	WBPARSER_KEY_USERIDX(createdat),
	WBPARSER_KEY_USERIDX(following),
	WBPARSER_KEY_USERIDX(verified),
	WBPARSER_KEY_USERIDX(last_status),

	// comments
	WBPARSER_KEY_COMMENTIDX(id),
	WBPARSER_KEY_COMMENTIDX(text),
	WBPARSER_KEY_COMMENTIDX(source),
	WBPARSER_KEY_COMMENTIDX(favorited),
	WBPARSER_KEY_COMMENTIDX(truncated),

	WBPARSER_KEY_COMMENTIDX(createdat),
	WBPARSER_KEY_COMMENTIDX(user),
	WBPARSER_KEY_COMMENTIDX(status),
	WBPARSER_KEY_COMMENTIDX(rcomment),

	// direct message
	WBPARSER_KEY_DIRMSGIDX(id),
	WBPARSER_KEY_DIRMSGIDX(text),
	WBPARSER_KEY_DIRMSGIDX(senderid),
	WBPARSER_KEY_DIRMSGIDX(recipientid),
	WBPARSER_KEY_DIRMSGIDX(createdat),
	WBPARSER_KEY_DIRMSGIDX(sndsname),
	WBPARSER_KEY_DIRMSGIDX(rsnsame),
	WBPARSER_KEY_DIRMSGIDX(sender),
	WBPARSER_KEY_DIRMSGIDX(recipient),

	// geo
	WBPARSER_KEY_GEO(type),
	WBPARSER_KEY_GEO(coordinates),


	// 评论计数
	WBPARSER_KEY_COMCOUNT(id),
	WBPARSER_KEY_COMCOUNT(comments),
	WBPARSER_KEY_COMCOUNT(rt),

	// unreader
	WBPARSER_KEY_UREADER(comments),
	WBPARSER_KEY_UREADER(mentions),
	WBPARSER_KEY_UREADER(dm),
	WBPARSER_KEY_UREADER(followers),
	WBPARSER_KEY_UREADER(new_status),

	// ACCES limite
	WBPARSER_KEY_ACCESS_LMT(hourlimit),
	WBPARSER_KEY_ACCESS_LMT(rstimeinseond),
	WBPARSER_KEY_ACCESS_LMT(remaining_hits),
	WBPARSER_KEY_ACCESS_LMT(reset_time),

	//Emotion
	WBPARSER_KEY_EMOTION(phrase),
	WBPARSER_KEY_EMOTION(type),
	WBPARSER_KEY_EMOTION(url),
	WBPARSER_KEY_EMOTION(is_hot),
	WBPARSER_KEY_EMOTION(is_common),
	WBPARSER_KEY_EMOTION(order_number),
	WBPARSER_KEY_EMOTION(category),

	// 分页
	WBPARSER_KEY_IDX(next_cursor),
	WBPARSER_KEY_IDX(previous_cursor),

	// XML ROOT KEYS
	WBPARSER_KEY_XMLROOT(statuses),
	WBPARSER_KEY_XMLROOT(status),

	WBPARSER_KEY_IDX(end),


}key_index;

static
const WBParseCHAR* weibo_key_array[] =
{
	// ptr
	_WBC("created_at"),
	_WBC("id"),
	_WBC("text"),
	_WBC("source"),
	_WBC("favorited"),
	_WBC("truncated"),
	_WBC("in_reply_to_status_id"),
	_WBC("in_reply_to_user_id"),
	_WBC("in_reply_to_screen_name"),
	_WBC("thumbnail_pic"),
	_WBC("bmiddle_pic"),
	_WBC("original_pic"),
	_WBC("geo"),
	_WBC("user"),
	_WBC("retweeted_status"),

	// users
	_WBC("id"),
	_WBC("screen_name"),
	_WBC("name"),
	_WBC("province"),
	_WBC("city"),
	_WBC("location"),
	_WBC("description"),
	_WBC("url"),
	_WBC("profile_image_url"),
	_WBC("domain"),
	_WBC("gender"),
	_WBC("followers_count"),
	_WBC("friends_count"),
	_WBC("statuses_count"),
	_WBC("favourites_count"),
	_WBC("allow_all_act_msg"),
	_WBC("geo_enabled"),
	_WBC("created_at"),
	_WBC("following"),
	_WBC("verified"),
	_WBC("status"),

	// comments
	_WBC("id"),
	_WBC("text"),
	_WBC("source"),
	_WBC("favorited"),
	_WBC("truncated"),
	_WBC("created_at"),
	_WBC("user"),
	_WBC("status"),
	_WBC("reply_comment"),

	// direct message
	_WBC("id"),
	_WBC("text"),
	_WBC("sender_id"),
	_WBC("recipient_id"),
	_WBC("created_at"),
	_WBC("sender_screen_name"),
	_WBC("recipient_screen_name"),
	_WBC("sender"),
	_WBC("recipient"),

	// geo
	_WBC("type"),
	_WBC("coordinates"),

	// 评论计数
	_WBC("id"),
	_WBC("comments"),
	_WBC("rt"),

	//未读消息数
	_WBC("comments"),
	_WBC("mentions"),
	_WBC("dm"),
	_WBC("followers"),
	_WBC("new_status"),

	// 访问限制
	_WBC("hourly_limit"),
	_WBC("reset_time_in_seconds"),
	_WBC("remaining_hits"),
	_WBC("reset_time"),

	//Emotion
	_WBC("phrase"),
	_WBC("type"),
	_WBC("url"),
	_WBC("is_hot"),
	_WBC("is_common"),
	_WBC("order_number"),
	_WBC("category"),


	// 分页
	_WBC("next_cursor"),
	_WBC("previous_cursor"),

	//
	// XML ROOT KEYS
	_WBC("statuses"),
	_WBC("status"),

};


const WBParseCHAR* wbParse_GetKey(int idx)
{
	if( idx >= WBPARSER_KEY_IDX(end) || 
		idx < 0 )
	{
		return 0;
	}
	return weibo_key_array[idx];
}

#define GET_VALUEKEY(idx) wbParse_GetKey(idx)
#define WEIBO_PARSER_CAST(t,ptr) (t*)ptr



#if defined(_USE_JSON_PARSER)

/** 解析状态 */
t_wbParse_Status  *CWBJsonParser::parse_status(/*[in]*/wbParserNS::REQOBJ *obj)
{
	t_wbParse_Status* ptr = 0;
	if( obj)
	{
		ptr = WEIBO_PARSER_CAST(t_wbParse_Status,wbParse_Malloc_Status(1));
		parse_status(obj, ptr);
	}
	return ptr;
}

void CWBJsonParser::parse_status(wbParserNS::REQOBJ *obj , t_wbParse_Status * ptr)
{
	if( !ptr || !obj )
		return ;
	// id
	GET_LONG_TO_STR( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(id)),ptr->id,WBPARSER_USE_LEN(id) );

	// created_at
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(createtime)),ptr->created_at,WBPARSER_USE_LEN(created_at) );

	// text
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(text)),ptr->text,WBPARSER_USE_LEN(text) );

	// source
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(source)),ptr->source,WBPARSER_USE_LEN(source) );

	// in_reply_to_status_id
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(rsid)),ptr->in_reply_to_status_id,WBPARSER_USE_LEN(in_reply_to_status_id) );

	// in_reply_to_screen_name
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(rtsname)),ptr->in_reply_to_screen_name,WBPARSER_USE_LEN(in_reply_to_screen_name) );

	// thumbnail_pic
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(thumbpic)),ptr->thumbnail_pic,WBPARSER_USE_LEN(thumbnail_pic) );

	// bmiddle_pic
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(midpic)),ptr->bmiddle_pic,WBPARSER_USE_LEN(bmiddle_pic) );

	// original_pic
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(originpic)),ptr->original_pic,WBPARSER_USE_LEN(original_pic) );

	// favorited
	GET_BOOL(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(favorited)),ptr->favorited );

	// truncated
	GET_BOOL(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(truncated)),ptr->truncated );

	//
	USE_WEIBOPARSE_OBJ_PTR ;

	// geo
	GET_OBJECT(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(geo)),ptr->geo,parse_geo);

	// user
	GET_OBJECT(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(user)),ptr->user,parse_user);

	// retweeted_status
	GET_OBJECT(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(retstatus)),ptr->retweeted_status,parse_status);
}


struct t_wbParse_Error* CWBJsonParser::parse_error(wbParserNS::REQOBJ *obj)
{
	t_wbParse_Error* ptr = 0;
	if( obj )
	{
		ptr = WEIBO_PARSER_CAST(t_wbParse_Error,wbParse_Malloc_Error(1) );

		parse_error(obj,ptr);
	}
	return ptr;
}

void CWBJsonParser::parse_error(wbParserNS::REQOBJ *obj , t_wbParse_Error * ptr)
{
	if( !ptr || !obj )
		return ;

	int len = 0;
	const char* txt = NULL;

	//
	//txt = wbParserNS::GetCHAR_Key_XML_EX("request" , obj , len );
	GET_STR_EX(JSON,obj,"request",txt,len );
	if( len && txt ) {
		strncpy( ptr->request ,txt , WBPARSER_USE_LEN(request) );
	}

	//txt = wbParserNS::GetCHAR_Key_JSON_EX("error_code" , obj, len);
	GET_STR_EX(JSON,obj,"error_code",txt,len);
	if( len && txt ) {
		strncpy( ptr->error_code,txt , WBPARSER_USE_LEN(error_code) );
	}

	//txt = wbParserNS::GetCHAR_Key_JSON_EX("error" , obj, len);
	GET_STR_EX(JSON,obj,"error",txt,len);
	if( len && txt ) {
		strncpy( ptr->error,txt,WBPARSER_USE_LEN(error) );
		ptr->error_sub_code = atoi(ptr->error);
	}
}

/** 解析用户 */
t_wbParse_User  *CWBJsonParser::parse_user(/*[in]*/wbParserNS::REQOBJ *obj)
{
	t_wbParse_User* ptr = 0;
	if( obj )
	{
		ptr = WEIBO_PARSER_CAST(t_wbParse_User,wbParse_Malloc_User(1));

		parse_user(obj, ptr );
	}
	return ptr;
}

void CWBJsonParser::parse_user(wbParserNS::REQOBJ *obj , t_wbParse_User * ptr)
{
	if( !ptr || !obj )
		return ;
	// id
	GET_LONG_TO_STR( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(id)),ptr->id,WBPARSER_USE_LEN(id) );

	// screen_name
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(name)),ptr->screen_name,WBPARSER_USE_LEN(screen_name) );

	// name
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(name)),ptr->name,WBPARSER_USE_LEN(name) );


	// province
	GET_LONG_TO_STR( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(province)),ptr->province,WBPARSER_USE_LEN(province) );

	// city
	GET_LONG_TO_STR( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(city)),ptr->city,WBPARSER_USE_LEN(city) );

	// location
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(location)),ptr->location,WBPARSER_USE_LEN(location) );

	// description
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(description)),ptr->description,WBPARSER_USE_LEN(description) );

	// url
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(url)),ptr->url,WBPARSER_USE_LEN(url) );

	// profile_image_url
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(prourl)),ptr->profile_image_url,WBPARSER_USE_LEN(profile_image_url) );

	// domain
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(domain)),ptr->domain,WBPARSER_USE_LEN(domain) );

	// gender
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(gender)),ptr->gender,WBPARSER_USE_LEN(gender) );

	// created_at
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(createdat)),ptr->created_at,WBPARSER_USE_LEN(created_at) );

	// followers_count
	GET_LONG(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(followcount)),ptr->followers_count );

	// followers_count
	GET_LONG(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(frcount)),ptr->friends_count );

	// statuses_count
	GET_LONG(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(stacount)),ptr->statuses_count );

	// favourites_count
	GET_LONG(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(favcount)),ptr->favourites_count );

	// allow_all_act_msg
	GET_BOOL(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(allowall)),ptr->allow_all_act_msg );

	// geo_enabled
	GET_BOOL(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(geoenable)),ptr->geo_enabled );

	// following
	GET_BOOL(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(following)),ptr->following );

	// following
	GET_BOOL(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(verified)),ptr->verified );

	// last status
	USE_WEIBOPARSE_OBJ_PTR;
	GET_OBJECT(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(last_status)),ptr->last_status,parse_status);
}

/** 解析地理位置 */
t_wbParse_Geo *CWBJsonParser::parse_geo(/*[in]*/wbParserNS::REQOBJ *obj)
{
	t_wbParse_Geo *ptr = 0;
	if( obj )
	{
		ptr = WEIBO_PARSER_CAST(t_wbParse_Geo,wbParse_Malloc_Geo(1));

		parse_geo(obj,ptr);
	}
	return ptr;
}

void CWBJsonParser::parse_geo(wbParserNS::REQOBJ *obj , t_wbParse_Geo * ptr)
{
	if( !ptr || !obj )
		return ;
	//
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_GEO(type)),ptr->type,WBPARSER_USE_LEN(type)) ;

	//
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_GEO(coordinates)),ptr->coordinates,WBPARSER_USE_LEN(coordinates)) ;
}

/** 解析评论 */
t_wbParse_Comment *CWBJsonParser::parse_comment(/*[in]*/wbParserNS::REQOBJ *obj)
{
	t_wbParse_Comment *ptr = 0;
	if( obj )
	{
		ptr = WEIBO_PARSER_CAST(t_wbParse_Comment,wbParse_Malloc_Comment(1));
		parse_comment(obj,ptr);
	}
	return ptr;
}

void CWBJsonParser::parse_comment(wbParserNS::REQOBJ *obj , t_wbParse_Comment * ptr)
{
	if( !ptr || !obj )
		return ;
	// id
	GET_LONG_TO_STR( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_COMMENTIDX(id)),ptr->id,WBPARSER_USE_LEN(id) );

	// text
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_COMMENTIDX(text)),ptr->text,WBPARSER_USE_LEN(text) );

	// source
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_COMMENTIDX(source)),ptr->source,WBPARSER_USE_LEN(source) );

	// favorited
	GET_BOOL(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_COMMENTIDX(favorited)),ptr->favorited );

	// created_at
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_COMMENTIDX(createdat)),ptr->created_at,WBPARSER_USE_LEN(created_at) );

	//
	USE_WEIBOPARSE_OBJ_PTR ;

	// user
	GET_OBJECT(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_COMMENTIDX(user)),ptr->user,parse_user);

	// status
	GET_OBJECT(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_COMMENTIDX(status)),ptr->status,parse_status);

	// reply_comment
	GET_OBJECT(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_COMMENTIDX(rcomment)),ptr->reply_comment,parse_comment);
}

/** 私信 */
t_wbParse_DirectMessage *CWBJsonParser::parse_directmessage(/*[in]*/wbParserNS::REQOBJ *obj)
{
	t_wbParse_DirectMessage* ptr =0;
	if( obj )
	{
		ptr = WEIBO_PARSER_CAST(t_wbParse_DirectMessage,wbParse_Malloc_Directmessage(1));
		parse_directmessage(obj,ptr);
	}
	return ptr;
}

void CWBJsonParser::parse_directmessage(wbParserNS::REQOBJ *obj , t_wbParse_DirectMessage * ptr)
{
	if( !ptr || !obj )
		return ;
	// id
	GET_LONG_TO_STR( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_DIRMSGIDX(id)),ptr->id,WBPARSER_USE_LEN(id) );

	// text
	GET_STR( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_DIRMSGIDX(text)),ptr->text,WBPARSER_USE_LEN(text) );

	// created_at
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_COMMENTIDX(createdat)),ptr->created_at,WBPARSER_USE_LEN(created_at) );

	// sender_id
	GET_STR( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_DIRMSGIDX(senderid)),ptr->sender_id,WBPARSER_USE_LEN(id) );

	// recipient_id
	GET_STR( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_DIRMSGIDX(recipientid)),ptr->recipient_id,WBPARSER_USE_LEN(id) );

	// sender_screen_name
	GET_STR( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_DIRMSGIDX(sndsname)),ptr->sender_screen_name,WBPARSER_USE_LEN(name) );

	//recipient_screen_name
	GET_STR( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_DIRMSGIDX(rsnsame)),ptr->recipient_screen_name,WBPARSER_USE_LEN(name) );

	//
	USE_WEIBOPARSE_OBJ_PTR ;

	//sender
	GET_OBJECT( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_DIRMSGIDX(sender)),ptr->sender,parse_user );

	//recipient
	GET_OBJECT( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_DIRMSGIDX(recipient)),ptr->recipient,parse_user );
}

/** 解析未读数 */
t_wbParse_Unread  *CWBJsonParser::parse_unread(/*[in]*/wbParserNS::REQOBJ *obj)
{
	t_wbParse_Unread *ptr = 0;
	if( obj )
	{
		ptr = WEIBO_PARSER_CAST(t_wbParse_Unread,wbParse_Malloc_Unread(1));

		parse_unread(obj ,ptr );
	}
	return ptr;
}

void CWBJsonParser::parse_unread(wbParserNS::REQOBJ *obj , t_wbParse_Unread * ptr)
{
	if( !ptr || !obj )
		return ;
	// comments
	GET_LONG(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_UREADER(comments)),ptr->comments);

	// mentions
	GET_LONG(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_UREADER(mentions)),ptr->mentions);

	// dm
	GET_LONG(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_UREADER(dm)),ptr->dm);

	// followers
	GET_LONG(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_UREADER(followers)),ptr->followers);

	GET_LONG(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_UREADER(new_status)),ptr->new_status);
	
}

/** 解析评论计数 */
t_wbParse_CommentCounts *CWBJsonParser::parse_commentcounts(/*[in]*/wbParserNS::REQOBJ *obj)
{
	t_wbParse_CommentCounts *ptr = 0;
	if( obj)
	{
		ptr = WEIBO_PARSER_CAST(t_wbParse_CommentCounts,wbParse_Malloc_Commentcount(1));
		parse_commentcounts(obj,ptr);
	}
	return ptr;
}

void CWBJsonParser::parse_commentcounts(wbParserNS::REQOBJ *obj , t_wbParse_CommentCounts * ptr)
{
	if( !ptr || !obj )
		return ;
	// id
	GET_LONG_TO_STR( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_COMCOUNT(id)),ptr->id,WBPARSER_USE_LEN(id) );

	// comments
	GET_LONG( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_COMCOUNT(comments)),ptr->comments );

	// rt
	GET_LONG( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_COMCOUNT(rt)),ptr->rt );
}


t_wbParse_LimitStatus *CWBJsonParser::parse_limite_status(/*[in]*/wbParserNS::REQOBJ *obj)
{
	t_wbParse_LimitStatus *ptr = 0;
	if( obj)
	{
		ptr = (t_wbParse_LimitStatus *)wbParse_Malloc_Limits(1);
		parse_limite_status(obj ,  ptr);
	}
	return ptr;
}

void CWBJsonParser::parse_limite_status(wbParserNS::REQOBJ *obj , t_wbParse_LimitStatus * ptr)
{
	if( !ptr || !obj )
		return ;
	// hourly_limit
	GET_LONG( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_ACCESS_LMT(hourlimit)),ptr->hourly_limit );

	// rstimeinseond
	GET_LONG( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_ACCESS_LMT(rstimeinseond)),ptr->reset_time_in_seconds );

	// remaining_hits
	GET_LONG( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_ACCESS_LMT(remaining_hits)),ptr->remaining_hits );

	// reset_time
	GET_STR( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_ACCESS_LMT(reset_time)),ptr->reset_time,WBPARSER_USE_LEN(reset_time) );
}


t_wbParse_Emotion *CWBJsonParser::parse_emotion(/*[in]*/wbParserNS::REQOBJ *obj)
{
	t_wbParse_Emotion *ptr = 0;
	if( obj )
	{
		ptr = (t_wbParse_Emotion *)wbParse_Malloc_Emotion(1);
		parse_emotion(obj , ptr);
	}
	return ptr;
}

void CWBJsonParser::parse_emotion(wbParserNS::REQOBJ *obj , t_wbParse_Emotion * ptr)
{
	if( !ptr || !obj )
		return ;
	// phrase
	GET_STR( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_EMOTION(phrase)),ptr->phrase,WBPARSER_USE_LEN(phrase) );

	// type
	GET_STR( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_EMOTION(type)),ptr->type,WBPARSER_USE_LEN(type) );

	// url
	GET_STR( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_EMOTION(url)),ptr->url,WBPARSER_USE_LEN(url) );

	//category
	GET_STR( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_EMOTION(category)),ptr->category,WBPARSER_USE_LEN(category) );

	// is_hot
	GET_LONG( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_EMOTION(is_hot)),ptr->is_hot );

	// is_common
	GET_LONG( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_EMOTION(is_common)),ptr->is_common );

	// order_number
	GET_LONG( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_EMOTION(order_number)),ptr->order_number );
}


t_wbParse_Shipshow *CWBJsonParser::parse_shipshow(/*[in]*/wbParserNS::REQOBJ *obj)
{
	t_wbParse_Shipshow *ptr = 0;
	if( obj )
	{
		ptr = (t_wbParse_Shipshow *)wbParse_Malloc_Friendship(1);
		parse_shipshow(obj , ptr);
	}
	return ptr;
}

void CWBJsonParser::parse_shipshow(/*[in]*/wbParserNS::REQOBJ *obj,t_wbParse_Shipshow *ptr)	
{
//{
 //   "source":{
 //       "id":245110499
 //       ,"screen_name":"245110499"
 //       ,"following":false
 //       ,"followed_by":false
 //       ,"notifications_enabled":false
 //  }
 //  ,"target":{
 //       "id":10503      
 //       ,"screen_name":"10503"
 //       ,"following":false
 //       ,"followed_by":false
 //       ,"notifications_enabled":false
 // }
 //}
	if( !ptr || !obj )
		return ;
	//
	USE_WEIBOPARSE_OBJ_PTR ;

	//source
	GET_OBJECT( JSON,obj,"source",ptr->source,parse_ship_item );

	//source
	GET_OBJECT( JSON,obj,"target",ptr->target,parse_ship_item );
}

t_wbParse_Shipshow::Item* CWBJsonParser::parse_ship_item(wbParserNS::REQOBJ *obj)
{
//   "source":{
 //       "id":245110499
 //       ,"screen_name":"245110499"
 //       ,"following":false
 //       ,"followed_by":false
 //       ,"notifications_enabled":false
 //  }
	if( !obj )
		return 0;
	t_wbParse_Shipshow::Item* ptr = (t_wbParse_Shipshow::Item*)malloc(sizeof(t_wbParse_Shipshow::Item));
	memset(ptr , 0 , sizeof(t_wbParse_Shipshow::Item));

	// id
	GET_LONG_TO_STR( JSON,obj,"id",ptr->id,WBPARSER_USE_LEN(id) );

	// screen_name
	GET_STR( JSON,obj,"screen_name",ptr->screen_name,WBPARSER_USE_LEN(screen_name) );

	// following
	GET_LONG( JSON,obj,"following",ptr->following);

	// following
	GET_LONG( JSON,obj,"followed_by",ptr->followed_by);

	// following
	GET_LONG( JSON,obj,"notifications_enabled",ptr->notifications_enabled);

	return ptr;
}


/** 邀请联系人用户 */
t_wbParse_InviteContact::ItemUsr *CWBJsonParser::parse_invite_contact_usr(/*[in]*/wbParserNS::REQOBJ *obj)
{
	t_wbParse_InviteContact::ItemUsr *ptr = 0;
	if( obj ) {
		ptr = (t_wbParse_InviteContact::ItemUsr*)wbParse_Malloc_InviteContact_Usr(1);
		parse_invite_contact_usr(obj,ptr);
	}

	return ptr;
}
void CWBJsonParser::parse_invite_contact_usr(/*[in]*/wbParserNS::REQOBJ *obj,t_wbParse_InviteContact::ItemUsr *ptr)
{
	if( !ptr || !obj )
		return ;

	// UID
	GET_STR( JSON,obj,"uid",ptr->uid_,WBPARSER_USE_LEN(id) );
	// NAME
	GET_STR( JSON,obj,"name",ptr->name_,WBPARSER_USE_LEN(screen_name) );
	char* outstr = NULL;
	if(  lo_Utf82C(&outstr , ptr->name_) ) {
		strcpy( ptr->name_,outstr );
		free(outstr);
	}
	// URL
	GET_STR( JSON,obj,"icon",ptr->iconurl_,WBPARSER_USE_LEN(url) );
	// EMAIL
	GET_STR( JSON,obj,"email",ptr->email_,WBPARSER_USE_LEN(email) );
}

/** 用户标签 */
t_wbParse_Tag *CWBJsonParser::parse_Tags(/*[in]*/wbParserNS::REQOBJ *obj,WBParseCHAR *idKey )
{
	t_wbParse_Tag *ptr = 0;
	if( obj ) {
		ptr = (t_wbParse_Tag*)wbParse_Malloc_Tag(1);
		parse_Tags(obj,ptr,idKey );
	}
	return ptr;
}
void CWBJsonParser::parse_Tags(/*[in]*/wbParserNS::REQOBJ *obj,t_wbParse_Tag *ptr,WBParseCHAR *idKey )
{
	if( !ptr || !obj )
		return ;

	Json::Value *val = (Json::Value*)obj;

	if( idKey && *idKey != '\0' )
	{
		std::basic_string<WBParseCHAR> cc;

		// tag id
		Json::Value &jvalTagID = ((*val)[idKey]);
		(jvalTagID.isString()) ? (cc = (jvalTagID.asString())) : 0;
		if( !cc.empty()){
			WBPARSER_COPY( ptr->tagId_,WBPARSER_REAL_LEN(TAGS_ID),cc.c_str(),cc.length() );
		}

		// tag name
		Json::Value &jvalTagName = ((*val)["value"]);
		( jvalTagName.isString()) ? (cc = (jvalTagName.asString())) : (0);
		if( cc.length()){
			WBPARSER_COPY(ptr->tagName_,WBPARSER_REAL_LEN(TAGS_ID),cc.c_str(),cc.length() );
		}
	}
	else {
		Json::Value::iterator it_begin = val->begin() ;

		// tag name
		WBPARSER_COPY(ptr->tagId_,WBPARSER_REAL_LEN(TAGS_ID),it_begin.memberName(),strlen(it_begin.memberName()));

		// tag id
		WBPARSER_COPY(ptr->tagName_,WBPARSER_REAL_LEN(TAGS_NAME),(*it_begin).asString().c_str(),(*it_begin).asString().length() );
	}

}


/** 话题 */
t_wbParse_Trend *CWBJsonParser::parse_Trends_getData(/*[in]*/wbParserNS::REQOBJ *obj)
{
	t_wbParse_Trend *ptr = 0;
	if( obj ) {
		ptr = (t_wbParse_Trend*)wbParse_Malloc_Trend(1);
		parse_Trends_getData(obj,ptr);
	}
	return ptr;
}
void CWBJsonParser::parse_Trends_getData(/*[in]*/wbParserNS::REQOBJ *obj,t_wbParse_Trend *ptr)
{
	if( !ptr || !obj )
		return ;

	//
	GET_STR( JSON,obj,"trend_id",ptr->trendId_     ,WBPARSER_USE_LEN(TREND_ID) );
	GET_STR( JSON,obj,"hotword" ,ptr->trendHotword_,WBPARSER_USE_LEN(TREND_NAME) );
	GET_STR( JSON,obj,"num"     ,ptr->trendNumber_ ,WBPARSER_USE_LEN(TREND_ID) );
}

//follow
t_wbParse_Trend *CWBJsonParser::parse_Trends_Follow(/*[in]*/wbParserNS::REQOBJ *obj)
{
	t_wbParse_Trend *ptr = 0;
	if( obj ) {
		ptr = (t_wbParse_Trend*)wbParse_Malloc_Trend(1);
		parse_Trends_Follow(obj,ptr);
	}
	return ptr;
}
void CWBJsonParser::parse_Trends_Follow(/*[in]*/wbParserNS::REQOBJ *obj,t_wbParse_Trend *ptr)
{
	if( !ptr || !obj )
		return ;

	GET_STR( JSON,obj,"topicid",ptr->trendId_,WBPARSER_USE_LEN(TREND_ID) );
}

// hot
t_wbParse_TrendHotQuery::TrendItem *CWBJsonParser::parse_Trends_getHot(/*[in]*/wbParserNS::REQOBJ *obj)
{
	t_wbParse_TrendHotQuery::TrendItem *ptr = 0;
	if( obj ) {
		ptr = (t_wbParse_TrendHotQuery::TrendItem*)wbParse_Malloc_TrendHotQuery_Item(1);
		parse_Trends_getHot(obj,ptr);
	}
	return ptr;
}
void CWBJsonParser::parse_Trends_getHot(/*[in]*/wbParserNS::REQOBJ *obj,t_wbParse_TrendHotQuery::TrendItem *ptr)
{
	if( !ptr || !obj )
		return ;

	// name
	GET_STR( JSON,obj,"name",ptr->trendName_,WBPARSER_USE_LEN(TREND_NAME) );
	//
	GET_STR( JSON,obj,"query",ptr->trendQuery_,WBPARSER_USE_LEN(TREND_NAME) );
}



/** 一个结果 */
t_wbParse_Ret *CWBJsonParser::parse_Ret(/*[in]*/wbParserNS::REQOBJ *obj)
{
	t_wbParse_Ret *ptr = 0;
	if( obj ) {
		ptr = (t_wbParse_Ret*)wbParse_Malloc_Ret(1);
		parse_Ret(obj,ptr);
	}

	return ptr;
}
void CWBJsonParser::parse_Ret(/*[in]*/wbParserNS::REQOBJ *obj,t_wbParse_Ret *ptr)
{
	if( !ptr || !obj )
		return ;

	// 错误码
	GET_STR_IDX( JSON,obj,0,ptr->error_code_,WBPARSER_REAL_LEN(error_code));
}


// media shorturl batch
t_wbParse_Media_ShortUrlBatch *CWBJsonParser::parse_Media_ShortURLBatch(/*[in]*/wbParserNS::REQOBJ *obj)
{
	t_wbParse_Media_ShortUrlBatch *ptr = 0;
	if( obj ) {
		ptr = (t_wbParse_Media_ShortUrlBatch*)wbParse_Malloc_Ret(1);
		parse_Media_ShortURLBatch(obj,ptr);
	}
	return ptr;
}
void CWBJsonParser::parse_Media_ShortURLBatch(/*[in]*/wbParserNS::REQOBJ *obj,t_wbParse_Media_ShortUrlBatch *ptr )
{
	if( !ptr || !obj )
		return ;

	// short url 
	GET_STR( JSON,obj,"url_short",ptr->urlshort_,WBPARSER_USE_LEN(url) );

	// long url
	GET_STR( JSON,obj,"url_long",ptr->urllong_,WBPARSER_USE_LEN(url) );

	// type
	GET_LONG( JSON,obj,"type",ptr->type_ );

	// title
	GET_STR( JSON,obj,"title",ptr->title_,WBPARSER_REAL_LEN(name) );

	// description
	GET_STR( JSON,obj,"description",ptr->description_,WBPARSER_REAL_LEN(text) );

	// last_modified
	GET_STR( JSON,obj,"last_modified",ptr->lastmodified_,WBPARSER_REAL_LEN(created_at));



	// object  anni
	wbParserNS::REQOBJ *pObjAnni = wbParserNS::GetObject_Key_JSON("annotations",obj) ;
	if( !pObjAnni){
		return ;
	}

	//
	GET_OBJECT_SIZE( JSON,pObjAnni,ptr->anCounts_);
	//
	if( 0 < ptr->anCounts_){
		typedef t_wbParse_Media_ShortUrlBatch::ItemAnnotions ItemAnnotions;
		ItemAnnotions* pAnniList = (ItemAnnotions*)wbParse_Malloc_Media_ShortURLBatch_ItemAnnotions(ptr->anCounts_);
		for( int i = 0 ; i < ptr->anCounts_ ; ++ i ) {
			ItemAnnotions* pAnni = (pAnniList + i);
			parse_Media_ShortURLBatch_ItemAnnotions(pObjAnni,pAnni);
		}
		ptr->annotions_ = pAnniList;
	}
	
}

// media shorturl batch annotions
t_wbParse_Media_ShortUrlBatch::ItemAnnotions *CWBJsonParser::parse_Media_ShortURLBatch_ItemAnnotions(/*[in]*/wbParserNS::REQOBJ *obj)
{
	t_wbParse_Media_ShortUrlBatch::ItemAnnotions *ptr = 0;
	if( obj ) {
		ptr = (t_wbParse_Media_ShortUrlBatch::ItemAnnotions *)wbParse_Malloc_Ret(1);
		parse_Media_ShortURLBatch_ItemAnnotions(obj,ptr);
	}
	return ptr;
}
void CWBJsonParser::parse_Media_ShortURLBatch_ItemAnnotions(/*[in]*/wbParserNS::REQOBJ *obj,t_wbParse_Media_ShortUrlBatch::ItemAnnotions *ptr)
{
	if( !ptr || !obj )
		return ;

	// url 
	GET_STR( JSON,obj,"url",ptr->url,WBPARSER_USE_LEN(url) );

	// pic url
	GET_STR( JSON,obj,"pic",ptr->pic,WBPARSER_USE_LEN(url) );

	// from
	GET_LONG( JSON,obj,"from",ptr->from );

	// title
	GET_STR( JSON,obj,"title",ptr->title,WBPARSER_REAL_LEN(name) );
}


t_wbParse_UploadPic *CWBJsonParser::parse_UploadPic(/*[in]*/wbParserNS::REQOBJ *obj)
{
	t_wbParse_UploadPic *ptr = 0;
	if( obj ) {
		ptr = (t_wbParse_UploadPic*)wbParse_Malloc_Ret(1);
		parse_UploadPic(obj,ptr);
	}
	return ptr;
}
void CWBJsonParser::parse_UploadPic(/*[in]*/wbParserNS::REQOBJ *obj,t_wbParse_UploadPic *ptr)
{
	if( !ptr || !obj )
		return ;

	// pic_id 
	GET_STR( JSON,obj,"pic_id",ptr->pic_id,WBPARSER_USE_LEN(url) );

	// thumbnail_pic
	GET_STR( JSON,obj,"thumbnail_pic",ptr->thumbnail_pic,WBPARSER_USE_LEN(url) );

	// bmiddle_pic
	GET_STR( JSON,obj,"bmiddle_pic",ptr->bmiddle_pic,WBPARSER_USE_LEN(url) );

	// original_pic
	GET_STR( JSON,obj,"original_pic",ptr->original_pic,WBPARSER_REAL_LEN(name) );
}

#endif //#if defined(_USE_JSON_PARSER)



#if defined(_USE_XML_PARSER)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// XML 解析


/** 解析状态 */
t_wbParse_Status  *CWBXmlParser::parse_status(/*[in]*/wbParserNS::REQOBJ *obj)
{
	t_wbParse_Status* ptr = 0;
	if( obj)
	{
		ptr = WEIBO_PARSER_CAST(t_wbParse_Status,wbParse_Malloc_Status(1));
		parse_status(obj, ptr);
	}
	return ptr;
}

void CWBXmlParser::parse_status(wbParserNS::REQOBJ *obj , t_wbParse_Status * ptr)
{
	if( !ptr || !obj )
		return ;
	// id
	GET_LONG_TO_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(id)),ptr->id,WBPARSER_USE_LEN(id) );

	// created_at
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(createtime)),ptr->created_at,WBPARSER_USE_LEN(created_at) );

	// text
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(text)),ptr->text,WBPARSER_USE_LEN(text) );

	// source
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(source)),ptr->source,WBPARSER_USE_LEN(source) );

	// in_reply_to_status_id
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(rsid)),ptr->in_reply_to_status_id,WBPARSER_USE_LEN(in_reply_to_status_id) );

	// in_reply_to_screen_name
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(rtsname)),ptr->in_reply_to_screen_name,WBPARSER_USE_LEN(in_reply_to_screen_name) );

	// thumbnail_pic
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(thumbpic)),ptr->thumbnail_pic,WBPARSER_USE_LEN(thumbnail_pic) );

	// bmiddle_pic
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(midpic)),ptr->bmiddle_pic,WBPARSER_USE_LEN(bmiddle_pic) );

	// original_pic
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(originpic)),ptr->original_pic,WBPARSER_USE_LEN(original_pic) );

	// favorited
	GET_BOOL(XML,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(favorited)),ptr->favorited );

	// truncated
	GET_BOOL(XML,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(truncated)),ptr->truncated );

	//
	USE_WEIBOPARSE_OBJ_PTR ;

	// geo
	GET_OBJECT(XML,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(geo)),ptr->geo,parse_geo);

	// user
	GET_OBJECT(XML,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(user)),ptr->user,parse_user);

	// retweeted_status
	GET_OBJECT(XML,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(retstatus)),ptr->retweeted_status,parse_status);
}


struct t_wbParse_Error* CWBXmlParser::parse_error(wbParserNS::REQOBJ *obj)
{
	t_wbParse_Error* ptr = 0;
	if( obj )
	{
		ptr = WEIBO_PARSER_CAST(t_wbParse_Error,wbParse_Malloc_Error(1) );

		parse_error(obj,ptr);
	}
	return ptr;
}

void CWBXmlParser::parse_error(wbParserNS::REQOBJ *obj , t_wbParse_Error * ptr)
{
	if( !ptr || !obj )
		return ;

	int len = 0;
	const char* txt = NULL;

	//
	//txt = wbParserNS::GetCHAR_Key_XML_EX("request" , obj , len );
	GET_STR_EX(XML,obj,"request",txt,len );
	if( len && txt ) {
		strncpy( ptr->request ,txt , WBPARSER_USE_LEN(request) );
	}

	//txt = wbParserNS::GetCHAR_Key_JSON_EX("error_code" , obj, len);
	GET_STR_EX(XML,obj,"error_code",txt,len);
	if( len && txt ) {
		strncpy( ptr->error_code,txt , WBPARSER_USE_LEN(error_code) );
	}

	//txt = wbParserNS::GetCHAR_Key_JSON_EX("error" , obj, len);
	GET_STR_EX(XML,obj,"error",txt,len);
	if( len && txt ) {
		strncpy( ptr->error,txt,WBPARSER_USE_LEN(error) );
		ptr->error_sub_code = atoi(ptr->error);
	}
}

/** 解析用户 */
t_wbParse_User  *CWBXmlParser::parse_user(/*[in]*/wbParserNS::REQOBJ *obj)
{
	t_wbParse_User* ptr = 0;
	if( obj )
	{
		ptr = WEIBO_PARSER_CAST(t_wbParse_User,wbParse_Malloc_User(1));

		parse_user(obj, ptr );
	}
	return ptr;
}

void CWBXmlParser::parse_user(wbParserNS::REQOBJ *obj , t_wbParse_User * ptr)
{
	if( !ptr || !obj )
		return ;
	// id
	GET_LONG_TO_STR( XML,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(id)),ptr->id,WBPARSER_USE_LEN(id) );

	// screen_name
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(name)),ptr->screen_name,WBPARSER_USE_LEN(screen_name) );

	// name
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(name)),ptr->name,WBPARSER_USE_LEN(name) );


	// province
	GET_LONG_TO_STR( XML,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(province)),ptr->province,WBPARSER_USE_LEN(province) );

	// city
	GET_LONG_TO_STR( XML,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(city)),ptr->city,WBPARSER_USE_LEN(city) );

	// location
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(location)),ptr->location,WBPARSER_USE_LEN(location) );

	// description
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(description)),ptr->description,WBPARSER_USE_LEN(description) );

	// url
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(url)),ptr->url,WBPARSER_USE_LEN(url) );

	// profile_image_url
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(prourl)),ptr->profile_image_url,WBPARSER_USE_LEN(profile_image_url) );

	// domain
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(domain)),ptr->domain,WBPARSER_USE_LEN(domain) );

	// gender
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(gender)),ptr->gender,WBPARSER_USE_LEN(gender) );

	// created_at
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(createdat)),ptr->created_at,WBPARSER_USE_LEN(created_at) );

	// followers_count
	GET_LONG(XML,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(followcount)),ptr->followers_count );

	// followers_count
	GET_LONG(XML,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(frcount)),ptr->friends_count );

	// statuses_count
	GET_LONG(XML,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(stacount)),ptr->statuses_count );

	// favourites_count
	GET_LONG(XML,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(favcount)),ptr->favourites_count );

	// allow_all_act_msg
	GET_BOOL(XML,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(allowall)),ptr->allow_all_act_msg );

	// geo_enabled
	GET_BOOL(XML,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(geoenable)),ptr->geo_enabled );

	// following
	GET_BOOL(XML,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(following)),ptr->following );

	// following
	GET_BOOL(XML,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(verified)),ptr->verified );

	// last status
	USE_WEIBOPARSE_OBJ_PTR;
	GET_OBJECT(XML,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(last_status)),ptr->last_status,parse_status);
}

/** 解析地理位置 */
t_wbParse_Geo *CWBXmlParser::parse_geo(/*[in]*/wbParserNS::REQOBJ *obj)
{
	t_wbParse_Geo *ptr = 0;
	if( obj )
	{
		ptr = WEIBO_PARSER_CAST(t_wbParse_Geo,wbParse_Malloc_Geo(1));

		parse_geo(obj,ptr);
	}
	return ptr;
}

void CWBXmlParser::parse_geo(wbParserNS::REQOBJ *obj , t_wbParse_Geo * ptr)
{
	if( !ptr || !obj )
		return ;
	//
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_GEO(type)),ptr->type,WBPARSER_USE_LEN(type)) ;

	//
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_GEO(coordinates)),ptr->coordinates,WBPARSER_USE_LEN(coordinates)) ;
}

/** 解析评论 */
t_wbParse_Comment *CWBXmlParser::parse_comment(/*[in]*/wbParserNS::REQOBJ *obj)
{
	t_wbParse_Comment *ptr = 0;
	if( obj )
	{
		ptr = WEIBO_PARSER_CAST(t_wbParse_Comment,wbParse_Malloc_Comment(1));
		parse_comment(obj,ptr);
	}
	return ptr;
}

void CWBXmlParser::parse_comment(wbParserNS::REQOBJ *obj , t_wbParse_Comment * ptr)
{
	if( !ptr || !obj )
		return ;
	// id
	GET_LONG_TO_STR( XML,obj,GET_VALUEKEY(WBPARSER_KEY_COMMENTIDX(id)),ptr->id,WBPARSER_USE_LEN(id) );

	// text
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_COMMENTIDX(text)),ptr->text,WBPARSER_USE_LEN(text) );

	// source
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_COMMENTIDX(source)),ptr->source,WBPARSER_USE_LEN(source) );

	// favorited
	GET_BOOL(XML,obj,GET_VALUEKEY(WBPARSER_KEY_COMMENTIDX(favorited)),ptr->favorited );

	// created_at
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_COMMENTIDX(createdat)),ptr->created_at,WBPARSER_USE_LEN(created_at) );

	//
	USE_WEIBOPARSE_OBJ_PTR ;

	// user
	GET_OBJECT(XML,obj,GET_VALUEKEY(WBPARSER_KEY_COMMENTIDX(user)),ptr->user,parse_user);

	// status
	GET_OBJECT(XML,obj,GET_VALUEKEY(WBPARSER_KEY_COMMENTIDX(status)),ptr->status,parse_status);

	// reply_comment
	GET_OBJECT(XML,obj,GET_VALUEKEY(WBPARSER_KEY_COMMENTIDX(rcomment)),ptr->reply_comment,parse_comment);
}

/** 私信 */
t_wbParse_DirectMessage *CWBXmlParser::parse_directmessage(/*[in]*/wbParserNS::REQOBJ *obj)
{
	t_wbParse_DirectMessage* ptr =0;
	if( obj )
	{
		ptr = WEIBO_PARSER_CAST(t_wbParse_DirectMessage,wbParse_Malloc_Directmessage(1));
		parse_directmessage(obj,ptr);
	}
	return ptr;
}

void CWBXmlParser::parse_directmessage(wbParserNS::REQOBJ *obj , t_wbParse_DirectMessage * ptr)
{
	if( !ptr || !obj )
		return ;
	// id
	GET_LONG_TO_STR( XML,obj,GET_VALUEKEY(WBPARSER_KEY_DIRMSGIDX(id)),ptr->id,WBPARSER_USE_LEN(id) );

	// text
	GET_STR( XML,obj,GET_VALUEKEY(WBPARSER_KEY_DIRMSGIDX(text)),ptr->text,WBPARSER_USE_LEN(text) );

	// created_at
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_COMMENTIDX(createdat)),ptr->created_at,WBPARSER_USE_LEN(created_at) );

	// sender_id
	GET_STR( XML,obj,GET_VALUEKEY(WBPARSER_KEY_DIRMSGIDX(senderid)),ptr->sender_id,WBPARSER_USE_LEN(id) );

	// recipient_id
	GET_STR( XML,obj,GET_VALUEKEY(WBPARSER_KEY_DIRMSGIDX(recipientid)),ptr->recipient_id,WBPARSER_USE_LEN(id) );

	// sender_screen_name
	GET_STR( XML,obj,GET_VALUEKEY(WBPARSER_KEY_DIRMSGIDX(sndsname)),ptr->sender_screen_name,WBPARSER_USE_LEN(name) );

	//recipient_screen_name
	GET_STR( XML,obj,GET_VALUEKEY(WBPARSER_KEY_DIRMSGIDX(rsnsame)),ptr->recipient_screen_name,WBPARSER_USE_LEN(name) );

	//
	USE_WEIBOPARSE_OBJ_PTR ;

	//sender
	GET_OBJECT( XML,obj,GET_VALUEKEY(WBPARSER_KEY_DIRMSGIDX(sender)),ptr->sender,parse_user );

	//recipient
	GET_OBJECT( XML,obj,GET_VALUEKEY(WBPARSER_KEY_DIRMSGIDX(recipient)),ptr->recipient,parse_user );
}

/** 解析未读数 */
t_wbParse_Unread  *CWBXmlParser::parse_unread(/*[in]*/wbParserNS::REQOBJ *obj)
{
	t_wbParse_Unread *ptr = 0;
	if( obj )
	{
		ptr = WEIBO_PARSER_CAST(t_wbParse_Unread,wbParse_Malloc_Unread(1));

		parse_unread(obj ,ptr );
	}
	return ptr;
}

void CWBXmlParser::parse_unread(wbParserNS::REQOBJ *obj , t_wbParse_Unread * ptr)
{
	if( !ptr || !obj )
		return ;
	// comments
	GET_LONG(XML,obj,GET_VALUEKEY(WBPARSER_KEY_UREADER(comments)),ptr->comments);

	// mentions
	GET_LONG(XML,obj,GET_VALUEKEY(WBPARSER_KEY_UREADER(mentions)),ptr->mentions);

	// dm
	GET_LONG(XML,obj,GET_VALUEKEY(WBPARSER_KEY_UREADER(dm)),ptr->dm);

	// followers
	GET_LONG(XML,obj,GET_VALUEKEY(WBPARSER_KEY_UREADER(followers)),ptr->followers);

	GET_LONG(XML,obj,GET_VALUEKEY(WBPARSER_KEY_UREADER(new_status)),ptr->new_status);
	
}

/** 解析评论计数 */
t_wbParse_CommentCounts *CWBXmlParser::parse_commentcounts(/*[in]*/wbParserNS::REQOBJ *obj)
{
	t_wbParse_CommentCounts *ptr = 0;
	if( obj)
	{
		ptr = WEIBO_PARSER_CAST(t_wbParse_CommentCounts,wbParse_Malloc_Commentcount(1));
		parse_commentcounts(obj,ptr);
	}
	return ptr;
}

void CWBXmlParser::parse_commentcounts(wbParserNS::REQOBJ *obj , t_wbParse_CommentCounts * ptr)
{
	if( !ptr || !obj )
		return ;
	// id
	GET_LONG_TO_STR( XML,obj,GET_VALUEKEY(WBPARSER_KEY_COMCOUNT(id)),ptr->id,WBPARSER_USE_LEN(id) );

	// comments
	GET_LONG( XML,obj,GET_VALUEKEY(WBPARSER_KEY_COMCOUNT(comments)),ptr->comments );

	// rt
	GET_LONG( XML,obj,GET_VALUEKEY(WBPARSER_KEY_COMCOUNT(rt)),ptr->rt );
}


t_wbParse_LimitStatus *CWBXmlParser::parse_limite_status(/*[in]*/wbParserNS::REQOBJ *obj)
{
	t_wbParse_LimitStatus *ptr = 0;
	if( obj)
	{
		ptr = (t_wbParse_LimitStatus *)wbParse_Malloc_Limits(1);
		parse_limite_status(obj ,  ptr);
	}
	return ptr;
}

void CWBXmlParser::parse_limite_status(wbParserNS::REQOBJ *obj , t_wbParse_LimitStatus * ptr)
{
	if( !ptr || !obj )
		return ;
	// hourly_limit
	GET_LONG( XML,obj,GET_VALUEKEY(WBPARSER_KEY_ACCESS_LMT(hourlimit)),ptr->hourly_limit );

	// rstimeinseond
	GET_LONG( XML,obj,GET_VALUEKEY(WBPARSER_KEY_ACCESS_LMT(rstimeinseond)),ptr->reset_time_in_seconds );

	// remaining_hits
	GET_LONG( XML,obj,GET_VALUEKEY(WBPARSER_KEY_ACCESS_LMT(remaining_hits)),ptr->remaining_hits );

	// reset_time
	GET_STR( XML,obj,GET_VALUEKEY(WBPARSER_KEY_ACCESS_LMT(reset_time)),ptr->reset_time,WBPARSER_USE_LEN(reset_time) );
}


t_wbParse_Emotion *CWBXmlParser::parse_emotion(/*[in]*/wbParserNS::REQOBJ *obj)
{
	t_wbParse_Emotion *ptr = 0;
	if( obj )
	{
		ptr = (t_wbParse_Emotion *)wbParse_Malloc_Emotion(1);
		parse_emotion(obj , ptr);
	}
	return ptr;
}

void CWBXmlParser::parse_emotion(wbParserNS::REQOBJ *obj , t_wbParse_Emotion * ptr)
{
	if( !ptr || !obj )
		return ;
	// phrase
	GET_STR( XML,obj,GET_VALUEKEY(WBPARSER_KEY_EMOTION(phrase)),ptr->phrase,WBPARSER_USE_LEN(phrase) );

	// type
	GET_STR( XML,obj,GET_VALUEKEY(WBPARSER_KEY_EMOTION(type)),ptr->type,WBPARSER_USE_LEN(type) );

	// url
	GET_STR( XML,obj,GET_VALUEKEY(WBPARSER_KEY_EMOTION(url)),ptr->url,WBPARSER_USE_LEN(url) );

	//category
	GET_STR( XML,obj,GET_VALUEKEY(WBPARSER_KEY_EMOTION(category)),ptr->category,WBPARSER_USE_LEN(category) );

	// is_hot
	GET_LONG( XML,obj,GET_VALUEKEY(WBPARSER_KEY_EMOTION(is_hot)),ptr->is_hot );

	// is_common
	GET_LONG( XML,obj,GET_VALUEKEY(WBPARSER_KEY_EMOTION(is_common)),ptr->is_common );

	// order_number
	GET_LONG( XML,obj,GET_VALUEKEY(WBPARSER_KEY_EMOTION(order_number)),ptr->order_number );
}


t_wbParse_Shipshow *CWBXmlParser::parse_shipshow(/*[in]*/wbParserNS::REQOBJ *obj)
{
	t_wbParse_Shipshow *ptr = 0;
	if( obj )
	{
		ptr = (t_wbParse_Shipshow *)wbParse_Malloc_Friendship(1);
		parse_shipshow(obj , ptr);
	}
	return ptr;
}

void CWBXmlParser::parse_shipshow(/*[in]*/wbParserNS::REQOBJ *obj,t_wbParse_Shipshow *ptr)	
{
//{
 //   "source":{
 //       "id":245110499
 //       ,"screen_name":"245110499"
 //       ,"following":false
 //       ,"followed_by":false
 //       ,"notifications_enabled":false
 //  }
 //  ,"target":{
 //       "id":10503      
 //       ,"screen_name":"10503"
 //       ,"following":false
 //       ,"followed_by":false
 //       ,"notifications_enabled":false
 // }
 //}
	if( !ptr || !obj )
		return ;
	//
	USE_WEIBOPARSE_OBJ_PTR ;

	//source
	GET_OBJECT( XML,obj,"source",ptr->source,parse_ship_item );

	//source
	GET_OBJECT( XML,obj,"target",ptr->target,parse_ship_item );
}

t_wbParse_Shipshow::Item* CWBXmlParser::parse_ship_item(wbParserNS::REQOBJ *obj)
{
//   "source":{
 //       "id":245110499
 //       ,"screen_name":"245110499"
 //       ,"following":false
 //       ,"followed_by":false
 //       ,"notifications_enabled":false
 //  }
	if( !obj )
		return 0;
	t_wbParse_Shipshow::Item* ptr = (t_wbParse_Shipshow::Item*)malloc(sizeof(t_wbParse_Shipshow::Item));
	memset(ptr , 0 , sizeof(t_wbParse_Shipshow::Item));

	// id
	GET_LONG_TO_STR( XML,obj,"id",ptr->id,WBPARSER_USE_LEN(id) );

	// screen_name
	GET_STR( XML,obj,"screen_name",ptr->screen_name,WBPARSER_USE_LEN(screen_name) );

	// following
	GET_LONG( XML,obj,"following",ptr->following);

	// following
	GET_LONG( XML,obj,"followed_by",ptr->followed_by);

	// following
	GET_LONG( XML,obj,"notifications_enabled",ptr->notifications_enabled);

	return ptr;
}

#endif //#if defined(_USE_XML_PARSER)

extern
void wbParserNS::CheckShortLink(struct t_wbParse_Status *pStatus ,const char* wbParentId , t_wbParse_Media** pMedia , int& size ,int cmd)
{
	if( !pStatus || !pMedia )
		return ;

	const char* szText = pStatus->text;
	const char* szEnd  = szText + WBPARSER_USE_LEN(text);
	const char* start = 0;
	const char* end = 0,*shortID = 0;

	while( 0 == wb_split_shortlink( szText , szEnd , start , end , shortID ) )
	{
		t_wbParse_Media* pmtemp = 0;
		if( 0 == *pMedia )
		{
			size = 1;
			*pMedia = (t_wbParse_Media*)wbParse_Malloc_Media(1);
			pmtemp = *pMedia;
		}
		else
		{// 累积
			*pMedia = (t_wbParse_Media*)realloc(*pMedia , sizeof(t_wbParse_Media)*(size+1));
			pmtemp  = *pMedia + (size++);
			memset(pmtemp , 0 , sizeof(t_wbParse_Media));
		}
		pmtemp->cmd = cmd;
		// fill media information
		if (wbParentId)
		{
			strncpy(pmtemp->parentid , wbParentId , WBPARSER_USE_LEN(id) );
		}
		strncpy(pmtemp->id ,pStatus->id , WBPARSER_USE_LEN(id) );
		strncpy(pmtemp->mitem.id , shortID , end - shortID );
		// 下一个
		szText = end;
	}

	if( pStatus ->retweeted_status )
	{
		CheckShortLink(pStatus ->retweeted_status , pStatus->id , pMedia , size,cmd);
	}
}

extern
void wbParserNS::MMIdCompare(struct t_wbParser_MMId* pMMId ,t_wbParser_MMId::eType type ,const char* id , const char* time)
{
	long long ll = 0;
#if defined(WIN32)
	ll = _atoi64(id);
#else
#error("not support")
#endif
	struct t_wbParser_MMId::IdValue* idv = &pMMId->_idVal[type];
	if( idv->maxId < ll )
		idv->maxId = ll;
	if( idv->minId > ll || (idv->minId == llMAX))
		idv->minId = ll;
}
