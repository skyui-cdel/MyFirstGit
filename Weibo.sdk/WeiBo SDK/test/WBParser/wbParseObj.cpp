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
#include "wbParseObj.h"
#include "wbParseMalloc.h"
#include "splitstr.h"

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
	// p
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
	// p
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
#define WEIBO_PARSER_CAST(t,p) (t*)p


#define GET_LONG(type,obj,key,out)       out = wbParserNS::GetLONG_Key_##type( key,obj );
#define GET_BOOL(type,obj,key,out)       out = ( wbParserNS::GetLONG_Key_##type( key,obj ) == 1 )? (WTRUE) : (WFALSE);
#define GET_STR(type,obj,key,out,outlen) wbParserNS::GetCHAR_Key_##type( key,obj,out,outlen )

#if defined(WIN32) || defined(WINCE)
#define GET_LONG_TO_STR(type,obj,key,out,outlen) _i64toa( wbParserNS::GetLONG_Key_##type( key,obj ),out,10 );
#else
#define GET_LONG_TO_STR(type,obj,key,out,outlen) sprintf( out,"%ld",GetLONG_Key_##type( key,obj ) );
#endif //


#define USE_WEIBOPARSE_OBJ_PTR wbParserNS::REQOBJ *pObj = NULL;
#define GET_OBJECT(type,obj,key,out,parse)\
	{\
		pObj = wbParserNS::GetObject_Key_##type(key,obj) ;\
		if( pObj ){ out = parse(pObj);}\
	}\



#if defined(_USE_JSON_PARSER)

/** 解析状态 */
t_wbParse_Status  *CWBJsonParser::parse_status(/*[in]*/wbParserNS::REQOBJ *obj)
{
	t_wbParse_Status* p = 0;
	if( obj)
	{
		p = WEIBO_PARSER_CAST(t_wbParse_Status,wbParse_Malloc_Status(1));
		parse_status(obj, p);
	}
	return p;
}

void CWBJsonParser::parse_status(wbParserNS::REQOBJ *obj , t_wbParse_Status * p)
{
	if( !p || !obj )
		return ;
	// id
	GET_LONG_TO_STR( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(id)),p->id,WBPARSER_USE_LEN(id) );

	// create_time
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(createtime)),p->create_time,WBPARSER_USE_LEN(create_time) );

	// text
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(text)),p->text,WBPARSER_USE_LEN(text) );

	// source
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(source)),p->source,WBPARSER_USE_LEN(source) );

	// in_reply_to_status_id
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(rsid)),p->in_reply_to_status_id,WBPARSER_USE_LEN(in_reply_to_status_id) );

	// in_reply_to_screen_name
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(rtsname)),p->in_reply_to_screen_name,WBPARSER_USE_LEN(in_reply_to_screen_name) );

	// thumbnail_pic
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(thumbpic)),p->thumbnail_pic,WBPARSER_USE_LEN(thumbnail_pic) );

	// bmiddle_pic
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(midpic)),p->bmiddle_pic,WBPARSER_USE_LEN(bmiddle_pic) );

	// original_pic
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(originpic)),p->original_pic,WBPARSER_USE_LEN(original_pic) );

	// favorited
	GET_BOOL(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(favorited)),p->favorited );

	// truncated
	GET_BOOL(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(truncated)),p->truncated );

	//
	USE_WEIBOPARSE_OBJ_PTR ;

	// geo
	GET_OBJECT(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(geo)),p->geo,parse_geo);

	// user
	GET_OBJECT(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(user)),p->user,parse_user);

	// retweeted_status
	GET_OBJECT(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(retstatus)),p->retweeted_status,parse_status);
}


struct t_wbParse_Error* CWBJsonParser::parse_error(wbParserNS::REQOBJ *obj)
{
	t_wbParse_Error* p = 0;
	if( obj )
	{
		p = WEIBO_PARSER_CAST(t_wbParse_Error,wbParse_Malloc_Error(1) );

		parse_error(obj,p);
	}
	return p;
}

