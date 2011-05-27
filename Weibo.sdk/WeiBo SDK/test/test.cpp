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

#include "option.h"

#define LIB_loadWEIBOSDK

#pragma warning(disable:4996)

#if defined(WIN32) || defined(WINCE)// WIN
//
#	if (defined(LIB_loadWEIBOSDK)) || (defined(WEIBO_STATICLIB))

#if defined(WEIBO_VC80)
#		pragma comment(lib ,"WeiBoU_VC80.lib")
#		pragma comment(lib ,"WBParseWrapper_VC80.lib")
#elif defined(WEIBO_VC90)
#		pragma comment(lib ,"WeiBoU_VC90.lib")
#		pragma comment(lib ,"WBParseWrapper_VC90.lib")
#endif

#	else
//	//动态加载
#	endif //LIB_loadWEIBOSDK || WEIBO_STATICLIB
//	
#	if defined(WINCE) // WINCE系统
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

/** 创建一个请求，每个事件都需要 创建一个请求（一定要通过 weibo_destroy_request（） 进行消毁 */
typedef WEIBORequest* (*fun_loWeibo_request_create)(void);

/** 进行一些参数操作填写，然后执行(回调函数的设置...) */
typedef  int (*fun_loWeibo_request_setopt)(WEIBORequest* request , WEIBOReqoption option , ...);

/** 执行,等待回调数据的执行，其执行结果，会通过 weibo_request_setopt（）设置的方法，告之开发者
*
* @param bAsyn = false 同步执行（线程阻塞 )
*              = true  表示异步
*/
typedef  int (*fun_loWeibo_request_start)(WEIBORequest* request , bool bAsyn);

/** 等待执行（与异步 loWeibo_request_start 执行可以结合 ， 达到同步作用 ）*/
typedef  void (*fun_loWeibo_request_wait)(WEIBORequest* request);

/** 停止 */
typedef  int (*fun_loWeibo_request_stop)(WEIBORequest* request);

/** 消毁这个请求 */
typedef  int (*fun_loWeibo_request_destroy)(WEIBORequest* request);

/** 生成URL信息 */
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

	// 动态方式加载 
#if (!defined(WEIBO_STATICLIB)) && (!defined(LIB_loadWEIBOSDK)) &&( (defined(WIN32)) || (defined(WINCE)))
	HMODULE dll_;
#endif
};


