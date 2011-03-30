/**
*  @brief URL Request.
*  @file  wbURL.cpp
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

#include "wbURL.h"
#include "wbRequest.h"
#include "wbLog.h"
#include "wbString.h"
#include "oauth/oauth.h"
#include "strconv.h"
#include "Urlcode.h"
#include <string.h>
#include <stdlib.h>
#include <string>
#include <time.h>

#if defined(WIN32) || defined(_WIN32)
#pragma warning(disable:4996)
#endif

#if defined(WIN32) || defined(WINCE)
#define STRDUP	_strdup
#else
#define STRDUP	strdup
#endif

//const int URI_MAX_LEN = 2048;
const int URI_MAX_LEN = 2048 * 2;

//
#define URICHECK_RETURN( funcallback ) ret = funcallback;\
	if( 0 != ret ) return ret;\
	Weibo_url_base( URI , pstruct , req_ex);


// source key
static
const lohttp::HTTPChar* Weibo_app_key(void)
{
	static lohttp::HTTPChar* app_key = HTTP_T("1016351065");
	return app_key;
}

static 
long Weibo_url_readfile(char** outdata , const WBChar* szFile)
{
	long fSize = 0;
	char* data = 0;
	FILE* fp = 0;
	wchar_t* outstrW = 0;

	if( *szFile == '\0' )
		return 0;

	
#ifdef WIN32
	
	fp = ::_wfopen( WBChar_2WChar(szFile) , L"rb" );
	SafeFreeA_lo( outstrW );
	
#else
	fp = fopen(szFile,"rb");
	
#endif //WIN32

	if( !fp )
		return 0;

	fseek(fp , 0 ,SEEK_END );
	fSize = ftell(fp);
	::fseek(fp,0,SEEK_SET);

	data = new char[ fSize ];
	if( !data )
	{
		fclose(fp);
		return 0;
	}

	if( fSize != ::fread(data,sizeof(char),fSize,fp) )
	{
		delete data;
		fclose(fp);
		return 0;
	}
	::fclose(fp);

	*outdata = data;
	return fSize;
}

//-----------------------------oauth------------------------------------------//
static
const char* Weibo_consumer_key(void)
{
	static const char* consumer_key = "1016351065";
	return consumer_key;
}

static
const char* Weibo_consumer_secret(void)
{
	static const char* consumer_secret = "186ea22b3ea58bfa90923f55fe8b2749";
	return consumer_secret;
}

// 上传文件 form方式 

static 
const int MAX_POSTCONTENT_LEN = 2048;

struct t_wb_upload_form
{
	char   file_formname[64];
	WBChar file[WB_REAL_LEN(MAX_PATH)];
	
	char   postcontent[MAX_POSTCONTENT_LEN+1];
	char*  data;
	long   fSize;
};

static
void Weibo_url_form(void** httpost , void** last , lohttp::fun_formadd_write_callback cb , const char* name , const char* value)
{
	char* outstr = 0;
	if( lo_UrlDecodeA(&outstr,value,-1) > 0 )
	{// 还原没有 urlencode之前， 这与 一般post 处理不一样
		value = outstr;
	}
	cb(httpost , last , lohttp::HTTP_FORMTYPE_COPYNAME , name ,lohttp::HTTP_FORMTYPE_COPYCONTENTS , value , lohttp::HTTP_FORMTYPE_END );
	SafeFreeA_lo( outstr );
}

static
void Weibo_url_uploadimage_form(void** httpost , void** last , lohttp::fun_formadd_write_callback cb , void* pformdata)
{
	struct t_wb_upload_form* pupload = (struct t_wb_upload_form*)pformdata;
	char* outstr = 0;
	char *argn = 0 , *argv = 0 ;
	char *post = pupload->postcontent;

	if( !pupload )
		return ;

	// 其它内部
	while( pupload->postcontent[0] != '\0' )
	{
		if( *post == '\0' )
		{// 最后一个参数或只有一个参数的情况
			if( argv && argn )
			{
				Weibo_url_form(httpost , last ,cb , argn , argv );
			}
			break;
		}

		if( !argn )
			argn = post;

		if( *post == '=' )
		{
			*post = '\0';
			argv = ++post;
		}
		else if( *post == '&' )
		{// 一个参数完成
			*post = '\0';
			post++;

			if( argv && argn )
			{
				Weibo_url_form(httpost , last ,cb , argn , argv );
				argv = 0;
				argn = 0;
			}
		}
		else post++;
	}

	// 读取文件
	pupload->fSize = Weibo_url_readfile(&pupload->data , pupload->file );
	if( pupload->fSize )
	{// pic
		cb(httpost , last , lohttp::HTTP_FORMTYPE_COPYNAME ,  pupload->file_formname , lohttp::HTTP_FORMTYPE_BUFFER   , WBChar_2Char(pupload->file) , lohttp::HTTP_FORMTYPE_BUFFERPTR,pupload->data,lohttp::HTTP_FORMTYPE_BUFFERLENGTH,pupload->fSize,lohttp::HTTP_FORMTYPE_END );
		SafeFreeA_lo(outstr);
	}
}

static
void Weibo_url_uploadform_free(void* pformdata)
{
	struct t_wb_upload_form* pupload = (struct t_wb_upload_form*)pformdata;
	if( pupload )
	{
		if( pupload->fSize && pupload->data )
		{
			delete[] pupload->data;
		}
		free( pupload );
	}
}

static 
struct t_wb_upload_form* Weibo_url_uploadform_create(void)
{
	struct t_wb_upload_form* pobject = (struct t_wb_upload_form*)malloc( sizeof(struct t_wb_upload_form));
	if( pobject )
	{
		pobject->data = 0;
		pobject->fSize= 0;
		pobject->postcontent[0] = '\0';
		pobject->file_formname[0] ='\0';
	}
	return pobject;
}


static
int Weibo_url_cookie_set(lohttp::LOHttp* pHttp , const char *sue , const char *sup )
{
	if( sue && sup )
	{// header 
		lohttp::HTTPChar  cookieval[ 1024 ];
		wchar_t* outstrW = 0;

		HTTP_TCSCPY_S( cookieval , HTTP_T("SUE=") );
		HTTP_TCSCAT_S( cookieval , Char_2HTTPChar(sue) );
		SafeFreeA_lo( outstrW );

		HTTP_TCSCAT_S( cookieval , HTTP_T(";SUP=") );
		HTTP_TCSCAT_S( cookieval , Char_2HTTPChar(sup) );
		SafeFreeA_lo( outstrW );

		// cookie 
		lohttp::Http_setopt(pHttp , lohttp::LOHTTP_OPT_addheader , HTTP_T("Cookie") , cookieval );
	}
	return 0;
}

static
int Weibo_url_oauth_url(char** req_url, char** postarg ,
						const lohttp::HTTPChar* uri , bool post ,
						const char *t_key , const char *t_secret,const char *c_key , const char *c_secret)
{	
	char* outstr = 0;

	if( !c_key || *c_key =='\0' )
	{// 默认的
		c_key =  Weibo_consumer_key(); ///< consumer key
	}

	if( !c_secret || *c_secret =='\0' )
	{// 默认的
	    c_secret = Weibo_consumer_secret(); ///< consumer secret
	}

	// get
	if( post )
	{
		// POST 
		*req_url = oauth_sign_url2( HTTPChar_2Char(uri) , postarg, OA_HMAC, NULL, c_key, c_secret, t_key , t_secret);
		if( !(*req_url) )
		{
			SafeFreeA_lo( outstr );
			return -1; // oauth 算出来uri为空
		}
	}
	else
	{// Get

		*req_url = oauth_sign_url2( HTTPChar_2Char(uri), NULL, OA_HMAC, NULL, c_key, c_secret , t_key , t_secret );

		if( !(*req_url) )
		{
			SafeFreeA_lo( outstr );
			return -1; // oauth 算出来uri为空
		}
	}
	SafeFreeA_lo( outstr );
	return 0;
}

//static 
//int Weibo_url_oauth_seturl(lohttp::LOHttp* pHttp , const lohttp::HTTPChar* uri , bool post , const char *t_key , const char *t_secret )
//{	
//	char *req_url = NULL;
//	char *postarg = NULL;
//	lohttp::HTTPChar* outstrW = 0;
//
//	// 计算
//	if( 0 != Weibo_url_oauth_url(&req_url , &postarg , uri , post , t_key , t_secret) )
//	{//失败
//		SafeFreeA_lo(postarg);
//		SafeFreeA_lo(req_url);
//		return -1;
//	}
//	wbPRINTF("\n\nHttp URL:\n	 %s\n	 postarvg:%s\n",req_url,postarg?postarg:"" );
//
//	// set url
//	lohttp::Http_seturl(pHttp , Char_2HTTPChar(req_url) );
//	SafeFreeA_lo(req_url);
//	SafeFreeA_lo(outstrW);
//
//	if( postarg )
//	{//post param
//
//		lohttp::Http_setopt(pHttp , lohttp::LOHTTP_OPT_addparam , Char_2HTTPChar(postarg) , NULL , lohttp::ParamNormal );
//
//		SafeFreeA_lo(postarg);
//		SafeFreeA_lo(outstrW);
//	}
//	return 0;
//}

//static 
//int Weibo_url_seturl(lohttp::LOHttp* pHttp , const lohttp::HTTPChar* src_uri , bool post , const char *t_key , const char *t_secret , int cookie )
//{
//	if( cookie )
//		return Weibo_url_cookie_seturl(pHttp ,  src_uri ,  post , t_key , t_secret);
//	else
//		return Weibo_url_oauth_seturl(pHttp  ,  src_uri ,  post , t_key , t_secret);
//}

// data format
static 
const lohttp::HTTPChar* Weibo_url_format_key( int format )
{
	static const lohttp::HTTPChar* format_key[] =
	{
		HTTP_T(".json"),
		HTTP_T(".xml"),
		HTTP_T(""),
	};
	if( format > E_FORMAT_XML || format < E_FORMAT_JSON )
		format = 2;
	return format_key[format];
}



static
void Weibo_url_random(lohttp::HTTPChar* URI)
{
	// 增加尾部随机数的处理
	HTTP_TCSCAT_S(URI,HTTP_T("rnd=0.") );
	srand((unsigned int)time(NULL));
	int paramval = rand();
	WBChar val[16] = {0};
	if( paramval < 0 )
		return ;
	_TSPRINTF(val ,"%d", HTTP_T(paramval) );
	HTTP_TCSCAT_S( URI,val );
}


//----------------------------------------------------------------------------------//
// for declare function
#define WEIBO_url_callback(name) \
	static int Weibo_url_url_##name##_cb(lohttp::HTTPChar URI[] , int* httpmethod  , const void* t_wb , const struct t_wb_REQ_EX* req_ex)

// for enum
#define WEIBO_url_fun(name) Weibo_url_url_##name##_cb

#ifndef WEIBO_struct_cast

#define WEIBO_struct_cast( STRUCT ) \
	const struct STRUCT* pstruct = (const struct STRUCT*)t_wb;
#endif

//
#define HTTP_SET_GET_METHOD() \
	if(httpmethod) *httpmethod = lohttp::E_HTTP_GET_METHOD;

#define HTTP_SET_POST_METHOD() \
	if(httpmethod) *httpmethod = lohttp::E_HTTP_POST_METHOD;

#define HTTP_SET_DELETE_METHOD() \
	if(httpmethod) *httpmethod = lohttp::E_HTTP_DELETE_METHOD;

#define HTTP_SET_UPLOAD_METHOD() \
	if(httpmethod) *httpmethod = lohttp::E_HTTP_UPLOAD_METHOD;

// URI ENCODE AND UTF8
#define PARAM_ENCODE_UTF8 (lohttp::ParamEncodedValue|lohttp::ParamUTF8Value)

enum
{
	ParamFMT = 0,
	ParamFMT_UTF8 = 0x01,
	ParamFMT_URLENCODE = 0x02,
};

static 
void Weibo_url_paramformat(lohttp::HTTPChar URI[] , const lohttp::HTTPChar* param , int paramformat)
{
	int vallen= 0;
	char* outstr = 0 , *urlenc = 0;
	wchar_t* outstrW = 0;

	if( paramformat & ParamFMT_UTF8 )
	{// utf8
		HTTPChar_2UTF8(param);
	}
	else
	{
		outstr = (char*)HTTPChar_2Char(param);
	}

	if( paramformat & ParamFMT_URLENCODE )
	{		
		vallen = lo_UrlEncodeA(&urlenc , outstr , -1 );
		if( vallen <= 0 )
		{
		    if( outstr != (char*)param && outstr ) free( outstr );
			return ;
		}
		if( outstr != (char*)param && outstr ) free( outstr );
	}
	else
	{
		urlenc = outstr;
	}
	
	HTTP_TCSCAT_S(URI , Char_2HTTPChar(urlenc) );
	SafeFreeA_lo(outstrW);
	if( urlenc != (char*)param && urlenc ) free( urlenc );
}

static
int Weibo_url_get_paramname_format(int paramformat)
{
	int paramf = 0;
    if( paramformat & lohttp::ParamEncodedName  )
		paramf |= ParamFMT_URLENCODE;
	
	if( paramformat & lohttp::ParamUTF8Name) 
		paramf |= ParamFMT_UTF8;
	return paramf;
}

static
int Weibo_url_get_paramvalue_format(int  paramformat)
{
	int paramf = 0;
    if( paramformat & lohttp::ParamEncodedValue  )
		paramf |= ParamFMT_URLENCODE;
	
	if( paramformat & lohttp::ParamUTF8Value) 
		paramf |= ParamFMT_UTF8;
	return paramf;
}
//
static 
void Weibo_url_generate_URI( lohttp::HTTPChar URI[] , const lohttp::HTTPChar* paramName,  const WBChar* paramval, int paramformat)
{
	if( *paramval == '\0' )
		return ;
	Weibo_url_paramformat(URI , paramName , Weibo_url_get_paramname_format(paramformat) );

	HTTP_TCSCAT_S(URI , HTTP_T("=") );

	Weibo_url_paramformat(URI , WBChar_2HTTPChar( paramval ) , Weibo_url_get_paramvalue_format(paramformat) );
}

static
void Weibo_url_generate_URI_INT(lohttp::HTTPChar URI[] , const lohttp::HTTPChar* paramName,  const int paramval)
{
	WBChar val[16] = {0};
	if( paramval < 0 )
		return ;

	_TSPRINTF(val ,"%d", paramval );

	Weibo_url_generate_URI( URI , paramName , val , 0);
}

// 认证后，每个接口都需要加上 oauth的认证机制（算法）
static 
void Weibo_url_base(lohttp::HTTPChar URI[] , const void* t_wb, const struct t_wb_REQ_EX* req_ex)
{
	WEIBO_struct_cast(t_wb_oauth);
	wchar_t* outstrW = 0;

	if( !req_ex || *(req_ex->appkey_) == '\0' )
		Weibo_url_generate_URI(URI , HTTP_T("?source") , Weibo_app_key() , 0 );
	else
		Weibo_url_generate_URI(URI , HTTP_T("?source") , Char_2HTTPChar(req_ex->appkey_) , 0 );

	SafeFreeA_lo(outstrW);
}

static
const lohttp::HTTPChar* Weibo_url_geturi( WEIBOoption option )
{
	struct t_urlstruct
	{
		int option;
		const lohttp::HTTPChar* url;
	};

	static const struct t_urlstruct url_vector[] = 
	{
		{WEIBO_OPTION(UNK) , HTTP_T("")},
		{WEIBO_OPTION(BASE) ,HTTP_T("")},

		//获取下行数据集(timeline)接口 
		{WEIBO_OPTION(GETSTATUSES_PUBLIC_TIMELINE)      , HTTP_T("/statuses/public_timeline")},// 获取最新更新的公共微博消息 
		{WEIBO_OPTION(GETSTATUSES_FRIENDS_TIMELINE)     , HTTP_T("/statuses/friends_timeline")},// 获取当前用户所关注用户的最新微博信息 (别名: statuses/home_timeline) 
		{WEIBO_OPTION(GETSTATUSES_USE_TIMELINE)         , HTTP_T("/statuses/user_timeline")},// 获取用户发布的微博信息列表 
		{WEIBO_OPTION(GETSTATUSES_MENTIONS)             , HTTP_T("/statuses/mentions")},// 获取@当前用户的微博列表 
		{WEIBO_OPTION(GETSTATUSES_COMMENTS_TIMELINE)    , HTTP_T("/statuses/comments_timeline")},// 获取当前用户发送及收到的评论列表 
		{WEIBO_OPTION(GETSTATUSES_COMMENTS_BYME)        , HTTP_T("/statuses/comments_by_me")},// 获取当前用户发出的评论 
		{WEIBO_OPTION(GETSTATUSES_COMMENTS_TOME)        , HTTP_T("/statuses/comments_to_me")},// 获取当前用户发出的评论 
		{WEIBO_OPTION(GETSTATUSES_COMMENTS_LIST)        , HTTP_T("/statuses/comments")},// 获取指定微博的评论列表 
		{WEIBO_OPTION(GETSTATUSES_COMMENTS_COUNTS)      , HTTP_T("/statuses/counts")},// 批量获取一组微博的评论数及转发数 
		{WEIBO_OPTION(GETSTATUSES_UNREAD)               , HTTP_T("/statuses/unread")},// 获取当前用户未读消息数 
		{WEIBO_OPTION(PUTSTATUSES_RESET_COUNT)          , HTTP_T("/statuses/reset_count")},// 将当前用户指定类型的未读消息数清0。

		//微博访问接口 
		{WEIBO_OPTION(GETSTATUSES_SHOW)           , HTTP_T("/statuses/show")},// 根据ID获取单条微博信息内容 
		{WEIBO_OPTION(GOTOSTATUSES_ID)            , HTTP_T("%s/%s/statuses/%s")},// 根据微博ID和用户ID跳转到单条微博页面 
		{WEIBO_OPTION(PUTSTATUSES_UPDATE)         , HTTP_T("/statuses/update")},// 发布一条微博信息 
		{WEIBO_OPTION(PUTSTATUSES_UPLOAD)         , HTTP_T("/statuses/upload")},// 上传图片并发布一条微博信息 
		{WEIBO_OPTION(PUTSTATUSES_DESTROY)        , HTTP_T("/statuses/destroy")},// 删除一条微博信息 
		{WEIBO_OPTION(PUTSTATUSES_REPOST)         , HTTP_T("/statuses/repost")},// 转发一条微博信息（可加评论） 
		{WEIBO_OPTION(PUTSTATUSES_COMMENT)        , HTTP_T("/statuses/comment")},// 对一条微博信息进行评论 
		{WEIBO_OPTION(PUTSTATUSES_COMMENT_DESTROY), HTTP_T("/statuses/comment_destroy")},// 删除当前用户的微博评论信息 
		{WEIBO_OPTION(PUTSTATUSES_REPLY)          , HTTP_T("/statuses/reply")},// 回复微博评论信息 

		//用户接口 
		{WEIBO_OPTION(GETUSER_INFO)          , HTTP_T("/users/show")},// 根据用户ID获取用户资料（授权用户） 
		{WEIBO_OPTION(GETFRINDS_LIST)        , HTTP_T("/statuses/friends")},// 获取当前用户关注对象列表及最新一条微博信息 
		{WEIBO_OPTION(GETFOLLOWERS_LIST)     , HTTP_T("/statuses/followers")},// 获取当前用户粉丝列表及最新一条微博信息 

		//私信接口 
		{WEIBO_OPTION(GETDIRECTMSG)         , HTTP_T("/direct_messages")},// 获取当前用户最新私信列表 
		{WEIBO_OPTION(GETDIRESTMSG_SENT)    , HTTP_T("/direct_messages/sent")},// 获取当前用户发送的最新私信列表 
		{WEIBO_OPTION(PUTDIRECTMSG_NEW)     , HTTP_T("/direct_messages/new")},// 发送一条私信 
		{WEIBO_OPTION(PUTDIRECTMSG_DESTROY) , HTTP_T("/direct_messages/destroy")},// 删除一条私信 
		{WEIBO_OPTION(GETDIRECTMSG_WITH)    , HTTP_T("/direct_messages/with")},// 获取往来私信列表

		//关注接口 
		{WEIBO_OPTION(PUTFRIENDSHIPS_CREATE)		 ,HTTP_T("/friendships/create")},// 关注某用户 
		{WEIBO_OPTION(PUTFRIENDSHIPS_CREATE_BATCH)	 ,HTTP_T("/friendships/create_batch")},// 批量关注
		{WEIBO_OPTION(PUTFRIENDSHIPS_DESTROY)		 ,HTTP_T("/friendships/destroy")},// 取消关注 
		{WEIBO_OPTION(GETFRIENDSHIPS_EXISTS)		 ,HTTP_T("/friendships/show")},// 获取两个用户关系的详细情况 
		{WEIBO_OPTION(GETFRIENDSHIPS_BATCH_EXISTS)   ,HTTP_T("/friendships/batch_exists")},// 批量获取一组用户是否为好友

		 //Social Graph接口
		{WEIBO_OPTION(GETFRIEND_IDS)           , HTTP_T("/friends/ids")},// 获取用户关注对象uid列表 
		{WEIBO_OPTION(GETFOLLOWER_IDS)         , HTTP_T("/followers/ids")},// 获取用户粉丝对象uid列表 

		//账号接口 
		{WEIBO_OPTION(GETACCOUNT_VERIFY)             , HTTP_T("/account/verify_credentials")},// 验证当前用户身份是否合法 
		{WEIBO_OPTION(GETACCOUNT_RATELIMIT)          , HTTP_T("/account/rate_limit_status")},// 获取当前用户API访问频率限制 
		{WEIBO_OPTION(PUTACCOUNT_QUITSESSION)        , HTTP_T("/account/end_session")},// 当前用户退出登录 
		{WEIBO_OPTION(PUTACCOUNT_UPDATE_PROFILEIMAGE), HTTP_T("/account/update_profile_image")},// 更改头像 
		{WEIBO_OPTION(PUTACCOUNT_UPDATE_PROFILE)     , HTTP_T("/account/update_profile")},// 更改资料 
		{WEIBO_OPTION(PUTACCOUNT_REGISTER)           , HTTP_T("/account/register")},// 注册新浪微博帐号 

		//收藏接口 
		{WEIBO_OPTION(GETFAVORITES)            , HTTP_T("/favorites")},// 获取当前用户的收藏列表 
		{WEIBO_OPTION(PUTFAVORITES_CREATE)     , HTTP_T("/favorites/create")},// 添加收藏 
		{WEIBO_OPTION(PUTFAVORITES_DESTROY)    , HTTP_T("/favorites/destroy")},// 删除当前用户收藏的微博信息 

		//登录/OAuth接口
		{WEIBO_OPTION(OAUTH_REQUEST_TOKEN)     , HTTP_T("/oauth/request_token")},// 获取未授权的Request Token 
		{WEIBO_OPTION(OAUTH_AUTHORIZE)         , HTTP_T("/oauth/authorize")},// 请求用户授权Token 
		{WEIBO_OPTION(OAUTH_ACCESS_TOKEN)      , HTTP_T("/oauth/access_token")},// 获取授权过的Access Token 

		// 表情
		{WEIBO_OPTION(GET_EMOTIONS)  , HTTP_T("/emotions")},// 返回新浪微博官方所有表情、魔法表情的相关信息。包括短语、表情类型、表情分类，是否热门等。

		// 用户搜索 
		{WEIBO_OPTION(GET_USERS_SEARCH)  , HTTP_T("/users/search")},// 返回关键字匹配的微博用户
		
		// 微博搜索 
		{WEIBO_OPTION(GET_WB_SEARCH)       , HTTP_T("/search")},// 46 返回关键字匹配的微博，(仅对新浪合作开发者开放) 
		{WEIBO_OPTION(GET_STATUSES_SEARCH) , HTTP_T("/statuses/search")},//47 搜索微博(多条件组合) (仅对合作开发者开放) 

		{WEIBO_OPTION(GET_PROVINCES) , HTTP_T("/provinces")},//48 省份城市编码表 
		{WEIBO_OPTION(REPORT)  , HTTP_T("/report_spam")},//49 举报

		// cookie
		{WEIBO_OPTION(COOKIE)  , HTTP_T("/sso/login.php")},// 获取COOKIE
		{WEIBO_OPTION(UPDATETGT)  , HTTP_T("/sso/updatetgt.php")},// Update TGT https://login.sina.com.cn/

		// custom
		{WEIBO_OPTION(CUSTOM)  , HTTP_T("")},

		// 热门接口
		{WEIBO_OPTION(HOT_REPOST_DAYLIY)   ,HTTP_T("/statuses/hot/repost_daily")   }, //热门转发-by day
		{WEIBO_OPTION(HOT_REPOST_WEEKLY)   ,HTTP_T("/statuses/hot/repost_weekly")  }, //热门转发-by week
		{WEIBO_OPTION(HOT_COMMENT_DAYLIY)  ,HTTP_T("/statuses/hot/comments_daily") }, //热门评论-by day
		{WEIBO_OPTION(HOT_COMMENT_WEEKLY)  ,HTTP_T("/statuses/hot/comments_weekly")}, //热门评论-by week

		// 用户接口（NEW）
		{WEIBO_OPTION(GET_USERS_HOT)		,HTTP_T("/users/hot")},// 获取系统推荐用户
		{WEIBO_OPTION(POST_USERS_REMARK)	,HTTP_T("/user/friends/update_remark")},//更新修改当前登录用户所关注的某个好友的备注信息New!
		{WEIBO_OPTION(GET_USERS_SUGGESTIONS),HTTP_T("/users/suggestions")},     //Users/suggestions 返回当前用户可能感兴趣的用户

		// 话题接口
		{WEIBO_OPTION(GET_TRENDS)			,HTTP_T("/trends")},			//trends 获取某人的话题
		{WEIBO_OPTION(GET_TRENDS_STATUSES)	,HTTP_T("/trends/statuses")},	//trends/statuses 获取某一话题下的微博
		{WEIBO_OPTION(POST_TRENDS_FOLLOW)	,HTTP_T("/trends/follow")},		//trends/follow 关注某一个话题
		{WEIBO_OPTION(DELETE_TRENDS_DESTROY),HTTP_T("/trends/destroy")},	//trends/destroy 取消关注的某一个话题
		{WEIBO_OPTION(GET_TRENDS_HOURLY)	,HTTP_T("/trends/hourly")},		//trends/destroy 按小时返回热门话题
		{WEIBO_OPTION(GET_TRENDS_DAYLIY)	,HTTP_T("/trends/daily")},		//trends/daily 按日期返回热门话题。返回某一日期的话题
		{WEIBO_OPTION(GET_TRENDS_WEEKLIY)	,HTTP_T("/trends/weekly")},		//trends/weekly 按周返回热门话题。返回某一日期之前某一周的话题

		// 黑名单接口 ,by welbon,2011-01-10
		{WEIBO_OPTION(POST_BLOCKS_CREATE)	,HTTP_T("/blocks/create")},//将某用户加入黑名单
		{WEIBO_OPTION(POST_BLOCKS_DESTROY)	,HTTP_T("/blocks/destroy")},//将某用户移出黑名单
		{WEIBO_OPTION(GET_BLOCKS_EXISTS)	,HTTP_T("/blocks/exists")},//检测某用户是否是黑名单用户
		{WEIBO_OPTION(GET_BLOCKS_BLOCKING)	,HTTP_T("/blocks/bloking")},//列出黑名单用户(输出用户详细信息)
		{WEIBO_OPTION(GET_BLOCKS_BLOCKING_IDS),HTTP_T("/blocks/bloking/ids")},//列出分页黑名单用户（只输出id）

		//用户标签接口
		{WEIBO_OPTION(GET_TAGS)				,HTTP_T("/tags")},				//tags 返回指定用户的标签列表
		{WEIBO_OPTION(POST_TAGS_CREATE)		,HTTP_T("/tags/create")},		//tags/create 添加用户标签
		{WEIBO_OPTION(GET_TAGS_SUGGESTIONS)	,HTTP_T("/tags/suggestions")},	//tags/suggestions 返回用户感兴趣的标签
		{WEIBO_OPTION(POST_TAGS_DESTROY)	,HTTP_T("/tags/destroy")},		//tags/destroy 删除标签
		{WEIBO_OPTION(POST_TAGS_DESTROY_BATCH),HTTP_T("/trends/destroy_batch")},//tags/destroy_batch 批量删除标签

		//Invite Mail
		{WEIBO_OPTION(POST_INVITE_MAILCONTACT),HTTP_T("/invite/aj_mailcontact.php")},//邀请邮箱联系人
		{WEIBO_OPTION(POST_INVITE_MSNCONTACT) ,HTTP_T("/invite/aj_msncontact.php")},//邀请MSN联系人
		{WEIBO_OPTION(POST_INVITE_SENDMAILS)  ,HTTP_T("/invite/aj_att_sendmails.php")},//发送邀请邮件

		//Media 
		{WEIBO_OPTION(GET_MEDIA_SHORTURL_BATCH),HTTP_T("/short_url/batch_info")},
		{WEIBO_OPTION(XAUTH_ACCESS_TOKEN), HTTP_T("/oauth/access_token")},// 获取授权过的Access Token 

	};

	if( WEIBO_OPTION(BASE) >= option ||
		WEIBO_OPTION(LAST) <= option )
	{
		return NULL;
	}
	return url_vector[option].url;
}

static
const char* Weibo_ip(WEIBOoption option , const struct t_wb_REQ_EX* req_ex)
{
	static const char* ip = "http://api.t.sina.com.cn";
	static const char* cip= "https://login.sina.com.cn";
	static const char* invip= "http://t.sina.com.cn";

	if( WEIBO_OPTION(COOKIE) == option || WEIBO_OPTION(UPDATETGT) == option)
	{
		return cip;
	}
	else if( WEIBO_OPTION(POST_INVITE_MAILCONTACT) == option || 
		WEIBO_OPTION(POST_INVITE_MSNCONTACT) == option || 
		WEIBO_OPTION(POST_INVITE_SENDMAILS) == option )
	{
		return invip;
	}
//#ifdef _DEBUG
//	// 测试代码 test short url id : hgn2ow,hk8kn
//	else if( WEIBO_OPTION(GET_MEDIA_SHORTURL_BATCH) == option){
//		return "10.73.14.64:8888";
//	}
//#endif //_DEBUG
	return (req_ex->ip_[0] == '\0')?ip:req_ex->ip_;
}

static
int Weibo_url_get_uri(lohttp::HTTPChar URI[] , WEIBOoption option , const struct t_wb_REQ_EX* req_ex , bool format = true )
{
	const lohttp::HTTPChar * uri = Weibo_url_geturi( option );
	if( !uri )
	{
		return -1;// URI 为空
	};
	HTTP_SPRINTF(URI , ("%s%s%s") , Weibo_ip(option,req_ex), uri , Weibo_url_format_key( format?req_ex->format_:-1 ) );
	return 0;
}

//static
//void Weibo_url_http_uid_param(lohttp::LOHttp* pHttp , const struct t_wb_uid* puid)
//{
//	static const lohttp::HTTPChar* szids[] =
//	{
//		HTTP_T(""), //
//		HTTP_T("id"), //IDType_id
//		HTTP_T("screen_name"), //IDType_screenname
//		HTTP_T("user_id"), //IDType_userid
//	};
//	if( szids[puid->uidtype_] && *szids[puid->uidtype_] != '\0' && puid->uid_[0] != '\0' )
//	{
//		lohttp::Http_setopt(pHttp , lohttp::LOHTTP_OPT_addparam , szids[puid->uidtype_] , WBChar_2HTTPChar(puid->uid_) , PARAM_ENCODE_UTF8 );
//	}
//}

static
void Weibo_url_http_uid_param_URI(lohttp::HTTPChar URI[] , const struct t_wb_uid* puid)
{
	static const lohttp::HTTPChar* szids[] =
	{
		HTTP_T(""), //
		HTTP_T("&id"), //IDType_id
		HTTP_T("&screen_name"), //IDType_screenname
		HTTP_T("&user_id"), //IDType_userid
	};

	if( szids[puid->uidtype_] && *szids[puid->uidtype_] != '\0' && puid->uid_[0] != '\0' )
	{
		Weibo_url_generate_URI(URI , szids[puid->uidtype_] ,  puid->uid_  , PARAM_ENCODE_UTF8 );
	}
}

//static
//void Weibo_url_http_option_param(lohttp::LOHttp* pHttp , const struct t_wb_option_params* poption)
//{
//	static const lohttp::HTTPChar* sz_option[] =
//	{
//		HTTP_T(""),
//		HTTP_T("since_id"), //WBOPTType_since_id
//		HTTP_T("max_id"), //WBOPTType_max_id
//		HTTP_T("count"), //WBOPTType_count
//		HTTP_T("page"), //WBOPTType_page
//		HTTP_T("cursor"), //WBOPTType_cursor
//		HTTP_T("role"), //WBOPTType_role
//		HTTP_T("follow"), //WBOPTType_follow
//	};
//
//	for( int i = 1 ; i < OPT_PARAMS_COUNTS  ; i++ )
//	{
//		if( (poption->type_ & ( 1 << (i-1) ) ) && poption->szOpt_[i][0] != '\0' )
//		{
//			lohttp::Http_setopt(pHttp , lohttp::LOHTTP_OPT_addparam , sz_option[i] , WBChar_2HTTPChar( poption->szOpt_[i] ) , lohttp::ParamNormal  );
//		}
//	}
//}

static
void Weibo_url_http_option_param_URI(lohttp::HTTPChar URI[] , const struct t_wb_option_params* poption)
{
	static const lohttp::HTTPChar* sz_option[] =
	{
		HTTP_T(""),
		HTTP_T("&since_id"),
		HTTP_T("&max_id"),
		HTTP_T("&count"),
		HTTP_T("&page"),
		HTTP_T("&cursor"),
		HTTP_T("&role"),
		HTTP_T("&follow"),
		HTTP_T("&with_new_status"),
	};
	for( int i = 1 ; i < OPT_PARAMS_COUNTS  ; i++ )
	{
		if( poption->szOpt_[i][0] != '\0' )
		{
		   Weibo_url_generate_URI(URI , sz_option[i] ,  poption->szOpt_[i] , PARAM_ENCODE_UTF8 );
		}
	}
}

//static
//void Weibo_url_http_file_addbin(lohttp::LOHttp* pHttp , const WBChar* szFile )
//{
//	long fSize = 0;
//	char* data = 0;
//
//	fSize = Weibo_url_readfile(&data , szFile );
//
//	if( fSize > 0 )
//	{
//		lohttp::Http_setopt(pHttp , lohttp::LOHTTP_OPT_addbinarydata, data , fSize );
//
//		delete[] data;
//	}
//}

static
int Weibo_url_http_geturl(char** outreq_url , char** outpostarg ,
						  const lohttp::HTTPChar* uri , int httpmethod ,const void* t_wb , const struct t_wb_REQ_EX* req_ex)
{
	const char *t_key    = 0;
	const char *t_secret = 0;
	char* req_url = 0;
	char* postarg = 0;
	char* outstr  = 0;
	bool  post = (httpmethod == lohttp::E_HTTP_GET_METHOD) ? false:true;

	WEIBO_struct_cast(t_wb_base);

	if( pstruct )
	{
		t_key = pstruct->wbauth_.oauth_token_;
		t_secret = pstruct->wbauth_.oauth_token_secret_;
	}

	if( 0 == req_ex->cookie_ )
	{// oauth
		if( 0 != Weibo_url_oauth_url(&req_url , &postarg , uri , post , t_key , t_secret , req_ex->appkey_ , req_ex->secret_ ) )
		{// 失败
			SafeFreeA_lo(postarg);
			SafeFreeA_lo(req_url);
			return -1;
		}
	}
	else if( post )
	{		
		const char* req_url_ = (const char*)HTTPChar_2Char(uri);
		const char* postarg_ = strchr( req_url_ , '?');
		if( postarg_ )
		{			
			*( (char* )postarg_ ) = '\0';

			postarg = STRDUP(postarg_ + sizeof(char) );
			req_url = STRDUP(req_url_);

			*( (char* )postarg_ ) = '?';
		}
		SafeFreeA_lo(outstr);
	}
	else
	{
		req_url = STRDUP(uri);
	}
	*outreq_url = req_url;
	*outpostarg = postarg;
	return 0;
}

static
int Weibo_url_http_seturl(lohttp::LOHttp* pHttp , const lohttp::HTTPChar* uri , int httpmethod , const void* t_wb ,const struct t_wb_REQ_EX* req_ex,bool bcookie = true)
{ 
	char* req_url = 0;
	char* postarg = 0;
	wchar_t* outstrW = 0;

	WEIBO_struct_cast(t_wb_base);
	
	if( 0 != Weibo_url_http_geturl( &req_url , &postarg , uri , httpmethod , t_wb , req_ex ) )
	{
		SafeFreeA_lo(req_url);
		SafeFreeA_lo(postarg);
		return -1;
	}
	wbPRINTF("\n\nHTTP URL:\n	%s\n	post:%s\n\n",req_url , postarg?postarg:"");

	// set url
	lohttp::Http_seturl(pHttp , Char_2HTTPChar(req_url) );
	SafeFreeA_lo(req_url);
	SafeFreeA_lo(outstrW);

	if( postarg )
	{//post param

		lohttp::Http_setopt(pHttp , lohttp::LOHTTP_OPT_addparam , Char_2HTTPChar(postarg) , NULL , lohttp::ParamNormal );

//		CURLEncodeA urlEncode(postarg,strlen(postarg));
//		lohttp::Http_setopt(pHttp , lohttp::LOHTTP_OPT_addparam , Char_2HTTPChar(urlEncode.c_str()) , NULL , lohttp::ParamNormal );

		SafeFreeA_lo(postarg);
		SafeFreeA_lo(outstrW);
	}

	if( req_ex->cookie_ && pstruct && bcookie )
	{// set cookie
		Weibo_url_cookie_set(pHttp , pstruct->wbauth_.sue_ , pstruct->wbauth_.sup_ );
	}

	// method
    lohttp::Http_setopt(pHttp , lohttp::LOHTTP_OPT_setmethod , httpmethod );

	return 0;
}

// 上传图片，用 表单方式
static
int Weibo_url_upload_form(lohttp::LOHttp* pHttp ,
						  const lohttp::HTTPChar* uri , int httpmethod , const void* t_wb , const char* file_formname, const struct t_wb_REQ_EX* req_ex )
{
	char* req_url = 0;
	char* postarg = 0;
	wchar_t* outstrW = 0;
	int formidx = 0;
	struct t_wb_upload_form* pformdata = 0;

	WEIBO_struct_cast(t_wb_put_account_up_profileimage);

	if( 0 != Weibo_url_http_geturl( &req_url , &postarg , uri , httpmethod , t_wb , req_ex ) )
	{
		SafeFreeA_lo(req_url);
		SafeFreeA_lo(postarg);
		return -1;
	}

	wbPRINTF("\n\nHTTP URL:\n	%s\n	post:%s\n\n",req_url , postarg?postarg:"");

	pformdata = Weibo_url_uploadform_create();

	strcpy(pformdata->file_formname , file_formname );
	_TCSCPY_S(pformdata->file , pstruct->szImagePath_ );

	if( postarg )
	{
		strncpy(pformdata->postcontent , postarg , MAX_POSTCONTENT_LEN );
		pformdata->postcontent[ MAX_POSTCONTENT_LEN ] = '\0';
	}

	// set form
	{
		lohttp::THttpForm uploadForm;
		uploadForm.fformadd_cb = Weibo_url_uploadimage_form;
		uploadForm.ffromdata_free_cb = Weibo_url_uploadform_free;
		uploadForm.pformdata = pformdata ;

		lohttp::Http_setopt(pHttp , lohttp::LOHTTP_OPT_setform , &uploadForm );
	}

	// set url
	lohttp::Http_seturl(pHttp , Char_2HTTPChar(req_url) );
	SafeFreeA_lo(req_url);
	SafeFreeA_lo(outstrW);

	if( req_ex->cookie_ && pstruct )
	{// set cookie
		Weibo_url_cookie_set(pHttp , pstruct->wbauth_.sue_ , pstruct->wbauth_.sup_ );
	}

	SafeFreeA_lo(postarg);
		
	// method
    lohttp::Http_setopt(pHttp , lohttp::LOHTTP_OPT_setmethod , httpmethod );

	return 0;
}

static
void Weibo_url_custom_seturl(lohttp::LOHttp* pHttp , const void* t_wb )
{
	WEIBO_struct_cast(t_wb_custom);

	lohttp::Http_setopt( pHttp, lohttp::LOHTTP_OPT_setmethod , pstruct->method_ );

	lohttp::Http_setopt( pHttp, lohttp::LOHTTP_OPT_addparam  , WBChar_2HTTPChar(pstruct->szPost_) , NULL , lohttp::ParamNormal );

	lohttp::Http_seturl( pHttp , WBChar_2HTTPChar(pstruct->szUrl_) );
}


//------------------------------------------------------------------------------------------------------------//
std::string StringReplace(const std::string&   input,			//   原始串
					 const std::string&   find,			//   替换源串
					 const std::string&   replaceWith)     //   替换目的串
{
	std::string strOut(input);
	int curPos = 0;

	int pos;
	while((pos = strOut.find(find, curPos)) != -1)
	{
		//   一次替换
		strOut.replace(pos, find.size(), replaceWith);
		//   防止循环替换!!
		curPos = pos + replaceWith.size();
	}
	return strOut;
}

//------------------------------------------------------------------------------------------------------------//

WEIBO_url_callback(UNK)
{
	return -1;//不支持
}

WEIBO_url_callback(BASE)
{
	return -1;
}

WEIBO_url_callback(GETSTATUSES_PUBLIC_TIMELINE)
{// 获取最新更新的公共微博消息
	int  ret = -1;
	WEIBO_struct_cast(t_wb_get_statuses_public_timeline);

	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(GETSTATUSES_PUBLIC_TIMELINE) , req_ex ) );

	HTTP_SET_GET_METHOD();

	return 0;
}

WEIBO_url_callback(GETSTATUSES_FRIENDS_TIMELINE)
{// 获取当前用户所关注用户的最新微博信息
	int  ret = -1;
	WEIBO_struct_cast(t_wb_get_statuses_friends_timeline);

	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(GETSTATUSES_FRIENDS_TIMELINE) , req_ex ) );

	Weibo_url_http_option_param_URI(URI , &pstruct->wbopt_ );

	HTTP_SET_GET_METHOD();

	return 0;
}

WEIBO_url_callback(GETSTATUSES_USE_TIMELINE)
{// 获取用户发布的微博信息列表 
	int   ret = -1;
	WEIBO_struct_cast(t_wb_get_statuses_user_timeline);

	if( t_wb_uid::IDType_id == pstruct->wbuid_.uidtype_ )
	{// set url		
		HTTP_SPRINTF(URI, HTTP_T("%s%s/%s%s"), Weibo_ip( WEIBO_OPTION(GETSTATUSES_USE_TIMELINE),req_ex),Weibo_url_geturi( WEIBO_OPTION(GETSTATUSES_USE_TIMELINE) ), WBChar_2HTTPChar( pstruct->wbuid_.uid_ ) , Weibo_url_format_key(req_ex->format_) );

		Weibo_url_base( URI , pstruct , req_ex);
	}
	else 
	{
		URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(GETSTATUSES_USE_TIMELINE), req_ex) );
		//Weibo_url_base( URI , pstruct , req_ex);

		Weibo_url_http_uid_param_URI(URI , &pstruct->wbuid_ );
	}

	Weibo_url_http_option_param_URI(URI , &pstruct->wbopt_ );

	HTTP_SET_GET_METHOD();

    return 0;
}

WEIBO_url_callback(GETSTATUSES_MENTIONS)
{// 获取@当前用户的微博列表
	int  ret = -1;
	WEIBO_struct_cast(t_wb_get_statuses_mentions);

	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(GETSTATUSES_MENTIONS), req_ex) );

	Weibo_url_http_option_param_URI(URI , &pstruct->wbopt_ );

    HTTP_SET_GET_METHOD();

	return 0;
}

WEIBO_url_callback(GETSTATUSES_COMMENTS_TIMELINE)
{// 获取当前用户发送及收到的评论列表 
	int  ret = -1;
	WEIBO_struct_cast(t_wb_get_statuses_comments_timeline);

	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(GETSTATUSES_COMMENTS_TIMELINE), req_ex) );

	Weibo_url_http_option_param_URI(URI , &pstruct->wbopt_ );

	HTTP_SET_GET_METHOD();

    return 0;
}

WEIBO_url_callback(GETSTATUSES_COMMENTS_BYME)
{// 获取当前用户发出的评论 
	int  ret = -1;
	WEIBO_struct_cast(t_wb_get_statuses_comments_byme);

	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(GETSTATUSES_COMMENTS_BYME), req_ex) );

	Weibo_url_http_option_param_URI(URI , &pstruct->wbopt_ );

	HTTP_SET_GET_METHOD();

    return 0;
}

WEIBO_url_callback(GETSTATUSES_COMMENTS_TOME)
{// 获取当前用户收到的评论 
	int  ret = -1;
	WEIBO_struct_cast(t_wb_get_statuses_comments_tome);

	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(GETSTATUSES_COMMENTS_TOME), req_ex) );

	Weibo_url_http_option_param_URI(URI , &pstruct->wbopt_ );

	HTTP_SET_GET_METHOD();

    return 0;
}

WEIBO_url_callback(GETSTATUSES_COMMENTS_LIST)
{// 获取指定微博的评论列表
	int  ret = -1;
	WEIBO_struct_cast(t_wb_get_statuses_comments_list);

	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(GETSTATUSES_COMMENTS_LIST), req_ex) );

	Weibo_url_generate_URI(URI , HTTP_T("&id")  , pstruct->wbId_    , PARAM_ENCODE_UTF8 );

	Weibo_url_http_option_param_URI(URI , &pstruct->wbopt_ );

	HTTP_SET_GET_METHOD();

    return 0;
}

WEIBO_url_callback(GETSTATUSES_COMMENTS_COUNTS)
{// 批量获取一组微博的评论数及转发数
	int    ret = -1;
	WBChar wbIds[ BATCH_WBIDS_COUNTS * ( WB_USE_LEN(WEIBO_ID) + 2 ) ] = {0};
	WEIBO_struct_cast(t_wb_get_statuses_counts );

	// 生成 , 分隔
	for( int i = 0 ; i < BATCH_WBIDS_COUNTS ; i++ )
	{
		if( pstruct->wbIds_[i][0] != '\0' )
		{
			if( wbIds[0] != '\0' )
			{
				_TCSCAT_S(wbIds ,WBCHAR_T(",") );
			}
			_TCSCAT_S(wbIds ,pstruct->wbIds_[i] );
		}
		else
		{
			break;
		}
	}
	if( wbIds[0] == '\0' )
	{
		return -1;
	}

	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(GETSTATUSES_COMMENTS_COUNTS), req_ex) );

	Weibo_url_generate_URI(URI , HTTP_T("&ids")   , wbIds  , PARAM_ENCODE_UTF8 );

	// post method (使用get方法，不然会有问题，引起URI太长）
	HTTP_SET_POST_METHOD();

    return 0;
}

WEIBO_url_callback(GETSTATUSES_UNREAD)
{// 获取当前用户未读消息数量(数字信息)
	int  ret = -1;
	WEIBO_struct_cast(t_wb_get_statuses_unread);

	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(GETSTATUSES_UNREAD), req_ex) );

	Weibo_url_http_option_param_URI(URI , &pstruct->wbopt_ );

	HTTP_SET_GET_METHOD();
	
	return 0;
}

WEIBO_url_callback(PUTSTATUSES_RESET_COUNT)
{// 获取当前用户未读消息数量(数字信息)
	int    ret = -1;
	WBChar buf[6]={0};
	WEIBO_struct_cast(t_wb_get_statuses_reset_count);

	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(PUTSTATUSES_RESET_COUNT), req_ex) );

	if( pstruct->type_ >= t_wb_get_statuses_reset_count::Ty_comments )
	{
		Weibo_url_generate_URI(URI , HTTP_T("&type")   , _ITOT(pstruct->type_ , buf , 10)  , 0 );
	}
	HTTP_SET_POST_METHOD();

	return 0;
}

//微博访问

WEIBO_url_callback(GETSTATUSES_SHOW)
{// 根据ID获取单条微博信息内容
	int   ret = -1;
	WEIBO_struct_cast(t_wb_get_statuses_show);

	HTTP_SPRINTF(URI, HTTP_T("%s%s/%s%s"), Weibo_ip( WEIBO_OPTION(GETSTATUSES_SHOW),req_ex),Weibo_url_geturi( WEIBO_OPTION(GETSTATUSES_SHOW) ), WBChar_2HTTPChar(pstruct->wbId_) , Weibo_url_format_key(req_ex->format_) );

	Weibo_url_base( URI , pstruct , req_ex);

	HTTP_SET_GET_METHOD();

	return 0;
}

WEIBO_url_callback(GOTOSTATUSES_ID)
{// 根据微博ID和用户ID跳转到单条微博页面( 界面呈现),只生成一串URI
	int   ret = -1;
	WEIBO_struct_cast(t_wb_goto_user_status_id);

	HTTP_SPRINTF(URI ,Weibo_url_geturi( WEIBO_OPTION(GOTOSTATUSES_ID) ) , Weibo_ip( WEIBO_OPTION(GOTOSTATUSES_ID) ,req_ex) , pstruct->uid_ , pstruct->wbId_ );

    // get method 
	HTTP_SET_GET_METHOD();

	return 0;
}

WEIBO_url_callback(PUTSTATUSES_UPDATE)
{//发布一条微博信息
	int   ret = -1;
	WEIBO_struct_cast(t_wb_put_statuses_update);

	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(PUTSTATUSES_UPDATE), req_ex) );


	//Weibo_url_generate_URI(URI , HTTP_T("&status")   , CURLEncodeA(pstruct->szwbInfo_,0).c_str() , PARAM_ENCODE_UTF8 );
	Weibo_url_generate_URI(URI , HTTP_T("&status")   , StringReplace(pstruct->szwbInfo_,"%","%25").c_str(), PARAM_ENCODE_UTF8 );

    // post method 
	HTTP_SET_POST_METHOD();

	return 0;
}

WEIBO_url_callback(PUTSTATUSES_UPLOAD)
{// 上传图片并发布一条微博信息
	int  ret = -1;
	WEIBO_struct_cast(t_wb_put_statuses_upload);

	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(PUTSTATUSES_UPLOAD), req_ex) );

	//Weibo_url_generate_URI(URI , HTTP_T("&status")   , CURLEncodeA(pstruct->szwbInfo_,0).c_str() , PARAM_ENCODE_UTF8 );
	Weibo_url_generate_URI(URI , HTTP_T("&status")   , StringReplace(pstruct->szwbInfo_,"%","%25").c_str() , PARAM_ENCODE_UTF8 );
	
	HTTP_SET_UPLOAD_METHOD();

	return 0;
}

WEIBO_url_callback(PUTSTATUSES_DESTROY)
{// 删除一条微博信息
	int   ret = -1;
	WEIBO_struct_cast(t_wb_put_statuses_destroy);
	
	HTTP_SPRINTF(URI, HTTP_T("%s%s/%s%s"), Weibo_ip( WEIBO_OPTION(PUTSTATUSES_DESTROY),req_ex),Weibo_url_geturi( WEIBO_OPTION(PUTSTATUSES_DESTROY) ), WBChar_2HTTPChar( pstruct->wbId_ ) , Weibo_url_format_key(req_ex->format_) );

	Weibo_url_base( URI , pstruct , req_ex);

	// post method 
	HTTP_SET_POST_METHOD();

	return 0;
}

WEIBO_url_callback(PUTSTATUSES_REPOST)
{// 转发一条微博信息（可加评论）
	int   ret = -1;
	WEIBO_struct_cast(t_wb_put_statuses_repost);

	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(PUTSTATUSES_REPOST), req_ex) );

	Weibo_url_generate_URI(URI ,HTTP_T("&id") , pstruct->wbId_, PARAM_ENCODE_UTF8 );
	Weibo_url_generate_URI_INT(URI,HTTP_T("&is_comment"), pstruct->iCommentFlags );

	// 可选项
	if( pstruct->szwbInfo_[0] != '\0' )
	{
		//Weibo_url_generate_URI(URI , HTTP_T("&status")  , CURLEncodeA(pstruct->szwbInfo_,0).c_str()    , PARAM_ENCODE_UTF8 );
		Weibo_url_generate_URI(URI , HTTP_T("&status")  , StringReplace(pstruct->szwbInfo_,"%","%25").c_str()  , PARAM_ENCODE_UTF8 );
	}

	// post method
	HTTP_SET_POST_METHOD();

    return 0;
}

WEIBO_url_callback(PUTSTATUSES_COMMENT)
{//对一条微博信息进行评论 
	int   ret = -1;
	WEIBO_struct_cast(t_wb_put_statuses_comment);

	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(PUTSTATUSES_COMMENT), req_ex) );

	// utf8 AND urlencode
	//Weibo_url_generate_URI(URI , HTTP_T("&comment")   , CURLEncodeA(pstruct->szwbInfo_,0).c_str()    , PARAM_ENCODE_UTF8 );
	Weibo_url_generate_URI(URI , HTTP_T("&comment")   , StringReplace(pstruct->szwbInfo_,"%","%25").c_str()  , PARAM_ENCODE_UTF8 );
	Weibo_url_generate_URI(URI , HTTP_T("&id")        , pstruct->wbId_    , PARAM_ENCODE_UTF8 );

	// 可选项
	if( pstruct->wbCId_[0] != '\0' )
	{
		Weibo_url_generate_URI(URI , HTTP_T("&cid")        , pstruct->wbCId_    , PARAM_ENCODE_UTF8);
	}

	// post method
	HTTP_SET_POST_METHOD();

    return 0;
}

WEIBO_url_callback(PUTSTATUSES_COMMENT_DESTROY)
{// 删除当前用户的微博评论信息 
	int   ret = -1;
	WEIBO_struct_cast(t_wb_put_statuses_comment_destroy);

	HTTP_SPRINTF(URI, HTTP_T("%s%s/%s%s") , Weibo_ip( WEIBO_OPTION(PUTSTATUSES_COMMENT_DESTROY),req_ex),Weibo_url_geturi( WEIBO_OPTION(PUTSTATUSES_COMMENT_DESTROY) ), WBChar_2HTTPChar(pstruct->wbCId_) , Weibo_url_format_key(req_ex->format_) );

	Weibo_url_base( URI , pstruct , req_ex);

	// delete method 
	//HTTP_SET_DELETE_METHOD();
	HTTP_SET_POST_METHOD();

	return 0;
}

WEIBO_url_callback(PUTSTATUSES_REPLY)
{// 回复微博评论信息
	int   ret = -1;
	WEIBO_struct_cast(t_wb_put_statuses_reply );

	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(PUTSTATUSES_REPLY), req_ex) );

	Weibo_url_generate_URI(URI , HTTP_T("&comment")   , pstruct->szwbInfo_  , PARAM_ENCODE_UTF8 );

	Weibo_url_generate_URI(URI , HTTP_T("&id")        , pstruct->wbId_      , PARAM_ENCODE_UTF8 );

	Weibo_url_generate_URI(URI , HTTP_T("&cid")       , pstruct->wbCId_     , PARAM_ENCODE_UTF8 );

	// post method 
	HTTP_SET_POST_METHOD();

	return 0;
}

//----------------------------------用户------------------------------------------//

WEIBO_url_callback(GETUSER_INFO)
{// 根据用户ID获取用户资料（授权用户）
	int   ret = -1;
	WEIBO_struct_cast(t_wb_get_users_show);

	if( t_wb_uid::IDType_id == pstruct->wbuid_.uidtype_ )
	{
		HTTP_SPRINTF(URI ,HTTP_T("%s%s/%s%s"),  Weibo_ip( WEIBO_OPTION(GETUSER_INFO),req_ex),Weibo_url_geturi( WEIBO_OPTION(GETUSER_INFO) ) , WBChar_2HTTPChar(pstruct->wbuid_.uid_) , Weibo_url_format_key(req_ex->format_) );

		Weibo_url_base( URI , pstruct , req_ex);
	}
	else
	{
		URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(GETUSER_INFO) , req_ex) );

		Weibo_url_http_uid_param_URI(URI , &pstruct->wbuid_ );
	}

    HTTP_SET_GET_METHOD();

	return 0;
}

WEIBO_url_callback(GETFRINDS_LIST)
{// 获取当前用户关注对象列表及最新一条微博信息
	int   ret = -1;
	WEIBO_struct_cast(t_wb_get_statuses_friends);

	if( t_wb_uid::IDType_id == pstruct->wbuid_.uidtype_ )
	{
		HTTP_SPRINTF(URI, HTTP_T("%s%s/%s%s"), Weibo_ip( WEIBO_OPTION(GETFRINDS_LIST),req_ex),Weibo_url_geturi( WEIBO_OPTION(GETFRINDS_LIST) ), WBChar_2HTTPChar(pstruct->wbuid_.uid_) , Weibo_url_format_key(req_ex->format_) );

		Weibo_url_base( URI , pstruct , req_ex);
	}
	else if(  t_wb_uid::IDType_unk != pstruct->wbuid_.uidtype_ )
	{
		URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(GETFRINDS_LIST), req_ex) );

		Weibo_url_http_uid_param_URI(URI , &pstruct->wbuid_ );
	}

	Weibo_url_http_option_param_URI(URI , &pstruct->wbopt_ );


	HTTP_SET_GET_METHOD();

	return 0;
}

WEIBO_url_callback(GETFOLLOWERS_LIST)
{// 获取当前用户粉丝列表及最新一条微博信息
	int   ret = -1;
	WEIBO_struct_cast(t_wb_get_statuses_followers);

	if( t_wb_uid::IDType_id == pstruct->wbuid_.uidtype_ )
	{
		HTTP_SPRINTF(URI, HTTP_T("%s%s/%s%s"),  Weibo_ip( WEIBO_OPTION(GETFOLLOWERS_LIST),req_ex),Weibo_url_geturi( WEIBO_OPTION(GETFOLLOWERS_LIST) ), WBChar_2HTTPChar(pstruct->wbuid_.uid_) , Weibo_url_format_key(req_ex->format_) );

		Weibo_url_base( URI , pstruct , req_ex);
	}
	else if(  t_wb_uid::IDType_unk != pstruct->wbuid_.uidtype_ )
	{
		URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(GETFOLLOWERS_LIST), req_ex) );

		Weibo_url_http_uid_param_URI(URI , &pstruct->wbuid_ );
	}

	Weibo_url_http_option_param_URI(URI , &pstruct->wbopt_ );

    HTTP_SET_GET_METHOD();

	return 0;
}

//-----------------------------------私信------------------------------------------//

WEIBO_url_callback(GETDIRECTMSG)
{// 获取当前用户最新私信列表
	int  ret = -1;
	WEIBO_struct_cast(t_wb_get_direct_message);

    URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(GETDIRECTMSG), req_ex) );

	Weibo_url_http_option_param_URI(URI , &pstruct->wbopt_ );

	HTTP_SET_GET_METHOD();

	return 0;
}

WEIBO_url_callback(GETDIRESTMSG_SENT)
{// 获取当前用户发送的最新私信列表 
	int  ret = -1;
	WEIBO_struct_cast(t_wb_get_direct_message_sent);

    URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(GETDIRESTMSG_SENT), req_ex) );

	Weibo_url_http_option_param_URI(URI , &pstruct->wbopt_ );

	HTTP_SET_GET_METHOD();

	return 0;
}

WEIBO_url_callback(PUTDIRECTMSG_NEW)
{// 发送一条私信
	int   ret = -1;
	WEIBO_struct_cast(t_wb_put_direct_message_new );

    URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(PUTDIRECTMSG_NEW), req_ex) );

	Weibo_url_http_uid_param_URI(URI, &pstruct->wbuid_ );

	Weibo_url_generate_URI(URI , HTTP_T("&text")  , pstruct->szText_  , PARAM_ENCODE_UTF8 );

	// post
	HTTP_SET_POST_METHOD();

	return 0;
}

WEIBO_url_callback(PUTDIRECTMSG_DESTROY)
{// 删除一条私信
	int   ret = -1;
	WEIBO_struct_cast(t_wb_put_direct_message_destroy);

	// 特别
	HTTP_SPRINTF(URI, HTTP_T("%s%s/%s%s"),  Weibo_ip( WEIBO_OPTION(PUTDIRECTMSG_DESTROY),req_ex),Weibo_url_geturi( WEIBO_OPTION(PUTDIRECTMSG_DESTROY) ), WBChar_2HTTPChar(pstruct->wbId_) , Weibo_url_format_key(req_ex->format_) );

	Weibo_url_base( URI , pstruct , req_ex);

	// modify by by welbon 应该是POST
	// delete method
	//HTTP_SET_DELETE_METHOD();

	HTTP_SET_POST_METHOD();
	
	return 0;
}


WEIBO_url_callback(GETDIRECTMSG_WITH)
{// 获取来往私信列表
	int   ret = -1;
	WEIBO_struct_cast(t_wb_get_direct_message_with);

	//
	// 特殊的
	HTTP_SPRINTF(URI, HTTP_T("%s%s/%s%s"),  Weibo_ip( WEIBO_OPTION(GETDIRECTMSG_WITH),req_ex),Weibo_url_geturi( WEIBO_OPTION(GETDIRECTMSG_WITH) ), 
		WBChar_2HTTPChar(pstruct->wbuid_) , Weibo_url_format_key(req_ex->format_) );

	Weibo_url_base( URI , pstruct , req_ex);

	//
	//page 
	pstruct->page_ ? Weibo_url_generate_URI_INT(URI,HTTP_T("&page"), pstruct->page_ ) : 0;
	//count
	pstruct->count_ ? Weibo_url_generate_URI_INT(URI,HTTP_T("&count"), pstruct->count_ ) : 0;

	//
	HTTP_SET_GET_METHOD();

	return 0;
}



//-----------------------------------------关注---------------------------------------------//

WEIBO_url_callback(PUTFRIENDSHIPS_CREATE)
{// 关注某用户
	int   ret = -1;
	WEIBO_struct_cast(t_wb_put_friendships_create);

	if( t_wb_uid::IDType_id == pstruct->wbuid_.uidtype_ )
	{
		HTTP_SPRINTF(URI, HTTP_T("%s%s/%s%s"), Weibo_ip( WEIBO_OPTION(PUTFRIENDSHIPS_CREATE),req_ex),Weibo_url_geturi( WEIBO_OPTION(PUTFRIENDSHIPS_CREATE) ), WBChar_2HTTPChar(pstruct->wbuid_.uid_) , Weibo_url_format_key(req_ex->format_) );

	    Weibo_url_base( URI , pstruct , req_ex);
	}
	else if( t_wb_uid::IDType_unk != pstruct->wbuid_.uidtype_ )
	{
		URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(PUTFRIENDSHIPS_CREATE), req_ex) );

		Weibo_url_http_uid_param_URI(URI , &pstruct->wbuid_);
	}

	HTTP_SET_POST_METHOD();

	return 0; 
}


WEIBO_url_callback(PUTFRIENDSHIPS_CREATE_BATCH)
{// 关注某用户

	//
	int   ret = -1;
	WEIBO_struct_cast(t_wb_put_friendships_create_batch );
	//
	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(PUTFRIENDSHIPS_CREATE_BATCH), req_ex) );
	//
	Weibo_url_generate_URI(URI , HTTP_T("&ids") , pstruct->wbIDs_  , PARAM_ENCODE_UTF8 );
	//
	HTTP_SET_POST_METHOD();

	return 0; 
}



WEIBO_url_callback(PUTFRIENDSHIPS_DESTROY)
{// 取消关注
	int   ret = -1;
	WEIBO_struct_cast(t_wb_put_friendships_destroy);

	// delete method
	if( t_wb_uid::IDType_id == pstruct->wbuid_.uidtype_ )
	{
		HTTP_SPRINTF(URI, HTTP_T("%s%s/%s%s"),  Weibo_ip( WEIBO_OPTION(PUTFRIENDSHIPS_DESTROY),req_ex),Weibo_url_geturi( WEIBO_OPTION(PUTFRIENDSHIPS_DESTROY) ), WBChar_2HTTPChar(pstruct->wbuid_.uid_) , Weibo_url_format_key(req_ex->format_) );

		Weibo_url_base( URI , pstruct , req_ex);
	}
	else if( t_wb_uid::IDType_unk != pstruct->wbuid_.uidtype_ )
	{
		URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(PUTFRIENDSHIPS_DESTROY), req_ex) );

		Weibo_url_http_uid_param_URI(URI , &pstruct->wbuid_);
	}

	// follower
	Weibo_url_generate_URI_INT(URI ,HTTP_T("&is_follower"), pstruct->is_follower );

	HTTP_SET_POST_METHOD();

	return 0;
}

WEIBO_url_callback(GETFRIENDSHIPS_EXISTS)
{
	int   ret = -1;
	WEIBO_struct_cast(t_wb_get_friendships_show);

	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(GETFRIENDSHIPS_EXISTS), req_ex) );

	// source(可填上
	if( t_wb_uid::IDType_screenname == pstruct->wbuid_source_.uidtype_ )
	{
		// utf8 AND urlencode 
		Weibo_url_generate_URI(URI , HTTP_T("&source_screen_name") , pstruct->wbuid_source_.uid_   , PARAM_ENCODE_UTF8 );

	}
	else if(  t_wb_uid::IDType_userid == pstruct->wbuid_source_.uidtype_)
	{
		Weibo_url_generate_URI(URI , HTTP_T("&source_id") , pstruct->wbuid_source_.uid_    , PARAM_ENCODE_UTF8 );
	}

	// target
	if( t_wb_uid::IDType_screenname == pstruct->wbuid_target_.uidtype_ )
	{
		Weibo_url_generate_URI(URI , HTTP_T("&target_screen_name")  , pstruct->wbuid_target_.uid_  , PARAM_ENCODE_UTF8 );
	}
	else if(  t_wb_uid::IDType_userid == pstruct->wbuid_target_.uidtype_)
	{
		Weibo_url_generate_URI(URI , HTTP_T("&target_id")   , pstruct->wbuid_target_.uid_    , PARAM_ENCODE_UTF8 );
	}

	HTTP_SET_GET_METHOD();

	return 0;
}

//
WEIBO_url_callback(GETFRIENDSHIPS_BATCH_EXISTS)
{
	int   ret = -1;
	WEIBO_struct_cast(t_wb_get_friendships_batchexist );
	//
	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(GETFRIENDSHIPS_BATCH_EXISTS), req_ex) );
	//
	Weibo_url_generate_URI( URI, HTTP_T("&uids") , pstruct->wbIDs_, PARAM_ENCODE_UTF8 );
	//
	HTTP_SET_GET_METHOD();

	return 0;
}

//Social Graph
WEIBO_url_callback(GETFOLLOWER_IDS)
{
	int   ret = -1;
	WEIBO_struct_cast(t_wb_get_followers_ids);

	if( t_wb_uid::IDType_id == pstruct->wbuid_.uidtype_ )
	{
		HTTP_SPRINTF(URI , HTTP_T("%s%s/%s%s"),  Weibo_ip( WEIBO_OPTION(GETFOLLOWER_IDS),req_ex),Weibo_url_geturi( WEIBO_OPTION(GETFOLLOWER_IDS) ) , WBChar_2HTTPChar( pstruct->wbuid_.uid_ ) , Weibo_url_format_key( req_ex->format_) );

		Weibo_url_base( URI , pstruct , req_ex);
	}
	else if( t_wb_uid::IDType_unk != pstruct->wbuid_.uidtype_ )
	{
		URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(GETFOLLOWER_IDS), req_ex)  );

		Weibo_url_http_uid_param_URI(URI , &pstruct->wbuid_ );
	}

	Weibo_url_http_option_param_URI(URI , &pstruct->wbopt_ );

	HTTP_SET_GET_METHOD();

	return 0;
}

WEIBO_url_callback(GETFRIEND_IDS)
{
	int   ret = -1;
	WEIBO_struct_cast(t_wb_get_friends_ids);

	if( t_wb_uid::IDType_id == pstruct->wbuid_.uidtype_ )
	{
		HTTP_SPRINTF(URI , HTTP_T("%s%s/%s%s"), Weibo_ip( WEIBO_OPTION(GETFRIEND_IDS),req_ex),Weibo_url_geturi( WEIBO_OPTION(GETFRIEND_IDS) ) , WBChar_2HTTPChar( pstruct->wbuid_.uid_ ) , Weibo_url_format_key( req_ex->format_) );

		Weibo_url_base( URI , pstruct , req_ex);
	}
	else if( t_wb_uid::IDType_unk != pstruct->wbuid_.uidtype_ )
	{
		URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(GETFRIEND_IDS), req_ex) );

		Weibo_url_http_uid_param_URI(URI , &pstruct->wbuid_ );
	}

	Weibo_url_http_option_param_URI(URI , &pstruct->wbopt_ );

	HTTP_SET_GET_METHOD();

	return 0;
}

//账号
WEIBO_url_callback(GETACCOUNT_VERIFY)
{
	int  ret = -1;
	WEIBO_struct_cast(t_wb_get_account_verify_credentials);

	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(GETACCOUNT_VERIFY), req_ex) );

	HTTP_SET_GET_METHOD();

	return 0;
}

WEIBO_url_callback(GETACCOUNT_RATELIMIT)
{
	int  ret = -1;
	WEIBO_struct_cast(t_wb_get_account_rate_limit_status);

	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(GETACCOUNT_RATELIMIT), req_ex) );

	HTTP_SET_GET_METHOD();

	return 0;
}

WEIBO_url_callback(PUTACCOUNT_QUITSESSION)
{
	int  ret = -1;
	WEIBO_struct_cast(t_wb_put_account_end_session);

	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(PUTACCOUNT_QUITSESSION), req_ex) );

	HTTP_SET_POST_METHOD();
	return 0;
}

WEIBO_url_callback(PUTACCOUNT_UPDATE_PROFILEIMAGE)
{// 更改头像 
	int  ret = -1;
	WEIBO_struct_cast(t_wb_put_account_up_profileimage);

	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(PUTACCOUNT_UPDATE_PROFILEIMAGE), req_ex) );

	HTTP_SET_UPLOAD_METHOD();

	return 0;
}

WEIBO_url_callback(PUTACCOUNT_UPDATE_PROFILE)
{
	int   ret = -1;
	WEIBO_struct_cast(t_wb_put_account_up_profile );

	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(PUTACCOUNT_UPDATE_PROFILE), req_ex) );

	Weibo_url_generate_URI(URI , HTTP_T("&name")        , pstruct->profile_.szName_     , PARAM_ENCODE_UTF8 );

	Weibo_url_generate_URI(URI , HTTP_T("&gender")      , pstruct->profile_.szGender_   , PARAM_ENCODE_UTF8 );

	Weibo_url_generate_URI(URI , HTTP_T("&province")    , pstruct->profile_.szProvince_ , PARAM_ENCODE_UTF8 );

	Weibo_url_generate_URI(URI , HTTP_T("&city")        , pstruct->profile_.szCity_     , PARAM_ENCODE_UTF8 );

	Weibo_url_generate_URI(URI , HTTP_T("&description") , pstruct->profile_.szDescription_ , PARAM_ENCODE_UTF8 );

	HTTP_SET_POST_METHOD();

	return 0;
}

WEIBO_url_callback(PUTACCOUNT_REGISTER)
{
	int  ret = -1;
	WEIBO_struct_cast(t_wb_put_account_register);

	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(PUTACCOUNT_REGISTER), req_ex) );

	if( pstruct->szNick_[0] == '\0' )
	{
		wbPRINTF("\n\nError:\n	 nick is null.\n\n");
		return -1;
	}

	if( pstruct->szGender_[0] == '\0' )
	{
		wbPRINTF("\n\nError:\n	 gender is null.\n\n");
		return -1;
	}

	if(	pstruct->szPwd_[0] == '\0' )
	{
		wbPRINTF("\n\nError:\n	 password is null.\n\n");
		return -1;
	}

	if( pstruct->szEmail_[0] == '\0')
	{
		wbPRINTF("\n\nError:\n	 email is null.\n\n");
		return -1;
	}

	if( pstruct->szIP_[0] == '\0' )
	{
		wbPRINTF("\n\nError:\n	 IP is null.\n\n");
		return -1;
	}	

	// 必填项
	Weibo_url_generate_URI(URI , HTTP_T("&nick")        , pstruct->szNick_     , PARAM_ENCODE_UTF8 );
	Weibo_url_generate_URI(URI , HTTP_T("&gender")      , pstruct->szGender_   , PARAM_ENCODE_UTF8 );

	// 可选项
	Weibo_url_generate_URI(URI , HTTP_T("&province")    , pstruct->szProvince_ , PARAM_ENCODE_UTF8 );
	Weibo_url_generate_URI(URI , HTTP_T("&city")        , pstruct->szCity_     , PARAM_ENCODE_UTF8 );

	// 必填项
	Weibo_url_generate_URI(URI , HTTP_T("&password")    , pstruct->szPwd_     , PARAM_ENCODE_UTF8 );
	Weibo_url_generate_URI(URI , HTTP_T("&email")       , pstruct->szEmail_   , PARAM_ENCODE_UTF8 );
	Weibo_url_generate_URI(URI , HTTP_T("&ip")          , pstruct->szIP_      , PARAM_ENCODE_UTF8 );

	HTTP_SET_POST_METHOD();

	return 0;
}

// 收藏
WEIBO_url_callback(GETFAVORITES)
{ // 获取当前用户的收藏列表 
	int  ret = -1;
	WEIBO_struct_cast(t_wb_get_favorites);

	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(GETFAVORITES), req_ex) );

	Weibo_url_http_option_param_URI(URI , &pstruct->wbopt_ );

	HTTP_SET_GET_METHOD();

	return 0;
}

WEIBO_url_callback(PUTFAVORITES_CREATE)// 添加收藏 
{
	int   ret = -1;
	WEIBO_struct_cast(t_wb_put_favorites_create);

	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(PUTFAVORITES_CREATE), req_ex) );

	// 微博 id
	Weibo_url_generate_URI(URI , HTTP_T("&id") ,  pstruct->wbId_ , lohttp::ParamNormal );

	HTTP_SET_POST_METHOD();

	return 0;
}

WEIBO_url_callback(PUTFAVORITES_DESTROY)// 删除当前用户收藏的微博信息 
{
	int   ret = -1;
	WEIBO_struct_cast(t_wb_put_favorites_destroy);

	HTTP_SPRINTF(URI , HTTP_T("%s%s/%s%s"), Weibo_ip( WEIBO_OPTION(PUTFAVORITES_DESTROY),req_ex),Weibo_url_geturi( WEIBO_OPTION(PUTFAVORITES_DESTROY) ) , WBChar_2HTTPChar( pstruct->wbId_) , Weibo_url_format_key(req_ex->format_) );

	Weibo_url_base( URI , pstruct , req_ex);

	//HTTP_SET_DELETE_METHOD();
	HTTP_SET_POST_METHOD();

	return 0;
}


WEIBO_url_callback(OAUTH_REQUEST_TOKEN)// 获取未授权的Request Token 
{
	int  ret = -1;
	
	// 测试，验证算法
//#if 0
//	const char* basestring = "GET&http%3A%2F%2Fapi.t.sina.com.cn%2Foauth%2Frequest_token&oauth_consumer_key%3D1016351065%26oauth_nonce%3DOK5PveIABGdDg0md90uuXyB01eUtnY%26oauth_signature_method%3DHMAC-SHA1%26oauth_timestamp%3D1276169859%26oauth_version%3D1.0";
//	char* okey = oauth_catenc(2, c_secret, NULL);
//    char * sign = oauth_sign_hmac_sha1(basestring ,okey);
//	if( sign )
//	{
//		free(sign);
//	}
//	if( okey )
//	{
//		free(okey);
//	}
//#endif

//#if 1
//	char* pEnc  = 0 , *pDec = 0;
//	int enclen = lo_UrlEncodeA(&pEnc, "中国123345" , 0);
//
//	int declen = lo_UrlDecodeA(&pDec , pEnc , enclen );
//
//	free( pEnc );
//	free( pDec );
//
//	wchar_t* pc2w = 0;
//	int c2wlen = 0;
//	c2wlen = lo_C2W(&pc2w , "中国人民");
//
//	char* pw2c = 0;
//	int w2clen = 0;
//	w2clen = lo_W2C(&pw2c , pc2w );
//
//	free(pw2c);
//	free(pc2w);
//#endif

	Weibo_url_get_uri(URI ,  WEIBO_OPTION(OAUTH_REQUEST_TOKEN) , req_ex , false/*不需要格式*/ );

	HTTP_SET_GET_METHOD();

	return 0;//Weibo_url_seturl(pHttp  ,URI , false ,  0 , 0 , cookie);
}