void CWBJsonParser::parse_error(wbParserNS::REQOBJ *obj , t_wbParse_Error * p)
{
	if( !p || !obj )
		return ;

	int len = 0;
	const char* txt = wbParserNS::GetCHAR_Key_JSON_EX("request" , obj , len );
	if( len && txt )
	{
		strncpy( p->request ,txt , WBPARSER_USE_LEN(request) );
	}
	txt = wbParserNS::GetCHAR_Key_JSON_EX("error_code" , obj, len);
	if( len && txt )
	{
		strncpy( p->error_code ,txt , WBPARSER_USE_LEN(error_code) );
	}
	txt = wbParserNS::GetCHAR_Key_JSON_EX("error" , obj, len);
	if( len && txt )
	{
		strncpy( p->error ,txt , WBPARSER_USE_LEN(error) );
		p->error_sub_code = atoi(p->error);
	}
}

/** 解析用户 */
t_wbParse_User  *CWBJsonParser::parse_user(/*[in]*/wbParserNS::REQOBJ *obj)
{
	t_wbParse_User* p = 0;
	if( obj )
	{
		p = WEIBO_PARSER_CAST(t_wbParse_User,wbParse_Malloc_User(1));

		parse_user(obj, p );
	}
	return p;
}

void CWBJsonParser::parse_user(wbParserNS::REQOBJ *obj , t_wbParse_User * p)
{
	if( !p || !obj )
		return ;
	// id
	GET_LONG_TO_STR( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(id)),p->id,WBPARSER_USE_LEN(id) );

	// screen_name
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(name)),p->screen_name,WBPARSER_USE_LEN(screen_name) );

	// name
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(name)),p->name,WBPARSER_USE_LEN(name) );


	// province
	GET_LONG_TO_STR( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(province)),p->province,WBPARSER_USE_LEN(province) );

	// city
	GET_LONG_TO_STR( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(city)),p->city,WBPARSER_USE_LEN(city) );

	// location
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(location)),p->location,WBPARSER_USE_LEN(location) );

	// description
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(description)),p->description,WBPARSER_USE_LEN(description) );

	// url
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(url)),p->url,WBPARSER_USE_LEN(url) );

	// profile_image_url
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(prourl)),p->profile_image_url,WBPARSER_USE_LEN(profile_image_url) );

	// domain
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(domain)),p->domain,WBPARSER_USE_LEN(domain) );

	// gender
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(gender)),p->gender,WBPARSER_USE_LEN(gender) );

	// created_at
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(createdat)),p->created_at,WBPARSER_USE_LEN(created_at) );

	// followers_count
	GET_LONG(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(followcount)),p->followers_count );

	// followers_count
	GET_LONG(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(frcount)),p->friends_count );

	// statuses_count
	GET_LONG(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(stacount)),p->statuses_count );

	// favourites_count
	GET_LONG(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(favcount)),p->favourites_count );

	// allow_all_act_msg
	GET_BOOL(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(allowall)),p->allow_all_act_msg );

	// geo_enabled
	GET_BOOL(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(geoenable)),p->geo_enabled );

	// following
	GET_BOOL(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(following)),p->following );

	// following
	GET_BOOL(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(verified)),p->verified );

	// last status
	USE_WEIBOPARSE_OBJ_PTR;
	GET_OBJECT(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(last_status)),p->last_status,parse_status);
}

/** 解析地理位置 */
t_wbParse_Geo *CWBJsonParser::parse_geo(/*[in]*/wbParserNS::REQOBJ *obj)
{
	t_wbParse_Geo *p = 0;
	if( obj )
	{
		p = WEIBO_PARSER_CAST(t_wbParse_Geo,wbParse_Malloc_Geo(1));

		parse_geo(obj,p);
	}
	return p;
}

void CWBJsonParser::parse_geo(wbParserNS::REQOBJ *obj , t_wbParse_Geo * p)
{
	if( !p || !obj )
		return ;
	//
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_GEO(type)),p->type,WBPARSER_USE_LEN(type)) ;

	//
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_GEO(coordinates)),p->coordinates,WBPARSER_USE_LEN(coordinates)) ;
}

/** 解析评论 */
t_wbParse_Comment *CWBJsonParser::parse_comment(/*[in]*/wbParserNS::REQOBJ *obj)
{
	t_wbParse_Comment *p = 0;
	if( obj )
	{
		p = WEIBO_PARSER_CAST(t_wbParse_Comment,wbParse_Malloc_Comment(1));
		parse_comment(obj,p);
	}
	return p;
}

