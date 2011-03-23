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

// ת��
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
{// ��ȡ���¸��µĹ���΢����Ϣ
	WEIBO_struct_cast(t_wb_get_statuses_friends_timeline);
	return Err_OK;
}

WEIBO_check_callback(GETSTATUSES_FRIENDS_TIMELINE)
{// ��ȡ��ǰ�û�����ע�û�������΢����Ϣ
	WEIBO_struct_cast(t_wb_get_statuses_user_timeline);
	return Err_OK;
}

WEIBO_check_callback(GETSTATUSES_USE_TIMELINE)
{// ��ȡ�û�������΢����Ϣ�б� 
	WEIBO_struct_cast(t_wb_get_statuses_user_timeline);

    return Err_OK;
}

WEIBO_check_callback(GETSTATUSES_MENTIONS)
{// ��ȡ@��ǰ�û���΢���б�
	WEIBO_struct_cast(t_wb_get_statuses_mentions);

	return Err_OK;
}

WEIBO_check_callback(GETSTATUSES_COMMENTS_TIMELINE)
{// ��ȡ��ǰ�û����ͼ��յ��������б� 
	WEIBO_struct_cast(t_wb_get_statuses_comments_timeline);

    return Err_OK;
}

WEIBO_check_callback(GETSTATUSES_COMMENTS_BYME)
{// ��ȡ��ǰ�û����������� 
	WEIBO_struct_cast(t_wb_get_statuses_comments_byme);

    return Err_OK;
}

WEIBO_check_callback(GETSTATUSES_COMMENTS_TOME)
{// ��ȡ��ǰ�û����������� 
	WEIBO_struct_cast(t_wb_get_statuses_comments_tome);

    return Err_OK;
}

