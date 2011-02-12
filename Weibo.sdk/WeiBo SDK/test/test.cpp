/**
*  @brief test.cpp,main
*  @file  test.cpp
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

//#if (defined(WIN32)) || (defined(WINCE))
#include "stdafx.h"
//#endif //WIN32||WINCE

#include "wbinit.h"
#include <libweibo/weibo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <assert.h>
#include "wbcallback.h"

#define LIB_loadWEIBOSDK

#if defined(WIN32) || defined(WINCE)// WIN
//
#	if (defined(LIB_loadWEIBOSDK)) || (defined(WEIBO_STATICLIB))

#if defined(WEIBO_VC80)
#		pragma comment(lib ,"Json_VC80.lib")
#		pragma comment(lib ,"WeiBoU_VC80.lib")
#elif defined(WEIBO_VC90)
#		pragma comment(lib ,"Json_VC90.lib")
#		pragma comment(lib ,"WeiBoU_VC90.lib")
#endif

#	else
//	//��̬����
#	endif //LIB_loadWEIBOSDK || WEIBO_STATICLIB
//	
#	if defined(WINCE) // WINCEϵͳ
#		define GETPROCADDRESS GetProcAddressA
#		define SYSTEM(s)
#	else				
#		define GETPROCADDRESS GetProcAddress
#		define SYSTEM(s)	  system(s) 
#	endif 
#	define TEST_MAIN _tmain
//
#	include <Windows.h>
#else
#	define GETPROCADDRESS
#	define SYSTEM(s)
#	define TEST_MAIN main
#endif // WIN


/** oauth copy */
typedef void (*fun_loWeibo_oauth_copy)(struct t_wb_oauth* pdest , const struct t_wb_oauth* psrc);

/** ����һ������ÿ���¼�����Ҫ ����һ������һ��Ҫͨ�� weibo_destroy_request���� �������� */
typedef WEIBORequest* (*fun_loWeibo_request_create)(void);

/** ����һЩ����������д��Ȼ��ִ��(�ص�����������...) */
typedef  int (*fun_loWeibo_request_setopt)(WEIBORequest* request , WEIBOReqoption option , ...);

/** ִ��,�ȴ��ص����ݵ�ִ�У���ִ�н������ͨ�� weibo_request_setopt�������õķ�������֮������
*
* @param bAsyn = false ͬ��ִ�У��߳����� )
*              = true  ��ʾ�첽
*/
typedef  int (*fun_loWeibo_request_start)(WEIBORequest* request , bool bAsyn);

/** �ȴ�ִ�У����첽 loWeibo_request_start ִ�п��Խ�� �� �ﵽͬ������ ��*/
typedef  void (*fun_loWeibo_request_wait)(WEIBORequest* request);

/** ֹͣ */
typedef  int (*fun_loWeibo_request_stop)(WEIBORequest* request);

/** ����������� */
typedef  int (*fun_loWeibo_request_destroy)(WEIBORequest* request);

/** ����URL��Ϣ */
typedef  int (*fun_loWeibo_get_url)(struct t_wb_URI* outURI , WEIBOoption option , const void* t_wb , const struct t_wb_REQ_EX* req_ex);

struct t_wb_interface
{
	fun_loWeibo_oauth_copy      pfun_oauth_copy;
	fun_loWeibo_request_create  pfun_request_create;
    fun_loWeibo_request_setopt  pfun_request_setopt;
	fun_loWeibo_request_start   pfun_request_start;
    fun_loWeibo_request_wait    pfun_request_wait;
    fun_loWeibo_request_stop    pfun_request_stop;
    fun_loWeibo_request_destroy pfun_request_destroy;
	fun_loWeibo_get_url         pfun_get_url;

	// ��̬��ʽ���� 
#if (!defined(WEIBO_STATICLIB)) && (!defined(LIB_loadWEIBOSDK)) &&( (defined(WIN32)) || (defined(WINCE)))
	HMODULE dll_;
#endif
};


static
bool load_wb_interface(t_wb_interface* pwb_interface)
{
	memset(pwb_interface , 0 , sizeof(struct t_wb_interface) );

  
	// ��û�ж��徲̬���ر�־������û�ж���WIN32��WINCE
#if (defined(WEIBO_STATICLIB)) || (defined(LIB_loadWEIBOSDK)) || (!defined(WIN32) && !defined(WINCE))

	pwb_interface->pfun_oauth_copy      = loWeibo_oauth_copy;
	pwb_interface->pfun_request_create  = loWeibo_request_create;
	pwb_interface->pfun_request_setopt  = loWeibo_request_setopt;
	pwb_interface->pfun_request_start   = loWeibo_request_start;
	pwb_interface->pfun_request_wait    = loWeibo_request_wait;
	pwb_interface->pfun_request_stop    = loWeibo_request_stop;
	pwb_interface->pfun_request_destroy = loWeibo_request_destroy;
	pwb_interface->pfun_get_url         = loWeibo_get_url;

#else // ��̬����

	WCHAR appPath[256] = {0};
/*	GetModuleFileNameW( NULL , appPath , 256 );
	WCHAR* pz = wcsrchr(appPath , '\\');
	if( !pz )
	{
		pz = wcsrchr(appPath , '/');
	}
	if( pz )
	{
		*(pz+1) = '\0';
	}*/
#if defined( WEIBO_VC90 )
	wcscat(appPath , L"WeiboU_VC90.dll");
	pwb_interface->dll_ = ::LoadLibraryW(appPath);
#elif defined( WEIBO_VC80)
	wcscat(appPath , L"WeiboU_VC80.dll");
	pwb_interface->dll_ = ::LoadLibraryW(appPath);
#endif

	if( !pwb_interface->dll_ )
	{
		FILE* fp = _wfopen(appPath,L"rb");
		if( !fp )
		{
			MessageBox(NULL,appPath,_T("file is not exist!"),MB_OK);
		}
		else
		{
			fclose(fp);
		}
		MessageBox(NULL,appPath,_T("load Weibo DLL failure!"),MB_OK);
		return false;
	}

	pwb_interface->pfun_oauth_copy      = (fun_loWeibo_oauth_copy)::GETPROCADDRESS( pwb_interface->dll_ , "loWeibo_oauth_copy");
	pwb_interface->pfun_request_create  = (fun_loWeibo_request_create)::GETPROCADDRESS( pwb_interface->dll_ , "loWeibo_request_create");
	pwb_interface->pfun_request_setopt  = (fun_loWeibo_request_setopt)::GETPROCADDRESS( pwb_interface->dll_ , "loWeibo_request_setopt");
	pwb_interface->pfun_request_start   = (fun_loWeibo_request_start)::GETPROCADDRESS( pwb_interface->dll_ , "loWeibo_request_start");
	pwb_interface->pfun_request_wait    = (fun_loWeibo_request_wait)::GETPROCADDRESS( pwb_interface->dll_ , "loWeibo_request_wait");
	pwb_interface->pfun_request_stop    = (fun_loWeibo_request_stop)::GETPROCADDRESS( pwb_interface->dll_ , "loWeibo_request_stop");
	pwb_interface->pfun_request_destroy = (fun_loWeibo_request_destroy)::GETPROCADDRESS( pwb_interface->dll_ , "loWeibo_request_destroy");
	pwb_interface->pfun_get_url         = (fun_loWeibo_get_url)::GETPROCADDRESS( pwb_interface->dll_ , "loWeibo_get_url");
	//
	return ( pwb_interface->pfun_oauth_copy &&	pwb_interface->pfun_request_create &&	pwb_interface->pfun_request_setopt &&
		pwb_interface->pfun_request_start && pwb_interface->pfun_request_wait && pwb_interface->pfun_request_stop &&
		pwb_interface->pfun_request_destroy &&	pwb_interface->pfun_get_url );

#endif //(defined(WEIBO_STATICLIB)) || (defined(LIB_loadWEIBOSDK)) 

	return true;
}