WEIBO_url_callback(OAUTH_AUTHORIZE)// 请求用户授权Token 
{
	WEIBO_struct_cast(t_wb_authorize);
	Weibo_url_get_uri(URI , WEIBO_OPTION(OAUTH_AUTHORIZE) , req_ex , false/*不需要格式*/ );

	//
	Weibo_url_generate_URI( URI , HTTP_T("?oauth_callback"),
		(pstruct->wbauth_.format_ == E_FORMAT_XML) ? WBCHAR_T("xml") : WBCHAR_T("json"),0 );

	//
	Weibo_url_generate_URI( URI , HTTP_T("&oauth_token"),pstruct->wbauth_.oauth_token_,0 );

	// 用户名/密码
	Weibo_url_generate_URI( URI , HTTP_T("&userId") ,  pstruct->usrid_ ,  0 );
	Weibo_url_generate_URI( URI , HTTP_T("&passwd") ,  pstruct->usrpwd_ , 0 );

	HTTP_SET_POST_METHOD();

	return 0;
}

WEIBO_url_callback(OAUTH_ACCESS_TOKEN)// 获取授权过的Access Token
{
	int   ret = -1;
	WEIBO_struct_cast(t_wb_oauth_access_token);
	wchar_t* outstrW = 0;

	Weibo_url_get_uri(URI ,  WEIBO_OPTION(OAUTH_ACCESS_TOKEN) , req_ex , false/*不需要格式*/ );

	// 加这个验证码参数
	Weibo_url_generate_URI( URI , HTTP_T("?oauth_verifier") , Char_2WBChar(pstruct->wbauth_.oauth_verifier_) , lohttp::ParamNormal );
	SafeFreeA_lo( outstrW );

	HTTP_SET_GET_METHOD();

	return 0;
}

