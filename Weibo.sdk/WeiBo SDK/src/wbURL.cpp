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

// �ϴ��ļ� form��ʽ 

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
	{// ��ԭû�� urlencode֮ǰ�� ���� һ��post ����һ��
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

	// �����ڲ�
	while( pupload->postcontent[0] != '\0' )
	{
		if( *post == '\0' )
		{// ���һ��������ֻ��һ�����������
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
		{// һ���������
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

	// ��ȡ�ļ�
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
	{// Ĭ�ϵ�
		c_key =  Weibo_consumer_key(); ///< consumer key
	}

	if( !c_secret || *c_secret =='\0' )
	{// Ĭ�ϵ�
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
			return -1; // oauth �����uriΪ��
		}
	}
	else
	{// Get

		*req_url = oauth_sign_url2( HTTPChar_2Char(uri), NULL, OA_HMAC, NULL, c_key, c_secret , t_key , t_secret );

		if( !(*req_url) )
		{
			SafeFreeA_lo( outstr );
			return -1; // oauth �����uriΪ��
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
//	// ����
//	if( 0 != Weibo_url_oauth_url(&req_url , &postarg , uri , post , t_key , t_secret) )
//	{//ʧ��
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
	// ����β��������Ĵ���
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

// ��֤��ÿ���ӿڶ���Ҫ���� oauth����֤���ƣ��㷨��
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

		//��ȡ�������ݼ�(timeline)�ӿ� 
		{WEIBO_OPTION(GETSTATUSES_PUBLIC_TIMELINE)      , HTTP_T("/statuses/public_timeline")},// ��ȡ���¸��µĹ���΢����Ϣ 
		{WEIBO_OPTION(GETSTATUSES_FRIENDS_TIMELINE)     , HTTP_T("/statuses/friends_timeline")},// ��ȡ��ǰ�û�����ע�û�������΢����Ϣ (����: statuses/home_timeline) 
		{WEIBO_OPTION(GETSTATUSES_USE_TIMELINE)         , HTTP_T("/statuses/user_timeline")},// ��ȡ�û�������΢����Ϣ�б� 
		{WEIBO_OPTION(GETSTATUSES_MENTIONS)             , HTTP_T("/statuses/mentions")},// ��ȡ@��ǰ�û���΢���б� 
		{WEIBO_OPTION(GETSTATUSES_COMMENTS_TIMELINE)    , HTTP_T("/statuses/comments_timeline")},// ��ȡ��ǰ�û����ͼ��յ��������б� 
		{WEIBO_OPTION(GETSTATUSES_COMMENTS_BYME)        , HTTP_T("/statuses/comments_by_me")},// ��ȡ��ǰ�û����������� 
		{WEIBO_OPTION(GETSTATUSES_COMMENTS_TOME)        , HTTP_T("/statuses/comments_to_me")},// ��ȡ��ǰ�û����������� 
		{WEIBO_OPTION(GETSTATUSES_COMMENTS_LIST)        , HTTP_T("/statuses/comments")},// ��ȡָ��΢���������б� 
		{WEIBO_OPTION(GETSTATUSES_COMMENTS_COUNTS)      , HTTP_T("/statuses/counts")},// ������ȡһ��΢������������ת���� 
		{WEIBO_OPTION(GETSTATUSES_UNREAD)               , HTTP_T("/statuses/unread")},// ��ȡ��ǰ�û�δ����Ϣ�� 
		{WEIBO_OPTION(PUTSTATUSES_RESET_COUNT)          , HTTP_T("/statuses/reset_count")},// ����ǰ�û�ָ�����͵�δ����Ϣ����0��

		//΢�����ʽӿ� 
		{WEIBO_OPTION(GETSTATUSES_SHOW)           , HTTP_T("/statuses/show")},// ����ID��ȡ����΢����Ϣ���� 
		{WEIBO_OPTION(GOTOSTATUSES_ID)            , HTTP_T("%s/%s/statuses/%s")},// ����΢��ID���û�ID��ת������΢��ҳ�� 
		{WEIBO_OPTION(PUTSTATUSES_UPDATE)         , HTTP_T("/statuses/update")},// ����һ��΢����Ϣ 
		{WEIBO_OPTION(PUTSTATUSES_UPLOAD)         , HTTP_T("/statuses/upload")},// �ϴ�ͼƬ������һ��΢����Ϣ 
		{WEIBO_OPTION(PUTSTATUSES_DESTROY)        , HTTP_T("/statuses/destroy")},// ɾ��һ��΢����Ϣ 
		{WEIBO_OPTION(PUTSTATUSES_REPOST)         , HTTP_T("/statuses/repost")},// ת��һ��΢����Ϣ���ɼ����ۣ� 
		{WEIBO_OPTION(PUTSTATUSES_COMMENT)        , HTTP_T("/statuses/comment")},// ��һ��΢����Ϣ�������� 
		{WEIBO_OPTION(PUTSTATUSES_COMMENT_DESTROY), HTTP_T("/statuses/comment_destroy")},// ɾ����ǰ�û���΢��������Ϣ 
		{WEIBO_OPTION(PUTSTATUSES_REPLY)          , HTTP_T("/statuses/reply")},// �ظ�΢��������Ϣ 

		//�û��ӿ� 
		{WEIBO_OPTION(GETUSER_INFO)          , HTTP_T("/users/show")},// �����û�ID��ȡ�û����ϣ���Ȩ�û��� 
		{WEIBO_OPTION(GETFRINDS_LIST)        , HTTP_T("/statuses/friends")},// ��ȡ��ǰ�û���ע�����б�����һ��΢����Ϣ 
		{WEIBO_OPTION(GETFOLLOWERS_LIST)     , HTTP_T("/statuses/followers")},// ��ȡ��ǰ�û���˿�б�����һ��΢����Ϣ 

		//˽�Žӿ� 
		{WEIBO_OPTION(GETDIRECTMSG)         , HTTP_T("/direct_messages")},// ��ȡ��ǰ�û�����˽���б� 
		{WEIBO_OPTION(GETDIRESTMSG_SENT)    , HTTP_T("/direct_messages/sent")},// ��ȡ��ǰ�û����͵�����˽���б� 
		{WEIBO_OPTION(PUTDIRECTMSG_NEW)     , HTTP_T("/direct_messages/new")},// ����һ��˽�� 
		{WEIBO_OPTION(PUTDIRECTMSG_DESTROY) , HTTP_T("/direct_messages/destroy")},// ɾ��һ��˽�� 
		{WEIBO_OPTION(GETDIRECTMSG_WITH)    , HTTP_T("/direct_messages/with")},// ��ȡ����˽���б�

		//��ע�ӿ� 
		{WEIBO_OPTION(PUTFRIENDSHIPS_CREATE)		 ,HTTP_T("/friendships/create")},// ��עĳ�û� 
		{WEIBO_OPTION(PUTFRIENDSHIPS_CREATE_BATCH)	 ,HTTP_T("/friendships/create_batch")},// ������ע
		{WEIBO_OPTION(PUTFRIENDSHIPS_DESTROY)		 ,HTTP_T("/friendships/destroy")},// ȡ����ע 
		{WEIBO_OPTION(GETFRIENDSHIPS_EXISTS)		 ,HTTP_T("/friendships/show")},// ��ȡ�����û���ϵ����ϸ��� 
		{WEIBO_OPTION(GETFRIENDSHIPS_BATCH_EXISTS)   ,HTTP_T("/friendships/batch_exists")},// ������ȡһ���û��Ƿ�Ϊ����

		 //Social Graph�ӿ�
		{WEIBO_OPTION(GETFRIEND_IDS)           , HTTP_T("/friends/ids")},// ��ȡ�û���ע����uid�б� 
		{WEIBO_OPTION(GETFOLLOWER_IDS)         , HTTP_T("/followers/ids")},// ��ȡ�û���˿����uid�б� 

		//�˺Žӿ� 
		{WEIBO_OPTION(GETACCOUNT_VERIFY)             , HTTP_T("/account/verify_credentials")},// ��֤��ǰ�û�����Ƿ�Ϸ� 
		{WEIBO_OPTION(GETACCOUNT_RATELIMIT)          , HTTP_T("/account/rate_limit_status")},// ��ȡ��ǰ�û�API����Ƶ������ 
		{WEIBO_OPTION(PUTACCOUNT_QUITSESSION)        , HTTP_T("/account/end_session")},// ��ǰ�û��˳���¼ 
		{WEIBO_OPTION(PUTACCOUNT_UPDATE_PROFILEIMAGE), HTTP_T("/account/update_profile_image")},// ����ͷ�� 
		{WEIBO_OPTION(PUTACCOUNT_UPDATE_PROFILE)     , HTTP_T("/account/update_profile")},// �������� 
		{WEIBO_OPTION(PUTACCOUNT_REGISTER)           , HTTP_T("/account/register")},// ע������΢���ʺ� 

		//�ղؽӿ� 
		{WEIBO_OPTION(GETFAVORITES)            , HTTP_T("/favorites")},// ��ȡ��ǰ�û����ղ��б� 
		{WEIBO_OPTION(PUTFAVORITES_CREATE)     , HTTP_T("/favorites/create")},// ����ղ� 
		{WEIBO_OPTION(PUTFAVORITES_DESTROY)    , HTTP_T("/favorites/destroy")},// ɾ����ǰ�û��ղص�΢����Ϣ 

		//��¼/OAuth�ӿ�
		{WEIBO_OPTION(OAUTH_REQUEST_TOKEN)     , HTTP_T("/oauth/request_token")},// ��ȡδ��Ȩ��Request Token 
		{WEIBO_OPTION(OAUTH_AUTHORIZE)         , HTTP_T("/oauth/authorize")},// �����û���ȨToken 
		{WEIBO_OPTION(OAUTH_ACCESS_TOKEN)      , HTTP_T("/oauth/access_token")},// ��ȡ��Ȩ����Access Token 

		// ����
		{WEIBO_OPTION(GET_EMOTIONS)  , HTTP_T("/emotions")},// ��������΢���ٷ����б��顢ħ������������Ϣ����������������͡�������࣬�Ƿ����ŵȡ�

		// �û����� 
		{WEIBO_OPTION(GET_USERS_SEARCH)  , HTTP_T("/users/search")},// ���عؼ���ƥ���΢���û�
		
		// ΢������ 
		{WEIBO_OPTION(GET_WB_SEARCH)       , HTTP_T("/search")},// 46 ���عؼ���ƥ���΢����(�������˺��������߿���) 
		{WEIBO_OPTION(GET_STATUSES_SEARCH) , HTTP_T("/statuses/search")},//47 ����΢��(���������) (���Ժ��������߿���) 

		{WEIBO_OPTION(GET_PROVINCES) , HTTP_T("/provinces")},//48 ʡ�ݳ��б���� 
		{WEIBO_OPTION(REPORT)  , HTTP_T("/report_spam")},//49 �ٱ�

		// cookie
		{WEIBO_OPTION(COOKIE)  , HTTP_T("/sso/login.php")},// ��ȡCOOKIE
		{WEIBO_OPTION(UPDATETGT)  , HTTP_T("/sso/updatetgt.php")},// Update TGT https://login.sina.com.cn/

		// custom
		{WEIBO_OPTION(CUSTOM)  , HTTP_T("")},

		// ���Žӿ�
		{WEIBO_OPTION(HOT_REPOST_DAYLIY)   ,HTTP_T("/statuses/hot/repost_daily")   }, //����ת��-by day
		{WEIBO_OPTION(HOT_REPOST_WEEKLY)   ,HTTP_T("/statuses/hot/repost_weekly")  }, //����ת��-by week
		{WEIBO_OPTION(HOT_COMMENT_DAYLIY)  ,HTTP_T("/statuses/hot/comments_daily") }, //��������-by day
		{WEIBO_OPTION(HOT_COMMENT_WEEKLY)  ,HTTP_T("/statuses/hot/comments_weekly")}, //��������-by week

		// �û��ӿڣ�NEW��
		{WEIBO_OPTION(GET_USERS_HOT)		,HTTP_T("/users/hot")},// ��ȡϵͳ�Ƽ��û�
		{WEIBO_OPTION(POST_USERS_REMARK)	,HTTP_T("/user/friends/update_remark")},//�����޸ĵ�ǰ��¼�û�����ע��ĳ�����ѵı�ע��ϢNew!
		{WEIBO_OPTION(GET_USERS_SUGGESTIONS),HTTP_T("/users/suggestions")},     //Users/suggestions ���ص�ǰ�û����ܸ���Ȥ���û�

		// ����ӿ�
		{WEIBO_OPTION(GET_TRENDS)			,HTTP_T("/trends")},			//trends ��ȡĳ�˵Ļ���
		{WEIBO_OPTION(GET_TRENDS_STATUSES)	,HTTP_T("/trends/statuses")},	//trends/statuses ��ȡĳһ�����µ�΢��
		{WEIBO_OPTION(POST_TRENDS_FOLLOW)	,HTTP_T("/trends/follow")},		//trends/follow ��עĳһ������
		{WEIBO_OPTION(DELETE_TRENDS_DESTROY),HTTP_T("/trends/destroy")},	//trends/destroy ȡ����ע��ĳһ������
		{WEIBO_OPTION(GET_TRENDS_HOURLY)	,HTTP_T("/trends/hourly")},		//trends/destroy ��Сʱ�������Ż���
		{WEIBO_OPTION(GET_TRENDS_DAYLIY)	,HTTP_T("/trends/daily")},		//trends/daily �����ڷ������Ż��⡣����ĳһ���ڵĻ���
		{WEIBO_OPTION(GET_TRENDS_WEEKLIY)	,HTTP_T("/trends/weekly")},		//trends/weekly ���ܷ������Ż��⡣����ĳһ����֮ǰĳһ�ܵĻ���

		// �������ӿ� ,by welbon,2011-01-10
		{WEIBO_OPTION(POST_BLOCKS_CREATE)	,HTTP_T("/blocks/create")},//��ĳ�û����������
		{WEIBO_OPTION(POST_BLOCKS_DESTROY)	,HTTP_T("/blocks/destroy")},//��ĳ�û��Ƴ�������
		{WEIBO_OPTION(GET_BLOCKS_EXISTS)	,HTTP_T("/blocks/exists")},//���ĳ�û��Ƿ��Ǻ������û�
		{WEIBO_OPTION(GET_BLOCKS_BLOCKING)	,HTTP_T("/blocks/bloking")},//�г��������û�(����û���ϸ��Ϣ)
		{WEIBO_OPTION(GET_BLOCKS_BLOCKING_IDS),HTTP_T("/blocks/bloking/ids")},//�г���ҳ�������û���ֻ���id��

		//�û���ǩ�ӿ�
		{WEIBO_OPTION(GET_TAGS)				,HTTP_T("/tags")},				//tags ����ָ���û��ı�ǩ�б�
		{WEIBO_OPTION(POST_TAGS_CREATE)		,HTTP_T("/tags/create")},		//tags/create ����û���ǩ
		{WEIBO_OPTION(GET_TAGS_SUGGESTIONS)	,HTTP_T("/tags/suggestions")},	//tags/suggestions �����û�����Ȥ�ı�ǩ
		{WEIBO_OPTION(POST_TAGS_DESTROY)	,HTTP_T("/tags/destroy")},		//tags/destroy ɾ����ǩ
		{WEIBO_OPTION(POST_TAGS_DESTROY_BATCH),HTTP_T("/trends/destroy_batch")},//tags/destroy_batch ����ɾ����ǩ

		//Invite Mail
		{WEIBO_OPTION(POST_INVITE_MAILCONTACT),HTTP_T("/invite/aj_mailcontact.php")},//����������ϵ��
		{WEIBO_OPTION(POST_INVITE_MSNCONTACT) ,HTTP_T("/invite/aj_msncontact.php")},//����MSN��ϵ��
		{WEIBO_OPTION(POST_INVITE_SENDMAILS)  ,HTTP_T("/invite/aj_att_sendmails.php")},//���������ʼ�

		//Media 
		{WEIBO_OPTION(GET_MEDIA_SHORTURL_BATCH),HTTP_T("/short_url/batch_info")},
		{WEIBO_OPTION(XAUTH_ACCESS_TOKEN), HTTP_T("/oauth/access_token")},// ��ȡ��Ȩ����Access Token 

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
//	// ���Դ��� test short url id : hgn2ow,hk8kn
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
		return -1;// URI Ϊ��
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
		{// ʧ��
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

// �ϴ�ͼƬ���� ����ʽ
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
std::string StringReplace(const std::string&   input,			//   ԭʼ��
					 const std::string&   find,			//   �滻Դ��
					 const std::string&   replaceWith)     //   �滻Ŀ�Ĵ�
{
	std::string strOut(input);
	int curPos = 0;

	int pos;
	while((pos = strOut.find(find, curPos)) != -1)
	{
		//   һ���滻
		strOut.replace(pos, find.size(), replaceWith);
		//   ��ֹѭ���滻!!
		curPos = pos + replaceWith.size();
	}
	return strOut;
}

//------------------------------------------------------------------------------------------------------------//

WEIBO_url_callback(UNK)
{
	return -1;//��֧��
}

WEIBO_url_callback(BASE)
{
	return -1;
}

WEIBO_url_callback(GETSTATUSES_PUBLIC_TIMELINE)
{// ��ȡ���¸��µĹ���΢����Ϣ
	int  ret = -1;
	WEIBO_struct_cast(t_wb_get_statuses_public_timeline);

	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(GETSTATUSES_PUBLIC_TIMELINE) , req_ex ) );

	HTTP_SET_GET_METHOD();

	return 0;
}

WEIBO_url_callback(GETSTATUSES_FRIENDS_TIMELINE)
{// ��ȡ��ǰ�û�����ע�û�������΢����Ϣ
	int  ret = -1;
	WEIBO_struct_cast(t_wb_get_statuses_friends_timeline);

	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(GETSTATUSES_FRIENDS_TIMELINE) , req_ex ) );

	Weibo_url_http_option_param_URI(URI , &pstruct->wbopt_ );

	HTTP_SET_GET_METHOD();

	return 0;
}

WEIBO_url_callback(GETSTATUSES_USE_TIMELINE)
{// ��ȡ�û�������΢����Ϣ�б� 
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
{// ��ȡ@��ǰ�û���΢���б�
	int  ret = -1;
	WEIBO_struct_cast(t_wb_get_statuses_mentions);

	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(GETSTATUSES_MENTIONS), req_ex) );

	Weibo_url_http_option_param_URI(URI , &pstruct->wbopt_ );

    HTTP_SET_GET_METHOD();

	return 0;
}

WEIBO_url_callback(GETSTATUSES_COMMENTS_TIMELINE)
{// ��ȡ��ǰ�û����ͼ��յ��������б� 
	int  ret = -1;
	WEIBO_struct_cast(t_wb_get_statuses_comments_timeline);

	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(GETSTATUSES_COMMENTS_TIMELINE), req_ex) );

	Weibo_url_http_option_param_URI(URI , &pstruct->wbopt_ );

	HTTP_SET_GET_METHOD();

    return 0;
}

