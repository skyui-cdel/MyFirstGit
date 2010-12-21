/**
*  @brief weibo initialize
*  @file  wbinit.cpp
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


#include "wbinit.h"
#include "wbcallback.h"
#include <libweibo/weibo.h>
#include <stdlib.h>
#include <string.h>

// for declare function
#define WEIBO_struct_init_callback(name) \
	static bool Wb_initstruct_##name##_cb(void* t_wb)

// for enum
#define WEIBO_struct_init_fun(name) Wb_initstruct_##name##_cb

//
#define WEIBO_struct_cast( STRUCT ) \
    struct STRUCT* pstruct = (struct STRUCT*)t_wb;\
	memset(pstruct ,0 , sizeof(struct STRUCT) );

// 设置结构的回调函数
#define SET_struct_callback_object(option , poauth ) \
	{\
        struct t_wb_callback_byloach* t_cb = Wb_get_struct_callback( option );\
		(poauth).fstatus_cb = t_cb->fstatus_cb;\
		(poauth).fbody_cb = t_cb->fbody_cb;\
		(poauth).fheader_cb = t_cb->fheader_cb;\
	}

WEIBO_struct_init_callback(BASE)
{
	WEIBO_struct_cast(t_wb_oauth);

	return true;
}

WEIBO_struct_init_callback(GETSTATUSES_PUBLIC_TIMELINE)
{
	WEIBO_struct_cast(t_wb_get_statuses_public_timeline);

	SET_struct_callback_object( WEIBO_OPTION(GETSTATUSES_PUBLIC_TIMELINE) , pstruct->wbauth_ );

	return true;
}

WEIBO_struct_init_callback(GETSTATUSES_FRIENDS_TIMELINE)
{
	WEIBO_struct_cast(t_wb_get_statuses_friends_timeline);

	SET_struct_callback_object( WEIBO_OPTION(GETSTATUSES_FRIENDS_TIMELINE) , pstruct->wbauth_ );

	return true;
}

WEIBO_struct_init_callback(GETSTATUSES_USE_TIMELINE)
{
	WEIBO_struct_cast(t_wb_get_statuses_user_timeline);

	SET_struct_callback_object( WEIBO_OPTION(GETSTATUSES_USE_TIMELINE) , pstruct->wbauth_ );
	return true;
}

WEIBO_struct_init_callback(GETSTATUSES_MENTIONS)
{
	WEIBO_struct_cast(t_wb_get_statuses_mentions);

	SET_struct_callback_object( WEIBO_OPTION(GETSTATUSES_MENTIONS) , pstruct->wbauth_ );
	return true;
}

WEIBO_struct_init_callback(GETSTATUSES_COMMENTS_TIMELINE)
{
	WEIBO_struct_cast(t_wb_get_statuses_comments_timeline);

	SET_struct_callback_object( WEIBO_OPTION(GETSTATUSES_COMMENTS_TIMELINE) , pstruct->wbauth_ );
	return true;
}

WEIBO_struct_init_callback(GETSTATUSES_COMMENTS_BYME)
{
	WEIBO_struct_cast(t_wb_get_statuses_comments_byme);

	SET_struct_callback_object( WEIBO_OPTION(GETSTATUSES_COMMENTS_BYME) , pstruct->wbauth_ );
	return true;
}

WEIBO_struct_init_callback(GETSTATUSES_COMMENTS_TOME)
{
	WEIBO_struct_cast(t_wb_get_statuses_comments_tome);

	SET_struct_callback_object( WEIBO_OPTION(GETSTATUSES_COMMENTS_TOME) , pstruct->wbauth_ );
	return true;
}

WEIBO_struct_init_callback(GETSTATUSES_COMMENTS_LIST)
{
	WEIBO_struct_cast(t_wb_get_statuses_comments_list);

	SET_struct_callback_object( WEIBO_OPTION(GETSTATUSES_COMMENTS_LIST) , pstruct->wbauth_ );
	return true;
}

WEIBO_struct_init_callback(GETSTATUSES_COMMENTS_COUNTS)
{
	WEIBO_struct_cast(t_wb_get_statuses_counts);

	SET_struct_callback_object( WEIBO_OPTION(GETSTATUSES_COMMENTS_COUNTS) , pstruct->wbauth_ );
	return true;
}

WEIBO_struct_init_callback(GETSTATUSES_UNREAD)
{
	WEIBO_struct_cast(t_wb_get_statuses_unread);

	SET_struct_callback_object( WEIBO_OPTION(GETSTATUSES_UNREAD) , pstruct->wbauth_ );
	return true;
}

WEIBO_struct_init_callback(PUTSTATUSES_RESET_COUNT)
{
	WEIBO_struct_cast(t_wb_get_statuses_reset_count);

	SET_struct_callback_object( WEIBO_OPTION(PUTSTATUSES_RESET_COUNT) , pstruct->wbauth_ );
	return true;
}

//微博访问
WEIBO_struct_init_callback(GETSTATUSES_SHOW)
{
	WEIBO_struct_cast(t_wb_get_statuses_show);

	SET_struct_callback_object( WEIBO_OPTION(GETSTATUSES_SHOW) , pstruct->wbauth_ );
	return true;
}

WEIBO_struct_init_callback(GOTOSTATUSES_ID)
{
	WEIBO_struct_cast(t_wb_goto_user_status_id);

	SET_struct_callback_object( WEIBO_OPTION(GOTOSTATUSES_ID) , pstruct->wbauth_ );

	return true;
}

WEIBO_struct_init_callback(PUTSTATUSES_UPDATE)
{
	WEIBO_struct_cast(t_wb_put_statuses_update);

	SET_struct_callback_object( WEIBO_OPTION(PUTSTATUSES_UPDATE) , pstruct->wbauth_ );

	return true;
}

WEIBO_struct_init_callback(PUTSTATUSES_UPLOAD)
{
	WEIBO_struct_cast(t_wb_put_statuses_upload);

	SET_struct_callback_object( WEIBO_OPTION(PUTSTATUSES_UPLOAD) , pstruct->wbauth_ );

	return true;
}


WEIBO_struct_init_callback(PUTSTATUSES_DESTROY)
{
	WEIBO_struct_cast(t_wb_put_statuses_destroy);

	SET_struct_callback_object( WEIBO_OPTION(PUTSTATUSES_DESTROY) , pstruct->wbauth_ );

	return true;
}


WEIBO_struct_init_callback(PUTSTATUSES_REPOST)
{
	WEIBO_struct_cast(t_wb_put_statuses_repost);

	SET_struct_callback_object( WEIBO_OPTION(PUTSTATUSES_REPOST) , pstruct->wbauth_ );

	return true;
}

WEIBO_struct_init_callback(PUTSTATUSES_COMMENT)
{
	WEIBO_struct_cast(t_wb_put_statuses_comment);

	SET_struct_callback_object( WEIBO_OPTION(PUTSTATUSES_COMMENT) , pstruct->wbauth_ );

	return true;
}


WEIBO_struct_init_callback(PUTSTATUSES_COMMENT_DESTROY)
{
	WEIBO_struct_cast(t_wb_put_statuses_comment_destroy);

	SET_struct_callback_object( WEIBO_OPTION(PUTSTATUSES_COMMENT_DESTROY) , pstruct->wbauth_ );

	return true;
}

WEIBO_struct_init_callback(PUTSTATUSES_REPLY)
{
	WEIBO_struct_cast(t_wb_put_statuses_reply);

	SET_struct_callback_object( WEIBO_OPTION(PUTSTATUSES_REPLY) , pstruct->wbauth_ );

	return true;
}

//用户
WEIBO_struct_init_callback(GETUSER_INFO)
{
	WEIBO_struct_cast(t_wb_get_users_show);

	SET_struct_callback_object( WEIBO_OPTION(GETUSER_INFO) , pstruct->wbauth_ );

	return true;
}


WEIBO_struct_init_callback(GETFRINDS_LIST)
{
	WEIBO_struct_cast(t_wb_get_statuses_friends);

	SET_struct_callback_object( WEIBO_OPTION(GETFRINDS_LIST) , pstruct->wbauth_ );

	return true;
}

WEIBO_struct_init_callback(GETFOLLOWERS_LIST)
{
	WEIBO_struct_cast(t_wb_get_statuses_followers);

	SET_struct_callback_object( WEIBO_OPTION(GETFOLLOWERS_LIST) , pstruct->wbauth_ );

	return true;
}


//私信
WEIBO_struct_init_callback(GETDIRECTMSG)
{
	WEIBO_struct_cast(t_wb_get_direct_message);

	SET_struct_callback_object( WEIBO_OPTION(GETDIRECTMSG) , pstruct->wbauth_ );

	return true;
}

WEIBO_struct_init_callback(GETDIRESTMSG_SENT)
{
	WEIBO_struct_cast(t_wb_get_direct_message_sent);

	SET_struct_callback_object( WEIBO_OPTION(GETDIRESTMSG_SENT) , pstruct->wbauth_ );

	return true;
}

WEIBO_struct_init_callback(PUTDIRECTMSG_NEW)
{
	WEIBO_struct_cast(t_wb_put_direct_message_new);

	SET_struct_callback_object( WEIBO_OPTION(PUTDIRECTMSG_NEW) , pstruct->wbauth_ );

	return true;
}

WEIBO_struct_init_callback(PUTDIRECTMSG_DESTROY)
{
	WEIBO_struct_cast(t_wb_put_direct_message_destroy);

	SET_struct_callback_object( WEIBO_OPTION(PUTDIRECTMSG_DESTROY) , pstruct->wbauth_ );

	return true;
}


//关注
WEIBO_struct_init_callback(PUTFRIENDSHIPS_CREATE)
{
	WEIBO_struct_cast(t_wb_put_friendships_create);

	SET_struct_callback_object( WEIBO_OPTION(PUTFRIENDSHIPS_CREATE) , pstruct->wbauth_ );

	return true;
}

WEIBO_struct_init_callback(PUTFRIENDSHIPS_DESTROY)
{
	WEIBO_struct_cast(t_wb_put_friendships_destroy);

	SET_struct_callback_object( WEIBO_OPTION(PUTFRIENDSHIPS_DESTROY) , pstruct->wbauth_ );

	return true;
}

WEIBO_struct_init_callback(GETFRIENDSHIPS_EXISTS)
{
	WEIBO_struct_cast(t_wb_get_friendships_show);

	SET_struct_callback_object( WEIBO_OPTION(GETFRIENDSHIPS_EXISTS) , pstruct->wbauth_ );

	return true;
}

//Social Graph
WEIBO_struct_init_callback(GETFRIEND_IDS)
{
	WEIBO_struct_cast(t_wb_get_friends_ids);

	SET_struct_callback_object( WEIBO_OPTION(GETFRIEND_IDS) , pstruct->wbauth_ );

	return true;
}

WEIBO_struct_init_callback(GETFOLLOWER_IDS)
{
	WEIBO_struct_cast(t_wb_get_followers_ids);

	SET_struct_callback_object( WEIBO_OPTION(GETFOLLOWER_IDS) , pstruct->wbauth_ );

	return true;
}

//账号
WEIBO_struct_init_callback(GETACCOUNT_VERIFY)
{
	WEIBO_struct_cast(t_wb_get_account_verify_credentials);

	SET_struct_callback_object( WEIBO_OPTION(GETACCOUNT_VERIFY) , pstruct->wbauth_ );

	return true;
}


WEIBO_struct_init_callback(GETACCOUNT_RATELIMIT)
{
	WEIBO_struct_cast(t_wb_get_account_rate_limit_status);

	SET_struct_callback_object( WEIBO_OPTION(GETACCOUNT_RATELIMIT) , pstruct->wbauth_ );

	return true;
}


WEIBO_struct_init_callback(PUTACCOUNT_QUITSESSION)
{
	WEIBO_struct_cast(t_wb_put_account_end_session);

	SET_struct_callback_object( WEIBO_OPTION(PUTACCOUNT_QUITSESSION) , pstruct->wbauth_ );

	return true;
}


WEIBO_struct_init_callback(PUTACCOUNT_UPDATE_PROFILEIMAGE)
{
	WEIBO_struct_cast(t_wb_put_account_up_profileimage);

	SET_struct_callback_object( WEIBO_OPTION(PUTACCOUNT_UPDATE_PROFILEIMAGE) , pstruct->wbauth_ );

	return true;
}


WEIBO_struct_init_callback(PUTACCOUNT_UPDATE_PROFILE)
{
	WEIBO_struct_cast(t_wb_put_account_up_profile);

	SET_struct_callback_object( WEIBO_OPTION(PUTACCOUNT_UPDATE_PROFILE) , pstruct->wbauth_ );

	return true;
}


WEIBO_struct_init_callback(PUTACCOUNT_REGISTER)
{
	return false;
}


// 收藏
WEIBO_struct_init_callback(GETFAVORITES)// 获取当前用户的收藏列表 
{
	WEIBO_struct_cast(t_wb_get_favorites);

	SET_struct_callback_object( WEIBO_OPTION(GETFAVORITES) , pstruct->wbauth_ );

	return true;
}


WEIBO_struct_init_callback(PUTFAVORITES_CREATE)// 添加收藏 
{
	WEIBO_struct_cast(t_wb_put_favorites_create);

	SET_struct_callback_object( WEIBO_OPTION(PUTFAVORITES_CREATE) , pstruct->wbauth_ );

	return true;
}


WEIBO_struct_init_callback(PUTFAVORITES_DESTROY)// 删除当前用户收藏的微博信息 
{
	WEIBO_struct_cast(t_wb_put_favorites_destroy);

	SET_struct_callback_object( WEIBO_OPTION(PUTFAVORITES_DESTROY) , pstruct->wbauth_ );

	return true;
}


//登录/OAuth
WEIBO_struct_init_callback(OAUTH_REQUEST_TOKEN)// 获取未授权的Request Token
{
	WEIBO_struct_cast(t_wb_oauth_request_token);

	SET_struct_callback_object( WEIBO_OPTION(OAUTH_REQUEST_TOKEN) , pstruct->wbauth_ );

	return true;
}

WEIBO_struct_init_callback(OAUTH_AUTHORIZE)// 请求用户授权Token 
{
	WEIBO_struct_cast(t_wb_authorize);

	SET_struct_callback_object( WEIBO_OPTION(OAUTH_AUTHORIZE) , pstruct->wbauth_ );

	return true;
}

WEIBO_struct_init_callback(OAUTH_ACCESS_TOKEN)// 获取授权过的Access Token
{
	WEIBO_struct_cast(t_wb_oauth_access_token);

	SET_struct_callback_object( WEIBO_OPTION(OAUTH_ACCESS_TOKEN) , pstruct->wbauth_ );

	return true;
}

//
WEIBO_struct_init_callback(GET_EMOTIONS)
{
	WEIBO_struct_cast(t_wb_emotions);
	SET_struct_callback_object( WEIBO_OPTION(GET_EMOTIONS) , pstruct->wbauth_ );
	return true;
}
//
WEIBO_struct_init_callback(GET_USERS_SEARCH)
{
	WEIBO_struct_cast(t_wb_users_search);
	SET_struct_callback_object( WEIBO_OPTION(GET_USERS_SEARCH) , pstruct->wbauth_ );
	return true;
}
//
WEIBO_struct_init_callback(GET_WB_SEARCH)
{
	WEIBO_struct_cast(t_wb_search);
	SET_struct_callback_object( WEIBO_OPTION(GET_WB_SEARCH) , pstruct->wbauth_ );
	return true;
}
//
WEIBO_struct_init_callback(GET_STATUSES_SEARCH)
{
	WEIBO_struct_cast(t_wb_statuses_search);
	SET_struct_callback_object( WEIBO_OPTION(GET_STATUSES_SEARCH) , pstruct->wbauth_ );
	return true;
}

//
WEIBO_struct_init_callback(GET_PROVINCES)
{
	WEIBO_struct_cast(t_wb_provinces);
	SET_struct_callback_object( WEIBO_OPTION(GET_PROVINCES) , pstruct->wbauth_ );
	return true;
}

WEIBO_struct_init_callback(REPORT)
{
	WEIBO_struct_cast(t_wb_report);
	SET_struct_callback_object( WEIBO_OPTION(REPORT) , pstruct->wbauth_ );
	return true;
}

//自定义URL
WEIBO_struct_init_callback(COOKIE)
{
	WEIBO_struct_cast(t_wb_cookie);
	SET_struct_callback_object( WEIBO_OPTION(COOKIE) , pstruct->wbauth_ );
	return true;
}

//自定义URL
WEIBO_struct_init_callback(CUSTOM)
{
	WEIBO_struct_cast(t_wb_custom);
    SET_struct_callback_object( WEIBO_OPTION(COOKIE) , pstruct->wbauth_ );
	return true;
}


///////////////////////////////////////////////////////////////////////////////////////////

typedef bool (*fun_init_wb_struct_callback)(void* t_wb);
static fun_init_wb_struct_callback vector_init_callback[ WEIBO_OPTION(LAST) ] =
{
	NULL,
	WEIBO_struct_init_fun(BASE),
	WEIBO_struct_init_fun(GETSTATUSES_PUBLIC_TIMELINE),
	WEIBO_struct_init_fun(GETSTATUSES_FRIENDS_TIMELINE),
	WEIBO_struct_init_fun(GETSTATUSES_USE_TIMELINE),
	WEIBO_struct_init_fun(GETSTATUSES_MENTIONS),
	WEIBO_struct_init_fun(GETSTATUSES_COMMENTS_TIMELINE),
	WEIBO_struct_init_fun(GETSTATUSES_COMMENTS_BYME),
	WEIBO_struct_init_fun(GETSTATUSES_COMMENTS_TOME),
	WEIBO_struct_init_fun(GETSTATUSES_COMMENTS_LIST),
	WEIBO_struct_init_fun(GETSTATUSES_COMMENTS_COUNTS),
	WEIBO_struct_init_fun(GETSTATUSES_UNREAD),
	WEIBO_struct_init_fun(PUTSTATUSES_RESET_COUNT),

	//微博访问
	WEIBO_struct_init_fun(GETSTATUSES_SHOW),
	WEIBO_struct_init_fun(GOTOSTATUSES_ID),
	WEIBO_struct_init_fun(PUTSTATUSES_UPDATE),
	WEIBO_struct_init_fun(PUTSTATUSES_UPLOAD),
	WEIBO_struct_init_fun(PUTSTATUSES_DESTROY),
	WEIBO_struct_init_fun(PUTSTATUSES_REPOST),
	WEIBO_struct_init_fun(PUTSTATUSES_COMMENT),
	WEIBO_struct_init_fun(PUTSTATUSES_COMMENT_DESTROY),
	WEIBO_struct_init_fun(PUTSTATUSES_REPLY),

	//用户
	WEIBO_struct_init_fun(GETUSER_INFO),
	WEIBO_struct_init_fun(GETFRINDS_LIST),
	WEIBO_struct_init_fun(GETFOLLOWERS_LIST),

	//私信
	WEIBO_struct_init_fun(GETDIRECTMSG),
	WEIBO_struct_init_fun(GETDIRESTMSG_SENT),
	WEIBO_struct_init_fun(PUTDIRECTMSG_NEW),
	WEIBO_struct_init_fun(PUTDIRECTMSG_DESTROY),

	//关注
	WEIBO_struct_init_fun(PUTFRIENDSHIPS_CREATE),
	WEIBO_struct_init_fun(PUTFRIENDSHIPS_DESTROY),
	WEIBO_struct_init_fun(GETFRIENDSHIPS_EXISTS),

	//Social Graph
	WEIBO_struct_init_fun(GETFRIEND_IDS),
	WEIBO_struct_init_fun(GETFOLLOWER_IDS),

	//账号
	WEIBO_struct_init_fun(GETACCOUNT_VERIFY),
	WEIBO_struct_init_fun(GETACCOUNT_RATELIMIT),
	WEIBO_struct_init_fun(PUTACCOUNT_QUITSESSION),
	WEIBO_struct_init_fun(PUTACCOUNT_UPDATE_PROFILEIMAGE),
	WEIBO_struct_init_fun(PUTACCOUNT_UPDATE_PROFILE),
	WEIBO_struct_init_fun(PUTACCOUNT_REGISTER),

	// 收藏
	WEIBO_struct_init_fun(GETFAVORITES),// 获取当前用户的收藏列表 
	WEIBO_struct_init_fun(PUTFAVORITES_CREATE),// 添加收藏 
	WEIBO_struct_init_fun(PUTFAVORITES_DESTROY),// 删除当前用户收藏的微博信息 

	//登录/OAuth
	WEIBO_struct_init_fun(OAUTH_REQUEST_TOKEN),// 获取未授权的Request Token 
	WEIBO_struct_init_fun(OAUTH_AUTHORIZE),// 请求用户授权Token 
	WEIBO_struct_init_fun(OAUTH_ACCESS_TOKEN),// 获取授权过的Access Token

	WEIBO_struct_init_fun(GET_EMOTIONS),
	WEIBO_struct_init_fun(GET_USERS_SEARCH),
	WEIBO_struct_init_fun(GET_WB_SEARCH),
	WEIBO_struct_init_fun(GET_STATUSES_SEARCH),
	WEIBO_struct_init_fun(GET_PROVINCES),
	WEIBO_struct_init_fun(REPORT),//49 举报
	WEIBO_struct_init_fun(COOKIE),
	//自定义URL
	WEIBO_struct_init_fun(CUSTOM),
};


bool Wb_init_wb_struct( int option , void* t_wb)
{
	if( WEIBO_OPTION(UNK) >= option ||
        WEIBO_OPTION(LAST) <= option )
	{
		return false;
	}
	return ( vector_init_callback[ option ] ) ? vector_init_callback[ option ](t_wb):false;
}