void CWBJsonParser::parse_comment(wbParserNS::REQOBJ *obj , t_wbParse_Comment * p)
{
	if( !p || !obj )
		return ;
	// id
	GET_LONG_TO_STR( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_COMMENTIDX(id)),p->id,WBPARSER_USE_LEN(id) );

	// text
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_COMMENTIDX(text)),p->text,WBPARSER_USE_LEN(text) );

	// source
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_COMMENTIDX(source)),p->source,WBPARSER_USE_LEN(source) );

	// favorited
	GET_BOOL(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_COMMENTIDX(favorited)),p->favorited );

	// created_at
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_COMMENTIDX(createdat)),p->created_at,WBPARSER_USE_LEN(created_at) );

	//
	USE_WEIBOPARSE_OBJ_PTR ;

	// user
	GET_OBJECT(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_COMMENTIDX(user)),p->user,parse_user);

	// status
	GET_OBJECT(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_COMMENTIDX(status)),p->status,parse_status);

	// reply_comment
	GET_OBJECT(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_COMMENTIDX(rcomment)),p->reply_comment,parse_comment);
}

/** 私信 */
t_wbParse_DirectMessage *CWBJsonParser::parse_directmessage(/*[in]*/wbParserNS::REQOBJ *obj)
{
	t_wbParse_DirectMessage* p =0;
	if( obj )
	{
		p = WEIBO_PARSER_CAST(t_wbParse_DirectMessage,wbParse_Malloc_Directmessage(1));
		parse_directmessage(obj,p);
	}
	return p;
}

void CWBJsonParser::parse_directmessage(wbParserNS::REQOBJ *obj , t_wbParse_DirectMessage * p)
{
	if( !p || !obj )
		return ;
	// id
	GET_LONG_TO_STR( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_DIRMSGIDX(id)),p->id,WBPARSER_USE_LEN(id) );

	// text
	GET_STR( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_DIRMSGIDX(text)),p->text,WBPARSER_USE_LEN(text) );

	// created_at
	GET_STR(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_COMMENTIDX(createdat)),p->created_at,WBPARSER_USE_LEN(created_at) );

	// sender_id
	GET_STR( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_DIRMSGIDX(senderid)),p->sender_id,WBPARSER_USE_LEN(id) );

	// recipient_id
	GET_STR( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_DIRMSGIDX(recipientid)),p->recipient_id,WBPARSER_USE_LEN(id) );

	// sender_screen_name
	GET_STR( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_DIRMSGIDX(sndsname)),p->sender_screen_name,WBPARSER_USE_LEN(name) );

	//recipient_screen_name
	GET_STR( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_DIRMSGIDX(rsnsame)),p->recipient_screen_name,WBPARSER_USE_LEN(name) );

	//
	USE_WEIBOPARSE_OBJ_PTR ;

	//sender
	GET_OBJECT( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_DIRMSGIDX(sender)),p->sender,parse_user );

	//recipient
	GET_OBJECT( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_DIRMSGIDX(recipient)),p->recipient,parse_user );
}

/** 解析未读数 */
t_wbParse_Unread  *CWBJsonParser::parse_unread(/*[in]*/wbParserNS::REQOBJ *obj)
{
	t_wbParse_Unread *p = 0;
	if( obj )
	{
		p = WEIBO_PARSER_CAST(t_wbParse_Unread,wbParse_Malloc_Unread(1));

		parse_unread(obj ,p );
	}
	return p;
}

void CWBJsonParser::parse_unread(wbParserNS::REQOBJ *obj , t_wbParse_Unread * p)
{
	if( !p || !obj )
		return ;
	// comments
	GET_LONG(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_UREADER(comments)),p->comments);

	// mentions
	GET_LONG(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_UREADER(mentions)),p->mentions);

	// dm
	GET_LONG(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_UREADER(dm)),p->dm);

	// followers
	GET_LONG(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_UREADER(followers)),p->followers);

	GET_LONG(JSON,obj,GET_VALUEKEY(WBPARSER_KEY_UREADER(new_status)),p->new_status);
	
}