WEIBO_url_callback(GETSTATUSES_COMMENTS_BYME)
{// ��ȡ��ǰ�û����������� 
	int  ret = -1;
	WEIBO_struct_cast(t_wb_get_statuses_comments_byme);

	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(GETSTATUSES_COMMENTS_BYME), req_ex) );

	Weibo_url_http_option_param_URI(URI , &pstruct->wbopt_ );

	HTTP_SET_GET_METHOD();

    return 0;
}

WEIBO_url_callback(GETSTATUSES_COMMENTS_TOME)
{// ��ȡ��ǰ�û��յ������� 
	int  ret = -1;
	WEIBO_struct_cast(t_wb_get_statuses_comments_tome);

	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(GETSTATUSES_COMMENTS_TOME), req_ex) );

	Weibo_url_http_option_param_URI(URI , &pstruct->wbopt_ );

	HTTP_SET_GET_METHOD();

    return 0;
}

WEIBO_url_callback(GETSTATUSES_COMMENTS_LIST)
{// ��ȡָ��΢���������б�
	int  ret = -1;
	WEIBO_struct_cast(t_wb_get_statuses_comments_list);

	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(GETSTATUSES_COMMENTS_LIST), req_ex) );

	Weibo_url_generate_URI(URI , HTTP_T("&id")  , pstruct->wbId_    , PARAM_ENCODE_UTF8 );

	Weibo_url_http_option_param_URI(URI , &pstruct->wbopt_ );

	HTTP_SET_GET_METHOD();

    return 0;
}