WEIBO_url_callback(XAUTH_ACCESS_TOKEN)// 获取授权过的Access Token
{
	int   ret = -1;
	WEIBO_struct_cast(t_wb_xauth_access_token);
	wchar_t* outstrW = 0;

	Weibo_url_get_uri(URI ,  WEIBO_OPTION(XAUTH_ACCESS_TOKEN) , req_ex , false/*不需要格式*/ );

	/// 用户名/密码
	Weibo_url_generate_URI( URI , HTTP_T("&x_auth_username") ,  pstruct->usrid_ ,  0 );
	Weibo_url_generate_URI( URI , HTTP_T("&x_auth_password") ,  pstruct->usrpwd_ , 0 );
	Weibo_url_generate_URI( URI , HTTP_T("&x_auth_mode") ,  pstruct->authmode_,  0 );
	//Weibo_url_generate_URI( URI , HTTP_T("&source") ,  "client_auth",  0 );

	SafeFreeA_lo( outstrW );

	HTTP_SET_POST_METHOD();

	return 0;
}


WEIBO_url_callback(GET_EMOTIONS)
{// 返回新浪微博官方所有表情、魔法表情的相关信息。包括短语、表情类型、表情分类，是否热门等。
	int   ret = -1;
	WEIBO_struct_cast(t_wb_emotions);
	wchar_t* outstrW = 0;

    URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(GET_EMOTIONS), req_ex)  );

	HTTP_SET_GET_METHOD();

	return 0;
}

		// 用户搜索 