/** 解析评论计数 */
t_wbParse_CommentCounts *CWBJsonParser::parse_commentcounts(/*[in]*/wbParserNS::REQOBJ *obj)
{
	t_wbParse_CommentCounts *p = 0;
	if( obj)
	{
		p = WEIBO_PARSER_CAST(t_wbParse_CommentCounts,wbParse_Malloc_Commentcount(1));
		parse_commentcounts(obj,p);
	}
	return p;
}

void CWBJsonParser::parse_commentcounts(wbParserNS::REQOBJ *obj , t_wbParse_CommentCounts * p)
{
	if( !p || !obj )
		return ;
	// id
	GET_LONG_TO_STR( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_COMCOUNT(id)),p->id,WBPARSER_USE_LEN(id) );

	// comments
	GET_LONG( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_COMCOUNT(comments)),p->comments );

	// rt
	GET_LONG( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_COMCOUNT(rt)),p->rt );
}


t_wbParse_LimitStatus *CWBJsonParser::parse_limite_status(/*[in]*/wbParserNS::REQOBJ *obj)
{
	t_wbParse_LimitStatus *p = 0;
	if( obj)
	{
		p = (t_wbParse_LimitStatus *)wbParse_Malloc_Limits(1);
		parse_emotion(obj ,  p);
	}
	return p;
}

void CWBJsonParser::parse_emotion(wbParserNS::REQOBJ *obj , t_wbParse_LimitStatus * p)
{
	if( !p || !obj )
		return ;
	// hourly_limit
	GET_LONG( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_ACCESS_LMT(hourlimit)),p->hourly_limit );

	// rstimeinseond
	GET_LONG( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_ACCESS_LMT(rstimeinseond)),p->reset_time_in_seconds );

	// remaining_hits
	GET_LONG( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_ACCESS_LMT(remaining_hits)),p->remaining_hits );

	// reset_time
	GET_STR( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_ACCESS_LMT(reset_time)),p->reset_time,WBPARSER_USE_LEN(reset_time) );
}


t_wbParse_Emotion *CWBJsonParser::parse_emotion(/*[in]*/wbParserNS::REQOBJ *obj)
{
	t_wbParse_Emotion *p = 0;
	if( obj )
	{
		p = (t_wbParse_Emotion *)wbParse_Malloc_Emotion(1);
		parse_emotion(obj , p);
	}
	return p;
}

void CWBJsonParser::parse_emotion(wbParserNS::REQOBJ *obj , t_wbParse_Emotion * p)
{
	if( !p || !obj )
		return ;
	// phrase
	GET_STR( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_EMOTION(phrase)),p->phrase,WBPARSER_USE_LEN(phrase) );

	// type
	GET_STR( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_EMOTION(type)),p->type,WBPARSER_USE_LEN(type) );

	// url
	GET_STR( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_EMOTION(url)),p->url,WBPARSER_USE_LEN(url) );

	//category
	GET_STR( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_EMOTION(category)),p->category,WBPARSER_USE_LEN(category) );

	// is_hot
	GET_LONG( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_EMOTION(is_hot)),p->is_hot );

	// is_common
	GET_LONG( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_EMOTION(is_common)),p->is_common );

	// order_number
	GET_LONG( JSON,obj,GET_VALUEKEY(WBPARSER_KEY_EMOTION(order_number)),p->order_number );
}


t_wbParse_Shipshow *CWBJsonParser::parse_shipshow(/*[in]*/wbParserNS::REQOBJ *obj)
{
	t_wbParse_Shipshow *p = 0;
	if( obj )
	{
		p = (t_wbParse_Shipshow *)wbParse_Malloc_Friendship(1);
		parse_shipshow(obj , p);
	}
	return p;
}

