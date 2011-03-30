/**
*  @brief weibo call back
*  @file  wbcallback.cpp
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

#include "stdafx.h"
#include "wbParser/splitstr.h"
#include "wbParser/wbParser.h"
#include "wbcallback.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>


////////////////////////////////////////////////////////////////////////////////////////////////////

// for declare function
#define WEIBO_struct_statuscallback_init(name) \
	static void Wb_struct_statuscallback_init_##name##_cb(int state , int errCode , void* pUserdata)

#define WEIBO_struct_statuscallback_init_fun(name) Wb_struct_statuscallback_init_##name##_cb

//
#define WEIBO_struct_bodycallback_init(name) \
	static void Wb_struct_bodycallback_init_##name##_cb(unsigned long httpCode , const char* body , unsigned long len , void* pUserdata)

#define WEIBO_struct_bodycallback_init_fun(name) Wb_struct_bodycallback_init_##name##_cb

//
#define WEIBO_struct_headerycallback_init(name) \
	static void Wb_struct_headercallback_init_##name##_cb(unsigned long httpCode , const char* header , unsigned long len , void* pUserdata)

#define WEIBO_struct_headercallback_init_fun(name) Wb_struct_headercallback_init_##name##_cb

#include "wbParser/wbParseWrapper.h"

#ifdef _MSC_VER
#	if defined(WEIBO_VC60)
#		pragma comment(lib,"WBParseWrapper_VC60.lib")
#	elif defined(WEIBO_VC70)
#		pragma comment(lib,"WBParseWrapper_VC70.lib")
#	elif defined(WEIBO_VC80)
#		pragma comment(lib,"WBParseWrapper_VC80.lib")
#	elif defined(WEIBO_VC90)
#		pragma comment(lib,"WBParseWrapper_VC90.lib")
#	endif //
#endif //_MSC_VER

//
WEIBO_struct_statuscallback_init(BASE)
{
}

WEIBO_struct_bodycallback_init(BASE)
{
	printf("   BASE : \n    %s \n\n" , body );
}

//------------------------------------��ȡ�������ݼ�(timeline)�ӿ�---------------------------------------------//
//

// 2.��ȡ���¸��µĹ���΢����Ϣ
WEIBO_struct_statuscallback_init(GETSTATUSES_PUBLIC_TIMELINE)
{

}

WEIBO_struct_bodycallback_init(GETSTATUSES_PUBLIC_TIMELINE)
{
	printf("GETSTATUSES_PUBLIC_TIMELINE : \n    %s \n\n" , body );

}

// 3.��ȡ��ǰ�û�����ע�û�������΢����Ϣ
WEIBO_struct_statuscallback_init(GETSTATUSES_FRIENDS_TIMELINE)
{
}

WEIBO_struct_bodycallback_init(GETSTATUSES_FRIENDS_TIMELINE)
{
	printf("GETSTATUSES_FRIENDS_TIMELINE : \n    %s \n\n" , body );

}

// 4.��ȡ�û�������΢����Ϣ�б�
WEIBO_struct_statuscallback_init(GETSTATUSES_USE_TIMELINE)
{

}
WEIBO_struct_bodycallback_init(GETSTATUSES_USE_TIMELINE)
{
	printf("   GETSTATUSES_USE_TIMELINE : \n    %s \n\n" , body );

}

// 5.��ȡ@��ǰ�û���΢���б�
WEIBO_struct_statuscallback_init(GETSTATUSES_MENTIONS)
{

}
WEIBO_struct_bodycallback_init(GETSTATUSES_MENTIONS)
{
	printf("   GETSTATUSES_MENTIONS : \n    %s \n\n" , body );

}


// 6.��ȡ��ǰ�û����ͼ��յ��������б�
WEIBO_struct_statuscallback_init(GETSTATUSES_COMMENTS_TIMELINE)
{
}

WEIBO_struct_bodycallback_init(GETSTATUSES_COMMENTS_TIMELINE)
{
	printf("   GETSTATUSES_COMMENTS_TIMELINE : \n    %s \n\n" , body );

}

// 7.��ȡ��ǰ�û�����������
WEIBO_struct_statuscallback_init(GETSTATUSES_COMMENTS_BYME)
{

}
WEIBO_struct_bodycallback_init(GETSTATUSES_COMMENTS_BYME)
{
	printf("   GETSTATUSES_COMMENTS_BYME : \n    %s \n\n" , body );
}

// 8 .��ȡָ��΢���������б�
WEIBO_struct_statuscallback_init(GETSTATUSES_COMMENTS_LIST)
{

}

WEIBO_struct_bodycallback_init(GETSTATUSES_COMMENTS_LIST)
{
	printf("   GETSTATUSES_COMMENTS_LIST : \n    %s \n\n" , body );

}

//  9 .������ȡһ��΢������������ת����
WEIBO_struct_statuscallback_init(GETSTATUSES_COMMENTS_COUNTS)
{

}

WEIBO_struct_bodycallback_init(GETSTATUSES_COMMENTS_COUNTS)
{
	printf("   GETSTATUSES_COMMENTS_COUNTS : \n    %s \n\n" , body );

}


// 10.��ȡ��ǰ�û�δ����Ϣ��
WEIBO_struct_statuscallback_init(GETSTATUSES_UNREAD)
{

}
WEIBO_struct_bodycallback_init(GETSTATUSES_UNREAD)
{
	printf("   GETSTATUSES_UNREAD : \n    %s \n\n" , body );

}

//---------------------------------------΢�����ʽӿ�--------------------------------------------------//

// 11 ����ID��ȡ����΢����Ϣ����
WEIBO_struct_statuscallback_init(GETSTATUSES_SHOW)
{
}
WEIBO_struct_bodycallback_init(GETSTATUSES_SHOW)
{
	printf("   GETSTATUSES_SHOW : \n    %s \n\n" , body );

}

// 12 ����΢��ID���û�ID��ת������΢��ҳ��
WEIBO_struct_statuscallback_init(GOTOSTATUSES_ID)
{
}
WEIBO_struct_bodycallback_init(GOTOSTATUSES_ID)
{
	// ��תҳ��
	printf("   GOTOSTATUSES_ID : \n    %s \n\n" , body );
}

// 13 ����һ��΢����Ϣ
WEIBO_struct_statuscallback_init(PUTSTATUSES_UPDATE)
{
}
WEIBO_struct_bodycallback_init(PUTSTATUSES_UPDATE)
{
	printf("   PUTSTATUSES_UPDATE : \n    %s \n\n" , body );

}


// 14 �ϴ�ͼƬ������һ��΢����Ϣ
WEIBO_struct_statuscallback_init(PUTSTATUSES_UPLOAD)
{

}
WEIBO_struct_bodycallback_init(PUTSTATUSES_UPLOAD)
{
	printf("   PUTSTATUSES_UPLOAD : \n    %s \n\n" , body );

}

// 15 ɾ��һ��΢����Ϣ
WEIBO_struct_statuscallback_init(PUTSTATUSES_DESTROY)
{

}
WEIBO_struct_bodycallback_init(PUTSTATUSES_DESTROY)
{
	printf("   PUTSTATUSES_DESTROY : \n    %s \n\n" , body );

}

// 16 ת��һ��΢����Ϣ���ɼ����ۣ�
WEIBO_struct_statuscallback_init(PUTSTATUSES_REPOST)
{

}

WEIBO_struct_bodycallback_init(PUTSTATUSES_REPOST)
{
	printf("   PUTSTATUSES_REPOST : \n    %s \n\n" , body );

}



// 17 ��һ��΢����Ϣ��������
WEIBO_struct_statuscallback_init(PUTSTATUSES_COMMENT)
{

}
WEIBO_struct_bodycallback_init(PUTSTATUSES_COMMENT)
{
	printf("   PUTSTATUSES_COMMENT : \n    %s \n\n" , body );

}

// 18 ɾ����ǰ�û���΢��������Ϣ
WEIBO_struct_statuscallback_init(PUTSTATUSES_COMMENT_DESTROY)
{

}
WEIBO_struct_bodycallback_init(PUTSTATUSES_COMMENT_DESTROY)
{
	printf("   PUTSTATUSES_COMMENT_DESTROY : \n    %s \n\n" , body );

}


// 19 �ظ�΢��������Ϣ
WEIBO_struct_statuscallback_init(PUTSTATUSES_REPLY)
{

}
WEIBO_struct_bodycallback_init(PUTSTATUSES_REPLY)
{
	printf("   PUTSTATUSES_REPLY : \n    %s \n\n" , body );

}

//---------------------------------------�û��ӿ�--------------------------------------------------//



// 20 �����û�ID��ȡ�û����ϣ���Ȩ�û���
WEIBO_struct_statuscallback_init(GETUSER_INFO)
{

}
WEIBO_struct_bodycallback_init(GETUSER_INFO)
{
	printf("   GETUSER_INFO : \n    %s \n\n" , body );
}


//  21 ��ȡ��ǰ�û���ע�����б�����һ��΢����Ϣ
WEIBO_struct_statuscallback_init(GETFRINDS_LIST)
{

}
WEIBO_struct_bodycallback_init(GETFRINDS_LIST)
{
	printf("   GETFRINDS_LIST : \n    %s \n\n" , body );

}


// 22 ��ȡ��ǰ�û���˿�б�����һ��΢����Ϣ
WEIBO_struct_statuscallback_init(GETFOLLOWERS_LIST)
{

}
WEIBO_struct_bodycallback_init(GETFOLLOWERS_LIST)
{
	printf("   GETFOLLOWERS_LIST : \n    %s \n\n" , body );

}


//---------------------------------------˽�Žӿ�--------------------------------------------------//

// 23 ��ȡ��ǰ�û�����˽���б�
WEIBO_struct_statuscallback_init(GETDIRECTMSG)
{

}
WEIBO_struct_bodycallback_init(GETDIRECTMSG)
{
	printf("   GETDIRECTMSG : \n    %s \n\n" , body );
}


// 24 ��ȡ��ǰ�û����͵�����˽���б�
WEIBO_struct_statuscallback_init(GETDIRESTMSG_SENT)
{

}
WEIBO_struct_bodycallback_init(GETDIRESTMSG_SENT)
{
	printf("   GETDIRESTMSG_SENT : \n    %s \n\n" , body );

}


// 25 ����һ��˽��
WEIBO_struct_statuscallback_init(PUTDIRECTMSG_NEW)
{

}
WEIBO_struct_bodycallback_init(PUTDIRECTMSG_NEW)
{
	printf("   PUTDIRECTMSG_NEW : \n    %s \n\n" , body );
}


//26 ɾ��һ��˽��
WEIBO_struct_statuscallback_init(PUTDIRECTMSG_DESTROY)
{

}
WEIBO_struct_bodycallback_init(PUTDIRECTMSG_DESTROY)
{
	printf("   PUTDIRECTMSG_DESTROY : \n    %s \n\n" , body );
}

//26 ɾ��һ��˽��
WEIBO_struct_statuscallback_init(GETDIRECTMSG_WITH)
{

}
WEIBO_struct_bodycallback_init(GETDIRECTMSG_WITH)
{
	printf("   PUTDIRECTMSG_DESTROY : \n    %s \n\n" , body );
}


//---------------------------------------��ע�ӿ�--------------------------------------------------//

// 27 ��עĳ�û�
WEIBO_struct_statuscallback_init(PUTFRIENDSHIPS_CREATE)
{

}
WEIBO_struct_bodycallback_init(PUTFRIENDSHIPS_CREATE)
{
	printf("   PUTFRIENDSHIPS_CREATE : \n    %s \n\n" , body );

}

// ������ע�ӿ�
WEIBO_struct_statuscallback_init(PUTFRIENDSHIPS_CREATE_BATCH)
{

}
WEIBO_struct_bodycallback_init(PUTFRIENDSHIPS_CREATE_BATCH)
{
	printf("   PUTFRIENDSHIPS_CREATE_BATCH : \n    %s \n\n" , body );

}

// 28 ȡ����ע
WEIBO_struct_statuscallback_init(PUTFRIENDSHIPS_DESTROY)
{

}
WEIBO_struct_bodycallback_init(PUTFRIENDSHIPS_DESTROY)
{
	printf("   PUTFRIENDSHIPS_DESTROY : \n    %s \n\n" , body );

}

// 29 �Ƿ��עĳ�û�
WEIBO_struct_statuscallback_init(GETFRIENDSHIPS_EXISTS)
{

}
WEIBO_struct_bodycallback_init(GETFRIENDSHIPS_EXISTS)
{
	printf("   GETFRIENDSHIPS_EXISTS : \n    %s \n\n" , body );
	// TODO:
}

//
WEIBO_struct_statuscallback_init(GETFRIENDSHIPS_BATCH_EXISTS)
{

}
WEIBO_struct_bodycallback_init(GETFRIENDSHIPS_BATCH_EXISTS)
{
	printf("   GETFRIENDSHIPS_BATCH_EXISTS : \n    %s \n\n" , body );
	// TODO:
}




//
//---------------------------------------Social Graph--------------------------------------------------//

//  30 ��ȡ�û���ע����uid�б�
WEIBO_struct_statuscallback_init(GETFRIEND_IDS)
{

}
WEIBO_struct_bodycallback_init(GETFRIEND_IDS)
{
	printf("   GETFRIEND_IDS : \n    %s \n\n" , body );

}


// 31 ��ȡ�û���˿����uid�б�
WEIBO_struct_statuscallback_init(GETFOLLOWER_IDS)
{

}
WEIBO_struct_bodycallback_init(GETFOLLOWER_IDS)
{
	printf("   GETFOLLOWER_IDS : \n    %s \n\n" , body );

}

//---------------------------------------�˺Žӿ�--------------------------------------------------//



//32 ��֤��ǰ�û�����Ƿ�Ϸ�
WEIBO_struct_statuscallback_init(GETACCOUNT_VERIFY)
{

}

WEIBO_struct_bodycallback_init(GETACCOUNT_VERIFY)
{
	printf("   GETACCOUNT_VERIFY : \n    %s \n\n" , body );
}

//33 ��ȡ��ǰ�û�API����Ƶ������
WEIBO_struct_statuscallback_init(GETACCOUNT_RATELIMIT)
{

}
WEIBO_struct_bodycallback_init(GETACCOUNT_RATELIMIT)
{
	printf("   GETACCOUNT_RATELIMIT : \n    %s \n\n" , body );

}

//34 ��ǰ�û��˳���¼
WEIBO_struct_statuscallback_init(PUTACCOUNT_QUITSESSION)
{

}
WEIBO_struct_bodycallback_init(PUTACCOUNT_QUITSESSION)
{
	printf("   PUTACCOUNT_QUITSESSION : \n    %s \n\n" , body );

}

// 35 ����ͷ��
WEIBO_struct_statuscallback_init(PUTACCOUNT_UPDATE_PROFILEIMAGE)
{

}
WEIBO_struct_bodycallback_init(PUTACCOUNT_UPDATE_PROFILEIMAGE)
{
	printf("   PUTACCOUNT_UPDATE_PROFILEIMAGE : \n    %s \n\n" , body );

}


// 36 ��������
WEIBO_struct_statuscallback_init(PUTACCOUNT_UPDATE_PROFILE)
{

}
WEIBO_struct_bodycallback_init(PUTACCOUNT_UPDATE_PROFILE)
{
	printf("   PUTACCOUNT_UPDATE_PROFILE : \n    %s \n\n" , body );

}

//37 ע������΢���ʺ�
WEIBO_struct_statuscallback_init(PUTACCOUNT_REGISTER)
{

}
WEIBO_struct_bodycallback_init(PUTACCOUNT_REGISTER)
{
	printf("   PUTACCOUNT_REGISTER : \n    %s \n\n" , body );
	//TODO :
}

//--------------------------------------- �ղؽӿ�--------------------------------------------------//
// 38 ��ȡ��ǰ�û����ղ��б� 
WEIBO_struct_statuscallback_init(GETFAVORITES)
{

}

WEIBO_struct_bodycallback_init(GETFAVORITES)
{
	printf("   GETFAVORITES : \n    %s \n\n" , body );

}

// 39 ����ղ� 
WEIBO_struct_statuscallback_init(PUTFAVORITES_CREATE)
{

}

WEIBO_struct_bodycallback_init(PUTFAVORITES_CREATE)
{
	printf("   PUTFAVORITES_CREATE : \n    %s \n\n" , body );

}

// 40 ɾ����ǰ�û��ղص�΢����Ϣ 
WEIBO_struct_statuscallback_init(PUTFAVORITES_DESTROY)
{

}

WEIBO_struct_bodycallback_init(PUTFAVORITES_DESTROY)
{
	printf("   PUTFAVORITES_DESTROY : \n    %s \n\n" , body );

}


//��¼/OAuth

WEIBO_struct_statuscallback_init(OAUTH_REQUEST_TOKEN)
{

}

//#define __USE_TEST_EXPLOER__

#ifdef __USE_TEST_EXPLOER__
#include <Windows.h>
#endif //#ifdef __USE_TEST_EXPLOER__

WEIBO_struct_bodycallback_init(OAUTH_REQUEST_TOKEN)// ��ȡδ��Ȩ��Request Token
{
	// ����������д�� t_wb_oauth_access_token
	struct t_wb_oauth_access_token* poauth_acess = (struct t_wb_oauth_access_token*)pUserdata;
	if( poauth_acess && httpCode == 200 )
	{
		wb_parse_oauth(body , poauth_acess->wbauth_.oauth_token_ , poauth_acess->wbauth_.oauth_token_secret_,0 );

		printf("   oauth_request_token =  %s \n" , poauth_acess->wbauth_.oauth_token_ );
		printf("   oauth_request_token_secret =  %s \n" , poauth_acess->wbauth_.oauth_token_secret_ );

#ifdef __USE_TEST_EXPLOER__
		//
		char open[255] = {0};
		sprintf_s(open,255,"http://api.t.sina.com.cn/oauth/authorize?oauth_token=%s",poauth_acess->wbauth_.oauth_token_);
		ShellExecuteA( NULL,"open",open,NULL,NULL,SW_SHOWNORMAL );
		//
#endif // __USE_TEST_EXPLOER__
	}
	else
	{
		printf("   OAUTH_REQUEST_TOKEN : \n    %s \n\n" , body );
	}
}

//
// �����û���ȨToken 
WEIBO_struct_statuscallback_init(OAUTH_AUTHORIZE)
{	
}

WEIBO_struct_bodycallback_init(OAUTH_AUTHORIZE)
{
	struct t_wb_oauth* poauth = (struct t_wb_oauth*)pUserdata;

	printf("   OAUTH_ACCESS_TOKEN : \n    %s \n\n" , body );

	if( poauth && httpCode == 200 )
	{
		wbParserNS::REQOBJ *pRoot = wbParserNS::Parse_data_JSON(body);
		if( pRoot )
		{
			wbParserNS::GetCHAR_Key_JSON("oauth_verifier", pRoot , poauth->oauth_verifier_ , 64);
			wbParserNS::Parse_free_JSON(pRoot);
		}
		printf("oauth_acess_token =  %s \n"          , poauth->oauth_token_);
		printf("oauth_acess_token_verifier =  %s \n" , poauth->oauth_verifier_ );
	}
}

//
// ��ȡ��Ȩ����Access Token
WEIBO_struct_statuscallback_init(OAUTH_ACCESS_TOKEN)
{

}

WEIBO_struct_bodycallback_init(OAUTH_ACCESS_TOKEN)
{
	// ����������д�� t_wb_oauth,��������ս����oauth_token AND oauth_token_secret)
	// �Ժ������������󣬾���Ҫ ʹ����� t_wb_oauth �е�������

	struct t_wb_oauth* poauth = (struct t_wb_oauth*)pUserdata;

	if( poauth && httpCode == 200 )
	{
		wb_parse_oauth(body , poauth->oauth_token_ , poauth->oauth_token_secret_ ,0);

		printf("   oauth_acess_token =  %s \n" , poauth->oauth_token_ );
		printf("   oauth_acess_token_secret =  %s \n" , poauth->oauth_token_secret_ );
	}
	else
	{
		printf("   OAUTH_ACCESS_TOKEN : \n    %s \n\n" , body );
	}
}
// ��ȡ��Ȩ����Access Token
WEIBO_struct_statuscallback_init(XAUTH_ACCESS_TOKEN)
{

}
WEIBO_struct_bodycallback_init(XAUTH_ACCESS_TOKEN)
{
	// ����������д�� t_wb_oauth,��������ս����oauth_token AND oauth_token_secret)
	// �Ժ������������󣬾���Ҫ ʹ����� t_wb_oauth �е�������

	struct t_wb_oauth* poauth = (struct t_wb_oauth*)pUserdata;

	if( poauth && httpCode == 200 )
	{
		wb_parse_oauth(body , poauth->oauth_token_ , poauth->oauth_token_secret_ ,0);

		printf("   oauth_acess_token =  %s \n" , poauth->oauth_token_ );
		printf("   oauth_acess_token_secret =  %s \n" , poauth->oauth_token_secret_ );
	}
	else
	{
		printf("   OAUTH_ACCESS_TOKEN : \n    %s \n\n" , body );
	}
}

// ����
WEIBO_struct_statuscallback_init(GET_EMOTIONS)
{

}

WEIBO_struct_bodycallback_init(GET_EMOTIONS)
{
	printf("   GET_EMOTIONS : \n    %s \n\n" , body );
}

// �û�SEARCH
WEIBO_struct_statuscallback_init(GET_USERS_SEARCH)
{

}

WEIBO_struct_bodycallback_init(GET_USERS_SEARCH)
{
	printf("   GET_USERS_SEARCH : \n    %s \n\n" , body );
}

// ΢��SEARCH
WEIBO_struct_statuscallback_init(GET_WB_SEARCH)
{

}

WEIBO_struct_bodycallback_init(GET_WB_SEARCH)
{
	printf("   GET_WB_SEARCH : \n    %s \n\n" , body );
}

// 
WEIBO_struct_statuscallback_init(GET_STATUSES_SEARCH)
{

}

WEIBO_struct_bodycallback_init(GET_STATUSES_SEARCH)
{
	printf("   GET_STATUSES_SEARCH : \n    %s \n\n" , body );
}

// ʡ��
WEIBO_struct_statuscallback_init(GET_PROVINCES)
{

}

WEIBO_struct_bodycallback_init(GET_PROVINCES)
{
	printf("   GET_PROVINCES : \n    %s \n\n" , body );
}

// ʡ��
WEIBO_struct_statuscallback_init(REPORT)
{

}

WEIBO_struct_bodycallback_init(REPORT)
{
	printf("   REPORT : \n    %s \n\n" , body );
}

//cookie��ʽ
WEIBO_struct_statuscallback_init(COOKIE)
{

}


WEIBO_struct_bodycallback_init(COOKIE)
{
	struct t_wb_oauth* poauth = (struct t_wb_oauth*)pUserdata;

	//
	int iCount = 0;
	wbParserNS::t_wbParse_Cookie cookie;
	wbParserNS::t_wbParse_Cookie *pCookie = &cookie;
	wbParserNS::USE_WBPARSE_FUNC(Cookie,BASE,body,len,iCount,(void**)&pCookie,NULL,NULL);
	
	strcpy(poauth->tgt_,pCookie->tgt);

	printf("   COOKIE : \n    %s \n\n" , body );
}

WEIBO_struct_headerycallback_init(COOKIE)
{
	struct t_wb_oauth* poauth = (struct t_wb_oauth*)pUserdata;

	printf("   COOKIE : \n    %s \n\n" , header );
	if( poauth && httpCode == 200 )
	{
		wb_parse_cookie(header , poauth->oauth_token_ , poauth->oauth_token_secret_ );

		printf("   SUE =  %s \n" , poauth->oauth_token_ );
		printf("   SUP =  %s \n" , poauth->oauth_token_secret_ );
	}
}

//cookie��ʽ
WEIBO_struct_statuscallback_init(UPDATETGT)
{

}

WEIBO_struct_bodycallback_init(UPDATETGT)
{
	struct t_wb_oauth* poauth = (struct t_wb_oauth*)pUserdata;

	printf("   COOKIE : \n    %s \n\n" , body );
}

WEIBO_struct_headerycallback_init(UPDATETGT)
{
	struct t_wb_oauth* poauth = (struct t_wb_oauth*)pUserdata;

	printf("   COOKIE : \n    %s \n\n" , header );
	if( poauth && httpCode == 200 )
	{
		wb_parse_cookie(header , poauth->oauth_token_ , poauth->oauth_token_secret_ );

		printf("   SUE =  %s \n" , poauth->oauth_token_ );
		printf("   SUP =  %s \n" , poauth->oauth_token_secret_ );
	}
}

//�Զ���URL
WEIBO_struct_statuscallback_init(CUSTOM)
{

}

WEIBO_struct_bodycallback_init(CUSTOM)
{

}

WEIBO_struct_statuscallback_init(GETSTATUSES_COMMENTS_TOME)
{
}
WEIBO_struct_bodycallback_init(GETSTATUSES_COMMENTS_TOME)
{
}

WEIBO_struct_statuscallback_init(PUTSTATUSES_RESET_COUNT)
{
}
WEIBO_struct_bodycallback_init(PUTSTATUSES_RESET_COUNT)
{
}


// ��ȡϵͳ�Ƽ��û�
WEIBO_struct_statuscallback_init(GET_USERS_HOT)
{

}

WEIBO_struct_bodycallback_init(GET_USERS_HOT)
{
	printf("   GET_USERS_HOT : \n    %s \n\n" , body );
}

//�����޸ĵ�ǰ��¼�û�����ע��ĳ�����ѵı�ע��ϢNew!
WEIBO_struct_statuscallback_init(POST_USERS_REMARK)
{

}

WEIBO_struct_bodycallback_init(POST_USERS_REMARK)
{
	printf("   POST_USERS_REMARK : \n    %s \n\n" , body );
}


//Users/suggestions ���ص�ǰ�û����ܸ���Ȥ���û�
WEIBO_struct_statuscallback_init(GET_USERS_SUGGESTIONS)
{

}

WEIBO_struct_bodycallback_init(GET_USERS_SUGGESTIONS)
{
	printf("   GET_USERS_SUGGESTIONS : \n    %s \n\n" , body );
}




#include "callback/wbcb_block.inl"
#include "callback/wbcb_hotpoint.inl"
#include "callback/wbcb_tags.inl"
#include "callback/wbcb_trends.inl"
#include "callback/wbcb_invitecontact.inl"

//media 
WEIBO_struct_statuscallback_init(GET_MEDIA_SHORTURL_BATCH)
{

}
WEIBO_struct_bodycallback_init(GET_MEDIA_SHORTURL_BATCH)
{
	printf("   GET_MEDIA_SHORTURL_BATCH : \n    %s \n\n" , body );

	//int iOutCount = 0;
	//wbParserNS::t_wbParse_Media_ShortUrlBatch *pRet = NULL;
	//wbParserNS::WBPARSE_HANDLE handle =  wbParserNS::USE_WBPARSE_FUNC( MEDIA,SHORTURL,body,len,iOutCount,(void**)&pRet,NULL,NULL );
	//if( handle ){
	//	wbParserNS::USE_WBFREE_FUNC( MEDIA,SHORTURL,handle);
	//}
}

//-------------------------------------------------------------------------------------------//

static
struct t_wb_callback_byloach callback_byloach[  WEIBO_OPTION(LAST) ] =
{
#define INIT_CALLBACK_BYLOACH(NAME) {  WEIBO_struct_statuscallback_init_fun(NAME) , WEIBO_struct_bodycallback_init_fun(NAME) , NULL}
#define INIT_CALLBACK_BYWELBON(NAME) { WEIBO_struct_statuscallback_init_fun(NAME) , WEIBO_struct_bodycallback_init_fun(NAME) , NULL}

	{0,0,0},
	INIT_CALLBACK_BYLOACH(BASE),
	INIT_CALLBACK_BYLOACH(GETSTATUSES_PUBLIC_TIMELINE),
	INIT_CALLBACK_BYLOACH(GETSTATUSES_FRIENDS_TIMELINE),
	INIT_CALLBACK_BYLOACH(GETSTATUSES_USE_TIMELINE),
	INIT_CALLBACK_BYLOACH(GETSTATUSES_MENTIONS),
	INIT_CALLBACK_BYLOACH(GETSTATUSES_COMMENTS_TIMELINE),
	INIT_CALLBACK_BYLOACH(GETSTATUSES_COMMENTS_BYME),
	INIT_CALLBACK_BYLOACH(GETSTATUSES_COMMENTS_TOME),
	INIT_CALLBACK_BYLOACH(GETSTATUSES_COMMENTS_LIST),
	INIT_CALLBACK_BYLOACH(GETSTATUSES_COMMENTS_COUNTS),
	INIT_CALLBACK_BYLOACH(GETSTATUSES_UNREAD),
	INIT_CALLBACK_BYLOACH(PUTSTATUSES_RESET_COUNT),

	//΢������
	INIT_CALLBACK_BYLOACH(GETSTATUSES_SHOW),
	INIT_CALLBACK_BYLOACH(GOTOSTATUSES_ID),
	INIT_CALLBACK_BYLOACH(PUTSTATUSES_UPDATE),
	INIT_CALLBACK_BYLOACH(PUTSTATUSES_UPLOAD),
	INIT_CALLBACK_BYLOACH(PUTSTATUSES_DESTROY),
	INIT_CALLBACK_BYLOACH(PUTSTATUSES_REPOST),
	INIT_CALLBACK_BYLOACH(PUTSTATUSES_COMMENT),
	INIT_CALLBACK_BYLOACH(PUTSTATUSES_COMMENT_DESTROY),
	INIT_CALLBACK_BYLOACH(PUTSTATUSES_REPLY),

	//�û�
	INIT_CALLBACK_BYLOACH(GETUSER_INFO),
	INIT_CALLBACK_BYLOACH(GETFRINDS_LIST),
	INIT_CALLBACK_BYLOACH(GETFOLLOWERS_LIST),

	//˽��
	INIT_CALLBACK_BYLOACH(GETDIRECTMSG),
	INIT_CALLBACK_BYLOACH(GETDIRESTMSG_SENT),
	INIT_CALLBACK_BYLOACH(PUTDIRECTMSG_NEW),
	INIT_CALLBACK_BYLOACH(PUTDIRECTMSG_DESTROY),
	INIT_CALLBACK_BYLOACH(GETDIRECTMSG_WITH),

	//��ע
	INIT_CALLBACK_BYLOACH(PUTFRIENDSHIPS_CREATE),
	INIT_CALLBACK_BYLOACH(PUTFRIENDSHIPS_CREATE_BATCH),
	INIT_CALLBACK_BYLOACH(PUTFRIENDSHIPS_DESTROY),
	INIT_CALLBACK_BYLOACH(GETFRIENDSHIPS_EXISTS),
	INIT_CALLBACK_BYLOACH(GETFRIENDSHIPS_BATCH_EXISTS),

	//Social Graph
	INIT_CALLBACK_BYLOACH(GETFRIEND_IDS),
	INIT_CALLBACK_BYLOACH(GETFOLLOWER_IDS),

	//�˺�
	INIT_CALLBACK_BYLOACH(GETACCOUNT_VERIFY),
	INIT_CALLBACK_BYLOACH(GETACCOUNT_RATELIMIT),
	INIT_CALLBACK_BYLOACH(PUTACCOUNT_QUITSESSION),
	INIT_CALLBACK_BYLOACH(PUTACCOUNT_UPDATE_PROFILEIMAGE),
	INIT_CALLBACK_BYLOACH(PUTACCOUNT_UPDATE_PROFILE),
	INIT_CALLBACK_BYLOACH(PUTACCOUNT_REGISTER),

	// �ղ�
	INIT_CALLBACK_BYLOACH(GETFAVORITES),
	INIT_CALLBACK_BYLOACH(PUTFAVORITES_CREATE),
	INIT_CALLBACK_BYLOACH(PUTFAVORITES_DESTROY),

	//��¼/OAuth
	INIT_CALLBACK_BYLOACH(OAUTH_REQUEST_TOKEN),
	INIT_CALLBACK_BYLOACH(OAUTH_AUTHORIZE),
	INIT_CALLBACK_BYLOACH(OAUTH_ACCESS_TOKEN),

	INIT_CALLBACK_BYLOACH(GET_EMOTIONS),
	INIT_CALLBACK_BYLOACH(GET_USERS_SEARCH),
	INIT_CALLBACK_BYLOACH(GET_WB_SEARCH),
	INIT_CALLBACK_BYLOACH(GET_STATUSES_SEARCH),
	INIT_CALLBACK_BYLOACH(GET_PROVINCES),
	INIT_CALLBACK_BYLOACH(REPORT),//49 �ٱ�

	// COOKIE
	{ WEIBO_struct_statuscallback_init_fun(COOKIE) , WEIBO_struct_bodycallback_init_fun(COOKIE) , WEIBO_struct_headercallback_init_fun(COOKIE)},
	INIT_CALLBACK_BYLOACH(UPDATETGT),// UPDATETGT,

	//�Զ���URL
	INIT_CALLBACK_BYLOACH(CUSTOM),

	//
	INIT_CALLBACK_BYWELBON(HOT_REPOST_DAYLIY), //����ת��-by day
	INIT_CALLBACK_BYWELBON(HOT_REPOST_WEEKLIY), //����ת��-by week
	INIT_CALLBACK_BYWELBON(HOT_COMMENT_DAYLIY), //��������-by day
	INIT_CALLBACK_BYWELBON(HOT_COMMENT_WEEKLY), //��������-by week

	//
	INIT_CALLBACK_BYWELBON(GET_USERS_HOT),// ��ȡϵͳ�Ƽ��û�
	INIT_CALLBACK_BYWELBON(POST_USERS_REMARK),//�����޸ĵ�ǰ��¼�û�����ע��ĳ�����ѵı�ע��ϢNew!
	INIT_CALLBACK_BYWELBON(GET_USERS_SUGGESTIONS), //Users/suggestions ���ص�ǰ�û����ܸ���Ȥ���û�

	// ����ӿ� ,by welbon,2011-01-10
	INIT_CALLBACK_BYWELBON(GET_TRENDS),//trends ��ȡĳ�˵Ļ���
	INIT_CALLBACK_BYWELBON(GET_TRENDS_STATUSES),//trends/statuses ��ȡĳһ�����µ�΢��
	INIT_CALLBACK_BYWELBON(POST_TRENDS_FOLLOW),//trends/follow ��עĳһ������
	INIT_CALLBACK_BYWELBON(DELETE_TRENDS_DESTROY),//trends/destroy ȡ����ע��ĳһ������
	INIT_CALLBACK_BYWELBON(GET_TRENDS_HOURLY),//trends/destroy ��Сʱ�������Ż���
	INIT_CALLBACK_BYWELBON(GET_TRENDS_DAYLIY),//trends/daily �����ڷ������Ż��⡣����ĳһ���ڵĻ���
	INIT_CALLBACK_BYWELBON(GET_TRENDS_WEEKLIY),//trends/weekly ���ܷ������Ż��⡣����ĳһ����֮ǰĳһ�ܵĻ���

	// �������ӿ� ,by welbon,2011-01-10
	INIT_CALLBACK_BYWELBON(POST_BLOCKS_CREATE),//��ĳ�û����������
	INIT_CALLBACK_BYWELBON(POST_BLOCKS_DESTROY),//��ĳ�û��Ƴ�������
	INIT_CALLBACK_BYWELBON(GET_BLOCKS_EXISTS),//���ĳ�û��Ƿ��Ǻ������û�
	INIT_CALLBACK_BYWELBON(GET_BLOCKS_BLOCKING),//�г��������û�(����û���ϸ��Ϣ)
	INIT_CALLBACK_BYWELBON(GET_BLOCKS_BLOCKING_IDS),//�г���ҳ�������û���ֻ���id��

	//�û���ǩ�ӿ� ,by welbon,2011-01-10
	INIT_CALLBACK_BYWELBON(GET_TAGS),//tags ����ָ���û��ı�ǩ�б�
	INIT_CALLBACK_BYWELBON(POST_TAGS_CREATE),//tags/create ����û���ǩ
	INIT_CALLBACK_BYWELBON(GET_TAGS_SUGGESTIONS),//tags/suggestions �����û�����Ȥ�ı�ǩ
	INIT_CALLBACK_BYWELBON(POST_TAGS_DESTROY),//tags/destroy ɾ����ǩ
	INIT_CALLBACK_BYWELBON(POST_TAGS_DESTROY_BATCH),//tags/destroy_batch ����ɾ����ǩ

	// ����ӿ�
	INIT_CALLBACK_BYWELBON(POST_INVITE_MAILCONTACT),//����������ϵ��
	INIT_CALLBACK_BYWELBON(POST_INVITE_MSNCONTACT), //����MSN��ϵ��
	INIT_CALLBACK_BYWELBON(POST_INVITE_SENDMAILS),  //���������ʼ�

	// media
	INIT_CALLBACK_BYWELBON(GET_MEDIA_SHORTURL_BATCH),// ������ȡ������
    //��¼/XAuth
	INIT_CALLBACK_BYLOACH(XAUTH_ACCESS_TOKEN),

};

struct t_wb_callback_byloach* Wb_get_struct_callback( int option )
{
	if( WEIBO_OPTION(UNK) >= option ||
		WEIBO_OPTION(LAST) <= option )
	{
		option = 0;
	}
	return &callback_byloach[option];
}