static
void unload_wb_interface(t_wb_interface* pwb_interface)
{
#if (!defined(WEIBO_STATICLIB)) && (!defined(LIB_loadWEIBOSDK)) && (defined(WIN32) || defined(WINCE))
	if( pwb_interface->dll_ )
	{
		FreeLibrary(pwb_interface->dll_);
		pwb_interface->dll_ = 0;
	}
#endif
}


#if (defined(WIN32) || defined(WINCE))

HMODULE _ParserDll = NULL;

#endif //(defined(WIN32) || defined(WINCE))

static 
bool load_wb_parser()
{
//#if (!defined(WEIBO_STATICLIB)) && (!defined(LIB_loadWEIBOSDK)) && (defined(WIN32) || defined(WINCE))
//
//#ifdef WEIBO_VC90
//	_ParserDll = ::LoadLibrary(L"WbParser_VC90.dll");
//#elif defined(WEIBO_VC80)
//	_ParserDll = ::LoadLibrary(L"WbParser_VC80.dll");
//#endif
//
//	if( !_ParserDll ){
//		return false;
//	}
//#endif //
	return true;
}

static 
void unload_wb_parser(void)
{
//#if (defined(WIN32) || defined(WINCE))
//	if( _ParserDll ){
//		::FreeLibrary(_ParserDll);
//		_ParserDll = NULL;
//	}
//#endif //#if (defined(WIN32) || defined(WINCE))
}



struct t_wb_allstruct
{
	struct t_wb_oauth  oauth;
	struct t_wb_get_statuses_public_timeline   stat_pub_tl;
	struct t_wb_get_statuses_friends_timeline  stat_fri_tl;
	struct t_wb_get_statuses_user_timeline     stat_usr_tl;
	struct t_wb_get_statuses_mentions          stat_at_mention;
	struct t_wb_get_statuses_comments_timeline stat_com_tl;
	struct t_wb_get_statuses_comments_byme     stat_com_byme;
	struct t_wb_get_statuses_comments_list     stat_coms_list;
	struct t_wb_get_statuses_counts            stat_coms_counts;
	struct t_wb_get_statuses_unread            stat_unread;
	//
	struct t_wb_get_statuses_show              stat_show;
	struct t_wb_goto_user_status_id            usr_statid;
	struct t_wb_put_statuses_update            stat_update;
	struct t_wb_put_statuses_upload            stat_upload;
	struct t_wb_put_statuses_destroy           stat_destroy;
	struct t_wb_put_statuses_repost            stat_repost;
	struct t_wb_put_statuses_comment           stat_comment;
	struct t_wb_put_statuses_comment_destroy   stat_comment_destroy;
	struct t_wb_put_statuses_reply             stat_reply;
	//
	struct t_wb_get_users_show                 usr_show;
	struct t_wb_get_statuses_friends           usr_friends;
	struct t_wb_get_statuses_followers         usr_followers;
	//
	struct t_wb_get_direct_message             dirt_message;
	struct t_wb_get_direct_message_sent        dirt_message_sent;
	struct t_wb_put_direct_message_new         dirt_message_new;
	struct t_wb_put_direct_message_destroy     dirt_message_destroy;
	//
	struct t_wb_put_friendships_create         ships_create;
	struct t_wb_put_friendships_destroy        ships_destroy;
	struct t_wb_get_friendships_show           ships_show;
	//
	struct t_wb_get_friends_ids                friends_ids;
	struct t_wb_get_followers_ids              follower_ids;
	//
	struct t_wb_get_account_verify_credentials account_verify;
	struct t_wb_get_account_rate_limit_status  account_ratelimit;
	struct t_wb_put_account_end_session        account_endsession;
	struct t_wb_put_account_up_profileimage    account_up_image;
	struct t_wb_put_account_up_profile         account_uprofile;
	//
	struct t_wb_get_favorites                  favos_list;
	struct t_wb_put_favorites_create           favos_create;
	struct t_wb_put_favorites_destroy          favos_destroy;
	//
	struct t_wb_oauth_request_token            oauth_reqtoken;
	struct t_wb_authorize					   oauth_authorize;
	struct t_wb_oauth_access_token             oauth_accesstoken;
	