WEIBO_url_callback(GET_USERS_SEARCH)
{// 返回关键字匹配的微博用户
	int   ret = -1;
	WEIBO_struct_cast(t_wb_users_search);

	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(GET_USERS_SEARCH), req_ex)  );

	Weibo_url_generate_URI( URI , HTTP_T("&q")     ,  pstruct->q_     , PARAM_ENCODE_UTF8 );
	Weibo_url_generate_URI( URI , HTTP_T("&snick") ,  pstruct->snick_ , PARAM_ENCODE_UTF8 );

	Weibo_url_generate_URI_INT( URI , HTTP_T("&sdomain")  ,  pstruct->sdomain_);
	Weibo_url_generate_URI_INT( URI , HTTP_T("&sintro")   ,  pstruct->sintro_ );
	Weibo_url_generate_URI_INT( URI , HTTP_T("&province") ,  pstruct->province_);
	Weibo_url_generate_URI_INT( URI , HTTP_T("&city")     ,  pstruct->city_   );

	Weibo_url_generate_URI( URI , HTTP_T("&gender")   ,  pstruct->gender_   , PARAM_ENCODE_UTF8 );
	Weibo_url_generate_URI( URI , HTTP_T("&comorsch") ,  pstruct->comorsch_ , PARAM_ENCODE_UTF8 );

	Weibo_url_generate_URI_INT( URI , HTTP_T("&sort")  ,  pstruct->sort_  );
	Weibo_url_generate_URI_INT( URI , HTTP_T("&page")  ,  pstruct->page_  );
	Weibo_url_generate_URI_INT( URI , HTTP_T("&count") ,  pstruct->count_ );

	HTTP_SET_GET_METHOD();

	return 0;
}		
		// 微博搜索 