WEIBO_check_callback(GETSTATUSES_COMMENTS_LIST)
{// ��ȡָ��΢���������б�
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
{// ��ȡ��ǰ�û�δ����Ϣ����(������Ϣ)
	WEIBO_struct_cast(t_wb_get_statuses_unread);
	return Err_OK;
}

WEIBO_check_callback(PUTSTATUSES_RESET_COUNT)
{// ��ȡ��ǰ�û�δ����Ϣ����(������Ϣ)
	WEIBO_struct_cast(t_wb_get_statuses_reset_count);
	return Err_OK;
}


//΢������

WEIBO_check_callback(GETSTATUSES_SHOW)
{// ����ID��ȡ����΢����Ϣ����
	WEIBO_struct_cast(t_wb_get_statuses_show);

	if( pstruct->wbId_[0] == '\0' )
	{
		return Err_WB_ID;
	}
	return Err_OK;
}

WEIBO_check_callback(GOTOSTATUSES_ID)
{// ����΢��ID���û�ID��ת������΢��ҳ��( �������),ֻ����һ��URI
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
{//����һ��΢����Ϣ

	WEIBO_struct_cast(t_wb_put_statuses_update);

	if( pstruct->szwbInfo_[0] == '\0' )
	{
		return Err_TEXT_ID;
	}

	return Err_OK;
}

WEIBO_check_callback(PUTSTATUSES_UPLOAD)
{// �ϴ�ͼƬ������һ��΢����Ϣ

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
{// �ϴ�ͼƬ

	WEIBO_struct_cast(t_wb_put_statuses_upload_pic);

	if( pstruct->szImagePath_[0] == '\0' ) {
		return Err_IMAGE_ID;
	}
	return Err_OK;
}


WEIBO_check_callback(PUTSTATUSES_DESTROY)
{// ɾ��һ��΢����Ϣ
	WEIBO_struct_cast(t_wb_put_statuses_destroy);
	
	if( pstruct->wbId_[0] == '\0' )
	{
		return Err_WB_ID;
	}

	return Err_OK;
}

WEIBO_check_callback(PUTSTATUSES_REPOST)
{// ת��һ��΢����Ϣ���ɼ����ۣ�
	WEIBO_struct_cast(t_wb_put_statuses_repost);

	if( pstruct->wbId_[0] == '\0' )
	{
		return Err_WB_ID;
	}

    return Err_OK;
}

WEIBO_check_callback(PUTSTATUSES_COMMENT)
{//��һ��΢����Ϣ�������� 

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
{// ɾ����ǰ�û���΢��������Ϣ 

	WEIBO_struct_cast(t_wb_put_statuses_comment_destroy);

	if( pstruct->wbCId_[0] == '\0' )
	{
		return Err_WBCOM_ID;
	}
	return Err_OK;
}

WEIBO_check_callback(PUTSTATUSES_REPLY)
{// �ظ�΢��������Ϣ

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

//----------------------------------�û�------------------------------------------//

WEIBO_check_callback(GETUSER_INFO)
{// �����û�ID��ȡ�û����ϣ���Ȩ�û���
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
{// ��ȡ��ǰ�û���ע�����б�����һ��΢����Ϣ
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
{// ��ȡ��ǰ�û���˿�б�����һ��΢����Ϣ
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

//-----------------------------------˽��------------------------------------------//

WEIBO_check_callback(GETDIRECTMSG)
{// ��ȡ��ǰ�û�����˽���б�
	WEIBO_struct_cast(t_wb_get_direct_message);

	return Err_OK;
}

WEIBO_check_callback(GETDIRESTMSG_SENT)
{// ��ȡ��ǰ�û����͵�����˽���б� 
	WEIBO_struct_cast(t_wb_get_direct_message_sent);
	return Err_OK;
}

WEIBO_check_callback(PUTDIRECTMSG_NEW)
{// ����һ��˽��
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
{// ɾ��һ��˽��
	WEIBO_struct_cast(t_wb_put_direct_message_destroy);

	if( pstruct->wbId_[0] == '\0' )
	{
		return Err_WB_ID;
	}
	return Err_OK;
}

WEIBO_check_callback(GETDIRECTMSG_WITH)
{//��ȡ�û�����˽���б�

	WEIBO_struct_cast(t_wb_get_direct_message_with);

	if( pstruct->wbuid_[0] == '\0' )
	{
		return Err_WB_ID;
	}
	return Err_OK;
}

//-----------------------------------------��ע---------------------------------------------//

WEIBO_check_callback(PUTFRIENDSHIPS_CREATE)
{// ��עĳ�û�
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
{// ��עĳ�û�
	WEIBO_struct_cast(t_wb_put_friendships_create_batch);

	if( pstruct->wbIDs_[0] == '\0' )
	{
		return Err_USE_ID;
	}
	return Err_OK;
}

WEIBO_check_callback(PUTFRIENDSHIPS_DESTROY)
{// ȡ����ע
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

//�˺�
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
{// ����ͷ�� 
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
{// ��ȡ��ǰ�û����ղ��б� 
	WEIBO_struct_cast(t_wb_get_favorites);

	return Err_OK;
}

WEIBO_check_callback(PUTFAVORITES_CREATE)
{// ����ղ� 
	WEIBO_struct_cast(t_wb_put_favorites_create);

	if( pstruct->wbId_[0] == '\0' )
	{
		return Err_WB_ID;
	}
	return Err_OK;
}

WEIBO_check_callback(PUTFAVORITES_DESTROY)
{// ɾ����ǰ�û��ղص�΢����Ϣ 
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

WEIBO_check_callback(OAUTH_ACCESS_TOKEN)// ��ȡ��Ȩ����Access Token
{
	WEIBO_struct_cast(t_wb_oauth_access_token);

	if( pstruct->wbauth_.oauth_verifier_[0] == '\0' )
	{
		return Err_OAUTHVERIFY_ID;
	}
	return Err_OK;
}


WEIBO_check_callback(XAUTH_ACCESS_TOKEN)// ��ȡ��Ȩ����Access Token
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

// ����
WEIBO_check_callback(GET_EMOTIONS)
{
	return Err_OK;
}

// �û����� 
WEIBO_check_callback(GET_USERS_SEARCH)
{
	return Err_OK;
}

// ΢������ 
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

	// �û���/����
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

// ����TGT
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



// �Զ���
WEIBO_check_callback(CUSTOM)
{
	return Err_OK;
}

// �ȵ��Ƽ�
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


//�û��ӿ�NEW
WEIBO_check_callback(GET_USERS_HOT)
{
	return Err_OK;
}

WEIBO_check_callback(POST_USERS_REMARK)
{
	WEIBO_struct_cast(t_wb_users_remark);

	// �û���
	if( pstruct->userId_[0] == '\0' ){
		return Err_USE_ID;
	}

	// ��ע��Ϣ
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
// ����΢��

//��ȡĳ�˵Ļ���
WEIBO_check_callback(GET_TRENDS)
{
	WEIBO_struct_cast(t_wb_trends);

	// �û���
	if( pstruct->usrid_[0] == '\0' ){
		return Err_USE_ID;
	}
	return Err_OK;
}

//��ȡĳһ�����µ�΢��
WEIBO_check_callback(GET_TRENDS_STATUSES)
{
	WEIBO_struct_cast(t_wb_trends_statuses);
	if( pstruct->terndname_[0] == '\0'){
		return Err_KEYWORD;
	}
	return Err_OK;
}

//��עĳһ������
WEIBO_check_callback(POST_TRENDS_FOLLOW)
{
	WEIBO_struct_cast(t_wb_trends_follow);
	if( pstruct->terndname_[0] == '\0'){
		return Err_KEYWORD;
	}
	return Err_OK;
}

//trends/destroy ȡ����ע��ĳһ������
WEIBO_check_callback(DELETE_TRENDS_DESTROY)
{
	WEIBO_struct_cast(t_wb_trends_destroy);
	if( pstruct->trendid_[0] == '\0'){
		return Err_TREND_ID;
	}
	return Err_OK;
}
//trends/destroy ��Сʱ�������Ż���
WEIBO_check_callback(GET_TRENDS_HOURLY)
{
	return Err_OK;
}
//trends/daily �����ڷ������Ż��⡣����ĳһ���ڵĻ���
WEIBO_check_callback(GET_TRENDS_DAYLIY)
{
	return Err_OK;
}
//trends/weekly ���ܷ������Ż��⡣����ĳһ����֮ǰĳһ�ܵĻ���
WEIBO_check_callback(GET_TRENDS_WEEKLIY)
{
	return Err_OK;
}

////////////////////////////////////////////////////////////////////////////////////////////
// �������ӿ� ,by welbon,2011-01-10

//��ĳ�û����������
WEIBO_check_callback(POST_BLOCKS_CREATE)
{
	WEIBO_struct_cast(t_wb_blocks_create);
	if( pstruct->usrid_[0] == '\0'){
		return Err_USE_ID;
	}
	return Err_OK;
}

//��ĳ�û��Ƴ�������
WEIBO_check_callback(POST_BLOCKS_DESTROY)
{
	WEIBO_struct_cast(t_wb_blocks_destroy);
	if( pstruct->usrid_[0] == '\0'){
		return Err_USE_ID;
	}
	return Err_OK;
}

//���ĳ�û��Ƿ��Ǻ������û�
WEIBO_check_callback(GET_BLOCKS_EXISTS)
{
	WEIBO_struct_cast(t_wb_blocks_exist);
	if( pstruct->usrid_[0] == '\0'){
		return Err_USE_ID;
	}
	return Err_OK;
}

//�г��������û�(����û���ϸ��Ϣ)
WEIBO_check_callback(GET_BLOCKS_BLOCKING)
{
	return Err_OK;
}

//�г���ҳ�������û���ֻ���id��
WEIBO_check_callback(GET_BLOCKS_BLOCKING_IDS)
{
	return Err_OK;
}

////////////////////////////////////////////////////////////////////////////////////////////
//�û���ǩ�ӿ� ,by welbon,2011-01-10

//tags ����ָ���û��ı�ǩ�б�
WEIBO_check_callback(GET_TAGS)
{
	WEIBO_struct_cast(t_wb_tags);
	if( pstruct->usrid_[0] == '\0'){
		return Err_USE_ID;
	}

	return Err_OK;
}
//tags/create ����û���ǩ
WEIBO_check_callback(POST_TAGS_CREATE)
{
	WEIBO_struct_cast(t_wb_tags_create);
	if( pstruct->tags_[0] == '\0'){
		return Err_KEYWORD;
	}
	return Err_OK;
}
//tags/suggestions �����û�����Ȥ�ı�ǩ
WEIBO_check_callback(GET_TAGS_SUGGESTIONS)
{
	return Err_OK;
}
//tags/destroy ɾ����ǩ
WEIBO_check_callback(POST_TAGS_DESTROY)
{
	WEIBO_struct_cast(t_wb_tags_destroy);
	if( pstruct->tagId_[0] == '\0'){
		return Err_TAGS_ID;
	}
	return Err_OK;
}
//tags/destroy_batch ����ɾ����ǩ
WEIBO_check_callback(POST_TAGS_DESTROY_BATCH)
{
	WEIBO_struct_cast(t_wb_tags_destroy_batch);
	if( pstruct->ids_[0] == '\0'){
		return Err_TAGS_ID;
	}
	return Err_OK;
}


//����������ϵ��
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


//����MSN��ϵ��
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

//���������ʼ�
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

	//��ȡ�������ݼ�(timeline)�ӿ� 
	WEIBO_check_fun(GETSTATUSES_PUBLIC_TIMELINE),// ��ȡ���¸��µĹ���΢����Ϣ 
	WEIBO_check_fun(GETSTATUSES_FRIENDS_TIMELINE),// ��ȡ��ǰ�û�����ע�û�������΢����Ϣ (����: statuses/home_timeline) 
	WEIBO_check_fun(GETSTATUSES_USE_TIMELINE),// ��ȡ�û�������΢����Ϣ�б� 
	WEIBO_check_fun(GETSTATUSES_MENTIONS),// ��ȡ@��ǰ�û���΢���б� 
	WEIBO_check_fun(GETSTATUSES_COMMENTS_TIMELINE),// ��ȡ��ǰ�û����ͼ��յ��������б�
	WEIBO_check_fun(GETSTATUSES_COMMENTS_BYME),// ��ȡ��ǰ�û����������� 
	WEIBO_check_fun(GETSTATUSES_COMMENTS_TOME),// ��ȡ��ǰ�û��յ������� 
	WEIBO_check_fun(GETSTATUSES_COMMENTS_LIST),// ��ȡָ��΢���������б� 
	WEIBO_check_fun(GETSTATUSES_COMMENTS_COUNTS),// ������ȡһ��΢������������ת���� 
	WEIBO_check_fun(GETSTATUSES_UNREAD), // ��ȡ��ǰ�û�δ����Ϣ��
	WEIBO_check_fun(PUTSTATUSES_RESET_COUNT),// δ����Ϣ������ӿ� 

	//΢�����ʽӿ� 
	WEIBO_check_fun(GETSTATUSES_SHOW),// ����ID��ȡ����΢����Ϣ���� 
	WEIBO_check_fun(GOTOSTATUSES_ID),// ����΢��ID���û�ID��ת������΢��ҳ�� 
	WEIBO_check_fun(PUTSTATUSES_UPDATE),// ����һ��΢����Ϣ 
	WEIBO_check_fun(PUTSTATUSES_UPLOAD),// �ϴ�ͼƬ������һ��΢����Ϣ
	WEIBO_check_fun(PUTSTATUSES_UPLOAD_PIC),// �ϴ�ͼƬ
	WEIBO_check_fun(PUTSTATUSES_DESTROY),// ɾ��һ��΢����Ϣ 
	WEIBO_check_fun(PUTSTATUSES_REPOST),// ת��һ��΢����Ϣ���ɼ����ۣ� 
	WEIBO_check_fun(PUTSTATUSES_COMMENT),// ��һ��΢����Ϣ�������� 
	WEIBO_check_fun(PUTSTATUSES_COMMENT_DESTROY),// ɾ����ǰ�û���΢��������Ϣ 
	WEIBO_check_fun(PUTSTATUSES_REPLY),// �ظ�΢��������Ϣ

	//�û��ӿ� 
	WEIBO_check_fun(GETUSER_INFO),// �����û�ID��ȡ�û����ϣ���Ȩ�û���
	WEIBO_check_fun(GETFRINDS_LIST),// ��ȡ��ǰ�û���ע�����б�����һ��΢����Ϣ
	WEIBO_check_fun(GETFOLLOWERS_LIST),// ��ȡ��ǰ�û���˿�б�����һ��΢����Ϣ 

	//˽�Žӿ� 
	WEIBO_check_fun(GETDIRECTMSG),// ��ȡ��ǰ�û�����˽���б� 
	WEIBO_check_fun(GETDIRESTMSG_SENT),// ��ȡ��ǰ�û����͵�����˽���б�
	WEIBO_check_fun(PUTDIRECTMSG_NEW),// ����һ��˽��
	WEIBO_check_fun(PUTDIRECTMSG_DESTROY),// ɾ��һ��˽��
	WEIBO_check_fun(GETDIRECTMSG_WITH),// ��ȡ������˽���б�

	//��ע�ӿ� 
	WEIBO_check_fun(PUTFRIENDSHIPS_CREATE),// ��עĳ�û�
	WEIBO_check_fun(PUTFRIENDSHIPS_CREATE_BATCH),//������ע
	WEIBO_check_fun(PUTFRIENDSHIPS_DESTROY),// ȡ����ע
	WEIBO_check_fun(GETFRIENDSHIPS_EXISTS),// �ж������û��Ƿ��й�ע��ϵ�����������û���ϵ����ϸ���
	WEIBO_check_fun(GETFRIENDSHIPS_BATCH_EXISTS),// ������ȡһ���û��Ƿ�Ϊ����
	

	//Social Graph�ӿ�
	WEIBO_check_fun(GETFRIEND_IDS),// ��ע�б�
	WEIBO_check_fun(GETFOLLOWER_IDS),// ��˿�б�

	//�˺Žӿ� 
	WEIBO_check_fun(GETACCOUNT_VERIFY),// ��֤��ǰ�û�����Ƿ�Ϸ� 
	WEIBO_check_fun(GETACCOUNT_RATELIMIT),// ��ȡ��ǰ�û�API����Ƶ������
	WEIBO_check_fun(PUTACCOUNT_QUITSESSION),// ��ǰ�û��˳���¼ 
	WEIBO_check_fun(PUTACCOUNT_UPDATE_PROFILEIMAGE),// ����ͷ��
	WEIBO_check_fun(PUTACCOUNT_UPDATE_PROFILE),// ��������
	WEIBO_check_fun(PUTACCOUNT_REGISTER),

	//�ղؽӿ� 
	WEIBO_check_fun(GETFAVORITES),// ��ȡ��ǰ�û����ղ��б� 
	WEIBO_check_fun(PUTFAVORITES_CREATE),// ����ղ�
	WEIBO_check_fun(PUTFAVORITES_DESTROY),// ɾ����ǰ�û��ղص�΢����Ϣ

	//��¼/OAuth�ӿ� 
	WEIBO_check_fun(OAUTH_REQUEST_TOKEN),// ��ȡδ��Ȩ��Request Token
	WEIBO_check_fun(OAUTH_AUTHORIZE),
	WEIBO_check_fun(OAUTH_ACCESS_TOKEN),// ��ȡ��Ȩ����Access Token

	// ����
	WEIBO_check_fun(GET_EMOTIONS),//  ��������΢���ٷ����б��顢ħ������������Ϣ����������������͡�������࣬�Ƿ����ŵȡ�

	// �û����� 
	WEIBO_check_fun(GET_USERS_SEARCH),//  ����΢���û�,���عؼ���ƥ���΢���û���(�������˺��������߿���) 

	// ΢������ 
	WEIBO_check_fun(GET_WB_SEARCH),//  ���عؼ���ƥ���΢����(�������˺��������߿���)
	WEIBO_check_fun(GET_STATUSES_SEARCH),// ����΢��(���������) (���Ժ��������߿���) 

	WEIBO_check_fun(GET_PROVINCES),//  ʡ�ݳ��б���� 
	WEIBO_check_fun(REPORT),// �ٱ�

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
	WEIBO_check_fun(GET_USERS_HOT),// ��ȡϵͳ�Ƽ��û�
	WEIBO_check_fun(POST_USERS_REMARK),//�����޸ĵ�ǰ��¼�û�����ע��ĳ�����ѵı�ע��ϢNew!
	WEIBO_check_fun(GET_USERS_SUGGESTIONS), //Users/suggestions ���ص�ǰ�û����ܸ���Ȥ���û�

	// ����ӿ� ,by welbon,2011-01-10
	WEIBO_check_fun(GET_TRENDS),//trends ��ȡĳ�˵Ļ���
	WEIBO_check_fun(GET_TRENDS_STATUSES),//trends/statuses ��ȡĳһ�����µ�΢��
	WEIBO_check_fun(POST_TRENDS_FOLLOW),//trends/follow ��עĳһ������
	WEIBO_check_fun(DELETE_TRENDS_DESTROY),//trends/destroy ȡ����ע��ĳһ������
	WEIBO_check_fun(GET_TRENDS_HOURLY),//trends/destroy ��Сʱ�������Ż���
	WEIBO_check_fun(GET_TRENDS_DAYLIY),//trends/daily �����ڷ������Ż��⡣����ĳһ���ڵĻ���
	WEIBO_check_fun(GET_TRENDS_WEEKLIY),//trends/weekly ���ܷ������Ż��⡣����ĳһ����֮ǰĳһ�ܵĻ���

	// �������ӿ� ,by welbon,2011-01-10
	WEIBO_check_fun(POST_BLOCKS_CREATE),//��ĳ�û����������
	WEIBO_check_fun(POST_BLOCKS_DESTROY),//��ĳ�û��Ƴ�������
	WEIBO_check_fun(GET_BLOCKS_EXISTS),//���ĳ�û��Ƿ��Ǻ������û�
	WEIBO_check_fun(GET_BLOCKS_BLOCKING),//�г��������û�(����û���ϸ��Ϣ)
	WEIBO_check_fun(GET_BLOCKS_BLOCKING_IDS),//�г���ҳ�������û���ֻ���id��

	//�û���ǩ�ӿ� ,by welbon,2011-01-10
	WEIBO_check_fun(GET_TAGS),//tags ����ָ���û��ı�ǩ�б�
	WEIBO_check_fun(POST_TAGS_CREATE),//tags/create ����û���ǩ
	WEIBO_check_fun(GET_TAGS_SUGGESTIONS),//tags/suggestions �����û�����Ȥ�ı�ǩ
	WEIBO_check_fun(POST_TAGS_DESTROY),//tags/destroy ɾ����ǩ
	WEIBO_check_fun(POST_TAGS_DESTROY_BATCH),//tags/destroy_batch ����ɾ����ǩ

	//Invite Mail
	WEIBO_check_fun(POST_INVITE_MAILCONTACT),//����������ϵ��
	WEIBO_check_fun(POST_INVITE_MSNCONTACT),//����MSN��ϵ��
	WEIBO_check_fun(POST_INVITE_SENDMAILS),//���������ʼ�

	//media batch
	WEIBO_check_fun(GET_MEDIA_SHORTURL_BATCH),
	WEIBO_check_fun(XAUTH_ACCESS_TOKEN),

};


int Weibo_check_valid(int option ,const void* t_wb)
{
	if( WEIBO_OPTION(UNK) >= option ||
		WEIBO_OPTION(LAST) <= option )
	{
		return -1;//������Χ
	}

	int err = vector_check_callbak[option](t_wb);
	if( Err_OK != err )
	{
		wbPRINTF( errorText[ err ] );
	}
	return err;
}