static
bool load_wb_interface(t_wb_interface* pwb_interface)
{
	memset(pwb_interface , 0 , sizeof(struct t_wb_interface) );

  
	// 若没有定义静态加载标志，或者没有定义WIN32和WINCE
#if (defined(WEIBO_STATICLIB)) || (defined(LIB_loadWEIBOSDK)) || (!defined(WIN32) && !defined(WINCE))

	pwb_interface->pfun_oauth_copy      = loWeibo_oauth_copy;
	pwb_interface->pfun_request_create  = loWeibo_request_create;
	pwb_interface->pfun_request_setopt  = loWeibo_request_setopt;
	pwb_interface->pfun_request_start   = loWeibo_request_start;
	pwb_interface->pfun_request_wait    = loWeibo_request_wait;
	pwb_interface->pfun_request_stop    = loWeibo_request_stop;
	pwb_interface->pfun_request_destroy = loWeibo_request_destroy;
	pwb_interface->pfun_get_url         = loWeibo_get_url;

#else // 动态加载

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
	struct t_wb_put_statuses_upload_pic        stat_upload_pic;
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
	struct t_wb_get_direct_message_with        dirt_message_with;
	//
	struct t_wb_put_friendships_create         ships_create;
	struct t_wb_put_friendships_create_batch   ships_create_batch;
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
	struct t_wb_put_account_register		   account_register;
	//
	struct t_wb_get_favorites                  favos_list;
	struct t_wb_put_favorites_create           favos_create;
	struct t_wb_put_favorites_destroy          favos_destroy;
	//
	struct t_wb_oauth_request_token            oauth_reqtoken;
	struct t_wb_authorize					   oauth_authorize;
	struct t_wb_oauth_access_token             oauth_accesstoken;
	
	// 表情
	struct t_wb_emotions                       emotions;
	//
	struct t_wb_users_search                   user_search;
	struct t_wb_search					       search;
	struct t_wb_statuses_search                statuses_search;
	
	struct t_wb_provinces                      provinces;

	struct t_wb_cookie                         cookie;
	struct t_wb_updateTGT					   updateTGT;

	//
	struct t_wb_users_hot					   users_hot;
	struct t_wb_users_remark				   users_remark;
	struct t_wb_users_suggestions			   users_suggestions;

	// 话题接口
	struct t_wb_trends							trends;
	struct t_wb_trends_statuses					trend_statuses;
	struct t_wb_trends_follow					trend_follow;
	struct t_wb_trends_destroy					trend_destroy;
	struct t_wb_trends_hourly					trend_hourly;
	struct t_wb_trends_daily					trend_daily;
	struct t_wb_trends_weekly					trend_weekly;

	// 黑名单接口
	struct t_wb_blocks_create					block_create;
	struct t_wb_blocks_destroy					block_destroy;
	struct t_wb_blocks_exist					block_exist;
	struct t_wb_blocks_blocking					block_bloking;
	struct t_wb_blocks_blocking_ids				block_ids;

	// 用户标签接口
	struct t_wb_tags							tags;
	struct t_wb_tags_create						tags_create;
	struct t_wb_tags_suggestions				tags_suggestions;
	struct t_wb_tags_destroy					tags_destroy;
	struct t_wb_tags_destroy_batch				tags_destroy_batch;

	// 邀请接口
	struct t_wb_invite_mailcontect						invitec_mail;
	struct t_wb_invite_msncontect						invitec_msn;
	struct t_wb_invite_sendmails						invitec_sendmail;

	//
	struct t_wb_xauth_access_token						xauth_accesstoken;

	//media
	struct t_wb_media_shorturl_batch					media_shorturl_batch;

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
		{// 获取最新更新的公共微博消息
			p_twb = &pall->stat_pub_tl;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// NOT NEED PARAM
		}
		break;
	case WEIBO_OPTION(GETSTATUSES_FRIENDS_TIMELINE):
		{// 获取当前用户所关注用户的最新微博信息
			
			p_twb = &pall->stat_fri_tl;
			
			
			Wb_init_wb_struct( option , p_twb);

			printf("\n\nPlease enter the feature 0:all/1:self/2:pic/3:video/4:music :");
			gets(content);

            strcpy(pall->stat_fri_tl.wbopt_.szOpt_[t_wb_option_params::WBOPTType_feature] , content);
			
			// to fill other information 

			// since_id: （微博信息ID) ;
			// max_id: 可选参数（微博信息ID）;
			// count: 可选参数. 每次返回的最大记录数，不能超过200，默认20. 
			// page:可选参数. 返回结果的页序号。注意：有分页限制。根据用户关注对象发表的数量，通常最多返回1,000条最新微博分页内容, 默认1 
			// feature: 可选参数.微博类型，0全部，1原创，2图片，3视频，4音乐. 返回指定类型的微博信息内容。 
		}
		break;
	case WEIBO_OPTION(GETSTATUSES_USE_TIMELINE):
		{// 获取用户发布的微博信息列表,此接口也可以请求其他用户的最新发表微博
			p_twb = &pall->stat_usr_tl;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// id： 可选参数. 根据指定用户UID或微博昵称来返回微博信息。 
			// user_id： 可选参数. 用户UID，主要是用来区分用户UID跟微博昵称一样，产生歧义的时候，特别是在微博昵称为数字导致和用户Uid发生歧义。 
			// screen_name：可选参数.微博昵称，主要是用来区分用户UID跟微博昵称一样，产生歧义的时候。 


			// since_id：可选参数（微博信息ID）. 只返回ID比since_id大（比since_id时间晚的）的微博信息内容 
			// max_id: 可选参数（微博信息ID）. 返回ID不大于max_id的微博信息内容。 
			// count: 可选参数. 每次返回的最大记录数，最多返回200条，默认20。 
			// page： 可选参数. 分页返回。注意：最多返回200条分页内容。 
		}
		break;
	case WEIBO_OPTION(GETSTATUSES_MENTIONS):
		{// 获取@当前用户的微博列表
			p_twb = &pall->stat_at_mention;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// since_id. 可选参数. 返回ID比数值since_id大（比since_id时间晚的）的提到。 
			// max_id. 可选参数. 返回ID不大于max_id(时间不晚于max_id)的提到。 
			// count. 可选参数. 每次返回的最大记录数（即页面大小），不大于200，默认为20。 
			// page. 可选参数. 返回结果的页序号。注意：有分页限制。 
		}
		break;
	case WEIBO_OPTION(GETSTATUSES_COMMENTS_TIMELINE):
		{// 获取当前用户发送及收到的评论列表
			p_twb = &pall->stat_com_tl;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// since_id: 可选参数（评论ID）. 只返回ID比since_id大（比since_id时间晚的）的评论。 
			// max_id: 可选参数（评论ID）. 返回ID不大于max_id的评论。 
			// count: 可选参数. 每次返回的最大记录数，不大于200，默认20。 
			// page: 可选参数. 返回结果的页序号。注意：有分页限制。
		}
		break;
	case WEIBO_OPTION(GETSTATUSES_COMMENTS_BYME):
		{// 获取当前用户发出的评论
			p_twb = &pall->stat_com_byme;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			//since_id：可选参数（评论ID）. 只返回比since_id大（比since_id时间晚的）的评论 
			//max_id: 可选参数（评论ID）. 返回ID不大于max_id的评论。 
			//count: 可选参数. 每次返回的最大记录数，最多返回200条，默认为20。 
			//page： 可选参数. 分页返回。注意：最多返回200条分页内容。

		}
		break;
	case WEIBO_OPTION(GETSTATUSES_COMMENTS_LIST):
		{// 获取指定微博的评论列表
			p_twb = &pall->stat_coms_list;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			//id. 必选参数. 返回指定的微博ID 
			//count. 可选参数. 每次返回的最大记录数（即页面大小），不大于200，默认为20。 
			//page. 可选参数. 返回结果的页序号。注意：有分页限制。

			printf("\n\nPlease enter the ID :");
			gets(content);
			strcpy( pall->stat_coms_list.wbId_,content );

		}
		break;
	case WEIBO_OPTION(GETSTATUSES_COMMENTS_COUNTS):
		{// 批量统计微博的(评论数，转发数 , 一次最多100个)
			p_twb = &pall->stat_coms_counts;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			//ids. 必填参数. 微博ID号列表
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
		{// 获取当前用户未读消息数
			p_twb = &pall->stat_unread;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// NOT NEED PARAM
		}
		break;

		//微博访问
	case WEIBO_OPTION(GETSTATUSES_SHOW):
		{// 根据ID获取单条微博信息内容
			p_twb = &pall->stat_show;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// id. 必须参数(微博信息ID)，要获取已发表的微博ID,如ID不存在返回空 
			printf("\n\nPlease enter the wb status ID :");
			gets(content);
			strcpy( pall->stat_show.wbId_,content );
			
		}
		break;
	case WEIBO_OPTION(GOTOSTATUSES_ID):
		{// 根据微博ID和用户ID跳转到单条微博页面
			p_twb = &pall->usr_statid;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// id. 必须参数(微博信息ID)，要获取已发表的微博ID,如ID不存在返回空 
			printf("\n\nPlease enter the ID :");
			gets(content);
			strcpy( pall->usr_statid.wbId_,content );


			// 用户ID
			printf("\n\nPlease enter the user ID :");
			gets(content);
			strcpy( pall->usr_statid.uid_,content );
		}
		break;
	case WEIBO_OPTION(PUTSTATUSES_UPDATE):
		{//发布一条微博信息
			p_twb = &pall->stat_update;

			Wb_init_wb_struct( option , p_twb);

			// to fill other information

			// status. 必填参数， 要更新的微博信息。必须做URLEncode,信息内容部超过140个汉字,为空返回400错误。 
			// in_reply_to_status_id. 可选参数，@ 需要回复的微博信息ID, 这个参数只有在微博内容以 @username 开头才有意义。（即将推出）。 
			// lat. 可选参数，纬度，发表当前微博所在的地理位置，有效范围 -90.0到+90.0, +表示北纬。只有用户设置中geo_enabled=true时候地理位置才有效。(仅对受邀请的合作开发者开放) 
			// long. 可选参数，经度。有效范围-180.0到+180.0, +表示东经。(仅对受邀请的合作开发者开放) 

			printf("\n\nPlease enter Content[ <= 140 words]:");
			gets(content);
			//
			strcpy( pall->stat_update.szwbInfo_ , content );
		}
		break;
	case WEIBO_OPTION(PUTSTATUSES_UPLOAD):
		{// 上传图片并发布一条微博信息
			p_twb = &pall->stat_upload;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// status. 必填参数， 要更新的微博信息。必须做URLEncode,信息内容部超过140个汉字。 
			// pic. 必填参数。仅支持JPEG,GIF,PNG图片,为空返回400错误。目前上传图片大小限制为<1M。 
			// lat. 可选参数，纬度，发表当前微博所在的地理位置，有效范围 -90.0到+90.0, +表示北纬。只有用户设置中geo_enabled=true时候地理位置才有效。(保留字段，暂不支持) 
			// long. 可选参数，经度。有效范围-180.0到+180.0, +表示东经。(保留字段，暂不支持) 

			printf("\n\nPlease enter image file:");			
			gets(content);
			strcpy( pall->stat_upload.szImagePath_ ,content );

			printf("\n\nPlease enter Content[ <= 140 words]:");			
			gets(content);
			strcpy( pall->stat_upload.szwbInfo_ , content);
		}
		break;
	case WEIBO_OPTION(PUTSTATUSES_DESTROY):
		{// 删除一条微博信息
			p_twb = &pall->stat_destroy;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			//id. 必须参数. 要删除的微博ID. 
			printf("\n\nPlease enter the need destroy status ID :");
			gets(content);
			strcpy( pall->stat_destroy.wbId_,content );
		}
		break;
	case WEIBO_OPTION(PUTSTATUSES_REPOST):
		{// 转发一条微博信息（可加评论）
			p_twb = &pall->stat_repost;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// id 必填参数， 转发的微博ID 
			printf("\n\nPlease Repost status ID :");
			gets(content);
			strcpy( pall->stat_repost.wbId_,content );

			// status. 可选参数， 添加的转发信息。信息内容不超过140个汉字。如不填则自动生成类似“转发 @author: 原内容”文字。
			printf("\n\nPlease Repost status comment text :");
			gets(content);
			strcpy( pall->stat_repost.szwbInfo_,content );

		}
		break;
	case WEIBO_OPTION(PUTSTATUSES_COMMENT):
		{// 对一条微博信息进行评论
			p_twb = &pall->stat_comment;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// id. 必须参数. 要删除的评论ID. 
			printf("\n\nPlease enter will status id :");
			gets(content);
			strcpy( pall->stat_comment.wbId_,content );

			// cid 选填参数， 要评论的评论id 如没有或非法则为对微博的评论 
			printf("\n\nPlease enter replay comment id,it can be igno:");
			gets(content);
			strcpy( pall->stat_comment.wbCId_,content );

			// comment. 必填参数， 评论内容。信息内容不超过140个汉字
			printf("\n\nPlease enter replay text:");
			gets(content);
			strcpy( pall->stat_comment.szwbInfo_,content );
		}
		break;
	case WEIBO_OPTION(PUTSTATUSES_COMMENT_DESTROY):
		{// 删除当前用户的微博评论信息
			p_twb = &pall->stat_comment_destroy;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// wbCId_. 必须参数. 要删除的评论ID. 
			printf("\n\nPlease enter comment id :");
			gets(content);
			strcpy( pall->stat_comment_destroy.wbCId_,content );
		}
		break;
	case WEIBO_OPTION(PUTSTATUSES_REPLY):
		{// 回复微博评论信息
			p_twb = &pall->stat_reply;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information


			// id. 必须参数. 要删除的评论ID. 
			printf("\n\nPlease enter will replay status id :");
			gets(content);
			strcpy( pall->stat_reply.wbId_,content );

			// cid 必填参数， 要评论的评论id 如没有或非法则为对微博的评论 
			printf("\n\nPlease enter will replay comment id :");
			gets(content);
			strcpy( pall->stat_reply.wbCId_,content );

			// comment. 必填参数， 评论内容。信息内容不超过140个汉字
			printf("\n\nPlease enter will replay text:");
			gets(content);
			strcpy( pall->stat_reply.szwbInfo_,content );

		}
		break;

	case WEIBO_OPTION(GETUSER_INFO):
		{// 根据用户ID获取用户资料（授权用户）
			p_twb = &pall->usr_show;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// id. 用户UID或微博昵称。 
			// user_id. 指定用户UID,主要是用来区分用户UID跟微博昵称一样，产生歧义的时候，特别是在用户账号为数字导致和用户Uid发生歧义。 
			// screen_name. 指定微博昵称，主要是用来区分用户UID跟微博昵称一样，产生歧义的时候。 

			printf("\n\nPlease to use id:");			
			gets(content);

			strcpy( pall->usr_show.wbuid_.uid_ ,content );
			pall->usr_show.wbuid_.uidtype_ = t_wb_uid::IDType_id;
		}
		break;
	case WEIBO_OPTION(GETFRINDS_LIST):
		{// 获取当前用户关注对象列表及最新一条微博信息
			p_twb = &pall->usr_friends;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// id. 选填参数. 要获取的 UID或微博昵称 
			// user_id. 选填参数. 要获取的UID 
			// screen_name. 选填参数. 要获取的微博昵称 
			// cursor. 选填参数. 单页只能包含100个关注列表，为了获取更多则cursor默认从-1开始，通过增加或减少cursor来获取更多, 如果没有下一页，则next_cursor返回0 
			// count. 可选参数. 每次返回的最大记录数（即页面大小），不大于200,默认返回20。

			printf("\n\nPlease to use id:");			
			gets(content);

			strcpy( pall->usr_friends.wbuid_.uid_ ,content );
			pall->usr_friends.wbuid_.uidtype_ = t_wb_uid::IDType_id;
		}
		break;
	case WEIBO_OPTION(GETFOLLOWERS_LIST):
		{// 获取当前用户粉丝列表及最新一条微博信息
			p_twb = &pall->usr_followers;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// id. 选填参数. 要获取的 UID或微博昵称 
			// user_id. 选填参数. 要获取的UID 
			// screen_name. 选填参数. 要获取的微博昵称 
			// cursor. 选填参数. 单页只能包含100个关注列表，为了获取更多则cursor默认从-1开始，通过增加或减少cursor来获取更多, 如果没有下一页，则next_cursor返回0 
			// count. 可选参数. 每次返回的最大记录数（即页面大小），不大于200,默认返回20。

			printf("\n\nPlease to use id:");			
			gets(content);

			strcpy( pall->usr_followers.wbuid_.uid_ ,content );
			pall->usr_followers.wbuid_.uidtype_ = t_wb_uid::IDType_id;
		}
		break;
		//私信
	case WEIBO_OPTION(GETDIRECTMSG):
		{// 获取当前用户最新私信列表 
			p_twb = &pall->dirt_message;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// since_id. 可选参数. 返回ID比数值since_id大（比since_id时间晚的）的私信。 
			// max_id. 可选参数. 返回ID不大于max_id(时间不晚于max_id)的私信。 
			// count. 可选参数. 每次返回的最大记录数（即页面大小），不大于200。 
			// page. 可选参数. 返回结果的页序号。注意：有分页限制。
		}
		break;
	case WEIBO_OPTION(GETDIRESTMSG_SENT):
		{// 获取当前用户发送的最新私信列表
			p_twb = &pall->dirt_message_sent;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// since_id. 可选参数. 返回ID比数值since_id大（比since_id时间晚的）的私信。 
			// max_id. 可选参数. 返回ID不大于max_id(时间不晚于max_id)的私信。 
			// count. 可选参数. 每次返回的最大记录数（即页面大小），不大于200。 
			// page. 可选参数. 返回结果的页序号。注意：有分页限制。
		}
		break;
	case WEIBO_OPTION(PUTDIRECTMSG_NEW):
		{// 发送一条私信 
			p_twb = &pall->dirt_message_new;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			//id: 必须参数. UID或微博昵称. 为了支持数字的微博昵称，需选填写下面2个参数screen_name或user_id: 
			//screen_name: 微博昵称 
			//user_id: 新浪UID 
			//text: 必须参数. 要发生的消息内容，需要做URLEncode，文本大小必须小于300个汉字. 

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
		{// 删除一条私信
			p_twb = &pall->dirt_message_destroy;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// id. 必填参数，要删除的私信主键ID. 
			printf("\n\nPlease to private message id:");
			gets(content);

			//
			strcpy( pall->dirt_message_destroy.wbId_ ,content );
		}
		break;

	case WEIBO_OPTION(GETDIRECTMSG_WITH):
		{// 获取来往私信列表
			p_twb = &pall->dirt_message_with;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// id. 必填参数. 用户ID. 
			printf("\n\nPlease enter user id:");
			gets(content);
			//
			strcpy( pall->dirt_message_with.wbuid_ ,content );
		}
		break;
		//关注
	case WEIBO_OPTION(PUTFRIENDSHIPS_CREATE):
		{// 关注某用户 
			p_twb = &pall->ships_create;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			//下面的参数必须有其中一个: 
			//    id. 必填参数. 要关注的用户UID或微博昵称 
			//    user_id. 必填参数. 要关注的用户UID,主要是用在区分用户UID跟微博昵称一样，产生歧义的时候。 
			//    screen_name.必填参数. 要关注的微博昵称,主要是用在区分用户UID跟微博昵称一样，产生歧义的时候。 

			//follow. 可选参数。暂不支持。

		}
		break;
	case WEIBO_OPTION(PUTFRIENDSHIPS_CREATE_BATCH):
		{// 批量关注
			p_twb = &pall->ships_create_batch;
			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			printf("\n\nPlease enter the ids:");
			gets(content);
			strcpy( pall->ships_create_batch.wbIDs_ ,content );
		}
		break;


	case WEIBO_OPTION(PUTFRIENDSHIPS_DESTROY):
		{// 取消关注
			p_twb = &pall->ships_destroy;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			//下面的参数必须有其中一个: 
			//    id. 必填参数. 要关注的用户UID或微博昵称 
			//    user_id. 必填参数. 要关注的用户UID,主要是用在区分用户UID跟微博昵称一样，产生歧义的时候。 
			//    screen_name.必填参数. 要关注的微博昵称,主要是用在区分用户UID跟微博昵称一样，产生歧义的时候。 

			pall->ships_destroy.wbuid_.uidtype_ = t_wb_uid::IDType_id ;
			printf("\n\nPlease enter the destroy attend usr id:");
			gets(content);
			strcpy( pall->ships_destroy.wbuid_.uid_ ,content );

		}
		break;
	case WEIBO_OPTION(GETFRIENDSHIPS_EXISTS):
		{// 获取两个用户关系的详细情况
			p_twb = &pall->ships_show;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			//以下参数可不填写，如不填，则取当前用户 
			//   source_id. 源用户UID 
			//   source_screen_name. 源微博昵称 

			//下面参数必须选填一个: 
			//   target_id. 要判断的目的用户UID 
			//   target_screen_name. 要判断的目的微博昵称 
			printf("\n\nPlease enter target id:");			
			gets(content);

			strcpy(pall->ships_show.wbuid_target_.uid_ , content);
			pall->ships_show.wbuid_target_.uidtype_ = t_wb_uid::IDType_userid;
		}
		break;

	case WEIBO_OPTION(GETFRIENDSHIPS_BATCH_EXISTS):
		{
			printf("\n\n Not implement.please add this interface !");
		}
		break;

		//Social Graph
	case WEIBO_OPTION(GETFRIEND_IDS):
		{// 获取用户关注对象uid列表
			p_twb = &pall->friends_ids;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// id. 选填参数. 要获取好友的UID或微博昵称 
			// user_id. 选填参数. 要获取的UID 
			// screen_name. 选填参数. 要获取的微博昵称 
			// cursor. 选填参数. 单页只能包含5000个id，为了获取更多则cursor默认从-1开始，通过增加或减少cursor来获取更多的关注列表 
			// count. 可选参数. 每次返回的最大记录数（即页面大小），不大于5000，默认返回500。 

			printf("\n\nPlease enter use id:");			
			gets(content);

			strcpy(pall->friends_ids.wbuid_.uid_ , content);
			pall->friends_ids.wbuid_.uidtype_ = t_wb_uid::IDType_id;

		}
		break;
	case WEIBO_OPTION(GETFOLLOWER_IDS):
		{// 获取用户粉丝对象uid列表 
			p_twb = &pall->follower_ids;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// id. 选填参数. 要获取好友的UID或微博昵称 
			// user_id. 选填参数. 要获取的UID 
			// screen_name. 选填参数. 要获取的微博昵称 
			// cursor. 选填参数. 单页只能包含5000个id，为了获取更多则cursor默认从-1开始，通过增加或减少cursor来获取更多的关注列表 
			// count. 可选参数. 每次返回的最大记录数（即页面大小），不大于5000，默认返回500。 

			printf("\n\nPlease enter use id:");			
			gets(content);

			strcpy(pall->follower_ids.wbuid_.uid_ , content);
			pall->follower_ids.wbuid_.uidtype_ = t_wb_uid::IDType_id;
		}
		break;
		//账号
	case WEIBO_OPTION(GETACCOUNT_VERIFY):
		{// 验证当前用户身份是否合法
			p_twb = &pall->account_verify;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// NOT NEED PARAM
		}
		break;
	case WEIBO_OPTION(GETACCOUNT_RATELIMIT):
		{// 获取当前用户API访问频率限制
			p_twb = &pall->account_ratelimit;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// NOT NEED PARAM
		}
		break;
	case WEIBO_OPTION(PUTACCOUNT_QUITSESSION):
		{// 当前用户退出登录
			p_twb = &pall->account_endsession;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// NOT NEED PARAM

		}
		break;
	case WEIBO_OPTION(PUTACCOUNT_UPDATE_PROFILEIMAGE):
		{// 更改头像
			p_twb = &pall->account_up_image;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			printf("\n\nPlease enter image file[ size < 1M ]:");			
			gets(content);
			strcpy( pall->account_up_image.szImagePath_ ,content );
		}
		break;
	case WEIBO_OPTION(PUTACCOUNT_UPDATE_PROFILE):
		{// 更改资料
			p_twb = &pall->account_uprofile;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			//必须有一下参数中的一个或多个，参数值为字符串. 进一步的限制，请参阅下面的各个参数描述. 
			//  name. 昵称，可选参数.不超过20个汉字 
			//  gender 性别，可选参数. m,男，f,女。 
			//  province 可选参数. 参考省份城市编码表 
			//  city 可选参数. 参考省份城市编码表,1000为不限 
			//  description. 可选参数. 不超过160个汉字.

		}
		break;
	case WEIBO_OPTION(PUTACCOUNT_REGISTER):
		{//
			//必须有一下参数中的一个或多个，参数值为字符串. 进一步的限制，请参阅下面的各个参数描述. 
			p_twb = &pall->account_register;
			Wb_init_wb_struct( option , p_twb);

			// nick. 昵称，必须参数.不超过20个汉字 
			printf("\n\nPlease enter nicke name:");
			gets(content);
			strcpy( pall->account_register.szNick_,content );

			// gender 性别，必须参数. m,男，f,女。 
			printf("\n\nPlease enter gender:");
			gets(content);
			strcpy( pall->account_register.szGender_,content );

			// password 密码 必须参数. 
			printf("\n\nPlease enter password:");
			gets(content);
			strcpy( pall->account_register.szPwd_,content );

			// email 注册邮箱 必须参数. 
			printf("\n\nPlease enter email:");
			gets(content);
			strcpy( pall->account_register.szEmail_,content );

			// province 可选参数. 参考省份城市编码表
			printf("\n\nPlease enter province:");
			gets(content);
			strcpy( pall->account_register.szProvince_,content );

			// city 可选参数. 参考省份城市编码表,1000为不限
			printf("\n\nPlease enter city:");
			gets(content);
			strcpy( pall->account_register.szCity_,content );

			// ip 必须参数，注册用户用户当前真实的IP。
			printf("\n\nPlease enter IP:");
			gets(content);
			strcpy( pall->account_register.szIP_,content );
		}
		break;

		// 收藏
	case WEIBO_OPTION(GETFAVORITES):
		{// 获取当前用户的收藏列表 
			p_twb = &pall->favos_list;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// page： 可选参数. 返回结果的页序号。注意：有分页限制。
		}
		break;
	case WEIBO_OPTION(PUTFAVORITES_CREATE):
		{// 添加收藏 
			p_twb = &pall->favos_create;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information

			// id 必填参数， 要收藏的微博id 
			printf("\n\nPlease enter status id:");			
			gets(content);


			//id. 必须参数. 要删除的收藏微博信息ID.
			strcpy( pall->favos_create.wbId_,content);
		}
		break;
	case WEIBO_OPTION(PUTFAVORITES_DESTROY):
		{// 删除当前用户收藏的微博信息 
			p_twb = &pall->favos_destroy;

			Wb_init_wb_struct( option , p_twb);
			// to fill other information
			printf("\n\nPlease enter status id:");			
			gets(content);


			//id. 必须参数. 要删除的收藏微博信息ID.
			strcpy( pall->favos_destroy.wbId_,content);
		}
		break;
		// 表情
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
		{// cookie方式
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

	case WEIBO_OPTION(GET_USERS_HOT):// 获取系统推荐用户
		{
			p_twb = &pall->users_hot;
			Wb_init_wb_struct(option , p_twb);

			//
			printf("\n\nPlease enter get hot category:");
			gets(content);
			strcpy( pall->users_hot.category_ ,content);
		}
		break;

	case WEIBO_OPTION(POST_USERS_REMARK)://更新修改当前登录用户所关注的某个好友的备注信息New!
		{
			p_twb = &pall->users_remark;
			Wb_init_wb_struct(option , p_twb);

			//
			printf("\n\nPlease enter user id:");
			gets(content);
			strcpy( pall->users_remark.userId_ ,content);

			//
			printf("\n\nPlease enter modify user mark:");
			gets(content);
			strcpy( pall->users_remark.remark_ ,content);
		}
		break;

	case WEIBO_OPTION(GET_USERS_SUGGESTIONS): // 返回当前用户可能感兴趣的用户
		{
			p_twb = &pall->users_suggestions;
			Wb_init_wb_struct(option , p_twb);

			printf("\n\nPlease enter with_reasons ? 1:0 :");
			gets(content);
			pall->users_suggestions.with_reason = atoi(content);
		}
		break;

		/////////////////////////////////////////////////////
		// 话题接口 ,by welbon,2011-01-10
	case WEIBO_OPTION(GET_TRENDS)://trends 获取某人的话题
		{
			p_twb = &pall->trends;
			Wb_init_wb_struct(option , p_twb);

			printf("\n\nPlease enter user id:");
			gets(content);
			strcpy( pall->trends.usrid_,content);
		}
		break;

	case WEIBO_OPTION(GET_TRENDS_STATUSES)://trends/statuses 获取某一话题下的微博
		{
			p_twb = &pall->trend_statuses;
			Wb_init_wb_struct(option , p_twb);

			printf("\n\nPlease enter trend name:");
			gets(content);
			strcpy( pall->trend_statuses.terndname_,content);
		}
		break;
	case WEIBO_OPTION(POST_TRENDS_FOLLOW)://trends/follow 关注某一个话题
		{
			p_twb = &pall->trend_follow;
			Wb_init_wb_struct(option , p_twb);

			printf("\n\nPlease enter trend name:");
			gets(content);
			strcpy( pall->trend_follow.terndname_,content);
		}
		break;
	case WEIBO_OPTION(DELETE_TRENDS_DESTROY)://trends/destroy 取消关注的某一个话题
		{
			p_twb = &pall->trend_destroy;
			Wb_init_wb_struct(option , p_twb);

			printf("\n\nPlease enter trend id:");
			gets(content);
			strcpy( pall->trend_destroy.trendid_,content);
		}
		break;

	case WEIBO_OPTION(GET_TRENDS_HOURLY)://trends/destroy 按小时返回热门话题
	case WEIBO_OPTION(GET_TRENDS_DAYLIY)://trends/daily 按日期返回热门话题。返回某一日期的话题
	case WEIBO_OPTION(GET_TRENDS_WEEKLIY)://trends/weekly 按周返回热门话题。返回某一日期之前某一周的话题
		{
			p_twb = &pall->trend_hourly;
			Wb_init_wb_struct(option , p_twb);

			//printf("\n\nPlease enter trend id:");
			//gets(content);
			//strcpy(pall->trend_hourly.baseapp_,content);
		}
		break;


		///////////////////////////////////////////////////
		// 黑名单接口 ,by welbon,2011-01-10
	case WEIBO_OPTION(POST_BLOCKS_CREATE)://将某用户加入黑名单
		{
			p_twb = &pall->block_create;
			Wb_init_wb_struct(option , p_twb);

			// user id
			printf("\n\nPlease enter user id:");
			gets(content);
			strcpy( pall->block_create.usrid_,content);

			// screen name 
			printf("\n\nPlease enter screen name:");
			gets(content);
			strcpy( pall->block_create.screenname_,content);
		}
		break;
	case WEIBO_OPTION(POST_BLOCKS_DESTROY)://将某用户移出黑名单
		{
			p_twb = &pall->block_destroy;
			Wb_init_wb_struct(option , p_twb);

			// user id
			printf("\n\nPlease enter user id:");
			gets(content);
			strcpy( pall->block_destroy.usrid_,content);

			// screen name 
			printf("\n\nPlease enter screen name:");
			gets(content);
			strcpy( pall->block_destroy.screenname_,content);
		}
		break;
	case WEIBO_OPTION(GET_BLOCKS_EXISTS)://检测某用户是否是黑名单用户
		{
			p_twb = &pall->block_exist;
			Wb_init_wb_struct(option , p_twb);

			// user id
			printf("\n\nPlease enter user id:");
			gets(content);
			strcpy( pall->block_exist.usrid_,content);

			// screen name 
			printf("\n\nPlease enter screen name:");
			gets(content);
			strcpy( pall->block_exist.screenname_,content);
		}
		break;
	case WEIBO_OPTION(GET_BLOCKS_BLOCKING)://列出黑名单用户(输出用户详细信息)
		{
			p_twb = &pall->block_exist;
			Wb_init_wb_struct(option , p_twb);

			// page
			printf("\n\nPlease enter page:");
			gets(content);
			pall->block_bloking.page_ = atoi(content);

			// count
			printf("\n\nPlease enter count:");
			gets(content);
			pall->block_bloking.count_ = atoi(content);
		}
		break;

	case WEIBO_OPTION(GET_BLOCKS_BLOCKING_IDS)://列出分页黑名单用户（只输出id）
		{
			p_twb = &pall->block_ids;
			Wb_init_wb_struct(option , p_twb);

			// page
			printf("\n\nPlease enter page:");
			gets(content);
			pall->block_bloking.page_ = atoi(content);

			// count
			printf("\n\nPlease enter count:");
			gets(content);
			pall->block_bloking.count_ = atoi(content);
		}
		break;

		/////////////////////////////////////////////////////
		// 用户标签接口 ,BY WELBON,2011-01-10
	case WEIBO_OPTION(GET_TAGS)://TAGS 返回指定用户的标签列表
		{
			p_twb = &pall->tags;
			Wb_init_wb_struct(option , p_twb);

			// user id
			printf("\n\nPlease enter user id:");
			gets(content);
			strcpy( pall->tags.usrid_,content);

			// page
			printf("\n\nPlease enter page:");
			gets(content);
			pall->tags.page_ = atoi(content);

			// count
			printf("\n\nPlease enter count:");
			gets(content);
			pall->tags.count_ = atoi(content);
		}
		break;

	case WEIBO_OPTION(POST_TAGS_CREATE)://TAGS/CREATE 添加用户标签
		{
			p_twb = &pall->tags_create;
			Wb_init_wb_struct(option , p_twb);

			// content
			printf("\n\nPlease enter tags create,splite by \",\":");
			gets(content);
			strcpy( pall->tags_create.tags_,content );
		}
		break;

	case WEIBO_OPTION(GET_TAGS_SUGGESTIONS)://TAGS/SUGGESTIONS 返回用户感兴趣的标签
		{
			p_twb = &pall->tags_suggestions;
			Wb_init_wb_struct(option , p_twb);

			// page
			printf("\n\nPlease enter page:");
			gets(content);
			pall->tags_suggestions.page_ = atoi(content);

			// count
			printf("\n\nPlease enter count:");
			gets(content);
			pall->tags_suggestions.count_ = atoi(content);
		}
		break;
	case WEIBO_OPTION(POST_TAGS_DESTROY)://TAGS/DESTROY 删除标签
		{
			p_twb = &pall->tags_destroy;
			Wb_init_wb_struct(option , p_twb);

			// tag_id
			printf("\n\nPlease enter tags id:");
			gets(content);
			strcpy( pall->tags_destroy.tagId_,content );
		}
		break;
	case WEIBO_OPTION(POST_TAGS_DESTROY_BATCH)://TAGS/DESTROY_BATCH 批量删除标签
		{
			p_twb = &pall->tags_destroy_batch ;
			Wb_init_wb_struct(option , p_twb);

			// tag_id
			printf("\n\nPlease enter tags create,splite by \",\":");
			gets(content);
			strcpy( pall->tags_destroy_batch.ids_,content );
		}
		break;

	case WEIBO_OPTION(POST_INVITE_MAILCONTACT)://邀请邮箱联系人
		{
			p_twb = &pall->invitec_mail ;
			Wb_init_wb_struct(option , p_twb);

			// 输入邮箱ID
			printf("\n\nPlease enter your email account:\n");
			gets(content);
			strcpy( pall->invitec_mail.usrid_,content );

			// 输入邮箱密码
			printf("\n\nPlease enter your email password:\n");
			gets(content);
			strcpy( pall->invitec_mail.usrpwd_,content );

			// 输入类型
			printf("\n\nPlease enter your mail type [1 sina,2 gmail,3 126Mail,4 163Mail]:\n");
			gets(content);
			pall->invitec_mail.type_ = atoi(content);

		}
		break;
	case WEIBO_OPTION(POST_INVITE_MSNCONTACT): //邀请MSN联系人
		{
			p_twb = &pall->invitec_msn ;
			Wb_init_wb_struct(option , p_twb);

			// 输入邮箱ID
			printf("\n\nPlease enter your msn account:\n");
			gets(content);
			strcpy( pall->invitec_msn.usrid_,content );

			// 输入邮箱密码
			printf("\n\nPlease enter your msn password:\n");
			gets(content);
			strcpy( pall->invitec_msn.usrpwd_,content );
		}
		break;
	case WEIBO_OPTION(POST_INVITE_SENDMAILS):  //发送邀请邮件
		{
			p_twb = &pall->invitec_sendmail ;
			Wb_init_wb_struct(option , p_twb);

			// 输入邮箱ID
			printf("\n\nPlease enter your mail account:\n");
			gets(content);
			strcpy( pall->invitec_sendmail.myusrid_,content );

			// 输入邮箱我的邮箱类型
			printf("\n\nPlease enter your mail type [msn/mail]:\n");
			gets(content);
			strcpy( pall->invitec_sendmail.mailtype_,content );

			// 输入邮箱我的昵称
			printf("\n\nPlease enter your invite NICK Name:\n");
			gets(content);
			strcpy( pall->invitec_sendmail.nickname_,content );

			// 输入邮箱我的邮箱列表
			printf("\n\nPlease enter your mail list ,splite by \",\":\n");
			gets(content);
			strcpy( pall->invitec_sendmail.maillist_,content );
		}
		break;

	case WEIBO_OPTION(UPDATETGT):
		{
			p_twb = &pall->cookie;
			Wb_init_wb_struct(option , p_twb);
		}
		break;

	case WEIBO_OPTION(GET_MEDIA_SHORTURL_BATCH):
		{
			p_twb = &pall->media_shorturl_batch ;
			Wb_init_wb_struct(option , p_twb);

			//
			printf("\n\nPlease enter your get media short url ids:\n");
			gets(content);
			strcpy( pall->media_shorturl_batch.urlids_,content );
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

	// 静态方式
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
	printf("Please enter request method[0 = oauth/1 = xauth/2 = cookie]:");
#if defined(WINCE)
	cookie = 1;
#else
	gets( enter );
	cookie = atoi(enter);
#endif
	WEIBORequest* wbRequest = wb_interface.pfun_request_create();
	
	if(cookie == 0 || cookie == 1)
	{
		//hack for old mode 0,1 oauth or xauth
		wb_interface.pfun_request_setopt(wbRequest , WEIBO_OPTION(REQ_COOKIE) , 0);
	}
	else
	{
		wb_interface.pfun_request_setopt(wbRequest , WEIBO_OPTION(REQ_COOKIE) , 1);
	}
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

			// 输入用户名(必填)
			printf("Please enter user id : ");
			gets(enter);
			strcpy(twball.oauth_authorize.usrid_,enter);

			// 输入密码(必填)
			printf("Please enter password : ");
			gets(enter);
			strcpy(twball.oauth_authorize.usrpwd_,enter);

			//第二步：直接请求 verify code
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

		//3.请求 access token
		wb_interface.pfun_request_setopt(wbRequest , WEIBO_OPTION(REQ_SENDDATA) , WEIBO_OPTION(OAUTH_ACCESS_TOKEN) , &twball.oauth_accesstoken );
		wb_interface.pfun_request_setopt(wbRequest , WEIBO_OPTION(REQ_USERDATA) , &twball.oauth );
		wb_interface.pfun_request_start(wbRequest , false);

		//
		wb_interface.pfun_request_stop(wbRequest);
		//
		SYSTEM("pause");
	}
	else if (cookie == 1)
	{
		Wb_init_wb_struct( WEIBO_OPTION(XAUTH_ACCESS_TOKEN)  , &twball.xauth_accesstoken);

		printf("Please enter user id : ");
		gets(enter);
		strcpy(twball.xauth_accesstoken.usrid_,enter);

		// 输入密码(必填)
		printf("Please enter password : ");
		gets(enter);
		strcpy(twball.xauth_accesstoken.usrpwd_,enter);
		strcpy(twball.xauth_accesstoken.authmode_,"client_auth");

		wb_interface.pfun_request_setopt(wbRequest , WEIBO_OPTION(REQ_SENDDATA) ,WEIBO_OPTION(XAUTH_ACCESS_TOKEN), &twball.xauth_accesstoken );
		wb_interface.pfun_request_setopt(wbRequest , WEIBO_OPTION(REQ_USERDATA) , &twball.oauth );
		wb_interface.pfun_request_start(wbRequest , false);

		//
		wb_interface.pfun_request_stop(wbRequest);
		SYSTEM("pause");
	}
	else if (cookie == 2)
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
		printf("\n\nPlease enter TEST number [exit:'q' keylist:list]:");
		gets(enter);

		// "q" exit
		if( strcmp(enter ,"q") == 0 ) {
			break;
		}
		else if( strcmp(enter,"list") == 0){
			print_weibo_command_list();
			continue;
		}


		void *pUsrData = NULL;

		//  其他所有微博接口
		test_weibo( wbRequest , atoi(enter) , &twball ,&wb_interface,pUsrData );

		//
		//memset(&twball.cookie,0,sizeof(twball.cookie));

	}while(1);

	//
	wb_interface.pfun_request_destroy(wbRequest);

	// 释放接口
	unload_wb_interface(&wb_interface);

	unload_wb_parser();

	return 0;
}