WEIBO_url_callback(GETSTATUSES_COMMENTS_COUNTS)
{// ������ȡһ��΢������������ת����
	int    ret = -1;
	WBChar wbIds[ BATCH_WBIDS_COUNTS * ( WB_USE_LEN(WEIBO_ID) + 2 ) ] = {0};
	WEIBO_struct_cast(t_wb_get_statuses_counts );

	// ���� , �ָ�
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

	// post method (ʹ��get��������Ȼ�������⣬����URI̫����
	HTTP_SET_POST_METHOD();

    return 0;
}

WEIBO_url_callback(GETSTATUSES_UNREAD)
{// ��ȡ��ǰ�û�δ����Ϣ����(������Ϣ)
	int  ret = -1;
	WEIBO_struct_cast(t_wb_get_statuses_unread);

	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(GETSTATUSES_UNREAD), req_ex) );

	Weibo_url_http_option_param_URI(URI , &pstruct->wbopt_ );

	HTTP_SET_GET_METHOD();
	
	return 0;
}

WEIBO_url_callback(PUTSTATUSES_RESET_COUNT)
{// ��ȡ��ǰ�û�δ����Ϣ����(������Ϣ)
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

//΢������

WEIBO_url_callback(GETSTATUSES_SHOW)
{// ����ID��ȡ����΢����Ϣ����
	int   ret = -1;
	WEIBO_struct_cast(t_wb_get_statuses_show);

	HTTP_SPRINTF(URI, HTTP_T("%s%s/%s%s"), Weibo_ip( WEIBO_OPTION(GETSTATUSES_SHOW),req_ex),Weibo_url_geturi( WEIBO_OPTION(GETSTATUSES_SHOW) ), WBChar_2HTTPChar(pstruct->wbId_) , Weibo_url_format_key(req_ex->format_) );

	Weibo_url_base( URI , pstruct , req_ex);

	HTTP_SET_GET_METHOD();

	return 0;
}

WEIBO_url_callback(GOTOSTATUSES_ID)
{// ����΢��ID���û�ID��ת������΢��ҳ��( �������),ֻ����һ��URI
	int   ret = -1;
	WEIBO_struct_cast(t_wb_goto_user_status_id);

	HTTP_SPRINTF(URI ,Weibo_url_geturi( WEIBO_OPTION(GOTOSTATUSES_ID) ) , Weibo_ip( WEIBO_OPTION(GOTOSTATUSES_ID) ,req_ex) , pstruct->uid_ , pstruct->wbId_ );

    // get method 
	HTTP_SET_GET_METHOD();

	return 0;
}

WEIBO_url_callback(PUTSTATUSES_UPDATE)
{//����һ��΢����Ϣ
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
{// �ϴ�ͼƬ������һ��΢����Ϣ
	int  ret = -1;
	WEIBO_struct_cast(t_wb_put_statuses_upload);

	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(PUTSTATUSES_UPLOAD), req_ex) );

	//Weibo_url_generate_URI(URI , HTTP_T("&status")   , CURLEncodeA(pstruct->szwbInfo_,0).c_str() , PARAM_ENCODE_UTF8 );
	Weibo_url_generate_URI(URI , HTTP_T("&status")   , StringReplace(pstruct->szwbInfo_,"%","%25").c_str() , PARAM_ENCODE_UTF8 );
	
	HTTP_SET_UPLOAD_METHOD();

	return 0;
}