WEIBO_url_callback(GET_WB_SEARCH)
{// 46 返回关键字匹配的微博，(仅对新浪合作开发者开放) 
	int   ret = -1;
	WEIBO_struct_cast(t_wb_search);
	wchar_t* outstrW = 0;

    URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(GET_WB_SEARCH), req_ex)  );

	Weibo_url_generate_URI( URI , HTTP_T("&q")     ,  pstruct->q_     , PARAM_ENCODE_UTF8 );
//	Weibo_url_generate_URI( URI , HTTP_T("&snick") ,  pstruct->snick_ , PARAM_ENCODE_UTF8 );

//	Weibo_url_generate_URI_INT( URI , HTTP_T("&sdomain")  ,  pstruct->sdomain_);
//	Weibo_url_generate_URI_INT( URI , HTTP_T("&sintro")   ,  pstruct->sintro_ );
//	Weibo_url_generate_URI_INT( URI , HTTP_T("&province") ,  pstruct->province_);
//	Weibo_url_generate_URI_INT( URI , HTTP_T("&city")     ,  pstruct->city_   );

//	Weibo_url_generate_URI( URI , HTTP_T("&gender")   ,  pstruct->gender_   , PARAM_ENCODE_UTF8 );
//	Weibo_url_generate_URI( URI , HTTP_T("&comorsch") ,  pstruct->comorsch_ , PARAM_ENCODE_UTF8 );