	// ����
	struct t_wb_emotions                       emotions;
	//
	struct t_wb_users_search                   user_search;
	struct t_wb_search					       search;
	struct t_wb_statuses_search                statuses_search;
	
	struct t_wb_provinces                      provinces;

	struct t_wb_cookie                         cookie;
};


static 
void test_weibo(WEIBORequest* wbRequest , int option , struct t_wb_allstruct* pall ,const struct t_wb_interface* pwb_interface,void *pUserdata = 0)
{
	struct t_wb_oauth* pdest_oauth = 0;
	void* p_twb = 0;
	char content[ 600 ];

	switch( option )
	{
	case WEIBO_OPTION(GETSTATUSES_PUBLIC_TIMELINE):
		{// ��ȡ���¸��µĹ���΢����Ϣ
			p_twb = &pall->stat_pub_tl;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// NOT NEED PARAM
		}
		break;
	case WEIBO_OPTION(GETSTATUSES_FRIENDS_TIMELINE):
		{// ��ȡ��ǰ�û�����ע�û�������΢����Ϣ
			p_twb = &pall->stat_fri_tl;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information 

			// since_id: ��΢����ϢID) ;
			// max_id: ��ѡ������΢����ϢID��;
			// count: ��ѡ����. ÿ�η��ص�����¼�������ܳ���200��Ĭ��20. 
			// page:��ѡ����. ���ؽ����ҳ��š�ע�⣺�з�ҳ���ơ������û���ע���󷢱��������ͨ����෵��1,000������΢����ҳ����, Ĭ��1 
		}
		break;
	case WEIBO_OPTION(GETSTATUSES_USE_TIMELINE):
		{// ��ȡ�û�������΢����Ϣ�б�,�˽ӿ�Ҳ�������������û������·���΢��
			p_twb = &pall->stat_usr_tl;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// id�� ��ѡ����. ����ָ���û�UID��΢���ǳ�������΢����Ϣ�� 
			// user_id�� ��ѡ����. �û�UID����Ҫ�����������û�UID��΢���ǳ�һ�������������ʱ���ر�����΢���ǳ�Ϊ���ֵ��º��û�Uid�������塣 
			// screen_name����ѡ����.΢���ǳƣ���Ҫ�����������û�UID��΢���ǳ�һ�������������ʱ�� 


			// since_id����ѡ������΢����ϢID��. ֻ����ID��since_id�󣨱�since_idʱ����ģ���΢����Ϣ���� 
			// max_id: ��ѡ������΢����ϢID��. ����ID������max_id��΢����Ϣ���ݡ� 
			// count: ��ѡ����. ÿ�η��ص�����¼������෵��200����Ĭ��20�� 
			// page�� ��ѡ����. ��ҳ���ء�ע�⣺��෵��200����ҳ���ݡ� 
		}
		break;
	case WEIBO_OPTION(GETSTATUSES_MENTIONS):
		{// ��ȡ@��ǰ�û���΢���б�
			p_twb = &pall->stat_at_mention;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// since_id. ��ѡ����. ����ID����ֵsince_id�󣨱�since_idʱ����ģ����ᵽ�� 
			// max_id. ��ѡ����. ����ID������max_id(ʱ�䲻����max_id)���ᵽ�� 
			// count. ��ѡ����. ÿ�η��ص�����¼������ҳ���С����������200��Ĭ��Ϊ20�� 
			// page. ��ѡ����. ���ؽ����ҳ��š�ע�⣺�з�ҳ���ơ� 
		}
		break;
	case WEIBO_OPTION(GETSTATUSES_COMMENTS_TIMELINE):
		{// ��ȡ��ǰ�û����ͼ��յ��������б�
			p_twb = &pall->stat_com_tl;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// since_id: ��ѡ����������ID��. ֻ����ID��since_id�󣨱�since_idʱ����ģ������ۡ� 
			// max_id: ��ѡ����������ID��. ����ID������max_id�����ۡ� 
			// count: ��ѡ����. ÿ�η��ص�����¼����������200��Ĭ��20�� 
			// page: ��ѡ����. ���ؽ����ҳ��š�ע�⣺�з�ҳ���ơ�
		}
		break;
	case WEIBO_OPTION(GETSTATUSES_COMMENTS_BYME):
		{// ��ȡ��ǰ�û�����������
			p_twb = &pall->stat_com_byme;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			//since_id����ѡ����������ID��. ֻ���ر�since_id�󣨱�since_idʱ����ģ������� 
			//max_id: ��ѡ����������ID��. ����ID������max_id�����ۡ� 
			//count: ��ѡ����. ÿ�η��ص�����¼������෵��200����Ĭ��Ϊ20�� 
			//page�� ��ѡ����. ��ҳ���ء�ע�⣺��෵��200����ҳ���ݡ�

		}
		break;
	case WEIBO_OPTION(GETSTATUSES_COMMENTS_LIST):
		{// ��ȡָ��΢���������б�
			p_twb = &pall->stat_coms_list;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			//id. ��ѡ����. ����ָ����΢��ID 
			//count. ��ѡ����. ÿ�η��ص�����¼������ҳ���С����������200��Ĭ��Ϊ20�� 
			//page. ��ѡ����. ���ؽ����ҳ��š�ע�⣺�з�ҳ���ơ�

			printf("\n\nPlease enter the ID :");
			gets(content);
			strcpy( pall->stat_coms_list.wbId_,content );

		}
		break;
	case WEIBO_OPTION(GETSTATUSES_COMMENTS_COUNTS):
		{// ����ͳ��΢����(��������ת���� , һ�����100��)
			p_twb = &pall->stat_coms_counts;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			//ids. �������. ΢��ID���б�
			for( int i = 0;  i <  BATCH_WBIDS_COUNTS ; i ++ )
			{
				printf("\n\nPlease Enter GETSTATUSES_COMMENTS_LIST id list:[enter \"q\" to quit the id get]:");
				gets(content);
				if( strcmp(content,"q") == 0 ) {
					break;
				}
				strcpy( pall->stat_coms_counts.wbIds_[i],content );
			}
		}
		break;
	case WEIBO_OPTION(GETSTATUSES_UNREAD):
		{// ��ȡ��ǰ�û�δ����Ϣ��
			p_twb = &pall->stat_unread;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// NOT NEED PARAM
		}
		break;

		//΢������
	case WEIBO_OPTION(GETSTATUSES_SHOW):
		{// ����ID��ȡ����΢����Ϣ����
			p_twb = &pall->stat_show;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// id. �������(΢����ϢID)��Ҫ��ȡ�ѷ����΢��ID,��ID�����ڷ��ؿ� 
			printf("\n\nPlease enter the wb status ID :");
			gets(content);
			strcpy( pall->stat_show.wbId_,content );
			
		}
		break;
	case WEIBO_OPTION(GOTOSTATUSES_ID):
		{// ����΢��ID���û�ID��ת������΢��ҳ��
			p_twb = &pall->usr_statid;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// id. �������(΢����ϢID)��Ҫ��ȡ�ѷ����΢��ID,��ID�����ڷ��ؿ� 
			printf("\n\nPlease enter the ID :");
			gets(content);
			strcpy( pall->usr_statid.wbId_,content );


			// �û�ID
			printf("\n\nPlease enter the user ID :");
			gets(content);
			strcpy( pall->usr_statid.uid_,content );
		}
		break;
	case WEIBO_OPTION(PUTSTATUSES_UPDATE):
		{//����һ��΢����Ϣ
			p_twb = &pall->stat_update;

			Wb_init_wb_struct( option , p_twb);

			// to fill other information

			// status. ��������� Ҫ���µ�΢����Ϣ��������URLEncode,��Ϣ���ݲ�����140������,Ϊ�շ���400���� 
			// in_reply_to_status_id. ��ѡ������@ ��Ҫ�ظ���΢����ϢID, �������ֻ����΢�������� @username ��ͷ�������塣�������Ƴ����� 
			// lat. ��ѡ������γ�ȣ�����ǰ΢�����ڵĵ���λ�ã���Ч��Χ -90.0��+90.0, +��ʾ��γ��ֻ���û�������geo_enabled=trueʱ�����λ�ò���Ч��(����������ĺ��������߿���) 
			// long. ��ѡ���������ȡ���Ч��Χ-180.0��+180.0, +��ʾ������(����������ĺ��������߿���) 

			printf("\n\nPlease enter Content[ <= 140 words]:");
			gets(content);
			//
			strcpy( pall->stat_update.szwbInfo_ , content );
		}
		break;
	case WEIBO_OPTION(PUTSTATUSES_UPLOAD):
		{// �ϴ�ͼƬ������һ��΢����Ϣ
			p_twb = &pall->stat_upload;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// status. ��������� Ҫ���µ�΢����Ϣ��������URLEncode,��Ϣ���ݲ�����140�����֡� 
			// pic. �����������֧��JPEG,GIF,PNGͼƬ,Ϊ�շ���400����Ŀǰ�ϴ�ͼƬ��С����Ϊ<1M�� 
			// lat. ��ѡ������γ�ȣ�����ǰ΢�����ڵĵ���λ�ã���Ч��Χ -90.0��+90.0, +��ʾ��γ��ֻ���û�������geo_enabled=trueʱ�����λ�ò���Ч��(�����ֶΣ��ݲ�֧��) 
			// long. ��ѡ���������ȡ���Ч��Χ-180.0��+180.0, +��ʾ������(�����ֶΣ��ݲ�֧��) 

			printf("\n\nPlease enter image file:");			
			gets(content);
			strcpy( pall->stat_upload.szImagePath_ ,content );

			printf("\n\nPlease enter Content[ <= 140 words]:");			
			gets(content);
			strcpy( pall->stat_upload.szwbInfo_ , content);
		}
		break;
	case WEIBO_OPTION(PUTSTATUSES_DESTROY):
		{// ɾ��һ��΢����Ϣ
			p_twb = &pall->stat_destroy;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			//id. �������. Ҫɾ����΢��ID. 
			printf("\n\nPlease enter the need destroy status ID :");
			gets(content);
			strcpy( pall->stat_destroy.wbId_,content );
		}
		break;
	case WEIBO_OPTION(PUTSTATUSES_REPOST):
		{// ת��һ��΢����Ϣ���ɼ����ۣ�
			p_twb = &pall->stat_repost;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// id ��������� ת����΢��ID 
			printf("\n\nPlease Repost status ID :");
			gets(content);
			strcpy( pall->stat_repost.wbId_,content );

			// status. ��ѡ������ ��ӵ�ת����Ϣ����Ϣ���ݲ�����140�����֡��粻�����Զ��������ơ�ת�� @author: ԭ���ݡ����֡�
			printf("\n\nPlease Repost status comment text :");
			gets(content);
			strcpy( pall->stat_repost.szwbInfo_,content );

		}
		break;
	case WEIBO_OPTION(PUTSTATUSES_COMMENT):
		{// ��һ��΢����Ϣ��������
			p_twb = &pall->stat_comment;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// id. �������. Ҫɾ��������ID. 
			printf("\n\nPlease enter will status id :");
			gets(content);
			strcpy( pall->stat_comment.wbId_,content );

			// cid ѡ������� Ҫ���۵�����id ��û�л�Ƿ���Ϊ��΢�������� 
			printf("\n\nPlease enter replay comment id,it can be igno:");
			gets(content);
			strcpy( pall->stat_comment.wbCId_,content );

			// comment. ��������� �������ݡ���Ϣ���ݲ�����140������
			printf("\n\nPlease enter replay text:");
			gets(content);
			strcpy( pall->stat_comment.szwbInfo_,content );
		}
		break;
	case WEIBO_OPTION(PUTSTATUSES_COMMENT_DESTROY):
		{// ɾ����ǰ�û���΢��������Ϣ
			p_twb = &pall->stat_comment_destroy;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// wbCId_. �������. Ҫɾ��������ID. 
			printf("\n\nPlease enter comment id :");
			gets(content);
			strcpy( pall->stat_comment_destroy.wbCId_,content );
		}
		break;
	case WEIBO_OPTION(PUTSTATUSES_REPLY):
		{// �ظ�΢��������Ϣ
			p_twb = &pall->stat_reply;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information


			// id. �������. Ҫɾ��������ID. 
			printf("\n\nPlease enter will replay status id :");
			gets(content);
			strcpy( pall->stat_reply.wbId_,content );

			// cid ��������� Ҫ���۵�����id ��û�л�Ƿ���Ϊ��΢�������� 
			printf("\n\nPlease enter will replay comment id :");
			gets(content);
			strcpy( pall->stat_reply.wbCId_,content );

			// comment. ��������� �������ݡ���Ϣ���ݲ�����140������
			printf("\n\nPlease enter will replay text:");
			gets(content);
			strcpy( pall->stat_reply.szwbInfo_,content );

		}
		break;

	case WEIBO_OPTION(GETUSER_INFO):
		{// �����û�ID��ȡ�û����ϣ���Ȩ�û���
			p_twb = &pall->usr_show;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// id. �û�UID��΢���ǳơ� 
			// user_id. ָ���û�UID,��Ҫ�����������û�UID��΢���ǳ�һ�������������ʱ���ر������û��˺�Ϊ���ֵ��º��û�Uid�������塣 
			// screen_name. ָ��΢���ǳƣ���Ҫ�����������û�UID��΢���ǳ�һ�������������ʱ�� 

			printf("\n\nPlease to use id:");			
			gets(content);

			strcpy( pall->usr_show.wbuid_.uid_ ,content );
			pall->usr_show.wbuid_.uidtype_ = t_wb_uid::IDType_id;
		}
		break;
	case WEIBO_OPTION(GETFRINDS_LIST):
		{// ��ȡ��ǰ�û���ע�����б�����һ��΢����Ϣ
			p_twb = &pall->usr_friends;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// id. ѡ�����. Ҫ��ȡ�� UID��΢���ǳ� 
			// user_id. ѡ�����. Ҫ��ȡ��UID 
			// screen_name. ѡ�����. Ҫ��ȡ��΢���ǳ� 
			// cursor. ѡ�����. ��ҳֻ�ܰ���100����ע�б�Ϊ�˻�ȡ������cursorĬ�ϴ�-1��ʼ��ͨ�����ӻ����cursor����ȡ����, ���û����һҳ����next_cursor����0 
			// count. ��ѡ����. ÿ�η��ص�����¼������ҳ���С����������200,Ĭ�Ϸ���20��

			printf("\n\nPlease to use id:");			
			gets(content);

			strcpy( pall->usr_friends.wbuid_.uid_ ,content );
			pall->usr_friends.wbuid_.uidtype_ = t_wb_uid::IDType_id;
		}
		break;
	case WEIBO_OPTION(GETFOLLOWERS_LIST):
		{// ��ȡ��ǰ�û���˿�б�����һ��΢����Ϣ
			p_twb = &pall->usr_followers;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// id. ѡ�����. Ҫ��ȡ�� UID��΢���ǳ� 
			// user_id. ѡ�����. Ҫ��ȡ��UID 
			// screen_name. ѡ�����. Ҫ��ȡ��΢���ǳ� 
			// cursor. ѡ�����. ��ҳֻ�ܰ���100����ע�б�Ϊ�˻�ȡ������cursorĬ�ϴ�-1��ʼ��ͨ�����ӻ����cursor����ȡ����, ���û����һҳ����next_cursor����0 
			// count. ��ѡ����. ÿ�η��ص�����¼������ҳ���С����������200,Ĭ�Ϸ���20��

			printf("\n\nPlease to use id:");			
			gets(content);

			strcpy( pall->usr_followers.wbuid_.uid_ ,content );
			pall->usr_followers.wbuid_.uidtype_ = t_wb_uid::IDType_id;
		}
		break;
		//˽��
	case WEIBO_OPTION(GETDIRECTMSG):
		{// ��ȡ��ǰ�û�����˽���б� 
			p_twb = &pall->dirt_message;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// since_id. ��ѡ����. ����ID����ֵsince_id�󣨱�since_idʱ����ģ���˽�š� 
			// max_id. ��ѡ����. ����ID������max_id(ʱ�䲻����max_id)��˽�š� 
			// count. ��ѡ����. ÿ�η��ص�����¼������ҳ���С����������200�� 
			// page. ��ѡ����. ���ؽ����ҳ��š�ע�⣺�з�ҳ���ơ�
		}
		break;
	case WEIBO_OPTION(GETDIRESTMSG_SENT):
		{// ��ȡ��ǰ�û����͵�����˽���б�
			p_twb = &pall->dirt_message_sent;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// since_id. ��ѡ����. ����ID����ֵsince_id�󣨱�since_idʱ����ģ���˽�š� 
			// max_id. ��ѡ����. ����ID������max_id(ʱ�䲻����max_id)��˽�š� 
			// count. ��ѡ����. ÿ�η��ص�����¼������ҳ���С����������200�� 
			// page. ��ѡ����. ���ؽ����ҳ��š�ע�⣺�з�ҳ���ơ�
		}
		break;
	case WEIBO_OPTION(PUTDIRECTMSG_NEW):
		{// ����һ��˽�� 
			p_twb = &pall->dirt_message_new;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			//id: �������. UID��΢���ǳ�. Ϊ��֧�����ֵ�΢���ǳƣ���ѡ��д����2������screen_name��user_id: 
			//screen_name: ΢���ǳ� 
			//user_id: ����UID 
			//text: �������. Ҫ��������Ϣ���ݣ���Ҫ��URLEncode���ı���С����С��300������. 

			printf("\n\nPlease enter text:[<= 300 words]");			
			gets(content);

			strcpy( pall->dirt_message_new.szText_ ,content );


			printf("\n\nPlease to use id:");			
			gets(content);

			strcpy( pall->dirt_message_new.wbuid_.uid_ ,content );
			pall->dirt_message_new.wbuid_.uidtype_ = t_wb_uid::IDType_id;
		}
		break;
	case WEIBO_OPTION(PUTDIRECTMSG_DESTROY):
		{// ɾ��һ��˽��
			p_twb = &pall->dirt_message_destroy;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// id. ���������Ҫɾ����˽������ID. 
			printf("\n\nPlease to private message id:");
			gets(content);

			//
			strcpy( pall->dirt_message_destroy.wbId_ ,content );
		}
		break;
		//��ע
	case WEIBO_OPTION(PUTFRIENDSHIPS_CREATE):
		{// ��עĳ�û� 
			p_twb = &pall->ships_create;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			//����Ĳ�������������һ��: 
			//    id. �������. Ҫ��ע���û�UID��΢���ǳ� 
			//    user_id. �������. Ҫ��ע���û�UID,��Ҫ�����������û�UID��΢���ǳ�һ�������������ʱ�� 
			//    screen_name.�������. Ҫ��ע��΢���ǳ�,��Ҫ�����������û�UID��΢���ǳ�һ�������������ʱ�� 

			//follow. ��ѡ�������ݲ�֧�֡�

		}
		break;
	case WEIBO_OPTION(PUTFRIENDSHIPS_DESTROY):
		{// ȡ����ע
			p_twb = &pall->ships_destroy;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			//����Ĳ�������������һ��: 
			//    id. �������. Ҫ��ע���û�UID��΢���ǳ� 
			//    user_id. �������. Ҫ��ע���û�UID,��Ҫ�����������û�UID��΢���ǳ�һ�������������ʱ�� 
			//    screen_name.�������. Ҫ��ע��΢���ǳ�,��Ҫ�����������û�UID��΢���ǳ�һ�������������ʱ�� 

		}
		break;
	case WEIBO_OPTION(GETFRIENDSHIPS_EXISTS):
		{// ��ȡ�����û���ϵ����ϸ���
			p_twb = &pall->ships_show;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			//���²����ɲ���д���粻���ȡ��ǰ�û� 
			//   source_id. Դ�û�UID 
			//   source_screen_name. Դ΢���ǳ� 

			//�����������ѡ��һ��: 
			//   target_id. Ҫ�жϵ�Ŀ���û�UID 
			//   target_screen_name. Ҫ�жϵ�Ŀ��΢���ǳ� 
			printf("\n\nPlease enter target id:");			
			gets(content);

			strcpy(pall->ships_show.wbuid_target_.uid_ , content);
			pall->ships_show.wbuid_target_.uidtype_ = t_wb_uid::IDType_userid;
		}
		break;
		//Social Graph
	case WEIBO_OPTION(GETFRIEND_IDS):
		{// ��ȡ�û���ע����uid�б�
			p_twb = &pall->friends_ids;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// id. ѡ�����. Ҫ��ȡ���ѵ�UID��΢���ǳ� 
			// user_id. ѡ�����. Ҫ��ȡ��UID 
			// screen_name. ѡ�����. Ҫ��ȡ��΢���ǳ� 
			// cursor. ѡ�����. ��ҳֻ�ܰ���5000��id��Ϊ�˻�ȡ������cursorĬ�ϴ�-1��ʼ��ͨ�����ӻ����cursor����ȡ����Ĺ�ע�б� 
			// count. ��ѡ����. ÿ�η��ص�����¼������ҳ���С����������5000��Ĭ�Ϸ���500�� 

			printf("\n\nPlease enter use id:");			
			gets(content);

			strcpy(pall->friends_ids.wbuid_.uid_ , content);
			pall->friends_ids.wbuid_.uidtype_ = t_wb_uid::IDType_id;

		}
		break;
	case WEIBO_OPTION(GETFOLLOWER_IDS):
		{// ��ȡ�û���˿����uid�б� 
			p_twb = &pall->follower_ids;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// id. ѡ�����. Ҫ��ȡ���ѵ�UID��΢���ǳ� 
			// user_id. ѡ�����. Ҫ��ȡ��UID 
			// screen_name. ѡ�����. Ҫ��ȡ��΢���ǳ� 
			// cursor. ѡ�����. ��ҳֻ�ܰ���5000��id��Ϊ�˻�ȡ������cursorĬ�ϴ�-1��ʼ��ͨ�����ӻ����cursor����ȡ����Ĺ�ע�б� 
			// count. ��ѡ����. ÿ�η��ص�����¼������ҳ���С����������5000��Ĭ�Ϸ���500�� 

			printf("\n\nPlease enter use id:");			
			gets(content);

			strcpy(pall->follower_ids.wbuid_.uid_ , content);
			pall->follower_ids.wbuid_.uidtype_ = t_wb_uid::IDType_id;
		}
		break;
		//�˺�
	case WEIBO_OPTION(GETACCOUNT_VERIFY):
		{// ��֤��ǰ�û�����Ƿ�Ϸ�
			p_twb = &pall->account_verify;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// NOT NEED PARAM
		}
		break;
	case WEIBO_OPTION(GETACCOUNT_RATELIMIT):
		{// ��ȡ��ǰ�û�API����Ƶ������
			p_twb = &pall->account_ratelimit;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// NOT NEED PARAM
		}
		break;
	case WEIBO_OPTION(PUTACCOUNT_QUITSESSION):
		{// ��ǰ�û��˳���¼
			p_twb = &pall->account_endsession;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// NOT NEED PARAM

		}
		break;
	case WEIBO_OPTION(PUTACCOUNT_UPDATE_PROFILEIMAGE):
		{// ����ͷ��
			p_twb = &pall->account_up_image;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			printf("\n\nPlease enter image file[ size < 1M ]:");			
			gets(content);
			strcpy( pall->account_up_image.szImagePath_ ,content );
		}
		break;
	case WEIBO_OPTION(PUTACCOUNT_UPDATE_PROFILE):
		{// ��������
			p_twb = &pall->account_uprofile;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			//������һ�²����е�һ������������ֵΪ�ַ���. ��һ�������ƣ����������ĸ�����������. 
			//  name. �ǳƣ���ѡ����.������20������ 
			//  gender �Ա𣬿�ѡ����. m,�У�f,Ů�� 
			//  province ��ѡ����. �ο�ʡ�ݳ��б���� 
			//  city ��ѡ����. �ο�ʡ�ݳ��б����,1000Ϊ���� 
			//  description. ��ѡ����. ������160������.

		}
		break;
	case WEIBO_OPTION(PUTACCOUNT_REGISTER):
		{// ��֧��


			//������һ�²����е�һ������������ֵΪ�ַ���. ��һ�������ƣ����������ĸ�����������. 
			// nick. �ǳƣ��������.������20������ 
			// gender �Ա𣬱������. m,�У�f,Ů�� 
			// password ���� �������. 
			// email ע������ �������. 
			// province ��ѡ����. �ο�ʡ�ݳ��б���� 
			// city ��ѡ����. �ο�ʡ�ݳ��б����,1000Ϊ���� 
			// ip ���������ע���û��û���ǰ��ʵ��IP��
		}
		break;

		// �ղ�
	case WEIBO_OPTION(GETFAVORITES):
		{// ��ȡ��ǰ�û����ղ��б� 
			p_twb = &pall->favos_list;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// page�� ��ѡ����. ���ؽ����ҳ��š�ע�⣺�з�ҳ���ơ�
		}
		break;
	case WEIBO_OPTION(PUTFAVORITES_CREATE):
		{// ����ղ� 
			p_twb = &pall->favos_create;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// id ��������� Ҫ�ղص�΢��id 
			printf("\n\nPlease enter status id:");			
			gets(content);


			//id. �������. Ҫɾ�����ղ�΢����ϢID.
			strcpy( pall->favos_create.wbId_,content);
		}
		break;
	case WEIBO_OPTION(PUTFAVORITES_DESTROY):
		{// ɾ����ǰ�û��ղص�΢����Ϣ 
			p_twb = &pall->favos_destroy;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information
			printf("\n\nPlease enter status id:");			
			gets(content);


			//id. �������. Ҫɾ�����ղ�΢����ϢID.
			strcpy( pall->favos_destroy.wbId_,content);
		}
		break;
		// ����
	case WEIBO_OPTION(GET_EMOTIONS): // number = 44
		{
			p_twb = &pall->emotions;

			Wb_init_wb_struct( option , p_twb);
		}
		break;
	case WEIBO_OPTION(GET_USERS_SEARCH): // number = 45
		{
			p_twb = &pall->user_search;

			Wb_init_wb_struct( option , p_twb);

			printf("\n\nPlease enter search key:");			
			gets(content);

			strcpy( pall->user_search.q_ , content);

			pall->user_search.count_ = -1;
			pall->user_search.page_ = -1;
			pall->user_search.sort_ = -1;

			pall->user_search.city_ = -1;
			pall->user_search.province_ = -1;
		}
		break;
	case WEIBO_OPTION(GET_WB_SEARCH): // number = 46
		{
			p_twb = &pall->search;

			Wb_init_wb_struct( option , p_twb);

			printf("\n\nPlease enter search key:");			
			gets(content);

			strcpy( pall->search.q_ , content);

			pall->search.count_ = -1;
			pall->search.page_ = -1;
			pall->search.sort_ = -1;
		}
		break;
	case WEIBO_OPTION(GET_STATUSES_SEARCH): // number = 47 
		{
			p_twb = &pall->statuses_search;

			Wb_init_wb_struct( option , p_twb);

			printf("\n\nPlease enter search key:");			
			gets(content);

			strcpy( pall->statuses_search.q_ , content);

			pall->statuses_search.count_ = -1;
			pall->statuses_search.page_ = -1;
			pall->statuses_search.sort_ = -1;

			pall->statuses_search.city_ = -1;
			pall->statuses_search.province_ = -1;
		}
		break;
	case WEIBO_OPTION(GET_PROVINCES): // number = 48
		{
			p_twb = &pall->provinces;

			Wb_init_wb_struct( option , p_twb);
		}
		break;

	case WEIBO_OPTION(COOKIE):
		{// cookie��ʽ
			p_twb = &pall->cookie;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			printf("\n\nPlease enter user id:");			
			gets(content);
			strcpy( pall->cookie.usrid_ ,content );

			printf("\n\nPlease enter user password:");			
			gets(content);
			strcpy( pall->cookie.usrpwd_ ,content );
		}
		break;
	}

	if( !p_twb )
		return ;

	// oauth copy 
	pwb_interface->pfun_oauth_copy( &((struct t_wb_base*)p_twb)->wbauth_ , &pall->oauth );
	pwb_interface->pfun_request_setopt(wbRequest  , WEIBO_OPTION(REQ_SENDDATA) , option , p_twb );
	pwb_interface->pfun_request_setopt(wbRequest  , WEIBO_OPTION(REQ_USERDATA) , pUserdata );
	pwb_interface->pfun_request_start(wbRequest   , false);
	pwb_interface->pfun_request_stop(wbRequest);
	SYSTEM("pause");
}