WEIBO_url_callback(PUTSTATUSES_DESTROY)
{// ɾ��һ��΢����Ϣ
	int   ret = -1;
	WEIBO_struct_cast(t_wb_put_statuses_destroy);
	
	HTTP_SPRINTF(URI, HTTP_T("%s%s/%s%s"), Weibo_ip( WEIBO_OPTION(PUTSTATUSES_DESTROY),req_ex),Weibo_url_geturi( WEIBO_OPTION(PUTSTATUSES_DESTROY) ), WBChar_2HTTPChar( pstruct->wbId_ ) , Weibo_url_format_key(req_ex->format_) );

	Weibo_url_base( URI , pstruct , req_ex);

	// post method 
	HTTP_SET_POST_METHOD();

	return 0;
}

WEIBO_url_callback(PUTSTATUSES_REPOST)
{// ת��һ��΢����Ϣ���ɼ����ۣ�
	int   ret = -1;
	WEIBO_struct_cast(t_wb_put_statuses_repost);

	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(PUTSTATUSES_REPOST), req_ex) );

	Weibo_url_generate_URI(URI ,HTTP_T("&id") , pstruct->wbId_, PARAM_ENCODE_UTF8 );
	Weibo_url_generate_URI_INT(URI,HTTP_T("&is_comment"), pstruct->iCommentFlags );

	// ��ѡ��
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
{//��һ��΢����Ϣ�������� 
	int   ret = -1;
	WEIBO_struct_cast(t_wb_put_statuses_comment);

	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(PUTSTATUSES_COMMENT), req_ex) );

	// utf8 AND urlencode
	//Weibo_url_generate_URI(URI , HTTP_T("&comment")   , CURLEncodeA(pstruct->szwbInfo_,0).c_str()    , PARAM_ENCODE_UTF8 );
	Weibo_url_generate_URI(URI , HTTP_T("&comment")   , StringReplace(pstruct->szwbInfo_,"%","%25").c_str()  , PARAM_ENCODE_UTF8 );
	Weibo_url_generate_URI(URI , HTTP_T("&id")        , pstruct->wbId_    , PARAM_ENCODE_UTF8 );

	// ��ѡ��
	if( pstruct->wbCId_[0] != '\0' )
	{
		Weibo_url_generate_URI(URI , HTTP_T("&cid")        , pstruct->wbCId_    , PARAM_ENCODE_UTF8);
	}

	// post method
	HTTP_SET_POST_METHOD();

    return 0;
}