//	Weibo_url_generate_URI_INT( URI , HTTP_T("&sort")  ,  pstruct->sort_  );
	Weibo_url_generate_URI_INT( URI , HTTP_T("&page")  ,  pstruct->page_  );
	Weibo_url_generate_URI_INT( URI , HTTP_T("&rpp") ,  pstruct->count_ );

	Weibo_url_generate_URI( URI , HTTP_T("&geocode") ,  pstruct->geocode_ , PARAM_ENCODE_UTF8 );

	HTTP_SET_GET_METHOD();

	return 0;
}

WEIBO_url_callback(GET_STATUSES_SEARCH)
{//47 搜索微博(多条件组合) (仅对合作开发者开放) 
	int   ret = -1;
	WEIBO_struct_cast(t_wb_statuses_search);
	wchar_t* outstrW = 0;

	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(GET_STATUSES_SEARCH), req_ex)  );

	Weibo_url_generate_URI( URI , HTTP_T("&q")     ,  pstruct->q_     , PARAM_ENCODE_UTF8 );
	Weibo_url_generate_URI( URI , HTTP_T("&fuid")   ,  pstruct->fuid_ , PARAM_ENCODE_UTF8 );

	Weibo_url_generate_URI( URI , HTTP_T("&starttime"),  pstruct->starttime_   , PARAM_ENCODE_UTF8 );
	Weibo_url_generate_URI( URI , HTTP_T("&endtime")  ,  pstruct->endtime_     , PARAM_ENCODE_UTF8 );

	Weibo_url_generate_URI_INT( URI , HTTP_T("&filter_ori") ,  pstruct->filter_ori_  );
	Weibo_url_generate_URI_INT( URI , HTTP_T("&filter_pic") ,  pstruct->filter_pic_ );
	Weibo_url_generate_URI_INT( URI , HTTP_T("&province") ,  pstruct->province_);
	Weibo_url_generate_URI_INT( URI , HTTP_T("&city")     ,  pstruct->city_   );

//	Weibo_url_generate_URI_INT( URI , HTTP_T("&sort")  ,  pstruct->sort_  );
	Weibo_url_generate_URI_INT( URI , HTTP_T("&page")  ,  pstruct->page_  );
	Weibo_url_generate_URI_INT( URI , HTTP_T("&count") ,  pstruct->count_ );

	HTTP_SET_GET_METHOD();

	return 0;
}

WEIBO_url_callback(GET_PROVINCES)
{//48 省份城市编码表
	int   ret = -1;
	WEIBO_struct_cast(t_wb_provinces);

	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(GET_PROVINCES), req_ex)  );

	HTTP_SET_GET_METHOD();

	return 0;
}

WEIBO_url_callback(REPORT)
{//49 
	int   ret = -1;
	WEIBO_struct_cast(t_wb_report);

	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(REPORT), req_ex)  );

	HTTP_SET_POST_METHOD();

	return 0;
}

WEIBO_url_callback(COOKIE)
{
	int   ret = -1;
	WEIBO_struct_cast(t_wb_cookie);

	Weibo_url_get_uri(URI ,  WEIBO_OPTION(COOKIE) , req_ex , false/*不需要格式*/ );
	
	// 用户名/密码
	Weibo_url_generate_URI( URI , HTTP_T("?username") ,  pstruct->usrid_  , lohttp::ParamEncodedValue/*PARAM_ENCODE_UTF8*/ );

	Weibo_url_generate_URI( URI , HTTP_T("&password") ,  pstruct->usrpwd_ , PARAM_ENCODE_UTF8 );

	Weibo_url_generate_URI( URI , HTTP_T("&service")  , WBCHAR_T("sso&returntype=TEXT2&client=app&usersa=1") , lohttp::ParamNormal );
	
	HTTP_SET_POST_METHOD();

	return 0;
}

WEIBO_url_callback(UPDATETGT)
{
	int   ret = -1;
	WEIBO_struct_cast(t_wb_updateTGT);

	Weibo_url_get_uri(URI ,  WEIBO_OPTION(UPDATETGT) , req_ex , false/*不需要格式*/ );
	
	Weibo_url_generate_URI( URI , HTTP_T("?tgt") ,  pstruct->wbauth_.tgt_  , PARAM_ENCODE_UTF8 );

	Weibo_url_generate_URI( URI , HTTP_T("&returntype") ,  WBCHAR_T("TEXT2") , PARAM_ENCODE_UTF8 );

	HTTP_SET_GET_METHOD();

	return 0;
}

// 自定义
WEIBO_url_callback(CUSTOM)
{
	return 0;
}

/**  热点实现 */
int WEIBO_HotPoint_url_callback(lohttp::HTTPChar *URI,const void* t_wb ,const struct t_wb_REQ_EX* req_ex,const int type)
{
	int ret = -1;
	WEIBO_struct_cast(t_wb_hotpoint);
	URICHECK_RETURN( Weibo_url_get_uri(URI , (WEIBOoption)type, req_ex) );
	//
	if( pstruct->count_) 
	{
		WBChar val[16] = {0};
		_TSPRINTF(val ,"%d",pstruct->count_ );
		Weibo_url_generate_URI( URI,HTTP_T("?counts"),val, PARAM_ENCODE_UTF8 );
	}
	return 0;
}


//热点
WEIBO_url_callback(HOT_REPOST_DAYLIY)
{
	WEIBO_HotPoint_url_callback(URI,t_wb,req_ex,WEIBO_OPTION(HOT_REPOST_DAYLIY));
	
	HTTP_SET_GET_METHOD();

	return 0;
}
WEIBO_url_callback(HOT_REPOST_WEEKLY)
{
	WEIBO_HotPoint_url_callback(URI,t_wb,req_ex,WEIBO_OPTION(HOT_REPOST_WEEKLY));
	HTTP_SET_GET_METHOD();

	return 0;
}
WEIBO_url_callback(HOT_COMMENT_DAYLIY)
{
	WEIBO_HotPoint_url_callback(URI,t_wb,req_ex,WEIBO_OPTION(HOT_COMMENT_DAYLIY));
	HTTP_SET_GET_METHOD();

	return 0;
}
WEIBO_url_callback(HOT_COMMENT_WEEKLY)
{
	WEIBO_HotPoint_url_callback(URI,t_wb,req_ex,WEIBO_OPTION(HOT_COMMENT_WEEKLY));
	HTTP_SET_GET_METHOD();

	return 0;
}