void CWBJsonParser::parse_shipshow(/*[in]*/wbParserNS::REQOBJ *obj,t_wbParse_Shipshow *p)	
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
	if( !p || !obj )
		return ;
	//
	USE_WEIBOPARSE_OBJ_PTR ;

	//source
	GET_OBJECT( JSON,obj,"source",p->source,parse_ship_item );

	//source
	GET_OBJECT( JSON,obj,"target",p->target,parse_ship_item );
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
	t_wbParse_Shipshow::Item* p = (t_wbParse_Shipshow::Item*)malloc(sizeof(t_wbParse_Shipshow::Item));
	memset(p , 0 , sizeof(t_wbParse_Shipshow::Item));

	// id
	GET_LONG_TO_STR( JSON,obj,"id",p->id,WBPARSER_USE_LEN(id) );

	// screen_name
	GET_STR( JSON,obj,"screen_name",p->screen_name,WBPARSER_USE_LEN(screen_name) );

	// following
	GET_LONG( JSON,obj,"following",p->following);

	// following
	GET_LONG( JSON,obj,"followed_by",p->following);

	// following
	GET_LONG( JSON,obj,"notifications_enabled",p->following);

	return p;
}


#endif //#if defined(_USE_JSON_PARSER)


#if defined(_USE_XML_PARSER)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// XML 解析


/** 解析状态 */
t_wbParse_Status  *CWBXmlParser::parse_status(/*[in]*/wbParserNS::REQOBJ *obj)
{
	if( !obj)
		return NULL;

	t_wbParse_Status* p = WEIBO_PARSER_CAST(t_wbParse_Status,wbParse_Malloc_Status());

	// id
	GET_LONG_TO_STR( XML,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(id)),p->id,WBPARSER_USE_LEN(id) );

	// create_time
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(createtime)),p->create_time,WBPARSER_USE_LEN(create_time) );

	// text
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(text)),p->text,WBPARSER_USE_LEN(text) );

	// source
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(source)),p->source,WBPARSER_USE_LEN(source) );

	// in_reply_to_status_id
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(rsid)),p->in_reply_to_status_id,WBPARSER_USE_LEN(in_reply_to_status_id) );

	// in_reply_to_screen_name
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(rtsname)),p->in_reply_to_screen_name,WBPARSER_USE_LEN(in_reply_to_screen_name) );

	// thumbnail_pic
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(thumbpic)),p->thumbnail_pic,WBPARSER_USE_LEN(thumbnail_pic) );

	// bmiddle_pic
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(midpic)),p->bmiddle_pic,WBPARSER_USE_LEN(bmiddle_pic) );

	// original_pic
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(originpic)),p->original_pic,WBPARSER_USE_LEN(original_pic) );

	// favorited
	GET_BOOL(XML,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(favorited)),p->favorited );

	// truncated
	GET_BOOL(XML,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(truncated)),p->truncated );

	//
	USE_WEIBOPARSE_OBJ_PTR ;

	// geo
	GET_OBJECT(XML,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(geo)),p->geo,parse_geo);

	// user
	GET_OBJECT(XML,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(user)),p->user,parse_user);

	// retweeted_status
	GET_OBJECT(XML,obj,GET_VALUEKEY(WBPARSER_KEY_STATEIDX(retstatus)),p->retweeted_status,parse_status);

	//
	return p;
}

/** 解析用户 */
t_wbParse_User  *CWBXmlParser::parse_user(/*[in]*/wbParserNS::REQOBJ *obj)
{
	t_wbParse_User* p = WEIBO_PARSER_CAST(t_wbParse_User,wbParse_Malloc_User());

	// id
	GET_LONG_TO_STR( XML,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(id)),p->id,WBPARSER_USE_LEN(id) );

	// screen_name
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(scname)),p->screen_name,WBPARSER_USE_LEN(screen_name) );

	// name
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(name)),p->name,WBPARSER_USE_LEN(name) );


	// province
	GET_LONG_TO_STR( XML,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(province)),p->province,WBPARSER_USE_LEN(province) );

	// city
	GET_LONG_TO_STR( XML,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(city)),p->city,WBPARSER_USE_LEN(city) );

	// location
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(location)),p->location,WBPARSER_USE_LEN(location) );

	// description
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(description)),p->description,WBPARSER_USE_LEN(description) );

	// url
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(url)),p->url,WBPARSER_USE_LEN(url) );

	// profile_image_url
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(prourl)),p->profile_image_url,WBPARSER_USE_LEN(profile_image_url) );

	// domain
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(domain)),p->domain,WBPARSER_USE_LEN(domain) );

	// gender
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(gender)),p->gender,WBPARSER_USE_LEN(gender) );

	// created_at
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(createdat)),p->created_at,WBPARSER_USE_LEN(created_at) );

	// followers_count
	GET_LONG(XML,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(followcount)),p->followers_count );

	// followers_count
	GET_LONG(XML,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(frcount)),p->friends_count );

	// statuses_count
	GET_LONG(XML,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(stacount)),p->statuses_count );

	// favourites_count
	GET_LONG(XML,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(favcount)),p->favourites_count );

	// allow_all_act_msg
	GET_BOOL(XML,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(allowall)),p->allow_all_act_msg );

	// geo_enabled
	GET_BOOL(XML,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(geoenable)),p->geo_enabled );

	// following
	GET_BOOL(XML,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(following)),p->following );

	// following
	GET_BOOL(XML,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(verified)),p->verified );


	// last status
	USE_WEIBOPARSE_OBJ_PTR;
	GET_OBJECT(XML,obj,GET_VALUEKEY(WBPARSER_KEY_USERIDX(last_status)),p->last_status,parse_status);

	return p;
}

