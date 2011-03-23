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

	"\n\nError:\n	keyword is null\n\n",
	"\n\nError:\n	trend id is null\n\n",
	"\n\nError:\n	tag id is null\n\n",

	// register
	"\n\nError:\n	Register:nike name is null\n\n",
	"\n\nError:\n	Register:gender is null\n\n",
	"\n\nError:\n	Register:mail is null\n\n",
	"\n\nError:\n	Register:password is null\n\n",
	"\n\nError:\n	Register:register ip is null\n\n",

	// update tgt
	"\n\nError:\n	UpdateTgt:SUE is null\n\n",
	"\n\nError:\n	UpdateTgt:SUP is null\n\n",
	"\n\nError:\n	UpdateTgt:Verify Tgt is null\n\n",

	// invite mail
	"\n\nError:\n	InviteMail:Mail ID is null\n\n",
	"\n\nError:\n	InviteMail:Mail Password is null\n\n",
	"\n\nError:\n	InviteMail:Mail Type is null\n\n",
	"\n\nError:\n	InviteMail:Mail List is null\n\n",
	"\n\nError:\n	InviteMail:Mail Nick name is null\n\n",

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

WEIBO_check_callback(PUTSTATUSES_UPLOAD_PIC)
{// 上传图片

	WEIBO_struct_cast(t_wb_put_statuses_upload_pic);

	if( pstruct->szImagePath_[0] == '\0' ) {
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

WEIBO_check_callback(GETDIRECTMSG_WITH)
{//获取用户往来私信列表

	WEIBO_struct_cast(t_wb_get_direct_message_with);

	if( pstruct->wbuid_[0] == '\0' )
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

WEIBO_check_callback(PUTFRIENDSHIPS_CREATE_BATCH)
{// 关注某用户
	WEIBO_struct_cast(t_wb_put_friendships_create_batch);

	if( pstruct->wbIDs_[0] == '\0' )
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

WEIBO_check_callback(GETFRIENDSHIPS_BATCH_EXISTS)
{
	WEIBO_struct_cast(t_wb_get_friendships_batchexist);
	//
    if( pstruct->wbIDs_[0] =='\0' )
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
	WEIBO_struct_cast(t_wb_put_account_register );

	// nick name
	if( pstruct->szNick_[0] == '\0'){
		return Err_REG_NICK ;
	}

	// gender
	if( pstruct->szGender_[0] == '\0'){
		return Err_REG_GENDER;
	}

	// email
	if( pstruct->szEmail_[0] == '\0'){
		return Err_REG_MAIL ;
	}

	// password
	if( pstruct->szPwd_[0] == '\0'){
		return Err_REG_PWD;
	}

	// city id
	if( pstruct->szIP_[0] == '\0'){
		return Err_REG_IP;
	}

	return Err_OK;
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

// 更新TGT
WEIBO_check_callback(UPDATETGT)
{
	WEIBO_struct_cast(t_wb_updateTGT);

	// SUE
	if( pstruct->wbauth_.sue_[0] == '\0' )
	{
		return Err_UPDATETGT_SUE;
	}

	//SUP
	if( pstruct->wbauth_.sup_[0] == '\0' ) 
	{
		return Err_UPDATETGT_SUP;
	}

	//Verify Tgt
	if( pstruct->wbauth_.tgt_[0] == '\0')
	{
		return Err_UPDATETGT_VERIFY_TGT;
	}

	return Err_OK;
}



// 自定义
WEIBO_check_callback(CUSTOM)
{
	return Err_OK;
}

// 热点推荐
WEIBO_check_callback(HOT_REPOST_DAYLIY)
{
	return Err_OK;
}

WEIBO_check_callback(HOT_REPOST_WEEKLY)
{
	return Err_OK;
}

WEIBO_check_callback(HOT_COMMENT_DAYLIY)
{
	return Err_OK;
}

WEIBO_check_callback(HOT_COMMENT_WEEKLY)
{
	return Err_OK;
}


//用户接口NEW
WEIBO_check_callback(GET_USERS_HOT)
{
	return Err_OK;
}

WEIBO_check_callback(POST_USERS_REMARK)
{
	WEIBO_struct_cast(t_wb_users_remark);

	// 用户名
	if( pstruct->userId_[0] == '\0' ){
		return Err_USE_ID;
	}

	// 备注信息
	if( pstruct->remark_[0] == '\0' ){
		return Err_TEXT_ID;
	}
	return Err_OK;
}

WEIBO_check_callback(GET_USERS_SUGGESTIONS)
{
	return Err_OK;
}


////////////////////////////////////////////////////////////////////////////////////////////
// 话题微博

//获取某人的话题
WEIBO_check_callback(GET_TRENDS)
{
	WEIBO_struct_cast(t_wb_trends);

	// 用户名
	if( pstruct->usrid_[0] == '\0' ){
		return Err_USE_ID;
	}
	return Err_OK;
}

//获取某一话题下的微博
WEIBO_check_callback(GET_TRENDS_STATUSES)
{
	WEIBO_struct_cast(t_wb_trends_statuses);
	if( pstruct->terndname_[0] == '\0'){
		return Err_KEYWORD;
	}
	return Err_OK;
}

//关注某一个话题
WEIBO_check_callback(POST_TRENDS_FOLLOW)
{
	WEIBO_struct_cast(t_wb_trends_follow);
	if( pstruct->terndname_[0] == '\0'){
		return Err_KEYWORD;
	}
	return Err_OK;
}

//trends/destroy 取消关注的某一个话题
WEIBO_check_callback(DELETE_TRENDS_DESTROY)
{
	WEIBO_struct_cast(t_wb_trends_destroy);
	if( pstruct->trendid_[0] == '\0'){
		return Err_TREND_ID;
	}
	return Err_OK;
}
//trends/destroy 按小时返回热门话题
WEIBO_check_callback(GET_TRENDS_HOURLY)
{
	return Err_OK;
}
//trends/daily 按日期返回热门话题。返回某一日期的话题
WEIBO_check_callback(GET_TRENDS_DAYLIY)
{
	return Err_OK;
}
//trends/weekly 按周返回热门话题。返回某一日期之前某一周的话题
WEIBO_check_callback(GET_TRENDS_WEEKLIY)
{
	return Err_OK;
}

////////////////////////////////////////////////////////////////////////////////////////////
// 黑名单接口 ,by welbon,2011-01-10

//将某用户加入黑名单
WEIBO_check_callback(POST_BLOCKS_CREATE)
{
	WEIBO_struct_cast(t_wb_blocks_create);
	if( pstruct->usrid_[0] == '\0'){
		return Err_USE_ID;
	}
	return Err_OK;
}

//将某用户移出黑名单
WEIBO_check_callback(POST_BLOCKS_DESTROY)
{
	WEIBO_struct_cast(t_wb_blocks_destroy);
	if( pstruct->usrid_[0] == '\0'){
		return Err_USE_ID;
	}
	return Err_OK;
}

//检测某用户是否是黑名单用户
WEIBO_check_callback(GET_BLOCKS_EXISTS)
{
	WEIBO_struct_cast(t_wb_blocks_exist);
	if( pstruct->usrid_[0] == '\0'){
		return Err_USE_ID;
	}
	return Err_OK;
}

//列出黑名单用户(输出用户详细信息)
WEIBO_check_callback(GET_BLOCKS_BLOCKING)
{
	return Err_OK;
}

//列出分页黑名单用户（只输出id）
WEIBO_check_callback(GET_BLOCKS_BLOCKING_IDS)
{
	return Err_OK;
}

////////////////////////////////////////////////////////////////////////////////////////////
//用户标签接口 ,by welbon,2011-01-10

//tags 返回指定用户的标签列表
WEIBO_check_callback(GET_TAGS)
{
	WEIBO_struct_cast(t_wb_tags);
	if( pstruct->usrid_[0] == '\0'){
		return Err_USE_ID;
	}

	return Err_OK;
}
//tags/create 添加用户标签
WEIBO_check_callback(POST_TAGS_CREATE)
{
	WEIBO_struct_cast(t_wb_tags_create);
	if( pstruct->tags_[0] == '\0'){
		return Err_KEYWORD;
	}
	return Err_OK;
}
//tags/suggestions 返回用户感兴趣的标签
WEIBO_check_callback(GET_TAGS_SUGGESTIONS)
{
	return Err_OK;
}
//tags/destroy 删除标签
WEIBO_check_callback(POST_TAGS_DESTROY)
{
	WEIBO_struct_cast(t_wb_tags_destroy);
	if( pstruct->tagId_[0] == '\0'){
		return Err_TAGS_ID;
	}
	return Err_OK;
}
//tags/destroy_batch 批量删除标签
WEIBO_check_callback(POST_TAGS_DESTROY_BATCH)
{
	WEIBO_struct_cast(t_wb_tags_destroy_batch);
	if( pstruct->ids_[0] == '\0'){
		return Err_TAGS_ID;
	}
	return Err_OK;
}


//邀请邮箱联系人
WEIBO_check_callback(POST_INVITE_MAILCONTACT)
{
	WEIBO_struct_cast(t_wb_invite_mailcontect);
	if( pstruct->usrid_[0] == '\0'){
		return Err_MAIL_ID;
	}

	if( pstruct->usrpwd_[0] == '\0'){
		return Err_MAIL_PWD;
	}
	return Err_OK;
}


//邀请MSN联系人
WEIBO_check_callback(POST_INVITE_MSNCONTACT)
{
	WEIBO_struct_cast(t_wb_invite_msncontect);
	if( pstruct->usrid_[0] == '\0'){
		return Err_MAIL_ID;
	}

	if( pstruct->usrpwd_[0] == '\0'){
		return Err_MAIL_PWD;
	}
	return Err_OK;
}

//发送邀请邮件
WEIBO_check_callback(POST_INVITE_SENDMAILS)
{
	WEIBO_struct_cast(t_wb_invite_sendmails);
	if( pstruct->myusrid_[0] == '\0'){
		return Err_MAIL_ID;
	}

	if( pstruct->mailtype_[0] == '\0'){
		return Err_MAIL_TYPE;
	}

	if( pstruct->nickname_[0] == '\0'){
		return Err_MAIL_NICKNAME;
	}

	if( pstruct->maillist_[0] == '\0'){
		return Err_MAIL_LIST;
	}
	return Err_OK;
}


WEIBO_check_callback(GET_MEDIA_SHORTURL_BATCH)
{
	WEIBO_struct_cast(t_wb_media_shorturl_batch);
	if( pstruct->urlids_[0] == '\0'){
		return Err_TEXT_ID;
	}
	return Err_OK;
}



typedef int (*f_check_callback)(const void* t_wb);
static 
f_check_callback vector_check_callbak[]=
{
	WEIBO_check_fun(UNK),
	WEIBO_check_fun(BASE),

	//获取下行数据集(timeline)接口 
	WEIBO_check_fun(GETSTATUSES_PUBLIC_TIMELINE),// 获取最新更新的公共微博消息 
	WEIBO_check_fun(GETSTATUSES_FRIENDS_TIMELINE),// 获取当前用户所关注用户的最新微博信息 (别名: statuses/home_timeline) 
	WEIBO_check_fun(GETSTATUSES_USE_TIMELINE),// 获取用户发布的微博信息列表 
	WEIBO_check_fun(GETSTATUSES_MENTIONS),// 获取@当前用户的微博列表 
	WEIBO_check_fun(GETSTATUSES_COMMENTS_TIMELINE),// 获取当前用户发送及收到的评论列表
	WEIBO_check_fun(GETSTATUSES_COMMENTS_BYME),// 获取当前用户发出的评论 
	WEIBO_check_fun(GETSTATUSES_COMMENTS_TOME),// 获取当前用户收到的评论 
	WEIBO_check_fun(GETSTATUSES_COMMENTS_LIST),// 获取指定微博的评论列表 
	WEIBO_check_fun(GETSTATUSES_COMMENTS_COUNTS),// 批量获取一组微博的评论数及转发数 
	WEIBO_check_fun(GETSTATUSES_UNREAD), // 获取当前用户未读消息数
	WEIBO_check_fun(PUTSTATUSES_RESET_COUNT),// 未读消息数清零接口 

	//微博访问接口 
	WEIBO_check_fun(GETSTATUSES_SHOW),// 根据ID获取单条微博信息内容 
	WEIBO_check_fun(GOTOSTATUSES_ID),// 根据微博ID和用户ID跳转到单条微博页面 
	WEIBO_check_fun(PUTSTATUSES_UPDATE),// 发布一条微博信息 
	WEIBO_check_fun(PUTSTATUSES_UPLOAD),// 上传图片并发布一条微博信息
	WEIBO_check_fun(PUTSTATUSES_UPLOAD_PIC),// 上传图片
	WEIBO_check_fun(PUTSTATUSES_DESTROY),// 删除一条微博信息 
	WEIBO_check_fun(PUTSTATUSES_REPOST),// 转发一条微博信息（可加评论） 
	WEIBO_check_fun(PUTSTATUSES_COMMENT),// 对一条微博信息进行评论 
	WEIBO_check_fun(PUTSTATUSES_COMMENT_DESTROY),// 删除当前用户的微博评论信息 
	WEIBO_check_fun(PUTSTATUSES_REPLY),// 回复微博评论信息

	//用户接口 
	WEIBO_check_fun(GETUSER_INFO),// 根据用户ID获取用户资料（授权用户）
	WEIBO_check_fun(GETFRINDS_LIST),// 获取当前用户关注对象列表及最新一条微博信息
	WEIBO_check_fun(GETFOLLOWERS_LIST),// 获取当前用户粉丝列表及最新一条微博信息 

	//私信接口 
	WEIBO_check_fun(GETDIRECTMSG),// 获取当前用户最新私信列表 
	WEIBO_check_fun(GETDIRESTMSG_SENT),// 获取当前用户发送的最新私信列表
	WEIBO_check_fun(PUTDIRECTMSG_NEW),// 发送一条私信
	WEIBO_check_fun(PUTDIRECTMSG_DESTROY),// 删除一条私信
	WEIBO_check_fun(GETDIRECTMSG_WITH),// 获取往来的私信列表

	//关注接口 
	WEIBO_check_fun(PUTFRIENDSHIPS_CREATE),// 关注某用户
	WEIBO_check_fun(PUTFRIENDSHIPS_CREATE_BATCH),//批量关注
	WEIBO_check_fun(PUTFRIENDSHIPS_DESTROY),// 取消关注
	WEIBO_check_fun(GETFRIENDSHIPS_EXISTS),// 判断两个用户是否有关注关系，返回两个用户关系的详细情况
	WEIBO_check_fun(GETFRIENDSHIPS_BATCH_EXISTS),// 批量获取一组用户是否为好友
	

	//Social Graph接口
	WEIBO_check_fun(GETFRIEND_IDS),// 关注列表
	WEIBO_check_fun(GETFOLLOWER_IDS),// 粉丝列表

	//账号接口 
	WEIBO_check_fun(GETACCOUNT_VERIFY),// 验证当前用户身份是否合法 
	WEIBO_check_fun(GETACCOUNT_RATELIMIT),// 获取当前用户API访问频率限制
	WEIBO_check_fun(PUTACCOUNT_QUITSESSION),// 当前用户退出登录 
	WEIBO_check_fun(PUTACCOUNT_UPDATE_PROFILEIMAGE),// 更改头像
	WEIBO_check_fun(PUTACCOUNT_UPDATE_PROFILE),// 更改资料
	WEIBO_check_fun(PUTACCOUNT_REGISTER),

	//收藏接口 
	WEIBO_check_fun(GETFAVORITES),// 获取当前用户的收藏列表 
	WEIBO_check_fun(PUTFAVORITES_CREATE),// 添加收藏
	WEIBO_check_fun(PUTFAVORITES_DESTROY),// 删除当前用户收藏的微博信息

	//登录/OAuth接口 
	WEIBO_check_fun(OAUTH_REQUEST_TOKEN),// 获取未授权的Request Token
	WEIBO_check_fun(OAUTH_AUTHORIZE),
	WEIBO_check_fun(OAUTH_ACCESS_TOKEN),// 获取授权过的Access Token

	// 表情
	WEIBO_check_fun(GET_EMOTIONS),//  返回新浪微博官方所有表情、魔法表情的相关信息。包括短语、表情类型、表情分类，是否热门等。

	// 用户搜索 
	WEIBO_check_fun(GET_USERS_SEARCH),//  搜索微博用户,返回关键字匹配的微博用户，(仅对新浪合作开发者开放) 

	// 微博搜索 
	WEIBO_check_fun(GET_WB_SEARCH),//  返回关键字匹配的微博，(仅对新浪合作开发者开放)
	WEIBO_check_fun(GET_STATUSES_SEARCH),// 搜索微博(多条件组合) (仅对合作开发者开放) 

	WEIBO_check_fun(GET_PROVINCES),//  省份城市编码表 
	WEIBO_check_fun(REPORT),// 举报

	// cookie 
	WEIBO_check_fun(COOKIE),
	WEIBO_check_fun(UPDATETGT),// UPDATETGT,

	//
	WEIBO_check_fun(CUSTOM),

	// hot point 
	WEIBO_check_fun(HOT_REPOST_DAYLIY),
	WEIBO_check_fun(HOT_REPOST_WEEKLY),
	WEIBO_check_fun(HOT_COMMENT_DAYLIY),
	WEIBO_check_fun(HOT_COMMENT_WEEKLY),

	//
	WEIBO_check_fun(GET_USERS_HOT),// 获取系统推荐用户
	WEIBO_check_fun(POST_USERS_REMARK),//更新修改当前登录用户所关注的某个好友的备注信息New!
	WEIBO_check_fun(GET_USERS_SUGGESTIONS), //Users/suggestions 返回当前用户可能感兴趣的用户

	// 话题接口 ,by welbon,2011-01-10
	WEIBO_check_fun(GET_TRENDS),//trends 获取某人的话题
	WEIBO_check_fun(GET_TRENDS_STATUSES),//trends/statuses 获取某一话题下的微博
	WEIBO_check_fun(POST_TRENDS_FOLLOW),//trends/follow 关注某一个话题
	WEIBO_check_fun(DELETE_TRENDS_DESTROY),//trends/destroy 取消关注的某一个话题
	WEIBO_check_fun(GET_TRENDS_HOURLY),//trends/destroy 按小时返回热门话题
	WEIBO_check_fun(GET_TRENDS_DAYLIY),//trends/daily 按日期返回热门话题。返回某一日期的话题
	WEIBO_check_fun(GET_TRENDS_WEEKLIY),//trends/weekly 按周返回热门话题。返回某一日期之前某一周的话题

	// 黑名单接口 ,by welbon,2011-01-10
	WEIBO_check_fun(POST_BLOCKS_CREATE),//将某用户加入黑名单
	WEIBO_check_fun(POST_BLOCKS_DESTROY),//将某用户移出黑名单
	WEIBO_check_fun(GET_BLOCKS_EXISTS),//检测某用户是否是黑名单用户
	WEIBO_check_fun(GET_BLOCKS_BLOCKING),//列出黑名单用户(输出用户详细信息)
	WEIBO_check_fun(GET_BLOCKS_BLOCKING_IDS),//列出分页黑名单用户（只输出id）

	//用户标签接口 ,by welbon,2011-01-10
	WEIBO_check_fun(GET_TAGS),//tags 返回指定用户的标签列表
	WEIBO_check_fun(POST_TAGS_CREATE),//tags/create 添加用户标签
	WEIBO_check_fun(GET_TAGS_SUGGESTIONS),//tags/suggestions 返回用户感兴趣的标签
	WEIBO_check_fun(POST_TAGS_DESTROY),//tags/destroy 删除标签
	WEIBO_check_fun(POST_TAGS_DESTROY_BATCH),//tags/destroy_batch 批量删除标签

	//Invite Mail
	WEIBO_check_fun(POST_INVITE_MAILCONTACT),//邀请邮箱联系人
	WEIBO_check_fun(POST_INVITE_MSNCONTACT),//邀请MSN联系人
	WEIBO_check_fun(POST_INVITE_SENDMAILS),//发送邀请邮件

	//media batch
	WEIBO_check_fun(GET_MEDIA_SHORTURL_BATCH),
	WEIBO_check_fun(XAUTH_ACCESS_TOKEN),

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