//新的用户接口
WEIBO_url_callback(GET_USERS_HOT)
{
	int  ret = -1;
	WEIBO_struct_cast( t_wb_users_hot );
	
	//
	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(GET_USERS_HOT), req_ex) );

	if( pstruct->category_ && *pstruct->category_ != '\0'){
		Weibo_url_generate_URI( URI,HTTP_T("&category"),pstruct->category_, PARAM_ENCODE_UTF8 );
	}
	HTTP_SET_GET_METHOD();

	return 0;
}
WEIBO_url_callback(POST_USERS_REMARK)
{
	int  ret = -1;
	WEIBO_struct_cast( t_wb_users_remark );

	//
	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(POST_USERS_REMARK), req_ex) );
	//
	Weibo_url_generate_URI(URI , HTTP_T("&user_id"), pstruct->userId_, PARAM_ENCODE_UTF8 );
	Weibo_url_generate_URI(URI , HTTP_T("&remark"),  pstruct->remark_, PARAM_ENCODE_UTF8 );

	HTTP_SET_POST_METHOD();

	return 0;
}
WEIBO_url_callback(GET_USERS_SUGGESTIONS)
{
	int  ret = -1;

	WEIBO_struct_cast(t_wb_users_suggestions);
	//
	//get url
	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(GET_USERS_SUGGESTIONS), req_ex) );
	//
	Weibo_url_generate_URI_INT(URI , HTTP_T("&with_reason"), pstruct->with_reason);

	//
	HTTP_SET_GET_METHOD();

	return 0;
}


// 话题接口 ,by welbon,2011-01-10
WEIBO_url_callback(GET_TRENDS)//trends 获取某人的话题
{
	int ret = -1;
	WEIBO_struct_cast(t_wb_trends);

	//get url
	URICHECK_RETURN( Weibo_url_get_uri(URI,WEIBO_OPTION(GET_TRENDS), req_ex) );

	//user id 
	Weibo_url_generate_URI(URI,HTTP_T("&user_id"), pstruct->usrid_, PARAM_ENCODE_UTF8 );

	WBChar val[16] = {0};

	//page 
	pstruct->page_ ? Weibo_url_generate_URI_INT(URI,HTTP_T("&page"), pstruct->page_ ) : 0;

	//count
	pstruct->count_ ? Weibo_url_generate_URI_INT(URI,HTTP_T("&count"), pstruct->count_ ) : 0;

	HTTP_SET_GET_METHOD();

	return 0;
}
WEIBO_url_callback(GET_TRENDS_STATUSES)//trends/statuses 获取某一话题下的微博
{
	int ret = -1;
	WEIBO_struct_cast(t_wb_trends_statuses);

	//get url
	URICHECK_RETURN( Weibo_url_get_uri(URI,WEIBO_OPTION(GET_TRENDS_STATUSES), req_ex) );

	//user id 
	Weibo_url_generate_URI(URI,HTTP_T("&trend_name"), pstruct->terndname_, PARAM_ENCODE_UTF8 );


	HTTP_SET_GET_METHOD();

	return 0;
}
WEIBO_url_callback(POST_TRENDS_FOLLOW)//trends/follow 关注某一个话题
{
	int ret = -1;
	WEIBO_struct_cast(t_wb_trends_follow);

	//get url
	URICHECK_RETURN( Weibo_url_get_uri(URI,WEIBO_OPTION(POST_TRENDS_FOLLOW), req_ex) );

	//user id 
	Weibo_url_generate_URI(URI,HTTP_T("&trend_name"), pstruct->terndname_, PARAM_ENCODE_UTF8 );

	HTTP_SET_POST_METHOD();

	return 0;
}
WEIBO_url_callback(DELETE_TRENDS_DESTROY)//trends/destroy 取消关注的某一个话题
{
	int ret = -1;
	WEIBO_struct_cast(t_wb_trends_destroy);
	//
	const lohttp::HTTPChar * uri = Weibo_url_geturi( WEIBO_OPTION(DELETE_TRENDS_DESTROY));
	if( !uri ) {
		return -1;// URI 为空
	}
	HTTP_SPRINTF(URI , ("%s%s/%s%s") , Weibo_ip(WEIBO_OPTION(DELETE_TRENDS_DESTROY),req_ex), 
		uri,pstruct->trendid_, Weibo_url_format_key( req_ex->format_) );

	Weibo_url_base( URI,pstruct,req_ex);
	//

	HTTP_SET_DELETE_METHOD();

	return 0;
}
WEIBO_url_callback(GET_TRENDS_HOURLY)//trends/destroy 按小时返回热门话题
{
	int ret = -1;
	WEIBO_struct_cast(t_wb_trends_hourly);

	URICHECK_RETURN( Weibo_url_get_uri(URI,WEIBO_OPTION(GET_TRENDS_HOURLY), req_ex) );

	//baseapp
	Weibo_url_generate_URI_INT(URI,HTTP_T("&base_app"), pstruct->baseapp_);

	//
	HTTP_SET_GET_METHOD();

	return 0;
}
WEIBO_url_callback(GET_TRENDS_DAYLIY)//trends/daily 按日期返回热门话题。返回某一日期的话题
{
	int ret = -1;
	WEIBO_struct_cast(t_wb_trends_daily);

	URICHECK_RETURN( Weibo_url_get_uri(URI,WEIBO_OPTION(GET_TRENDS_DAYLIY), req_ex) );

	//baseapp
	Weibo_url_generate_URI_INT(URI,HTTP_T("&base_app"), pstruct->baseapp_ );

	HTTP_SET_GET_METHOD();

	return 0;
}
WEIBO_url_callback(GET_TRENDS_WEEKLIY)//trends/weekly 按周返回热门话题。返回某一日期之前某一周的话题
{
	int ret = -1;
	WEIBO_struct_cast(t_wb_trends_weekly);

	URICHECK_RETURN( Weibo_url_get_uri(URI,WEIBO_OPTION(GET_TRENDS_WEEKLIY), req_ex) );

	//baseapp
	Weibo_url_generate_URI_INT(URI,HTTP_T("&base_app"), pstruct->baseapp_ );

	HTTP_SET_GET_METHOD();

	return 0;
}

// 黑名单接口 ,by welbon,2011-01-10
WEIBO_url_callback(POST_BLOCKS_CREATE)//将某用户加入黑名单
{
	int ret = -1;
	WEIBO_struct_cast(t_wb_blocks_create);

	URICHECK_RETURN( Weibo_url_get_uri(URI,WEIBO_OPTION(POST_BLOCKS_CREATE), req_ex) );
	//
	Weibo_url_generate_URI(URI,HTTP_T("&user_id")     ,pstruct->usrid_, PARAM_ENCODE_UTF8 );
	Weibo_url_generate_URI(URI,HTTP_T("&screen_name") ,pstruct->screenname_, PARAM_ENCODE_UTF8 );


	HTTP_SET_POST_METHOD();

	return 0;
}
WEIBO_url_callback(POST_BLOCKS_DESTROY)//将某用户移出黑名单
{
	int ret = -1;
	WEIBO_struct_cast(t_wb_blocks_destroy);

	URICHECK_RETURN( Weibo_url_get_uri(URI,WEIBO_OPTION(POST_BLOCKS_DESTROY), req_ex) );
	//
	Weibo_url_generate_URI(URI,HTTP_T("&user_id")     ,pstruct->usrid_, PARAM_ENCODE_UTF8 );
	Weibo_url_generate_URI(URI,HTTP_T("&screen_name") ,pstruct->screenname_, PARAM_ENCODE_UTF8 );


	HTTP_SET_POST_METHOD();

	return 0;
}
WEIBO_url_callback(GET_BLOCKS_EXISTS)//检测某用户是否是黑名单用户
{
	int ret = -1;
	WEIBO_struct_cast(t_wb_blocks_exist);

	URICHECK_RETURN( Weibo_url_get_uri(URI,WEIBO_OPTION(GET_BLOCKS_EXISTS), req_ex) );
	//
	Weibo_url_generate_URI(URI,HTTP_T("&user_id")     ,pstruct->usrid_, PARAM_ENCODE_UTF8 );
	Weibo_url_generate_URI(URI,HTTP_T("&screen_name") ,pstruct->screenname_, PARAM_ENCODE_UTF8 );

	HTTP_SET_GET_METHOD();

	return 0;
}
WEIBO_url_callback(GET_BLOCKS_BLOCKING)//列出黑名单用户(输出用户详细信息)
{
	int ret = -1;
	WEIBO_struct_cast(t_wb_blocks_blocking);

	URICHECK_RETURN( Weibo_url_get_uri(URI,WEIBO_OPTION(GET_BLOCKS_BLOCKING), req_ex) );

	//page 
	Weibo_url_generate_URI_INT(URI,HTTP_T("&page"), pstruct->page_);
	//count
	Weibo_url_generate_URI_INT(URI,HTTP_T("&count"), pstruct->count_);
	//

	//
	HTTP_SET_GET_METHOD();

	return 0;
}
WEIBO_url_callback(GET_BLOCKS_BLOCKING_IDS)//列出分页黑名单用户（只输出id）
{
	int ret = -1;
	WEIBO_struct_cast(t_wb_blocks_blocking_ids);

	//
	URICHECK_RETURN( Weibo_url_get_uri(URI,WEIBO_OPTION(GET_BLOCKS_BLOCKING_IDS), req_ex) );
	//page 
	Weibo_url_generate_URI_INT(URI,HTTP_T("&page"), pstruct->page_);
	//count
	Weibo_url_generate_URI_INT(URI,HTTP_T("&count"), pstruct->count_);
	
	//
	HTTP_SET_GET_METHOD();

	return 0;
}

//用户标签接口 ,by welbon,2011-01-10
WEIBO_url_callback(GET_TAGS)//tags 返回指定用户的标签列表
{
	int ret = -1;
	WEIBO_struct_cast(t_wb_tags);
	//
	URICHECK_RETURN( Weibo_url_get_uri(URI,WEIBO_OPTION(GET_TAGS), req_ex) );
	//user id
	Weibo_url_generate_URI(URI,HTTP_T("&user_id"), pstruct->usrid_,PARAM_ENCODE_UTF8);
	//page 
	Weibo_url_generate_URI_INT(URI,HTTP_T("&page"), pstruct->page_);
	//count
	Weibo_url_generate_URI_INT(URI,HTTP_T("&count"), pstruct->count_);

	//Http method
	HTTP_SET_GET_METHOD();

	return 0;
}
WEIBO_url_callback(POST_TAGS_CREATE)//tags/create 添加用户标签
{
	int ret = -1;
	WEIBO_struct_cast(t_wb_tags_create);
	//
	URICHECK_RETURN( Weibo_url_get_uri(URI,WEIBO_OPTION(POST_TAGS_CREATE), req_ex) );
	//user id
	Weibo_url_generate_URI(URI,HTTP_T("&tags"), pstruct->tags_,PARAM_ENCODE_UTF8);

	//Http method
	HTTP_SET_POST_METHOD();

	return 0;
}
WEIBO_url_callback(GET_TAGS_SUGGESTIONS)//tags/suggestions 返回用户感兴趣的标签
{
	int ret = -1;
	WEIBO_struct_cast(t_wb_tags_suggestions);

	//
	URICHECK_RETURN( Weibo_url_get_uri(URI,WEIBO_OPTION(GET_TAGS_SUGGESTIONS), req_ex) );
	//page 
	Weibo_url_generate_URI_INT(URI,HTTP_T("&page"), pstruct->page_);
	//count
	Weibo_url_generate_URI_INT(URI,HTTP_T("&count"), pstruct->count_);

	//Http method
	HTTP_SET_GET_METHOD();

	return 0;
}
WEIBO_url_callback(POST_TAGS_DESTROY)//tags/destroy 删除标签
{
	int ret = -1;
	WEIBO_struct_cast(t_wb_tags_destroy);

	//
	URICHECK_RETURN( Weibo_url_get_uri(URI,WEIBO_OPTION(POST_TAGS_DESTROY), req_ex) );
	//id 
	Weibo_url_generate_URI(URI,HTTP_T("&tag_id"), pstruct->tagId_,PARAM_ENCODE_UTF8);

	//Http method
	HTTP_SET_POST_METHOD();

	return 0;
}
WEIBO_url_callback(POST_TAGS_DESTROY_BATCH)//tags/destroy_batch 批量删除标签
{
	int ret = -1;
	WEIBO_struct_cast(t_wb_tags_destroy_batch);

	//
	URICHECK_RETURN( Weibo_url_get_uri(URI,WEIBO_OPTION(POST_TAGS_DESTROY_BATCH), req_ex) );
	//id 
	Weibo_url_generate_URI(URI,HTTP_T("&ids"), pstruct->ids_,PARAM_ENCODE_UTF8);

	//Http method
	HTTP_SET_POST_METHOD();

	return 0;
}


WEIBO_url_callback(POST_INVITE_MAILCONTACT)//邀请邮箱联系人
{
	int ret = -1;
	WEIBO_struct_cast(t_wb_invite_mailcontect);
	//
	Weibo_url_get_uri(URI,WEIBO_OPTION(POST_INVITE_MAILCONTACT),req_ex,false);
	// 增加尾部随机数的处理
	HTTP_TCSCAT_S(URI,HTTP_T("?"));
	Weibo_url_random(URI);
	//
	Weibo_url_generate_URI(URI,HTTP_T("&user"), pstruct->usrid_,PARAM_ENCODE_UTF8);
	Weibo_url_generate_URI(URI,HTTP_T("&password"), pstruct->usrpwd_,PARAM_ENCODE_UTF8);
	Weibo_url_generate_URI_INT(URI,HTTP_T("&type"), pstruct->type_ );
	

	//Http method
	HTTP_SET_POST_METHOD();

	return 0;
}

WEIBO_url_callback(POST_INVITE_MSNCONTACT)//邀请MSN联系人
{
	int ret = -1;
	WEIBO_struct_cast(t_wb_invite_msncontect);
	//
	Weibo_url_get_uri(URI,WEIBO_OPTION(POST_INVITE_MSNCONTACT),req_ex,false);

	// 增加尾部随机数的处理
	HTTP_TCSCAT_S(URI,HTTP_T("?"));
	Weibo_url_random(URI);
	//
	Weibo_url_generate_URI(URI,HTTP_T("&user"), pstruct->usrid_,PARAM_ENCODE_UTF8 );
	Weibo_url_generate_URI(URI,HTTP_T("&password"), pstruct->usrpwd_,PARAM_ENCODE_UTF8 );

	//Http method
	HTTP_SET_POST_METHOD();

	return 0;
}

WEIBO_url_callback(POST_INVITE_SENDMAILS)//发送邀请邮件
{
	int ret = -1;
	WEIBO_struct_cast(t_wb_invite_sendmails);
	//
	Weibo_url_get_uri(URI,WEIBO_OPTION(POST_INVITE_SENDMAILS),req_ex,false);

	// 增加尾部随机数的处理
	HTTP_TCSCAT_S(URI,HTTP_T("?"));
	Weibo_url_random(URI);
	//
	Weibo_url_generate_URI(URI,HTTP_T("&mymail")  , pstruct->myusrid_ , PARAM_ENCODE_UTF8);
	Weibo_url_generate_URI(URI,HTTP_T("&mailtype"), pstruct->mailtype_, PARAM_ENCODE_UTF8);
	Weibo_url_generate_URI(URI,HTTP_T("&emails")  , pstruct->maillist_, PARAM_ENCODE_UTF8);
	Weibo_url_generate_URI(URI,HTTP_T("&nickName"), pstruct->nickname_, PARAM_ENCODE_UTF8);
	
	//Http method
	HTTP_SET_POST_METHOD();

	return 0;
}