WEIBO_url_callback(PUTSTATUSES_COMMENT_DESTROY)
{// ɾ����ǰ�û���΢��������Ϣ 
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
{// �ظ�΢��������Ϣ
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

//----------------------------------�û�------------------------------------------//

WEIBO_url_callback(GETUSER_INFO)
{// �����û�ID��ȡ�û����ϣ���Ȩ�û���
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
{// ��ȡ��ǰ�û���ע�����б�����һ��΢����Ϣ
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
{// ��ȡ��ǰ�û���˿�б�����һ��΢����Ϣ
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

//-----------------------------------˽��------------------------------------------//

WEIBO_url_callback(GETDIRECTMSG)
{// ��ȡ��ǰ�û�����˽���б�
	int  ret = -1;
	WEIBO_struct_cast(t_wb_get_direct_message);

    URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(GETDIRECTMSG), req_ex) );

	Weibo_url_http_option_param_URI(URI , &pstruct->wbopt_ );

	HTTP_SET_GET_METHOD();

	return 0;
}

WEIBO_url_callback(GETDIRESTMSG_SENT)
{// ��ȡ��ǰ�û����͵�����˽���б� 
	int  ret = -1;
	WEIBO_struct_cast(t_wb_get_direct_message_sent);

    URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(GETDIRESTMSG_SENT), req_ex) );

	Weibo_url_http_option_param_URI(URI , &pstruct->wbopt_ );

	HTTP_SET_GET_METHOD();

	return 0;
}

WEIBO_url_callback(PUTDIRECTMSG_NEW)
{// ����һ��˽��
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
{// ɾ��һ��˽��
	int   ret = -1;
	WEIBO_struct_cast(t_wb_put_direct_message_destroy);

	// �ر�
	HTTP_SPRINTF(URI, HTTP_T("%s%s/%s%s"),  Weibo_ip( WEIBO_OPTION(PUTDIRECTMSG_DESTROY),req_ex),Weibo_url_geturi( WEIBO_OPTION(PUTDIRECTMSG_DESTROY) ), WBChar_2HTTPChar(pstruct->wbId_) , Weibo_url_format_key(req_ex->format_) );

	Weibo_url_base( URI , pstruct , req_ex);

	// modify by by welbon Ӧ����POST
	// delete method
	//HTTP_SET_DELETE_METHOD();

	HTTP_SET_POST_METHOD();
	
	return 0;
}


WEIBO_url_callback(GETDIRECTMSG_WITH)
{// ��ȡ����˽���б�
	int   ret = -1;
	WEIBO_struct_cast(t_wb_get_direct_message_with);

	//
	// �����
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



//-----------------------------------------��ע---------------------------------------------//

WEIBO_url_callback(PUTFRIENDSHIPS_CREATE)
{// ��עĳ�û�
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
{// ��עĳ�û�

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
{// ȡ����ע
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

	// source(������
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

//�˺�
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
{// ����ͷ�� 
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

	// ������
	Weibo_url_generate_URI(URI , HTTP_T("&nick")        , pstruct->szNick_     , PARAM_ENCODE_UTF8 );
	Weibo_url_generate_URI(URI , HTTP_T("&gender")      , pstruct->szGender_   , PARAM_ENCODE_UTF8 );

	// ��ѡ��
	Weibo_url_generate_URI(URI , HTTP_T("&province")    , pstruct->szProvince_ , PARAM_ENCODE_UTF8 );
	Weibo_url_generate_URI(URI , HTTP_T("&city")        , pstruct->szCity_     , PARAM_ENCODE_UTF8 );

	// ������
	Weibo_url_generate_URI(URI , HTTP_T("&password")    , pstruct->szPwd_     , PARAM_ENCODE_UTF8 );
	Weibo_url_generate_URI(URI , HTTP_T("&email")       , pstruct->szEmail_   , PARAM_ENCODE_UTF8 );
	Weibo_url_generate_URI(URI , HTTP_T("&ip")          , pstruct->szIP_      , PARAM_ENCODE_UTF8 );

	HTTP_SET_POST_METHOD();

	return 0;
}

// �ղ�
WEIBO_url_callback(GETFAVORITES)
{ // ��ȡ��ǰ�û����ղ��б� 
	int  ret = -1;
	WEIBO_struct_cast(t_wb_get_favorites);

	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(GETFAVORITES), req_ex) );

	Weibo_url_http_option_param_URI(URI , &pstruct->wbopt_ );

	HTTP_SET_GET_METHOD();

	return 0;
}

WEIBO_url_callback(PUTFAVORITES_CREATE)// ����ղ� 
{
	int   ret = -1;
	WEIBO_struct_cast(t_wb_put_favorites_create);

	URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(PUTFAVORITES_CREATE), req_ex) );

	// ΢�� id
	Weibo_url_generate_URI(URI , HTTP_T("&id") ,  pstruct->wbId_ , lohttp::ParamNormal );

	HTTP_SET_POST_METHOD();

	return 0;
}

WEIBO_url_callback(PUTFAVORITES_DESTROY)// ɾ����ǰ�û��ղص�΢����Ϣ 
{
	int   ret = -1;
	WEIBO_struct_cast(t_wb_put_favorites_destroy);

	HTTP_SPRINTF(URI , HTTP_T("%s%s/%s%s"), Weibo_ip( WEIBO_OPTION(PUTFAVORITES_DESTROY),req_ex),Weibo_url_geturi( WEIBO_OPTION(PUTFAVORITES_DESTROY) ) , WBChar_2HTTPChar( pstruct->wbId_) , Weibo_url_format_key(req_ex->format_) );

	Weibo_url_base( URI , pstruct , req_ex);

	//HTTP_SET_DELETE_METHOD();
	HTTP_SET_POST_METHOD();

	return 0;
}


WEIBO_url_callback(OAUTH_REQUEST_TOKEN)// ��ȡδ��Ȩ��Request Token 
{
	int  ret = -1;
	
	// ���ԣ���֤�㷨
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
//	int enclen = lo_UrlEncodeA(&pEnc, "�й�123345" , 0);
//
//	int declen = lo_UrlDecodeA(&pDec , pEnc , enclen );
//
//	free( pEnc );
//	free( pDec );
//
//	wchar_t* pc2w = 0;
//	int c2wlen = 0;
//	c2wlen = lo_C2W(&pc2w , "�й�����");
//
//	char* pw2c = 0;
//	int w2clen = 0;
//	w2clen = lo_W2C(&pw2c , pc2w );
//
//	free(pw2c);
//	free(pc2w);
//#endif

	Weibo_url_get_uri(URI ,  WEIBO_OPTION(OAUTH_REQUEST_TOKEN) , req_ex , false/*����Ҫ��ʽ*/ );

	HTTP_SET_GET_METHOD();

	return 0;//Weibo_url_seturl(pHttp  ,URI , false ,  0 , 0 , cookie);
}

WEIBO_url_callback(OAUTH_AUTHORIZE)// �����û���ȨToken 
{
	WEIBO_struct_cast(t_wb_authorize);
	Weibo_url_get_uri(URI , WEIBO_OPTION(OAUTH_AUTHORIZE) , req_ex , false/*����Ҫ��ʽ*/ );

	//
	Weibo_url_generate_URI( URI , HTTP_T("?oauth_callback"),
		(pstruct->wbauth_.format_ == E_FORMAT_XML) ? WBCHAR_T("xml") : WBCHAR_T("json"),0 );

	//
	Weibo_url_generate_URI( URI , HTTP_T("&oauth_token"),pstruct->wbauth_.oauth_token_,0 );

	// �û���/����
	Weibo_url_generate_URI( URI , HTTP_T("&userId") ,  pstruct->usrid_ ,  0 );
	Weibo_url_generate_URI( URI , HTTP_T("&passwd") ,  pstruct->usrpwd_ , 0 );

	HTTP_SET_POST_METHOD();

	return 0;
}

WEIBO_url_callback(OAUTH_ACCESS_TOKEN)// ��ȡ��Ȩ����Access Token
{
	int   ret = -1;
	WEIBO_struct_cast(t_wb_oauth_access_token);
	wchar_t* outstrW = 0;

	Weibo_url_get_uri(URI ,  WEIBO_OPTION(OAUTH_ACCESS_TOKEN) , req_ex , false/*����Ҫ��ʽ*/ );

	// �������֤�����
	Weibo_url_generate_URI( URI , HTTP_T("?oauth_verifier") , Char_2WBChar(pstruct->wbauth_.oauth_verifier_) , lohttp::ParamNormal );
	SafeFreeA_lo( outstrW );

	HTTP_SET_GET_METHOD();

	return 0;
}

WEIBO_url_callback(XAUTH_ACCESS_TOKEN)// ��ȡ��Ȩ����Access Token
{
	int   ret = -1;
	WEIBO_struct_cast(t_wb_xauth_access_token);
	wchar_t* outstrW = 0;

	Weibo_url_get_uri(URI ,  WEIBO_OPTION(XAUTH_ACCESS_TOKEN) , req_ex , false/*����Ҫ��ʽ*/ );

	/// �û���/����
	Weibo_url_generate_URI( URI , HTTP_T("&x_auth_username") ,  pstruct->usrid_ ,  0 );
	Weibo_url_generate_URI( URI , HTTP_T("&x_auth_password") ,  pstruct->usrpwd_ , 0 );
	Weibo_url_generate_URI( URI , HTTP_T("&x_auth_mode") ,  pstruct->authmode_,  0 );
	//Weibo_url_generate_URI( URI , HTTP_T("&source") ,  "client_auth",  0 );

	SafeFreeA_lo( outstrW );

	HTTP_SET_POST_METHOD();

	return 0;
}


WEIBO_url_callback(GET_EMOTIONS)
{// ��������΢���ٷ����б��顢ħ������������Ϣ����������������͡�������࣬�Ƿ����ŵȡ�
	int   ret = -1;
	WEIBO_struct_cast(t_wb_emotions);
	wchar_t* outstrW = 0;

    URICHECK_RETURN( Weibo_url_get_uri(URI , WEIBO_OPTION(GET_EMOTIONS), req_ex)  );

	HTTP_SET_GET_METHOD();

	return 0;
}

		// �û����� 
WEIBO_url_callback(GET_USERS_SEARCH)
{// ���عؼ���ƥ���΢���û�
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
		// ΢������ 
WEIBO_url_callback(GET_WB_SEARCH)
{// 46 ���عؼ���ƥ���΢����(�������˺��������߿���) 
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
{//47 ����΢��(���������) (���Ժ��������߿���) 
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
{//48 ʡ�ݳ��б����
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

	Weibo_url_get_uri(URI ,  WEIBO_OPTION(COOKIE) , req_ex , false/*����Ҫ��ʽ*/ );
	
	// �û���/����
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

	Weibo_url_get_uri(URI ,  WEIBO_OPTION(UPDATETGT) , req_ex , false/*����Ҫ��ʽ*/ );
	
	Weibo_url_generate_URI( URI , HTTP_T("?tgt") ,  pstruct->wbauth_.tgt_  , PARAM_ENCODE_UTF8 );

	Weibo_url_generate_URI( URI , HTTP_T("&returntype") ,  WBCHAR_T("TEXT2") , PARAM_ENCODE_UTF8 );

	HTTP_SET_GET_METHOD();

	return 0;
}

// �Զ���
WEIBO_url_callback(CUSTOM)
{
	return 0;
}

/**  �ȵ�ʵ�� */
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


//�ȵ�
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

//�µ��û��ӿ�
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


// ����ӿ� ,by welbon,2011-01-10
WEIBO_url_callback(GET_TRENDS)//trends ��ȡĳ�˵Ļ���
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
WEIBO_url_callback(GET_TRENDS_STATUSES)//trends/statuses ��ȡĳһ�����µ�΢��
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
WEIBO_url_callback(POST_TRENDS_FOLLOW)//trends/follow ��עĳһ������
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
WEIBO_url_callback(DELETE_TRENDS_DESTROY)//trends/destroy ȡ����ע��ĳһ������
{
	int ret = -1;
	WEIBO_struct_cast(t_wb_trends_destroy);
	//
	const lohttp::HTTPChar * uri = Weibo_url_geturi( WEIBO_OPTION(DELETE_TRENDS_DESTROY));
	if( !uri ) {
		return -1;// URI Ϊ��
	}
	HTTP_SPRINTF(URI , ("%s%s/%s%s") , Weibo_ip(WEIBO_OPTION(DELETE_TRENDS_DESTROY),req_ex), 
		uri,pstruct->trendid_, Weibo_url_format_key( req_ex->format_) );

	Weibo_url_base( URI,pstruct,req_ex);
	//

	HTTP_SET_DELETE_METHOD();

	return 0;
}
WEIBO_url_callback(GET_TRENDS_HOURLY)//trends/destroy ��Сʱ�������Ż���
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
WEIBO_url_callback(GET_TRENDS_DAYLIY)//trends/daily �����ڷ������Ż��⡣����ĳһ���ڵĻ���
{
	int ret = -1;
	WEIBO_struct_cast(t_wb_trends_daily);

	URICHECK_RETURN( Weibo_url_get_uri(URI,WEIBO_OPTION(GET_TRENDS_DAYLIY), req_ex) );

	//baseapp
	Weibo_url_generate_URI_INT(URI,HTTP_T("&base_app"), pstruct->baseapp_ );

	HTTP_SET_GET_METHOD();

	return 0;
}
WEIBO_url_callback(GET_TRENDS_WEEKLIY)//trends/weekly ���ܷ������Ż��⡣����ĳһ����֮ǰĳһ�ܵĻ���
{
	int ret = -1;
	WEIBO_struct_cast(t_wb_trends_weekly);

	URICHECK_RETURN( Weibo_url_get_uri(URI,WEIBO_OPTION(GET_TRENDS_WEEKLIY), req_ex) );

	//baseapp
	Weibo_url_generate_URI_INT(URI,HTTP_T("&base_app"), pstruct->baseapp_ );

	HTTP_SET_GET_METHOD();

	return 0;
}

// �������ӿ� ,by welbon,2011-01-10
WEIBO_url_callback(POST_BLOCKS_CREATE)//��ĳ�û����������
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
WEIBO_url_callback(POST_BLOCKS_DESTROY)//��ĳ�û��Ƴ�������
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
WEIBO_url_callback(GET_BLOCKS_EXISTS)//���ĳ�û��Ƿ��Ǻ������û�
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
WEIBO_url_callback(GET_BLOCKS_BLOCKING)//�г��������û�(����û���ϸ��Ϣ)
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
WEIBO_url_callback(GET_BLOCKS_BLOCKING_IDS)//�г���ҳ�������û���ֻ���id��
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

//�û���ǩ�ӿ� ,by welbon,2011-01-10
WEIBO_url_callback(GET_TAGS)//tags ����ָ���û��ı�ǩ�б�
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
WEIBO_url_callback(POST_TAGS_CREATE)//tags/create ����û���ǩ
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
WEIBO_url_callback(GET_TAGS_SUGGESTIONS)//tags/suggestions �����û�����Ȥ�ı�ǩ
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
WEIBO_url_callback(POST_TAGS_DESTROY)//tags/destroy ɾ����ǩ
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
WEIBO_url_callback(POST_TAGS_DESTROY_BATCH)//tags/destroy_batch ����ɾ����ǩ
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


WEIBO_url_callback(POST_INVITE_MAILCONTACT)//����������ϵ��
{
	int ret = -1;
	WEIBO_struct_cast(t_wb_invite_mailcontect);
	//
	Weibo_url_get_uri(URI,WEIBO_OPTION(POST_INVITE_MAILCONTACT),req_ex,false);
	// ����β��������Ĵ���
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

WEIBO_url_callback(POST_INVITE_MSNCONTACT)//����MSN��ϵ��
{
	int ret = -1;
	WEIBO_struct_cast(t_wb_invite_msncontect);
	//
	Weibo_url_get_uri(URI,WEIBO_OPTION(POST_INVITE_MSNCONTACT),req_ex,false);

	// ����β��������Ĵ���
	HTTP_TCSCAT_S(URI,HTTP_T("?"));
	Weibo_url_random(URI);
	//
	Weibo_url_generate_URI(URI,HTTP_T("&user"), pstruct->usrid_,PARAM_ENCODE_UTF8 );
	Weibo_url_generate_URI(URI,HTTP_T("&password"), pstruct->usrpwd_,PARAM_ENCODE_UTF8 );

	//Http method
	HTTP_SET_POST_METHOD();

	return 0;
}

WEIBO_url_callback(POST_INVITE_SENDMAILS)//���������ʼ�
{
	int ret = -1;
	WEIBO_struct_cast(t_wb_invite_sendmails);
	//
	Weibo_url_get_uri(URI,WEIBO_OPTION(POST_INVITE_SENDMAILS),req_ex,false);

	// ����β��������Ĵ���
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

WEIBO_url_callback(GET_MEDIA_SHORTURL_BATCH)//���������ʼ�
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

	//��ȡ�������ݼ�(timeline)�ӿ� 
	WEIBO_url_fun(GETSTATUSES_PUBLIC_TIMELINE),//2 ��ȡ���¸��µĹ���΢����Ϣ 
	WEIBO_url_fun(GETSTATUSES_FRIENDS_TIMELINE),//3 ��ȡ��ǰ�û�����ע�û�������΢����Ϣ (����: statuses/home_timeline) 
	WEIBO_url_fun(GETSTATUSES_USE_TIMELINE),//4 ��ȡ�û�������΢����Ϣ�б� 
	WEIBO_url_fun(GETSTATUSES_MENTIONS),//5 ��ȡ@��ǰ�û���΢���б� 
	WEIBO_url_fun(GETSTATUSES_COMMENTS_TIMELINE),//6 ��ȡ��ǰ�û����ͼ��յ��������б�
	WEIBO_url_fun(GETSTATUSES_COMMENTS_BYME),//7 ��ȡ��ǰ�û����������� 
	WEIBO_url_fun(GETSTATUSES_COMMENTS_TOME),//7 ��ȡ��ǰ�û��յ������� 
	WEIBO_url_fun(GETSTATUSES_COMMENTS_LIST),//8 ��ȡָ��΢���������б� 
	WEIBO_url_fun(GETSTATUSES_COMMENTS_COUNTS),//9 ������ȡһ��΢������������ת���� 
	WEIBO_url_fun(GETSTATUSES_UNREAD), //10 ��ȡ��ǰ�û�δ����Ϣ��
	WEIBO_url_fun(PUTSTATUSES_RESET_COUNT),//10��1δ����Ϣ������ӿ� 

	//΢�����ʽӿ� 
	WEIBO_url_fun(GETSTATUSES_SHOW),//11 ����ID��ȡ����΢����Ϣ���� 
	WEIBO_url_fun(GOTOSTATUSES_ID),//12 ����΢��ID���û�ID��ת������΢��ҳ�� 
	WEIBO_url_fun(PUTSTATUSES_UPDATE),//13 ����һ��΢����Ϣ 
	WEIBO_url_fun(PUTSTATUSES_UPLOAD),//14 �ϴ�ͼƬ������һ��΢����Ϣ
	WEIBO_url_fun(PUTSTATUSES_DESTROY),//15 ɾ��һ��΢����Ϣ 
	WEIBO_url_fun(PUTSTATUSES_REPOST),//16 ת��һ��΢����Ϣ���ɼ����ۣ� 
	WEIBO_url_fun(PUTSTATUSES_COMMENT),//17 ��һ��΢����Ϣ�������� 
	WEIBO_url_fun(PUTSTATUSES_COMMENT_DESTROY),//18 ɾ����ǰ�û���΢��������Ϣ 
	WEIBO_url_fun(PUTSTATUSES_REPLY),//19 �ظ�΢��������Ϣ

	//�û��ӿ� 
	WEIBO_url_fun(GETUSER_INFO),//20 �����û�ID��ȡ�û����ϣ���Ȩ�û���
	WEIBO_url_fun(GETFRINDS_LIST),//21 ��ȡ��ǰ�û���ע�����б�����һ��΢����Ϣ
	WEIBO_url_fun(GETFOLLOWERS_LIST),//22 ��ȡ��ǰ�û���˿�б�����һ��΢����Ϣ 

	//˽�Žӿ� 
	WEIBO_url_fun(GETDIRECTMSG),//23 ��ȡ��ǰ�û�����˽���б� 
	WEIBO_url_fun(GETDIRESTMSG_SENT),//24 ��ȡ��ǰ�û����͵�����˽���б�
	WEIBO_url_fun(PUTDIRECTMSG_NEW),//25 ����һ��˽��
	WEIBO_url_fun(PUTDIRECTMSG_DESTROY),//26 ɾ��һ��˽��
	WEIBO_url_fun(GETDIRECTMSG_WITH),//��ȡ����˽���б�

	//��ע�ӿ� 
	WEIBO_url_fun(PUTFRIENDSHIPS_CREATE),//27 ��עĳ�û�
	WEIBO_url_fun(PUTFRIENDSHIPS_CREATE_BATCH),//������ע
	WEIBO_url_fun(PUTFRIENDSHIPS_DESTROY),//28 ȡ����ע
	WEIBO_url_fun(GETFRIENDSHIPS_EXISTS),//29 �ж������û��Ƿ��й�ע��ϵ�����������û���ϵ����ϸ���
	WEIBO_url_fun(GETFRIENDSHIPS_BATCH_EXISTS),//������ȡһ���û��Ƿ�Ϊ����

	//Social Graph�ӿ�
	WEIBO_url_fun(GETFRIEND_IDS),//30 ��ע�б�
	WEIBO_url_fun(GETFOLLOWER_IDS),//31 ��˿�б�

	//�˺Žӿ� 
	WEIBO_url_fun(GETACCOUNT_VERIFY),//32 ��֤��ǰ�û�����Ƿ�Ϸ� 
	WEIBO_url_fun(GETACCOUNT_RATELIMIT),//33 ��ȡ��ǰ�û�API����Ƶ������
	WEIBO_url_fun(PUTACCOUNT_QUITSESSION),//34 ��ǰ�û��˳���¼ 
	WEIBO_url_fun(PUTACCOUNT_UPDATE_PROFILEIMAGE),//35 ����ͷ��
	WEIBO_url_fun(PUTACCOUNT_UPDATE_PROFILE),//36 ��������
	WEIBO_url_fun(PUTACCOUNT_REGISTER),

	//�ղؽӿ� 
	WEIBO_url_fun(GETFAVORITES),// 38��ȡ��ǰ�û����ղ��б� 
	WEIBO_url_fun(PUTFAVORITES_CREATE),// 39����ղ�
	WEIBO_url_fun(PUTFAVORITES_DESTROY),// 40ɾ����ǰ�û��ղص�΢����Ϣ

	//��¼/OAuth�ӿ� 
	WEIBO_url_fun(OAUTH_REQUEST_TOKEN),// 41��ȡδ��Ȩ��Request Token
	WEIBO_url_fun(OAUTH_AUTHORIZE),
	WEIBO_url_fun(OAUTH_ACCESS_TOKEN),// 43��ȡ��Ȩ����Access Token

	// ����
	WEIBO_url_fun(GET_EMOTIONS),// 44 ��������΢���ٷ����б��顢ħ������������Ϣ����������������͡�������࣬�Ƿ����ŵȡ�

	// �û����� 
	WEIBO_url_fun(GET_USERS_SEARCH),// 45 ����΢���û�,���عؼ���ƥ���΢���û���(�������˺��������߿���) 

	// ΢������ 
	WEIBO_url_fun(GET_WB_SEARCH),// 46 ���عؼ���ƥ���΢����(�������˺��������߿���)
	WEIBO_url_fun(GET_STATUSES_SEARCH),//47 ����΢��(���������) (���Ժ��������߿���) 

	WEIBO_url_fun(GET_PROVINCES),// 48 ʡ�ݳ��б���� 
	WEIBO_url_fun(REPORT),//49 �ٱ�
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
	WEIBO_url_fun(GET_USERS_HOT),// ��ȡϵͳ�Ƽ��û�
	WEIBO_url_fun(POST_USERS_REMARK),//�����޸ĵ�ǰ��¼�û�����ע��ĳ�����ѵı�ע��ϢNew!
	WEIBO_url_fun(GET_USERS_SUGGESTIONS), //Users/suggestions ���ص�ǰ�û����ܸ���Ȥ���û�

	// ����ӿ� ,by welbon,2011-01-10
	WEIBO_url_fun(GET_TRENDS),//trends ��ȡĳ�˵Ļ���
	WEIBO_url_fun(GET_TRENDS_STATUSES),//trends/statuses ��ȡĳһ�����µ�΢��
	WEIBO_url_fun(POST_TRENDS_FOLLOW),//trends/follow ��עĳһ������
	WEIBO_url_fun(DELETE_TRENDS_DESTROY),//trends/destroy ȡ����ע��ĳһ������
	WEIBO_url_fun(GET_TRENDS_HOURLY),//trends/destroy ��Сʱ�������Ż���
	WEIBO_url_fun(GET_TRENDS_DAYLIY),//trends/daily �����ڷ������Ż��⡣����ĳһ���ڵĻ���
	WEIBO_url_fun(GET_TRENDS_WEEKLIY),//trends/weekly ���ܷ������Ż��⡣����ĳһ����֮ǰĳһ�ܵĻ���

	// �������ӿ� ,by welbon,2011-01-10
	WEIBO_url_fun(POST_BLOCKS_CREATE),//��ĳ�û����������
	WEIBO_url_fun(POST_BLOCKS_DESTROY),//��ĳ�û��Ƴ�������
	WEIBO_url_fun(GET_BLOCKS_EXISTS),//���ĳ�û��Ƿ��Ǻ������û�
	WEIBO_url_fun(GET_BLOCKS_BLOCKING),//�г��������û�(����û���ϸ��Ϣ)
	WEIBO_url_fun(GET_BLOCKS_BLOCKING_IDS),//�г���ҳ�������û���ֻ���id��

	//�û���ǩ�ӿ� ,by welbon,2011-01-10
	WEIBO_url_fun(GET_TAGS),//tags ����ָ���û��ı�ǩ�б�
	WEIBO_url_fun(POST_TAGS_CREATE),//tags/create ����û���ǩ
	WEIBO_url_fun(GET_TAGS_SUGGESTIONS),//tags/suggestions �����û�����Ȥ�ı�ǩ
	WEIBO_url_fun(POST_TAGS_DESTROY),//tags/destroy ɾ����ǩ
	WEIBO_url_fun(POST_TAGS_DESTROY_BATCH),//tags/destroy_batch ����ɾ����ǩ

	// Invite mail
	WEIBO_url_fun(POST_INVITE_MAILCONTACT),
	WEIBO_url_fun(POST_INVITE_MSNCONTACT) ,
	WEIBO_url_fun(POST_INVITE_SENDMAILS)  ,

	// Media
	WEIBO_url_fun(GET_MEDIA_SHORTURL_BATCH) ,
	//��¼/XAuth�ӿ� 
	WEIBO_url_fun(XAUTH_ACCESS_TOKEN),

};

int Weibo_url_set(lohttp::LOHttp* pHttp , WEIBOoption option , const void* t_wb , const struct t_wb_REQ_EX* req_ex)
{
	lohttp::HTTPChar URI[URI_MAX_LEN] = {0};
	int httpmethod = 0;

	
	if( WEIBO_OPTION(UNK) >= option ||
		WEIBO_OPTION(LAST) <= option )
	{
		return -1;//������Χ
	}

	if( 0 == vector_url_callbak[option](URI , &httpmethod ,  t_wb ,req_ex) ) 
	{
		// ִ��( �ر��� )
	    if( WEIBO_OPTION(OAUTH_REQUEST_TOKEN) == option )
		{// ����Ҫ t_key AND t_scecet
			return Weibo_url_http_seturl(pHttp , URI  , httpmethod , 0 , req_ex);
		}
		else if( WEIBO_OPTION(OAUTH_AUTHORIZE) == option )
		{// �����ǲ���Ҫ��������� add by welbon,2010-09-10

			lohttp::Http_setopt(pHttp , lohttp::LOHTTP_OPT_setmethod , httpmethod );
			lohttp::Http_seturl(pHttp , Char_2HTTPChar(URI) );
			return 0;
		}
		else if( WEIBO_OPTION(XAUTH_ACCESS_TOKEN) == option )
		{
			return Weibo_url_http_seturl(pHttp , URI  , httpmethod , 0 , req_ex);
		}
		else if( WEIBO_OPTION(PUTSTATUSES_UPLOAD) == option )
		{// ����ʽ			
			return Weibo_url_upload_form(pHttp ,URI  , httpmethod ,  t_wb , "pic" , req_ex );
		}
		else if( WEIBO_OPTION(PUTACCOUNT_UPDATE_PROFILEIMAGE) == option )
		{// (�޸ĸ���ͷ��) ����ʽ
			return Weibo_url_upload_form(pHttp ,URI  , httpmethod ,  t_wb ,  "image", req_ex );
		}
		else if( WEIBO_OPTION(GET_PROVINCES) == option )
		{// ����Ҫ����
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

// ��ȡ
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
		return -1;//������Χ
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