/** 解析地理位置 */
t_wbParse_Geo *CWBXmlParser::parse_geo(/*[in]*/wbParserNS::REQOBJ *obj)
{
	if( !obj )
		return NULL;

	t_wbParse_Geo *p = WEIBO_PARSER_CAST(t_wbParse_Geo,wbParse_Malloc_Geo());

	//
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_GEO(type)),p->type,WBPARSER_USE_LEN(type)) ;

	//
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_GEO(coordinates)),p->coordinates,WBPARSER_USE_LEN(coordinates)) ;

	return p;
}

/** 解析评论 */
t_wbParse_Comment *CWBXmlParser::parse_comment(/*[in]*/wbParserNS::REQOBJ *obj)
{
	if( !obj )
		return NULL;

	t_wbParse_Comment *p = WEIBO_PARSER_CAST(t_wbParse_Comment,wbParse_Malloc_Comment());

	// id
	GET_LONG_TO_STR( XML,obj,GET_VALUEKEY(WBPARSER_KEY_COMMENTIDX(id)),p->id,WBPARSER_USE_LEN(id) );

	// text
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_COMMENTIDX(text)),p->text,WBPARSER_USE_LEN(text) );

	// source
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_COMMENTIDX(source)),p->source,WBPARSER_USE_LEN(source) );

	// favorited
	GET_BOOL(XML,obj,GET_VALUEKEY(WBPARSER_KEY_COMMENTIDX(favorited)),p->favorited );

	// created_at
	GET_STR(XML,obj,GET_VALUEKEY(WBPARSER_KEY_COMMENTIDX(createdat)),p->created_at,WBPARSER_USE_LEN(created_at) );

	//
	USE_WEIBOPARSE_OBJ_PTR ;

	// user
	GET_OBJECT(XML,obj,GET_VALUEKEY(WBPARSER_KEY_COMMENTIDX(user)),p->user,parse_user);

	// status
	GET_OBJECT(XML,obj,GET_VALUEKEY(WBPARSER_KEY_COMMENTIDX(status)),p->status,parse_status);

	// reply_comment
	GET_OBJECT(XML,obj,GET_VALUEKEY(WBPARSER_KEY_COMMENTIDX(rcomment)),p->reply_comment,parse_comment);

	return p;
}

