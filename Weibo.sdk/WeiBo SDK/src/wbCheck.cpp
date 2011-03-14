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
	return -1;
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

// �Զ���
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

	//��ȡ�������ݼ�(timeline)�ӿ� 
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

	//΢�����ʽӿ� 
	WEIBO_check_fun(GETSTATUSES_SHOW),
	WEIBO_check_fun(GOTOSTATUSES_ID),
	WEIBO_check_fun(PUTSTATUSES_UPDATE),
	WEIBO_check_fun(PUTSTATUSES_UPLOAD),
	WEIBO_check_fun(PUTSTATUSES_DESTROY),
	WEIBO_check_fun(PUTSTATUSES_REPOST),
	WEIBO_check_fun(PUTSTATUSES_COMMENT),
	WEIBO_check_fun(PUTSTATUSES_COMMENT_DESTROY),
	WEIBO_check_fun(PUTSTATUSES_REPLY),

	//�û��ӿ� 
	WEIBO_check_fun(GETUSER_INFO),
	WEIBO_check_fun(GETFRINDS_LIST),
	WEIBO_check_fun(GETFOLLOWERS_LIST),

	//˽�Žӿ� 
	WEIBO_check_fun(GETDIRECTMSG),
	WEIBO_check_fun(GETDIRESTMSG_SENT),
	WEIBO_check_fun(PUTDIRECTMSG_NEW),
	WEIBO_check_fun(PUTDIRECTMSG_DESTROY),

	//��ע�ӿ� 
	WEIBO_check_fun(PUTFRIENDSHIPS_CREATE),
	WEIBO_check_fun(PUTFRIENDSHIPS_DESTROY),
	WEIBO_check_fun(GETFRIENDSHIPS_EXISTS),

	//Social Graph�ӿ�
	WEIBO_check_fun(GETFRIEND_IDS),
	WEIBO_check_fun(GETFOLLOWER_IDS),

	//�˺Žӿ� 
	WEIBO_check_fun(GETACCOUNT_VERIFY),
	WEIBO_check_fun(GETACCOUNT_RATELIMIT),
	WEIBO_check_fun(PUTACCOUNT_QUITSESSION),
	WEIBO_check_fun(PUTACCOUNT_UPDATE_PROFILEIMAGE),
	WEIBO_check_fun(PUTACCOUNT_UPDATE_PROFILE),
	WEIBO_check_fun(PUTACCOUNT_REGISTER),

	//�ղؽӿ� 
	WEIBO_check_fun(GETFAVORITES),
	WEIBO_check_fun(PUTFAVORITES_CREATE),
	WEIBO_check_fun(PUTFAVORITES_DESTROY),

	//��¼/OAuth�ӿ� 
	WEIBO_check_fun(OAUTH_REQUEST_TOKEN),
	WEIBO_check_fun(OAUTH_AUTHORIZE),
	WEIBO_check_fun(OAUTH_ACCESS_TOKEN),

	// ����
	WEIBO_check_fun(GET_EMOTIONS),

	// �û����� 
	WEIBO_check_fun(GET_USERS_SEARCH),

	// ΢������ 
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
		return -1;//������Χ
	}

	int err = vector_check_callbak[option](t_wb);
	if( Err_OK != err )
	{
		wbPRINTF( errorText[ err ] );
	}
	return err;
}