WEIBO_url_callback(GET_MEDIA_SHORTURL_BATCH)//发送邀请邮件
{
	int ret = -1;
	WEIBO_struct_cast(t_wb_media_shorturl_batch);

	URICHECK_RETURN( Weibo_url_get_uri(URI,WEIBO_OPTION(GET_MEDIA_SHORTURL_BATCH), req_ex) );
	//
	//Weibo_url_get_uri(URI,WEIBO_OPTION(GET_MEDIA_SHORTURL_BATCH),req_ex,false);
	Weibo_url_generate_URI(URI,HTTP_T("&url_short")  , pstruct->urlids_, PARAM_ENCODE_UTF8);

	//Http method
	HTTP_SET_GET_METHOD();

	return 0;
}





typedef int (*f_url_callback)(lohttp::HTTPChar URI[] , int* httpmethod , const void* t_wb , const struct t_wb_REQ_EX* req_ex);
static 
f_url_callback vector_url_callbak[]=
{
	WEIBO_url_fun(UNK),
	WEIBO_url_fun(BASE),

	//获取下行数据集(timeline)接口 
	WEIBO_url_fun(GETSTATUSES_PUBLIC_TIMELINE),//2 获取最新更新的公共微博消息 
	WEIBO_url_fun(GETSTATUSES_FRIENDS_TIMELINE),//3 获取当前用户所关注用户的最新微博信息 (别名: statuses/home_timeline) 
	WEIBO_url_fun(GETSTATUSES_USE_TIMELINE),//4 获取用户发布的微博信息列表 
	WEIBO_url_fun(GETSTATUSES_MENTIONS),//5 获取@当前用户的微博列表 
	WEIBO_url_fun(GETSTATUSES_COMMENTS_TIMELINE),//6 获取当前用户发送及收到的评论列表
	WEIBO_url_fun(GETSTATUSES_COMMENTS_BYME),//7 获取当前用户发出的评论 
	WEIBO_url_fun(GETSTATUSES_COMMENTS_TOME),//7 获取当前用户收到的评论 
	WEIBO_url_fun(GETSTATUSES_COMMENTS_LIST),//8 获取指定微博的评论列表 
	WEIBO_url_fun(GETSTATUSES_COMMENTS_COUNTS),//9 批量获取一组微博的评论数及转发数 
	WEIBO_url_fun(GETSTATUSES_UNREAD), //10 获取当前用户未读消息数
	WEIBO_url_fun(PUTSTATUSES_RESET_COUNT),//10－1未读消息数清零接口 

	//微博访问接口 
	WEIBO_url_fun(GETSTATUSES_SHOW),//11 根据ID获取单条微博信息内容 
	WEIBO_url_fun(GOTOSTATUSES_ID),//12 根据微博ID和用户ID跳转到单条微博页面 
	WEIBO_url_fun(PUTSTATUSES_UPDATE),//13 发布一条微博信息 
	WEIBO_url_fun(PUTSTATUSES_UPLOAD),//14 上传图片并发布一条微博信息
	WEIBO_url_fun(PUTSTATUSES_DESTROY),//15 删除一条微博信息 
	WEIBO_url_fun(PUTSTATUSES_REPOST),//16 转发一条微博信息（可加评论） 
	WEIBO_url_fun(PUTSTATUSES_COMMENT),//17 对一条微博信息进行评论 
	WEIBO_url_fun(PUTSTATUSES_COMMENT_DESTROY),//18 删除当前用户的微博评论信息 
	WEIBO_url_fun(PUTSTATUSES_REPLY),//19 回复微博评论信息

	//用户接口 
	WEIBO_url_fun(GETUSER_INFO),//20 根据用户ID获取用户资料（授权用户）
	WEIBO_url_fun(GETFRINDS_LIST),//21 获取当前用户关注对象列表及最新一条微博信息
	WEIBO_url_fun(GETFOLLOWERS_LIST),//22 获取当前用户粉丝列表及最新一条微博信息 

	//私信接口 
	WEIBO_url_fun(GETDIRECTMSG),//23 获取当前用户最新私信列表 
	WEIBO_url_fun(GETDIRESTMSG_SENT),//24 获取当前用户发送的最新私信列表
	WEIBO_url_fun(PUTDIRECTMSG_NEW),//25 发送一条私信
	WEIBO_url_fun(PUTDIRECTMSG_DESTROY),//26 删除一条私信
	WEIBO_url_fun(GETDIRECTMSG_WITH),//获取来往私信列表

	//关注接口 
	WEIBO_url_fun(PUTFRIENDSHIPS_CREATE),//27 关注某用户
	WEIBO_url_fun(PUTFRIENDSHIPS_CREATE_BATCH),//批量关注
	WEIBO_url_fun(PUTFRIENDSHIPS_DESTROY),//28 取消关注
	WEIBO_url_fun(GETFRIENDSHIPS_EXISTS),//29 判断两个用户是否有关注关系，返回两个用户关系的详细情况
	WEIBO_url_fun(GETFRIENDSHIPS_BATCH_EXISTS),//批量获取一组用户是否为好友

	//Social Graph接口
	WEIBO_url_fun(GETFRIEND_IDS),//30 关注列表
	WEIBO_url_fun(GETFOLLOWER_IDS),//31 粉丝列表

	//账号接口 
	WEIBO_url_fun(GETACCOUNT_VERIFY),//32 验证当前用户身份是否合法 
	WEIBO_url_fun(GETACCOUNT_RATELIMIT),//33 获取当前用户API访问频率限制
	WEIBO_url_fun(PUTACCOUNT_QUITSESSION),//34 当前用户退出登录 
	WEIBO_url_fun(PUTACCOUNT_UPDATE_PROFILEIMAGE),//35 更改头像
	WEIBO_url_fun(PUTACCOUNT_UPDATE_PROFILE),//36 更改资料
	WEIBO_url_fun(PUTACCOUNT_REGISTER),

	//收藏接口 
	WEIBO_url_fun(GETFAVORITES),// 38获取当前用户的收藏列表 
	WEIBO_url_fun(PUTFAVORITES_CREATE),// 39添加收藏
	WEIBO_url_fun(PUTFAVORITES_DESTROY),// 40删除当前用户收藏的微博信息

	//登录/OAuth接口 
	WEIBO_url_fun(OAUTH_REQUEST_TOKEN),// 41获取未授权的Request Token
	WEIBO_url_fun(OAUTH_AUTHORIZE),
	WEIBO_url_fun(OAUTH_ACCESS_TOKEN),// 43获取授权过的Access Token

	// 表情
	WEIBO_url_fun(GET_EMOTIONS),// 44 返回新浪微博官方所有表情、魔法表情的相关信息。包括短语、表情类型、表情分类，是否热门等。

	// 用户搜索 
	WEIBO_url_fun(GET_USERS_SEARCH),// 45 搜索微博用户,返回关键字匹配的微博用户，(仅对新浪合作开发者开放) 

	// 微博搜索 
	WEIBO_url_fun(GET_WB_SEARCH),// 46 返回关键字匹配的微博，(仅对新浪合作开发者开放)
	WEIBO_url_fun(GET_STATUSES_SEARCH),//47 搜索微博(多条件组合) (仅对合作开发者开放) 

	WEIBO_url_fun(GET_PROVINCES),// 48 省份城市编码表 
	WEIBO_url_fun(REPORT),//49 举报
	// cookie 
	WEIBO_url_fun(COOKIE),
	WEIBO_url_fun(UPDATETGT),// UPDATETGT,
	// buffer
	WEIBO_url_fun(CUSTOM),

	// hot point 
	WEIBO_url_fun(HOT_REPOST_DAYLIY),
	WEIBO_url_fun(HOT_REPOST_WEEKLY),
	WEIBO_url_fun(HOT_COMMENT_DAYLIY),
	WEIBO_url_fun(HOT_COMMENT_WEEKLY),

	//user interface new
	WEIBO_url_fun(GET_USERS_HOT),// 获取系统推荐用户
	WEIBO_url_fun(POST_USERS_REMARK),//更新修改当前登录用户所关注的某个好友的备注信息New!
	WEIBO_url_fun(GET_USERS_SUGGESTIONS), //Users/suggestions 返回当前用户可能感兴趣的用户

	// 话题接口 ,by welbon,2011-01-10
	WEIBO_url_fun(GET_TRENDS),//trends 获取某人的话题
	WEIBO_url_fun(GET_TRENDS_STATUSES),//trends/statuses 获取某一话题下的微博
	WEIBO_url_fun(POST_TRENDS_FOLLOW),//trends/follow 关注某一个话题
	WEIBO_url_fun(DELETE_TRENDS_DESTROY),//trends/destroy 取消关注的某一个话题
	WEIBO_url_fun(GET_TRENDS_HOURLY),//trends/destroy 按小时返回热门话题
	WEIBO_url_fun(GET_TRENDS_DAYLIY),//trends/daily 按日期返回热门话题。返回某一日期的话题
	WEIBO_url_fun(GET_TRENDS_WEEKLIY),//trends/weekly 按周返回热门话题。返回某一日期之前某一周的话题

	// 黑名单接口 ,by welbon,2011-01-10
	WEIBO_url_fun(POST_BLOCKS_CREATE),//将某用户加入黑名单
	WEIBO_url_fun(POST_BLOCKS_DESTROY),//将某用户移出黑名单
	WEIBO_url_fun(GET_BLOCKS_EXISTS),//检测某用户是否是黑名单用户
	WEIBO_url_fun(GET_BLOCKS_BLOCKING),//列出黑名单用户(输出用户详细信息)
	WEIBO_url_fun(GET_BLOCKS_BLOCKING_IDS),//列出分页黑名单用户（只输出id）

	//用户标签接口 ,by welbon,2011-01-10
	WEIBO_url_fun(GET_TAGS),//tags 返回指定用户的标签列表
	WEIBO_url_fun(POST_TAGS_CREATE),//tags/create 添加用户标签
	WEIBO_url_fun(GET_TAGS_SUGGESTIONS),//tags/suggestions 返回用户感兴趣的标签
	WEIBO_url_fun(POST_TAGS_DESTROY),//tags/destroy 删除标签
	WEIBO_url_fun(POST_TAGS_DESTROY_BATCH),//tags/destroy_batch 批量删除标签

	// Invite mail
	WEIBO_url_fun(POST_INVITE_MAILCONTACT),
	WEIBO_url_fun(POST_INVITE_MSNCONTACT) ,
	WEIBO_url_fun(POST_INVITE_SENDMAILS)  ,

	// Media
	WEIBO_url_fun(GET_MEDIA_SHORTURL_BATCH) ,
	//登录/XAuth接口 
	WEIBO_url_fun(XAUTH_ACCESS_TOKEN),

};

int Weibo_url_set(lohttp::LOHttp* pHttp , WEIBOoption option , const void* t_wb , const struct t_wb_REQ_EX* req_ex)
{
	lohttp::HTTPChar URI[URI_MAX_LEN] = {0};
	int httpmethod = 0;

	
	if( WEIBO_OPTION(UNK) >= option ||
		WEIBO_OPTION(LAST) <= option )
	{
		return -1;//超出范围
	}

	if( 0 == vector_url_callbak[option](URI , &httpmethod ,  t_wb ,req_ex) ) 
	{
		// 执行( 特别处理 )
	    if( WEIBO_OPTION(OAUTH_REQUEST_TOKEN) == option )
		{// 不需要 t_key AND t_scecet
			return Weibo_url_http_seturl(pHttp , URI  , httpmethod , 0 , req_ex);
		}
		else if( WEIBO_OPTION(OAUTH_AUTHORIZE) == option )
		{// 这里是不需要参与运算的 add by welbon,2010-09-10

			lohttp::Http_setopt(pHttp , lohttp::LOHTTP_OPT_setmethod , httpmethod );
			lohttp::Http_seturl(pHttp , Char_2HTTPChar(URI) );
			return 0;
		}
		else if( WEIBO_OPTION(XAUTH_ACCESS_TOKEN) == option )
		{
			return Weibo_url_http_seturl(pHttp , URI  , httpmethod , 0 , req_ex);
		}
		else if( WEIBO_OPTION(PUTSTATUSES_UPLOAD) == option )
		{// 表单方式			
			return Weibo_url_upload_form(pHttp ,URI  , httpmethod ,  t_wb , "pic" , req_ex );
		}
		else if( WEIBO_OPTION(PUTACCOUNT_UPDATE_PROFILEIMAGE) == option )
		{// (修改个人头像) 表单方式
			return Weibo_url_upload_form(pHttp ,URI  , httpmethod ,  t_wb ,  "image", req_ex );
		}
		else if( WEIBO_OPTION(GET_PROVINCES) == option )
		{// 不需要运算
			lohttp::Http_setopt(pHttp , lohttp::LOHTTP_OPT_setmethod , httpmethod );
			lohttp::Http_seturl(pHttp , Char_2HTTPChar(URI) );
			return 0;
		}
		else if( (WEIBO_OPTION(UPDATETGT) == option)/* || 
			(WEIBO_OPTION(POST_INVITE_MAILCONTACT) == option) ||
			(WEIBO_OPTION(POST_INVITE_MSNCONTACT) == option)*/ ) 
		{
			WEIBO_struct_cast(t_wb_oauth);
			Weibo_url_cookie_set(pHttp , pstruct->sue_ , pstruct->sup_ );
			lohttp::Http_seturl(pHttp , Char_2HTTPChar(URI) );
			return 0;
		}
		else if( WEIBO_OPTION(CUSTOM) == option )
		{
			Weibo_url_custom_seturl(pHttp , t_wb );
			return 0;
		}
		else
		{
			return Weibo_url_http_seturl(pHttp , URI  , httpmethod , t_wb , req_ex);
		}
	}
	return -1;
}

// 获取
int Weibo_url_get(struct t_wb_URI* outURI , WEIBOoption option , const void* t_wb , const struct t_wb_REQ_EX* req_ex)
{
	int  ret = -1;
	lohttp::HTTPChar URI[URI_MAX_LEN]={0};
	int httpmethod = 0;
	char* req_url = 0;
	char* postarg = 0;

	if( WEIBO_OPTION(BASE) >= option ||
		WEIBO_OPTION(LAST) <= option || !outURI )
	{
		return -1;//超出范围
	}

	if( 0 != vector_url_callbak[option](URI , &httpmethod ,  t_wb , req_ex) ) 
		return -1;

	ret = Weibo_url_http_geturl( &req_url , &postarg , URI , httpmethod , t_wb , req_ex );

	if( 0 == ret )
	{
		if( req_url) strcpy(outURI->uri_ , req_url );
		if(postarg)  strcpy(outURI->post_ , postarg );
	}

	SafeFreeA_lo(req_url);
	SafeFreeA_lo(postarg);

	return ret;
}