/** 私信 */
t_wbParse_DirectMessage *CWBXmlParser::parse_directmessage(/*[in]*/wbParserNS::REQOBJ *obj)
{
	if( !obj )
		return NULL;

	t_wbParse_DirectMessage* p = WEIBO_PARSER_CAST(t_wbParse_DirectMessage,wbParse_Malloc_Directmessage());

	// id
	GET_LONG_TO_STR( XML,obj,GET_VALUEKEY(WBPARSER_KEY_DIRMSGIDX(id)),p->id,WBPARSER_USE_LEN(id) );

	// text
	GET_STR( XML,obj,GET_VALUEKEY(WBPARSER_KEY_DIRMSGIDX(text)),p->text,WBPARSER_USE_LEN(text) );

	// sender_id
	GET_STR( XML,obj,GET_VALUEKEY(WBPARSER_KEY_DIRMSGIDX(senderid)),p->sender_id,WBPARSER_USE_LEN(id) );

	// recipient_id
	GET_STR( XML,obj,GET_VALUEKEY(WBPARSER_KEY_DIRMSGIDX(recipientid)),p->recipient_id,WBPARSER_USE_LEN(id) );

	// sender_screen_name
	GET_STR( XML,obj,GET_VALUEKEY(WBPARSER_KEY_DIRMSGIDX(sndsname)),p->sender_screen_name,WBPARSER_USE_LEN(name) );

	//recipient_screen_name
	GET_STR( XML,obj,GET_VALUEKEY(WBPARSER_KEY_DIRMSGIDX(rsnsame)),p->recipient_screen_name,WBPARSER_USE_LEN(name) );

	//
	USE_WEIBOPARSE_OBJ_PTR ;

	//sender
	GET_OBJECT( XML,obj,GET_VALUEKEY(WBPARSER_KEY_DIRMSGIDX(sender)),p->sender,parse_user );

	//recipient
	GET_OBJECT( XML,obj,GET_VALUEKEY(WBPARSER_KEY_DIRMSGIDX(recipient)),p->recipient,parse_user );

	return p;
}

/** 解析未读数 */
t_wbParse_Unread  *CWBXmlParser::parse_unread(/*[in]*/wbParserNS::REQOBJ *obj)
{
	if( !obj )
		return NULL;

	t_wbParse_Unread *p = WEIBO_PARSER_CAST(t_wbParse_Unread,wbParse_Malloc_Unread());

	// comments
	GET_LONG(XML,obj,GET_VALUEKEY(WBPARSER_KEY_UREADER(comments)),p->comments);

	// mentions
	GET_LONG(XML,obj,GET_VALUEKEY(WBPARSER_KEY_UREADER(mentions)),p->mentions);

	// dm
	GET_LONG(XML,obj,GET_VALUEKEY(WBPARSER_KEY_UREADER(dm)),p->dm);

	// followers
	GET_LONG(XML,obj,GET_VALUEKEY(WBPARSER_KEY_UREADER(followers)),p->followers);

	return p;
}

/** 解析评论计数 */
t_wbParse_CommentCounts *CWBXmlParser::parse_commentcounts(/*[in]*/wbParserNS::REQOBJ *obj)
{
	if( !obj)
		return NULL;


	t_wbParse_CommentCounts *p = WEIBO_PARSER_CAST(t_wbParse_CommentCounts,wbParse_Malloc_Commentcount());

	// id
	GET_LONG_TO_STR( XML,obj,GET_VALUEKEY(WBPARSER_KEY_COMCOUNT(id)),p->id,WBPARSER_USE_LEN(id) );

	// comments
	GET_LONG( XML,obj,GET_VALUEKEY(WBPARSER_KEY_COMCOUNT(comments)),p->comments );

	// rt
	GET_LONG( XML,obj,GET_VALUEKEY(WBPARSER_KEY_COMCOUNT(rt)),p->rt );

	return p;
}

/** 解析访问限制 */
t_wbParse_LimitStatus *CWBXmlParser::parse_limite_status(/*[in]*/wbParserNS::REQOBJ *obj)
{
	if( !obj)
		return NULL;

	t_wbParse_LimitStatus *p = (t_wbParse_LimitStatus *)wbParse_Malloc_Limits();

	// hourly_limit
	GET_LONG( XML,obj,GET_VALUEKEY(WBPARSER_KEY_ACCESS_LMT(hourlimit)),p->hourly_limit );

	// rstimeinseond
	GET_LONG( XML,obj,GET_VALUEKEY(WBPARSER_KEY_ACCESS_LMT(rstimeinseond)),p->reset_time_in_seconds );

	// remaining_hits
	GET_LONG( XML,obj,GET_VALUEKEY(WBPARSER_KEY_ACCESS_LMT(remaining_hits)),p->remaining_hits );

	// reset_time
	GET_STR( XML,obj,GET_VALUEKEY(WBPARSER_KEY_ACCESS_LMT(reset_time)),p->reset_time,WBPARSER_USE_LEN(reset_time) );

	return p;
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