int TEST_MAIN(int argc,char** argv)
{
	int cookie = 0;
	char enter[128] = {0};
	struct t_wb_allstruct  twball;
	struct t_wb_interface  wb_interface;
	memset(&twball , 0 , sizeof( struct t_wb_allstruct ) );
	
	//struct t_wb_cookie* ptest1 = (struct t_wb_cookie*)malloc( 1024 );

	// ��̬��ʽ
	if( !load_wb_interface(&wb_interface) )
	{
		unload_wb_interface(&wb_interface);
		return -1;
	}

#ifdef _USE_WEIBOPARSER
	if( !load_wb_parser() )
	{
		unload_wb_parser();
//		assert(0);
	}
#endif //_USE_WEIBOPARSER


	//	struct t_wb_cookie* ptest5 = (struct t_wb_cookie*)malloc( 1024 );
	//
	printf("Please enter request method[0 = oauth/1 = cookie]:");
#if defined(WINCE)
	cookie = 1;
#else
	gets( enter );
	cookie = atoi(enter);
#endif
	WEIBORequest* wbRequest = wb_interface.pfun_request_create();

	// cookie 
	wb_interface.pfun_request_setopt(wbRequest , WEIBO_OPTION(REQ_COOKIE) , cookie);

	// set app key
	printf("Please enter APP KEY:");
	gets( enter );
	wb_interface.pfun_request_setopt(wbRequest , WEIBO_OPTION(REQ_APPKEY) , enter);

	// set app secret
	printf("Please enter APP SECRET:");
	gets( enter );
	wb_interface.pfun_request_setopt(wbRequest , WEIBO_OPTION(REQ_SECRETKEY) , enter);

	// set 
	printf("Please enter result data format(josn=0/xml=1):");
#if defined(WINCE)
	wb_interface.pfun_request_setopt(wbRequest , WEIBO_OPTION(REQ_RESULT_DATAFORMAT) ,0);
#else
	gets( enter );
	wb_interface.pfun_request_setopt(wbRequest , WEIBO_OPTION(REQ_RESULT_DATAFORMAT) ,atoi(enter));
#endif

	Wb_init_wb_struct( WEIBO_OPTION(BASE) , &twball.oauth);

	//		struct t_wb_cookie* ptest7 = (struct t_wb_cookie*)malloc( 1024 );
	if( 0 == cookie )
	{
		Wb_init_wb_struct( WEIBO_OPTION(OAUTH_REQUEST_TOKEN) , &twball.oauth_reqtoken );
		Wb_init_wb_struct( WEIBO_OPTION(OAUTH_ACCESS_TOKEN)  , &twball.oauth_accesstoken);

		// 1 oauth request token
		wb_interface.pfun_request_setopt(wbRequest , WEIBO_OPTION(REQ_SENDDATA) ,WEIBO_OPTION(OAUTH_REQUEST_TOKEN), &twball.oauth_reqtoken );
		wb_interface.pfun_request_setopt(wbRequest , WEIBO_OPTION(REQ_USERDATA) , &twball.oauth_accesstoken );
		wb_interface.pfun_request_start(wbRequest , false);

		//
		wb_interface.pfun_request_stop(wbRequest);

		// 2 go to run IE  to oauth authrioze
		//
		// add by welbon,2010-09-10
		printf("Please enter how to get verify code[0 = website,1 = direct access] : ");
#if defined(WINCE)
		strcpy(enter ,"1");
#else
		gets(enter);
#endif
		if( strcmp(enter,"1") == 0 )
		{
			Wb_init_wb_struct( WEIBO_OPTION(OAUTH_AUTHORIZE), &twball.oauth_authorize);
			strcpy(twball.oauth_authorize.wbauth_.oauth_token_,twball.oauth_accesstoken.wbauth_.oauth_token_ );

			// �����û���(����)
			printf("Please enter user id : ");
			gets(enter);
			strcpy(twball.oauth_authorize.usrid_,enter);

			// ��������(����)
			printf("Please enter password : ");
			gets(enter);
			strcpy(twball.oauth_authorize.usrpwd_,enter);

			//�ڶ�����ֱ������ verify code
			wb_interface.pfun_request_setopt(wbRequest , WEIBO_OPTION(REQ_SENDDATA) , WEIBO_OPTION(OAUTH_AUTHORIZE) , &twball.oauth_authorize );
			wb_interface.pfun_request_setopt(wbRequest , WEIBO_OPTION(REQ_USERDATA) , &twball.oauth_accesstoken.wbauth_ );
			wb_interface.pfun_request_start(wbRequest , false);
		}
		else{

#if !defined(WINCE) && defined(WIN32)
			char open[255] = {0};
			sprintf(open,"http://api.t.sina.com.cn/oauth/authorize?oauth_token=%s",twball.oauth_accesstoken.wbauth_.oauth_token_);
			ShellExecuteA( NULL,"open",open,NULL,NULL,SW_SHOWNORMAL );
#endif //((defined(WIN32)||(defined(_WIN32))

			// get website vertify code
			printf("Please enter website's vertifycode:");
			gets( enter );
			strcpy(twball.oauth_accesstoken.wbauth_.oauth_verifier_,enter);
		}

		//3.���� access token
		wb_interface.pfun_request_setopt(wbRequest , WEIBO_OPTION(REQ_SENDDATA) , WEIBO_OPTION(OAUTH_ACCESS_TOKEN) , &twball.oauth_accesstoken );
		wb_interface.pfun_request_setopt(wbRequest , WEIBO_OPTION(REQ_USERDATA) , &twball.oauth );
		wb_interface.pfun_request_start(wbRequest , false);

		//
		wb_interface.pfun_request_stop(wbRequest);

		//
		SYSTEM("pause");
	}
	else
	{
	//	struct t_wb_cookie* ptest2 = (struct t_wb_cookie*)malloc( 1024 );
		Wb_init_wb_struct( WEIBO_OPTION(COOKIE) , &twball.cookie );

		printf("Please enter user id:");
		gets( enter );
		strcpy(twball.cookie.usrid_ , enter);

		printf("Please enter user password:");
		gets( enter );
		strcpy(twball.cookie.usrpwd_ , enter);

		wb_interface.pfun_request_setopt(wbRequest , WEIBO_OPTION(REQ_SENDDATA) , WEIBO_OPTION(COOKIE) , &twball.cookie );
		wb_interface.pfun_request_setopt(wbRequest , WEIBO_OPTION(REQ_USERDATA) , &twball.oauth );
		wb_interface.pfun_request_start(wbRequest , false);

		wb_interface.pfun_request_stop(wbRequest);
		SYSTEM("pause");
	}

	do
	{
		printf("\n\nPlease enter TEST number[exit:'q']:");
		gets(enter);

		// "q" exit
		if( strcmp(enter ,"q") == 0 ) 
			break;


		void *pUsrData = NULL;

		//  ��������΢���ӿ�
		test_weibo( wbRequest , atoi(enter) , &twball ,&wb_interface,pUsrData );

		//
		//memset(&twball.cookie,0,sizeof(twball.cookie));

	}while(1);

	//
	wb_interface.pfun_request_destroy(wbRequest);

	// �ͷŽӿ�
	unload_wb_interface(&wb_interface);

	unload_wb_parser();

	return 0;
}

