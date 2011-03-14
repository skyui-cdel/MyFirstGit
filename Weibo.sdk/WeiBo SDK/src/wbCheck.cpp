/**
*  @brief Check the option is validate
*  @file  wbCheck.cpp
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


#include "wbCheck.h"
#include "weibo.h"
#include "wbLog.h"
#include <assert.h>

#define WEIBO_check_callback(name) \
	static int Weibo_check_valid_##name##_cb(const void* t_wb)

// for enum
#define WEIBO_check_fun(name) Weibo_check_valid_##name##_cb

// 转化
#ifndef WEIBO_struct_cast

#define WEIBO_struct_cast( STRUCT ) \
	const struct STRUCT* pstruct = (const struct STRUCT*)t_wb;

#endif

static const char* errorText[] =
{
	"",
	"\n\nError:\n	weibo id is null.\n\n",
	"\n\nError:\n	user id is null.\n\n",
	"\n\nError:\n	text is null.\n\n",
	"\n\nError:\n	image file name is null.\n\n",
	"\n\nError:\n	weibo comment id is null.\n\n",
	"\n\nError:\n	user password is null.\n\n",
	"\n\nError:\n	oauth verify code is null.\n\n",
	"\n\nError:\n	pstruct->wbuid_.uidtype_ == t_wb_uid::IDType_unk.\n\n",
};


WEIBO_check_callback(UNK)
{
	return Err_OK;
}

WEIBO_check_callback(BASE)
{
	return Err_OK;
}

WEIBO_check_callback(GETSTATUSES_PUBLIC_TIMELINE)
{// 获取最新更新的公共微博消息
	WEIBO_struct_cast(t_wb_get_statuses_friends_timeline);
	return Err_OK;
}

WEIBO_check_callback(GETSTATUSES_FRIENDS_TIMELINE)
{// 获取当前用户所关注用户的最新微博信息
	WEIBO_struct_cast(t_wb_get_statuses_user_timeline);
	return Err_OK;
}

WEIBO_check_callback(GETSTATUSES_USE_TIMELINE)
{// 获取用户发布的微博信息列表 
	WEIBO_struct_cast(t_wb_get_statuses_user_timeline);

    return Err_OK;
}

WEIBO_check_callback(GETSTATUSES_MENTIONS)
{// 获取@当前用户的微博列表
	WEIBO_struct_cast(t_wb_get_statuses_mentions);

	return Err_OK;
}

WEIBO_check_callback(GETSTATUSES_COMMENTS_TIMELINE)
{// 获取当前用户发送及收到的评论列表 
	WEIBO_struct_cast(t_wb_get_statuses_comments_timeline);

    return Err_OK;
}

WEIBO_check_callback(GETSTATUSES_COMMENTS_BYME)
{// 获取当前用户发出的评论 
	WEIBO_struct_cast(t_wb_get_statuses_comments_byme);

    return Err_OK;
}

WEIBO_check_callback(GETSTATUSES_COMMENTS_TOME)
{// 获取当前用户发出的评论 
	WEIBO_struct_cast(t_wb_get_statuses_comments_tome);

    return Err_OK;
}

WEIBO_check_callback(GETSTATUSES_COMMENTS_LIST)
{// 获取指定微博的评论列表
	WEIBO_struct_cast(t_wb_get_statuses_comments_list);

	if( pstruct->wbId_[0] == '\0' )
	{
		return Err_WB_ID;
	}
	return Err_OK;
}

WEIBO_check_callback(GETSTATUSES_COMMENTS_COUNTS)
{
	WEIBO_struct_cast(t_wb_get_statuses_counts );
    return Err_OK;
}

WEIBO_check_callback(GETSTATUSES_UNREAD)
{// 获取当前用户未读消息数量(数字信息)
	WEIBO_struct_cast(t_wb_get_statuses_unread);
	return Err_OK;
}

WEIBO_check_callback(PUTSTATUSES_RESET_COUNT)
{// 获取当前用户未读消息数量(数字信息)
	WEIBO_struct_cast(t_wb_get_statuses_reset_count);
	return Err_OK;
}


//微博访问

WEIBO_check_callback(GETSTATUSES_SHOW)
{// 根据ID获取单条微博信息内容
	WEIBO_struct_cast(t_wb_get_statuses_show);

	if( pstruct->wbId_[0] == '\0' )
	{
		return Err_WB_ID;
	}
	return Err_OK;
}

WEIBO_check_callback(GOTOSTATUSES_ID)
{// 根据微博ID和用户ID跳转到单条微博页面( 界面呈现),只生成一串URI
	WEIBO_struct_cast(t_wb_goto_user_status_id);

	if( pstruct->uid_[0] == '\0' )
	{
		return Err_USE_ID;
	}

	if( pstruct->wbId_[0] == '\0' )
	{
		return Err_WB_ID;
	}

	return Err_OK;
}

WEIBO_check_callback(PUTSTATUSES_UPDATE)
{//发布一条微博信息

	WEIBO_struct_cast(t_wb_put_statuses_update);

	if( pstruct->szwbInfo_[0] == '\0' )
	{
		return Err_TEXT_ID;
	}

	return Err_OK;
}

WEIBO_check_callback(PUTSTATUSES_UPLOAD)
{// 上传图片并发布一条微博信息

	WEIBO_struct_cast(t_wb_put_statuses_upload);

	if( pstruct->szwbInfo_[0] == '\0' )
	{
		return Err_TEXT_ID;
	}
	if( pstruct->szImagePath_[0] == '\0' )
	{
		return Err_IMAGE_ID;
	}

	return Err_OK;
}

WEIBO_check_callback(PUTSTATUSES_DESTROY)
{// 删除一条微博信息
	WEIBO_struct_cast(t_wb_put_statuses_destroy);
	
	if( pstruct->wbId_[0] == '\0' )
	{
		return Err_WB_ID;
	}

	return Err_OK;
}

WEIBO_check_callback(PUTSTATUSES_REPOST)
{// 转发一条微博信息（可加评论）
	WEIBO_struct_cast(t_wb_put_statuses_repost);

	if( pstruct->wbId_[0] == '\0' )
	{
		return Err_WB_ID;
	}

    return Err_OK;
}

WEIBO_check_callback(PUTSTATUSES_COMMENT)
{//对一条微博信息进行评论 

	WEIBO_struct_cast(t_wb_put_statuses_comment);

	if( pstruct->wbId_[0] == '\0' )
	{
		return Err_WB_ID;
	}

	if( pstruct->szwbInfo_[0] == '\0' )
	{
		return Err_TEXT_ID;
	}
	return Err_OK;
}

WEIBO_check_callback(PUTSTATUSES_COMMENT_DESTROY)
{// 删除当前用户的微博评论信息 

	WEIBO_struct_cast(t_wb_put_statuses_comment_destroy);

	if( pstruct->wbCId_[0] == '\0' )
	{
		return Err_WBCOM_ID;
	}
	return Err_OK;
}

WEIBO_check_callback(PUTSTATUSES_REPLY)
{// 回复微博评论信息

	WEIBO_struct_cast(t_wb_put_statuses_reply );

	if( pstruct->szwbInfo_[0] == '\0' )
	{
		return Err_TEXT_ID;
	}

	if( pstruct->wbId_[0] == '\0' )
	{
		return Err_WB_ID;
	}

	if( pstruct->wbCId_[0] == '\0' )
	{
		return Err_WBCOM_ID;
	}
	return Err_OK;
}

//----------------------------------用户------------------------------------------//

WEIBO_check_callback(GETUSER_INFO)
{// 根据用户ID获取用户资料（授权用户）
	WEIBO_struct_cast(t_wb_get_users_show);

	if(  t_wb_uid::IDType_unk == pstruct->wbuid_.uidtype_ )
	{
		assert(0);
		return Err_UIDTYPE_ID;
	}
	if( pstruct->wbuid_.uid_[0] == '\0' )
	{
		assert(0);
		return Err_USE_ID;
	}
	return Err_OK;
}

WEIBO_check_callback(GETFRINDS_LIST)
{// 获取当前用户关注对象列表及最新一条微博信息
	WEIBO_struct_cast(t_wb_get_statuses_friends);

	if(  t_wb_uid::IDType_unk == pstruct->wbuid_.uidtype_ )
	{
		return Err_UIDTYPE_ID;
	}
	if( pstruct->wbuid_.uid_[0] == '\0' )
	{
		return Err_USE_ID;
	}
	return Err_OK;
}

WEIBO_check_callback(GETFOLLOWERS_LIST)
{// 获取当前用户粉丝列表及最新一条微博信息
	WEIBO_struct_cast(t_wb_get_statuses_followers);

	if(  t_wb_uid::IDType_unk == pstruct->wbuid_.uidtype_ )
	{
		return Err_UIDTYPE_ID;
	}
	if( pstruct->wbuid_.uid_[0] == '\0' )
	{
		return Err_USE_ID;
	}
	return Err_OK;
}

//-----------------------------------私信------------------------------------------//

WEIBO_check_callback(GETDIRECTMSG)
{// 获取当前用户最新私信列表
	WEIBO_struct_cast(t_wb_get_direct_message);

	return Err_OK;
}

WEIBO_check_callback(GETDIRESTMSG_SENT)
{// 获取当前用户发送的最新私信列表 
	WEIBO_struct_cast(t_wb_get_direct_message_sent);
	return Err_OK;
}

WEIBO_check_callback(PUTDIRECTMSG_NEW)
{// 发送一条私信
	WEIBO_struct_cast(t_wb_put_direct_message_new );

	if( pstruct->szText_[0] == '\0' )
	{
		return Err_TEXT_ID;
	}
	if( pstruct->wbuid_.uid_[0] == '\0' )
	{
		return Err_USE_ID;
	}
	return Err_OK;
}

WEIBO_check_callback(PUTDIRECTMSG_DESTROY)
{// 删除一条私信
	WEIBO_struct_cast(t_wb_put_direct_message_destroy);

	if( pstruct->wbId_[0] == '\0' )
	{
		return Err_WB_ID;
	}
	return Err_OK;
}

//-----------------------------------------关注---------------------------------------------//

WEIBO_check_callback(PUTFRIENDSHIPS_CREATE)
{// 关注某用户
	WEIBO_struct_cast(t_wb_put_friendships_create);

	if(  t_wb_uid::IDType_unk == pstruct->wbuid_.uidtype_ )
	{
		return Err_UIDTYPE_ID;
	}
	if( pstruct->wbuid_.uid_[0] == '\0' )
	{
		return Err_USE_ID;
	}
	return Err_OK;
}

WEIBO_check_callback(PUTFRIENDSHIPS_DESTROY)
{// 取消关注
	WEIBO_struct_cast(t_wb_put_friendships_destroy);

	if(  t_wb_uid::IDType_unk == pstruct->wbuid_.uidtype_ )
	{
		return Err_UIDTYPE_ID;
	}

	if( pstruct->wbuid_.uid_[0] == '\0' )
	{
		return Err_USE_ID;
	}
	return Err_OK;
}


WEIBO_check_callback(GETFRIENDSHIPS_EXISTS)
{
	WEIBO_struct_cast(t_wb_get_friendships_show);

	if(  t_wb_uid::IDType_unk == pstruct->wbuid_target_.uidtype_ )
	{
		return Err_UIDTYPE_ID;
	}
    if( pstruct->wbuid_target_.uid_[0] =='\0' )
	{
		return Err_USE_ID;
	}
	return Err_OK;
}

//Social Graph
WEIBO_check_callback(GETFOLLOWER_IDS)
{
	WEIBO_struct_cast(t_wb_get_followers_ids);

	if(  t_wb_uid::IDType_unk == pstruct->wbuid_.uidtype_ )
	{
		return Err_UIDTYPE_ID;
	}
    if( pstruct->wbuid_.uid_[0] =='\0' )
	{
		return Err_USE_ID;
	}
	return Err_OK;
}

WEIBO_check_callback(GETFRIEND_IDS)
{
	WEIBO_struct_cast(t_wb_get_friends_ids);

	if(  t_wb_uid::IDType_unk == pstruct->wbuid_.uidtype_ )
	{
		return Err_UIDTYPE_ID;
	}
    if( pstruct->wbuid_.uid_[0] =='\0' )
	{
		return Err_USE_ID;
	}
	return Err_OK;
}

//账号
WEIBO_check_callback(GETACCOUNT_VERIFY)
{
	WEIBO_struct_cast(t_wb_get_account_verify_credentials);

	return Err_OK;
}

WEIBO_check_callback(GETACCOUNT_RATELIMIT)
{
	WEIBO_struct_cast(t_wb_get_account_rate_limit_status);

	return Err_OK;
}

WEIBO_check_callback(PUTACCOUNT_QUITSESSION)
{
	WEIBO_struct_cast(t_wb_put_account_end_session);
	return Err_OK;
}

WEIBO_check_callback(PUTACCOUNT_UPDATE_PROFILEIMAGE)
{// 更改头像 
	WEIBO_struct_cast(t_wb_put_account_up_profileimage);

	if( pstruct->szImagePath_[0] == '\0' )
	{
		return Err_IMAGE_ID;
	}
	return Err_OK;
}

WEIBO_check_callback(PUTACCOUNT_UPDATE_PROFILE)
{
	WEIBO_struct_cast(t_wb_put_account_up_profile );

	return Err_OK;
}

WEIBO_check_callback(PUTACCOUNT_REGISTER)
{
	return -1;
}


WEIBO_check_callback(GETFAVORITES)
{// 获取当前用户的收藏列表 
	WEIBO_struct_cast(t_wb_get_favorites);

	return Err_OK;
}

WEIBO_check_callback(PUTFAVORITES_CREATE)
{// 添加收藏 
	WEIBO_struct_cast(t_wb_put_favorites_create);

	if( pstruct->wbId_[0] == '\0' )
	{
		return Err_WB_ID;
	}
	return Err_OK;
}

WEIBO_check_callback(PUTFAVORITES_DESTROY)
{// 删除当前用户收藏的微博信息 
	WEIBO_struct_cast(t_wb_put_favorites_destroy);

	if( pstruct->wbId_[0] == '\0' )
	{
		return Err_WB_ID;
	}
	return Err_OK;
}


WEIBO_check_callback(OAUTH_REQUEST_TOKEN)
{
	return Err_OK;
}

WEIBO_check_callback(OAUTH_AUTHORIZE)
{
	WEIBO_struct_cast(t_wb_authorize);

	if( pstruct->usrid_[0] == '\0' )
	{
		return Err_WB_ID;
	}
	if( pstruct->usrpwd_[0] == '\0')
	{
		return Err_PWD_ID ;
	}
	return Err_OK;
}

WEIBO_check_callback(OAUTH_ACCESS_TOKEN)// 获取授权过的Access Token
{
	WEIBO_struct_cast(t_wb_oauth_access_token);

	if( pstruct->wbauth_.oauth_verifier_[0] == '\0' )
	{
		return Err_OAUTHVERIFY_ID;
	}
	return Err_OK;
}

WEIBO_check_callback(XAUTH_ACCESS_TOKEN)// 获取授权过的Access Token
{
	WEIBO_struct_cast(t_wb_xauth_access_token);

	if( pstruct->usrid_[0] == '\0' )
	{
		return Err_WB_ID;
	}
	if( pstruct->usrpwd_[0] == '\0')
	{
		return Err_PWD_ID ;
	}
	if( pstruct->authmode_[0] == '\0')
	{
		return Err_AUTH_MODE ;
	}
	return Err_OK;
}

// 表情
WEIBO_check_callback(GET_EMOTIONS)
{
	return Err_OK;
}

// 用户搜索 
WEIBO_check_callback(GET_USERS_SEARCH)
{
	return Err_OK;
}

// 微博搜索 
WEIBO_check_callback(GET_WB_SEARCH)
{
	return Err_OK;
}

WEIBO_check_callback(GET_STATUSES_SEARCH)
{
	return Err_OK;
}

WEIBO_check_callback(GET_PROVINCES)
{
	return Err_OK;
}

WEIBO_check_callback(REPORT)
{
	return Err_OK;
}


WEIBO_check_callback(COOKIE)
{
	WEIBO_struct_cast(t_wb_cookie);

	// 用户名/密码
	if( pstruct->usrid_[0] == '\0' )
	{
		return Err_USE_ID;
	}

	if( pstruct->usrpwd_[0] == '\0' )
	{
		return Err_PWD_ID;
	}

	return Err_OK;
}

// 自定义
WEIBO_check_callback(CUSTOM)
{
	return Err_OK;
}

typedef int (*f_check_callback)(const void* t_wb);
static 
f_check_callback vector_check_callbak[]=
{
	WEIBO_check_fun(UNK),
	WEIBO_check_fun(BASE),

	//获取下行数据集(timeline)接口 
	WEIBO_check_fun(GETSTATUSES_PUBLIC_TIMELINE),
	WEIBO_check_fun(GETSTATUSES_FRIENDS_TIMELINE),
	WEIBO_check_fun(GETSTATUSES_USE_TIMELINE),
	WEIBO_check_fun(GETSTATUSES_MENTIONS),
	WEIBO_check_fun(GETSTATUSES_COMMENTS_TIMELINE),
	WEIBO_check_fun(GETSTATUSES_COMMENTS_BYME),
	WEIBO_check_fun(GETSTATUSES_COMMENTS_TOME),
	WEIBO_check_fun(GETSTATUSES_COMMENTS_LIST),
	WEIBO_check_fun(GETSTATUSES_COMMENTS_COUNTS),
	WEIBO_check_fun(GETSTATUSES_UNREAD),
	WEIBO_check_fun(PUTSTATUSES_RESET_COUNT),

	//微博访问接口 
	WEIBO_check_fun(GETSTATUSES_SHOW),
	WEIBO_check_fun(GOTOSTATUSES_ID),
	WEIBO_check_fun(PUTSTATUSES_UPDATE),
	WEIBO_check_fun(PUTSTATUSES_UPLOAD),
	WEIBO_check_fun(PUTSTATUSES_DESTROY),
	WEIBO_check_fun(PUTSTATUSES_REPOST),
	WEIBO_check_fun(PUTSTATUSES_COMMENT),
	WEIBO_check_fun(PUTSTATUSES_COMMENT_DESTROY),
	WEIBO_check_fun(PUTSTATUSES_REPLY),

	//用户接口 
	WEIBO_check_fun(GETUSER_INFO),
	WEIBO_check_fun(GETFRINDS_LIST),
	WEIBO_check_fun(GETFOLLOWERS_LIST),

	//私信接口 
	WEIBO_check_fun(GETDIRECTMSG),
	WEIBO_check_fun(GETDIRESTMSG_SENT),
	WEIBO_check_fun(PUTDIRECTMSG_NEW),
	WEIBO_check_fun(PUTDIRECTMSG_DESTROY),

	//关注接口 
	WEIBO_check_fun(PUTFRIENDSHIPS_CREATE),
	WEIBO_check_fun(PUTFRIENDSHIPS_DESTROY),
	WEIBO_check_fun(GETFRIENDSHIPS_EXISTS),

	//Social Graph接口
	WEIBO_check_fun(GETFRIEND_IDS),
	WEIBO_check_fun(GETFOLLOWER_IDS),

	//账号接口 
	WEIBO_check_fun(GETACCOUNT_VERIFY),
	WEIBO_check_fun(GETACCOUNT_RATELIMIT),
	WEIBO_check_fun(PUTACCOUNT_QUITSESSION),
	WEIBO_check_fun(PUTACCOUNT_UPDATE_PROFILEIMAGE),
	WEIBO_check_fun(PUTACCOUNT_UPDATE_PROFILE),
	WEIBO_check_fun(PUTACCOUNT_REGISTER),

	//收藏接口 
	WEIBO_check_fun(GETFAVORITES),
	WEIBO_check_fun(PUTFAVORITES_CREATE),
	WEIBO_check_fun(PUTFAVORITES_DESTROY),

	//登录/OAuth接口 
	WEIBO_check_fun(OAUTH_REQUEST_TOKEN),
	WEIBO_check_fun(OAUTH_AUTHORIZE),
	WEIBO_check_fun(OAUTH_ACCESS_TOKEN),

	// 表情
	WEIBO_check_fun(GET_EMOTIONS),

	// 用户搜索 
	WEIBO_check_fun(GET_USERS_SEARCH),

	// 微博搜索 
	WEIBO_check_fun(GET_WB_SEARCH),
	WEIBO_check_fun(GET_STATUSES_SEARCH),

	WEIBO_check_fun(GET_PROVINCES),
	WEIBO_check_fun(REPORT),
	// cookie 
	WEIBO_check_fun(COOKIE),
	//xauth
	WEIBO_check_fun(XAUTH_ACCESS_TOKEN),

	// buffer
	WEIBO_check_fun(CUSTOM),
};


int Weibo_check_valid(int option ,const void* t_wb)
{
	if( WEIBO_OPTION(UNK) >= option ||
		WEIBO_OPTION(LAST) <= option )
	{
		return -1;//超出范围
	}

	int err = vector_check_callbak[option](t_wb);
	if( Err_OK != err )
	{
		wbPRINTF( errorText[ err ] );
	}
	return err;
}
