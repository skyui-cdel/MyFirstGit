/**
*  @brief  Interface function implements
*  @file   weibo.h
*  @author loach 
*  @Email <loachmr@sina.com>
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


#ifndef __weibo_SDK_H__
#define __weibo_SDK_H__

/*
 *
 * ������˵Ŀ���ƽ̨�Ľӿڽ���C++����(http://open.t.sina.com.cn/wiki/index.php)
 * ���㿪���ߵ���
 *
 * ��Ȩ���� �����й�
 *
 * ���ߣ�loach
 *
 * ʱ�䣺2010-06-04
 *
 * �޸ģ� 2010-09-26
 *    1 ��� ��ȡ����Ľӿ�
 *    2 ��� �����ӿ�
 *   
*/

#ifdef  __cplusplus
extern "C" {
#endif

#if (defined(WIN32) || defined(_WIN32) ) && !defined(WEIBO_STATICLIB)

#if defined(BUILDING_LIBWEIBO)
#define WEIBO_API  __declspec(dllexport)
#else
#define WEIBO_API  __declspec(dllimport)
#endif

#else
	// ����ƽ̨�ģ������Ǿ�̬����
#define WEIBO_API
#endif

#ifndef BUILD_CHARACTER_SET

	typedef char WBChar;

#else

    // for unicode �ַ�ֵ
#if (defined(UNICODE) || defined(_UNICODE) )

	typedef wchar_t WBChar;

#else

	typedef char WBChar;

#endif

#endif // BUILD_CHARACTER_SET

	//
#define WB_MAX_LEN_DEFINE(name , val ) const unsigned int WB_##name##_LEN = val
#define WB_USE_LEN(name) WB_##name##_LEN

	// �����ĳ���
	WB_MAX_LEN_DEFINE(UID , 128 );
	WB_MAX_LEN_DEFINE(PWD , 64 );
	WB_MAX_LEN_DEFINE(OAUTH_TOKEN,1024); // ==> cookie's SUE
	WB_MAX_LEN_DEFINE(OAUTH_TOKEN_SECRET,2048); // ==> cookie's SUP		
	WB_MAX_LEN_DEFINE(OAUTH_VERIFIER,256);
	//
	WB_MAX_LEN_DEFINE(OPT_VALUE, 64);
	WB_MAX_LEN_DEFINE(WEIBO_ID , 64);
	WB_MAX_LEN_DEFINE(WEIBO_IDS, 2048); ///< ID�б��Զ��ŷָ�
	WB_MAX_LEN_DEFINE(WB_INFO  , 280);
	
	//
	WB_MAX_LEN_DEFINE(PROFILE_NAME, 20);
	WB_MAX_LEN_DEFINE(PROFILE_GENDER, 2);
	WB_MAX_LEN_DEFINE(PROFILE_EMAIL,100);
	WB_MAX_LEN_DEFINE(PROFILE_PROVINCE, 20);
	WB_MAX_LEN_DEFINE(PROFILE_CITY, 20);
	WB_MAX_LEN_DEFINE(PROFILE_LOCATION,30);
	WB_MAX_LEN_DEFINE(PROFILE_DESCRIPTION,160);

	// ������Ϣ����
	WB_MAX_LEN_DEFINE(GEOCODE,128);

	// ʱ��
	WB_MAX_LEN_DEFINE(TIME,32);

	//
	WB_MAX_LEN_DEFINE(DIRECTMSG, 600);

	//
    WB_MAX_LEN_DEFINE(POST, 2048);
	// url length
	WB_MAX_LEN_DEFINE(URL , 1024);
	WB_MAX_LEN_DEFINE(MAX_PATH, 512);
	WB_MAX_LEN_DEFINE(IP, 100);

	// tags
	WB_MAX_LEN_DEFINE(SCREEN_NAME,100);
	WB_MAX_LEN_DEFINE(TAGS_INFO,2048);

	// ʵ�ʵĳ���
#define WB_REAL_LEN(name) WB_##name##_LEN + 1

	// �����ʽ���������ݸ�ʽ
	typedef enum
	{
		E_FORMAT_JSON = 0,
		E_FORMAT_XML,
	}WEIBORequestFormat;

	// ���� OPTION ���Զ����Ͻṹ��ͨ��weibo_opt_initstruct������ʼ��
#define WEIBO_OPTION(name) WEIBO_OPT_##name
#define WEIBO_OPTION_DEFINE(name , val) WEIBO_OPTION(name) = val

	typedef enum
	{
		WEIBO_OPTION_DEFINE(UNK , 0),
		WEIBO_OPTION(BASE), //1

		//��ȡ�������ݼ�(timeline)
		WEIBO_OPTION(GETSTATUSES_PUBLIC_TIMELINE), //2 ��ȡ���¸��µĹ���΢����Ϣ 
		WEIBO_OPTION(GETSTATUSES_FRIENDS_TIMELINE), //3 ��ȡ��ǰ�û�����ע�û�������΢����Ϣ (����: statuses/home_timeline) 
		WEIBO_OPTION(GETSTATUSES_USE_TIMELINE), //4 ��ȡ�û�������΢����Ϣ�б� 
		WEIBO_OPTION(GETSTATUSES_MENTIONS), //5 ��ȡ@��ǰ�û���΢���б� 
		WEIBO_OPTION(GETSTATUSES_COMMENTS_TIMELINE), //6 ��ȡ��ǰ�û����ͼ��յ��������б�
		WEIBO_OPTION(GETSTATUSES_COMMENTS_BYME), //7 ��ȡ��ǰ�û����������� 
		WEIBO_OPTION(GETSTATUSES_COMMENTS_TOME), //8 ��ȡ��ǰ�û��յ������� 
		WEIBO_OPTION(GETSTATUSES_COMMENTS_LIST), //9 ��ȡָ��΢���������б� 
		WEIBO_OPTION(GETSTATUSES_COMMENTS_COUNTS), //10 ������ȡһ��΢������������ת���� 
		WEIBO_OPTION(GETSTATUSES_UNREAD), //11 ��ȡ��ǰ�û�δ����Ϣ�� 
		WEIBO_OPTION(PUTSTATUSES_RESET_COUNT),//12 δ����Ϣ������ӿ� 

		//΢������
		WEIBO_OPTION(GETSTATUSES_SHOW), //13 ����ID��ȡ����΢����Ϣ���� 
		WEIBO_OPTION(GOTOSTATUSES_ID),//14 ����΢��ID���û�ID��ת������΢��ҳ�� 
		WEIBO_OPTION(PUTSTATUSES_UPDATE),//15 ����һ��΢����Ϣ 
		WEIBO_OPTION(PUTSTATUSES_UPLOAD),//16 �ϴ�ͼƬ������һ��΢����Ϣ 
		WEIBO_OPTION(PUTSTATUSES_UPLOAD_PIC),//17 �ϴ�ͼƬ
		WEIBO_OPTION(PUTSTATUSES_DESTROY),//18 ɾ��һ��΢����Ϣ 
		WEIBO_OPTION(PUTSTATUSES_REPOST),//19 ת��һ��΢����Ϣ���ɼ����ۣ� 
		WEIBO_OPTION(PUTSTATUSES_COMMENT),//20 ��һ��΢����Ϣ�������� 
		WEIBO_OPTION(PUTSTATUSES_COMMENT_DESTROY),//21 ɾ����ǰ�û���΢��������Ϣ 
		WEIBO_OPTION(PUTSTATUSES_REPLY),//22 �ظ�΢��������Ϣ 

		//�û�
		WEIBO_OPTION(GETUSER_INFO),//23 �����û�ID��ȡ�û����ϣ���Ȩ�û��� 
		WEIBO_OPTION(GETFRINDS_LIST),//24 ��ȡ��ǰ�û���ע�����б�����һ��΢����Ϣ 
		WEIBO_OPTION(GETFOLLOWERS_LIST),//25 ��ȡ��ǰ�û���˿�б�����һ��΢����Ϣ 

		//˽��
		WEIBO_OPTION(GETDIRECTMSG),//26 ��ȡ��ǰ�û�����˽���б� 
		WEIBO_OPTION(GETDIRESTMSG_SENT),//27 ��ȡ��ǰ�û����͵�����˽���б�
		WEIBO_OPTION(PUTDIRECTMSG_NEW),//28 ����һ��˽�� 
		WEIBO_OPTION(PUTDIRECTMSG_DESTROY),//29 ɾ��һ��˽�� 
		WEIBO_OPTION(GETDIRECTMSG_WITH),//30��ȡ�û�����˽���б�
		
		//��ע
		WEIBO_OPTION(PUTFRIENDSHIPS_CREATE),//31 ��עĳ�û� 
		WEIBO_OPTION(PUTFRIENDSHIPS_CREATE_BATCH),//32 add by welbon,2011-01-21 ������ע�ӿ�
		WEIBO_OPTION(PUTFRIENDSHIPS_DESTROY),//33 ȡ����ע 
		WEIBO_OPTION(GETFRIENDSHIPS_EXISTS),//34 �ж������û��Ƿ��й�ע��ϵ�����������û���ϵ����ϸ���
		WEIBO_OPTION(GETFRIENDSHIPS_BATCH_EXISTS),//35 ������ȡһ���û��Ƿ�Ϊ����

        //Social Graph
		WEIBO_OPTION(GETFRIEND_IDS),//36 ��ע�б�
		WEIBO_OPTION(GETFOLLOWER_IDS),// 37 ��˿�б�
		
		//�˺� 
	    WEIBO_OPTION(GETACCOUNT_VERIFY),//38 ��֤��ǰ�û�����Ƿ�Ϸ� 
		WEIBO_OPTION(GETACCOUNT_RATELIMIT),//39 ��ȡ��ǰ�û�API����Ƶ������ 
		WEIBO_OPTION(PUTACCOUNT_QUITSESSION),//40 ��ǰ�û��˳���¼ 
		WEIBO_OPTION(PUTACCOUNT_UPDATE_PROFILEIMAGE),//41 ����ͷ��
		WEIBO_OPTION(PUTACCOUNT_UPDATE_PROFILE),//42 ��������
		WEIBO_OPTION(PUTACCOUNT_REGISTER),//43
		
		// �ղ�
		WEIBO_OPTION(GETFAVORITES),//44 ��ȡ��ǰ�û����ղ��б� 
		WEIBO_OPTION(PUTFAVORITES_CREATE),//45 ����ղ� 
		WEIBO_OPTION(PUTFAVORITES_DESTROY),// 46 ɾ����ǰ�û��ղص�΢����Ϣ 

		//��¼/OAuth
		WEIBO_OPTION(OAUTH_REQUEST_TOKEN),//47 ��ȡδ��Ȩ��Request Token 
		WEIBO_OPTION(OAUTH_AUTHORIZE),//48 �����û���ȨToken 
		WEIBO_OPTION(OAUTH_ACCESS_TOKEN),// 49 ��ȡ��Ȩ����Access Token

		// ����
		WEIBO_OPTION(GET_EMOTIONS),// 50 ��������΢���ٷ����б��顢ħ������������Ϣ����������������͡�������࣬�Ƿ����ŵȡ� 
		
		// �û����� 
		WEIBO_OPTION(GET_USERS_SEARCH),// 51 ����΢���û�,���عؼ���ƥ���΢���û���(�������˺��������߿���) 
		
		// ΢������ 
		WEIBO_OPTION(GET_WB_SEARCH),//52 ���عؼ���ƥ���΢����(�������˺��������߿���) 
		WEIBO_OPTION(GET_STATUSES_SEARCH),// 53 ����΢��(���������) (���Ժ��������߿���) 

		WEIBO_OPTION(GET_PROVINCES), //54 ʡ�ݳ��б���� 
		WEIBO_OPTION(REPORT),// 55 �ٱ�

		WEIBO_OPTION(COOKIE),// 56 cookie
		WEIBO_OPTION(UPDATETGT), // 57 ����cookie

		//�Զ���URL
		WEIBO_OPTION(CUSTOM),// 58

		// �¼ӽӿ�,by welbon,2011-01-07
		WEIBO_OPTION(HOT_REPOST_DAYLIY), //59 ����ת��-by day
		WEIBO_OPTION(HOT_REPOST_WEEKLY), //60 ����ת��-by week
		WEIBO_OPTION(HOT_COMMENT_DAYLIY), //61 ��������-by day
		WEIBO_OPTION(HOT_COMMENT_WEEKLY), // 62 ��������-by week

		WEIBO_OPTION(GET_USERS_HOT),//63 ��ȡϵͳ�Ƽ��û�
		WEIBO_OPTION(POST_USERS_REMARK),//64 �����޸ĵ�ǰ��¼�û�����ע��ĳ�����ѵı�ע��ϢNew!
		WEIBO_OPTION(GET_USERS_SUGGESTIONS), // 65 Users/suggestions ���ص�ǰ�û����ܸ���Ȥ���û�

		// ����ӿ� ,by welbon,2011-01-10
		WEIBO_OPTION(GET_TRENDS),// 66 trends ��ȡĳ�˵Ļ���
		WEIBO_OPTION(GET_TRENDS_STATUSES),//67 trends/statuses ��ȡĳһ�����µ�΢��
		WEIBO_OPTION(POST_TRENDS_FOLLOW),//68 trends/follow ��עĳһ������
		WEIBO_OPTION(DELETE_TRENDS_DESTROY),//69 trends/destroy ȡ����ע��ĳһ������
		WEIBO_OPTION(GET_TRENDS_HOURLY),//70 trends/destroy ��Сʱ�������Ż���
		WEIBO_OPTION(GET_TRENDS_DAYLIY),//71 trends/daily �����ڷ������Ż��⡣����ĳһ���ڵĻ���
		WEIBO_OPTION(GET_TRENDS_WEEKLIY),// 72 trends/weekly ���ܷ������Ż��⡣����ĳһ����֮ǰĳһ�ܵĻ���

		// �������ӿ� ,by welbon,2011-01-10
		WEIBO_OPTION(POST_BLOCKS_CREATE),//73 ��ĳ�û����������
		WEIBO_OPTION(POST_BLOCKS_DESTROY),//74 ��ĳ�û��Ƴ�������
		WEIBO_OPTION(GET_BLOCKS_EXISTS),//75 ���ĳ�û��Ƿ��Ǻ������û�
		WEIBO_OPTION(GET_BLOCKS_BLOCKING),//76 �г��������û�(����û���ϸ��Ϣ)
		WEIBO_OPTION(GET_BLOCKS_BLOCKING_IDS),//77 �г���ҳ�������û���ֻ���id��

		//�û���ǩ�ӿ� ,by welbon,2011-01-10
		WEIBO_OPTION(GET_TAGS),//78 tags ����ָ���û��ı�ǩ�б�
		WEIBO_OPTION(POST_TAGS_CREATE),//79 tags/create ����û���ǩ
		WEIBO_OPTION(GET_TAGS_SUGGESTIONS),//80 tags/suggestions �����û�����Ȥ�ı�ǩ
		WEIBO_OPTION(POST_TAGS_DESTROY),//81 tags/destroy ɾ����ǩ
		WEIBO_OPTION(POST_TAGS_DESTROY_BATCH),// 82 tags/destroy_batch ����ɾ����ǩ

		//��ϵ����
		WEIBO_OPTION(POST_INVITE_MAILCONTACT),//83 ����������ϵ��
		WEIBO_OPTION(POST_INVITE_MSNCONTACT), //84 ����MSN��ϵ��
		WEIBO_OPTION(POST_INVITE_SENDMAILS),  // 85 ���������ʼ�

		//
		WEIBO_OPTION(GET_MEDIA_SHORTURL_BATCH),///< 86 ����ȡ�ö�������ĸ�������Ϣ

		//��¼/XAuth
		WEIBO_OPTION(XAUTH_ACCESS_TOKEN),//87  ��ȡ��Ȩ����Access Token

		
		WEIBO_OPTION(LAST),
	}WEIBOoption;

	const int OPT_PARAMS_COUNTS = 9;
	/** ��̬���� */
	struct t_wb_option_params
	{
		enum
		{
			WBOPTType_unk = 0,
			WBOPTType_since_id,///< ����΢��idID����ֵָ����since_id�󣨱�since_idʱ����ģ���΢����Ϣ��
			WBOPTType_max_id,///< ����΢��idID������ָ����max_id(ʱ�䲻����max_id)��΢����Ϣ��
			WBOPTType_count,///< ���ص�����¼��count����ҳ���С��
			WBOPTType_page,///< ��ҳ�Ų�ѯ 
			WBOPTType_cursor,
			WBOPTType_role,
			WBOPTType_follow,
			WBOPTType_with_new_status, ///< for ��ȡ��ǰ�û�Webδ����Ϣ��
		};
		WBChar szOpt_[OPT_PARAMS_COUNTS][ WB_REAL_LEN(OPT_VALUE) ];///< �������õ�type_�������� 4��
	};
    

	//�û� id ֵ�����ֱ�ʾ)
	struct t_wb_uid
	{
		// uidtype ����,�û� id ֵ�����ֱ�ʾ)
		enum
		{ 
			IDType_unk=0,
			IDType_id,
			IDType_screenname,
			IDType_userid,
		};
		int uidtype_;
		WBChar uid_[ WB_REAL_LEN(UID) ];
	};

	/** ����״̬�ص����� */
	typedef void (*fun_wb_status_callback)(int state , int errCode , void* pUserdata);

	// body ���� 
	typedef void (*fun_wb_body_callback)(unsigned long httpCode , const char* body , unsigned long len , void* pUserdata);

	// header ����
	typedef void (*fun_wb_header_callback)(unsigned long httpcode,const char* header , unsigned long len , void* pUsrData);

	/** ÿ�����ض���Ҫ��� */
	struct t_wb_oauth
	{
		union{
			char oauth_token_[ WB_REAL_LEN(OAUTH_TOKEN) ];
			char sue_[ WB_REAL_LEN(OAUTH_TOKEN) ];
		};
		union{
			char oauth_token_secret_[ WB_REAL_LEN(OAUTH_TOKEN_SECRET) ];
			char sup_[ WB_REAL_LEN(OAUTH_TOKEN_SECRET) ];
		};
		union{
			//  �ӹٷ���ҳ��ȡ�� ��֤���ص���֤�� ��Ϊ��oauth������ ,��ȡ��Ȩ����Access Token
			char oauth_verifier_[ WB_REAL_LEN(OAUTH_VERIFIER) ];
			char tgt_[ WB_REAL_LEN(OAUTH_VERIFIER) ];
		};
		int  format_; ///< json/xml
		int  cookie_; ///< �Ƿ�cookie��ʽ

		// ÿ��������������ص�����
		fun_wb_status_callback fstatus_cb;
		fun_wb_body_callback   fbody_cb;
		fun_wb_header_callback fheader_cb;
	};

	/** ���ṹ����������У�����ת�� �� */
	struct t_wb_base
	{
		t_wb_oauth wbauth_;
	};

	//////////////////////////////////////////////////////////////////////////////////////
	//��ȡ�������ݼ�

	// ��ȡ���¸��µĹ���΢����Ϣ 
	struct t_wb_get_statuses_public_timeline
	{
		t_wb_oauth wbauth_;
	};

	// �����û����й�ע�û�����n��΢����Ϣ�����û����ҵ���ҳ������������ͬ��
	struct t_wb_get_statuses_friends_timeline
	{
		t_wb_oauth wbauth_;

		t_wb_option_params wbopt_;
	};

	// �����û��ķ��������n����Ϣ�����û�΢��ҳ�淵��������һ�µġ��˽ӿ�Ҳ�������������û������·���΢����
	struct t_wb_get_statuses_user_timeline
	{
		t_wb_oauth wbauth_;

		// �����������������û� 
		t_wb_uid wbuid_;

		t_wb_option_params wbopt_;
	};

	//��������n��@�ҵ�΢�� 
	struct t_wb_get_statuses_mentions
	{
		t_wb_oauth wbauth_;

		t_wb_option_params wbopt_;
	};

	//��ȡ��ǰ�û����ͼ��յ��������б� 
	struct t_wb_get_statuses_comments_timeline
	{
		t_wb_oauth wbauth_;

		t_wb_option_params wbopt_;
	};

	//��ȡ��ǰ�û����������� 
	struct t_wb_get_statuses_comments_byme
	{
		t_wb_oauth wbauth_;

		t_wb_option_params wbopt_;
	};

	//��ȡ��ǰ�û��յ������� 
	#define t_wb_get_statuses_comments_tome t_wb_get_statuses_comments_byme


	// ��ȡָ��΢���������б� 
	struct t_wb_get_statuses_comments_list
	{
		t_wb_oauth wbauth_;

		WBChar wbId_[ WB_REAL_LEN(WEIBO_ID) ]; 

		t_wb_option_params wbopt_;
	};

	const int BATCH_WBIDS_COUNTS = 100;
	/*
	 * ����ͳ��΢����(��������ת���� , һ�����100��)
	*/
	struct t_wb_get_statuses_counts 
	{
		t_wb_oauth wbauth_;

		WBChar wbIds_[BATCH_WBIDS_COUNTS][WB_REAL_LEN(WEIBO_ID)];///<΢��ID , ��� 100
	};

	//��ȡ��ǰ�û�Webδ����Ϣ��������@�ҵ�, �����ۣ���˽�ţ��·�˿����
	struct t_wb_get_statuses_unread
	{
		t_wb_oauth wbauth_;

		t_wb_option_params wbopt_;
	};

	// ����ǰ�û�ָ�����͵�δ����Ϣ����0��
	struct t_wb_get_statuses_reset_count
	{
		t_wb_oauth wbauth_;

		enum Type
		{
			Ty_comments = 1,
			///<1. ������
			Ty_atme,
			///< 2. @me��
			Ty_directmsg,
			///<3. ˽����
			Ty_friends,
			///<4. ��ע�� 
		};
		int type_; ///< �ο� Typeֵ
	};
	//////////////////////////////////////////////////////////////////////////////////////
	//΢������

	//��ȡ����ID��΢����Ϣ��������Ϣ��ͬʱ���ء�
	struct t_wb_get_statuses_show
	{
		t_wb_oauth wbauth_;

		WBChar wbId_[ WB_REAL_LEN(WEIBO_ID) ]; 
	};

	//����΢����Web����·��������ͨ����url��ת��΢����Ӧ��Web��ҳ��
	struct t_wb_goto_user_status_id
	{
		t_wb_oauth wbauth_;

		WBChar uid_[  WB_REAL_LEN(UID) ];
		WBChar wbId_[ WB_REAL_LEN(WEIBO_ID) ];
	};

	/*
	 * ����һ��΢����Ϣ��Ϊ��ֹ�ظ�����������Ϣ�뵱ǰ������Ϣһ���������ᱻ���ԡ� 
	*/
	struct t_wb_put_statuses_update
	{
		t_wb_oauth wbauth_;

		WBChar szwbInfo_[WB_REAL_LEN(WB_INFO)];// Ҫ���µ�΢����Ϣ,��Ϣ���ݲ�����140�����֡� 

	};

	/*�ϴ�ͼƬ������΢����Ϣ
	 * ͼƬ��������ͼƬ�ļ���·����Ҳ����ֱ������ͼƬ�Ķ����������������ϴ�
	*/
	struct t_wb_put_statuses_upload
	{
		t_wb_oauth wbauth_;

		WBChar szImagePath_[WB_REAL_LEN(MAX_PATH) ];///< ͼƬ·��

		WBChar szwbInfo_[WB_REAL_LEN(WB_INFO)];// Ҫ���µ�΢����Ϣ,��Ϣ���ݲ�����140�����֡� 
	};

	/*�ϴ�ͼƬ
	* ͼƬ��������ͼƬ�ļ���·����Ҳ����ֱ������ͼƬ�Ķ����������������ϴ�
	*/
	struct t_wb_put_statuses_upload_pic
	{
		t_wb_oauth wbauth_;
		WBChar szImagePath_[ WB_REAL_LEN(MAX_PATH) ];///< ͼƬ·��
	};


	/*ɾ��΢����ע�⣺ֻ��ɾ���Լ���������Ϣ�� 
	*/
	struct t_wb_put_statuses_destroy
	{
		t_wb_oauth wbauth_;

		WBChar wbId_[WB_REAL_LEN(WEIBO_ID)];//ɾ����΢��ID
	};

	/*ת��һ��΢����Ϣ�����������POST��ʽ�ύ*/
	struct t_wb_put_statuses_repost
	{
		t_wb_oauth wbauth_;

		WBChar wbId_[WB_REAL_LEN(WEIBO_ID)];///< ת����΢��ID 

		WBChar szwbInfo_[WB_REAL_LEN(WB_INFO)]; ///< ת����Ϣ����Ϣ���ݲ�����140������
		
		int iCommentFlags ; ///< �Ƿ���ת����ͬʱ��������
	};

	//��һ��΢����Ϣ��������
	struct t_wb_put_statuses_comment
	{
		t_wb_oauth wbauth_;

		WBChar wbId_[WB_REAL_LEN(WEIBO_ID)];///< Ҫ���۵�΢��id 
		WBChar wbCId_[WB_REAL_LEN(WEIBO_ID)]; ///< ѡ������� Ҫ���۵�����id 
		WBChar szwbInfo_[WB_REAL_LEN(WB_INFO)]; ///< �������ݡ�,��Ϣ���ݲ�����140�����֡� 
	};

	//ɾ��΢�������ۡ�ע�⣺ֻ��ɾ���Լ���������Ϣ�� 
	struct t_wb_put_statuses_comment_destroy
	{
		t_wb_oauth wbauth_;

		WBChar wbId_[WB_REAL_LEN(WEIBO_ID)];///< ΢��id[ֻΪ�ͻ��˱���ʹ��]

		WBChar wbCId_[WB_REAL_LEN(WEIBO_ID)];///< Ҫɾ��������id 
	};

	/*��һ��΢��������Ϣ���лظ������������POST��ʽ�ύ��Ϊ��ֹ�ظ�����������Ϣ�����һ������/�ظ���Ϣһ���������ᱻ���ԡ� 
	*/ 
	struct t_wb_put_statuses_reply 
	{
		t_wb_oauth wbauth_;

		WBChar wbId_[WB_REAL_LEN(WEIBO_ID)];//Ҫ�ظ������۵�΢��id 
		WBChar wbCId_[WB_REAL_LEN(WEIBO_ID)];//Ҫ���۵�����id 
		WBChar szwbInfo_[WB_REAL_LEN(WB_INFO)];// ��������,��Ϣ���ݲ�����140�����֡� 
	};

	///////////////////////////////////////////////////////////////////////////////////////
	//�û��ӿ� 

	// �����û�ID��ȡ�û����ϣ���Ȩ�û���Ϊ�˱����û���˽��ֻ���û������˹�����Է�˿�����˹��������ݲŻ᷵�ء�  
	struct t_wb_get_users_show
	{
		t_wb_oauth wbauth_;

		t_wb_uid  wbuid_;
	};

	/** ��ȡ��ǰ�û���ע�����б�����һ��΢����Ϣ 
	 * �����û���ע�����б�����������΢�����¡�����ע�˵Ĺ�עʱ�䵹�򷵻أ�
	 * ÿ�η���N��,ͨ��cursor������ȡ�ö���N�Ĺ�ע�ˡ���ȻҲ����ͨ��ID,nickname,user_id��������ȡ�����˵Ĺ�ע���б� 
	 */
	struct t_wb_get_statuses_friends
	{
		t_wb_oauth wbauth_;

		t_wb_uid  wbuid_;

		// ֧�� cursor AND count
		t_wb_option_params wbopt_;
	};

	/**
	 * �����û��ķ�˿�б������ط�˿������΢����
	 * ����˿�Ĺ�עʱ�䵹�򷵻أ�ÿ�η���100��,ͨ��cursor������ȡ�ö���100�ķ�˿��ע��Ŀǰ�ӿ����ֻ����5000����˿��
	*/
    #define t_wb_get_statuses_followers t_wb_get_statuses_friends

	/* ϵͳ�Ƽ��û� 
	* ���࣬����ĳһ�����Ƽ��û���Ĭ��Ϊdefault��
	* default��������ע
	* ent��Ӱ������
	* hk_famous����̨����
	* model��ģ��
	* cooking����ʳ&����
	* sport����������
	* finance���̽�����
	* tech��IT������
	* singer������
	* writer������
	* moderator��������
	* medium��ý���ܱ�
	* stockplayer�����ɸ���
	*/
	struct t_wb_users_hot
	{
		t_wb_oauth wbauth_;

		// �Ƽ����࣬����������Ϸ����У����ؿ��б�
		WBChar category_[WB_REAL_LEN(WEIBO_ID)];
	};

	/** ���µ�ǰ��¼�û�����ע��ĳ�����ѵı�ע��Ϣ */
	struct t_wb_users_remark
	{
		t_wb_oauth wbauth_;

		//�û�ID��
		WBChar userId_[WB_REAL_LEN(WEIBO_ID)];

		//���µı�ע
		WBChar remark_[WB_REAL_LEN(WB_INFO)];
	};

	/** ���ص�ǰ�û����ܸ���Ȥ���û� */
	struct t_wb_users_suggestions
	{
		t_wb_oauth wbauth_;

		//�Ƿ񷵻��Ƽ�ԭ�򣬿�ѡֵ1/0����ֵΪ1�����ؽ���������Ƽ�ԭ�򣬻����ı䷵��ֵ��ʽ��
		int with_reason;
	};


	/////////////////////////////////////////////////////////////////////////////////////
	//˽��

	/** ��ȡ��ǰ�û�����˽���б� 
	 * �����û�������n��˽�ţ������������ߺͽ����ߵ���ϸ���ϡ�
	 */
	struct t_wb_get_direct_message
	{
		t_wb_oauth wbauth_;

		t_wb_option_params wbopt_;
	};

	// ���ص�¼�û��ѷ�������˽��
	struct t_wb_get_direct_message_sent
	{
		t_wb_oauth wbauth_;

		t_wb_option_params wbopt_;
	};

	/*
	 * ����һ��˽��
	*/
	struct t_wb_put_direct_message_new 
	{
		t_wb_oauth wbauth_;

		t_wb_uid  wbuid_;

		WBChar szText_[WB_REAL_LEN(DIRECTMSG)];//˽������
	};

	/*
	 * ��IDɾ��˽�š������û�����Ϊ˽�ŵĽ����ˡ�
	*/
	struct t_wb_put_direct_message_destroy
	{
		t_wb_oauth wbauth_;

		WBChar wbId_[WB_REAL_LEN(WEIBO_ID)];//Ҫɾ����˽������ID. 
	};

	/*
	 * ��ID��ȡ����˽���б������û�����Ϊ˽�ŵĽ����ˡ�
	*/
	struct t_wb_get_direct_message_with
	{
		t_wb_oauth wbauth_;

		int count_; //����˽��������Ĭ��Ϊ50��
		int page_;  //���ؽ����ҳ��š�

		WBChar wbuid_[WB_REAL_LEN(WEIBO_ID)];//ID
	};

	////////////////////////////////////////////////////////////////////////////
	// ��ע�ӿ�

	// ��עĳ�û� 
	struct t_wb_put_friendships_create
	{
		t_wb_oauth wbauth_;

		t_wb_uid wbuid_;
	};

	// ������ע�ӿ�
	struct t_wb_put_friendships_create_batch
	{
		t_wb_oauth wbauth_;

		WBChar wbIDs_[WB_REAL_LEN(WEIBO_IDS)];//������עID�б����ŷָ� 
	};

	// ȡ����ע 
	struct t_wb_put_friendships_destroy 
	{
		t_wb_oauth wbauth_;

		t_wb_uid wbuid_;

		int is_follower;
	};


	/* �ж������û��Ƿ��й�ע��ϵ�����������û���ϵ����ϸ���  */
	struct t_wb_get_friendships_show
	{
		t_wb_oauth wbauth_;

		t_wb_uid wbuid_source_;

		t_wb_uid wbuid_target_;
	};

	/* ������ȡһ���û��Ƿ�Ϊ���� */
	struct t_wb_get_friendships_batchexist
	{
		t_wb_oauth wbauth_;

		//
		WBChar wbIDs_[WB_REAL_LEN(WEIBO_IDS)];//������ȡ�û���ϵ��ID�б����ŷָ�
	};


	////////////////////////////////////////////////////////////////////////////
	// Social Graph

	//��ȡ�û���ע����uid�б� 
	struct t_wb_get_friends_ids
	{
		t_wb_oauth wbauth_;

		t_wb_uid wbuid_;

		// support count AND cursor 
		t_wb_option_params wbopt_;
	};

	// ��ȡ�û���˿����uid�б�
    #define t_wb_get_followers_ids t_wb_get_friends_ids


	////////////////////////////////////////////////////////////////////////////
	//�˺Žӿ�

	/**
	 *
	 * ����û�����ͨ��֤�����֤�ɹ����û��Ѿ���ͨ΢���򷵻� http״̬Ϊ 200��
	 ����ǲ��򷵻�401��״̬�ʹ�����Ϣ���˷��������ж��û�����Ƿ�Ϸ����Ѿ���ͨ΢����
	 */
	struct t_wb_get_account_verify_credentials
	{
		t_wb_oauth wbauth_;
	};

	// ����API�ķ���Ƶ�����ơ����ص�ǰСʱ���ܷ��ʵĴ�����Ƶ�������Ǹ����û��������������ƣ�������Բμ�Ƶ������˵���� 
    #define t_wb_get_account_rate_limit_status t_wb_get_account_verify_credentials
	
	//�������֤�û���session���˳���¼������cookie��Ϊnull����Ҫ����widget��webӦ�ó��ϡ�
    #define t_wb_put_account_end_session t_wb_get_account_verify_credentials


	/** �����Լ���ͷ�� , ����ΪС��700K����Ч��GIF, JPG, �� PNG ͼƬ. ���ͼƬ����500���ؽ����������� */
	struct t_wb_put_account_up_profileimage
	{
		t_wb_oauth wbauth_;

		WBChar szImagePath_[ WB_REAL_LEN(MAX_PATH) ];///< �����ļ���
	};

	/** �û���Ϣ */
	struct t_wb_profile
	{
		//�ǳ�
		WBChar szName_[WB_REAL_LEN(PROFILE_NAME)];
		//�Ա�
		WBChar szGender_[WB_REAL_LEN(PROFILE_GENDER)];
		//ʡ��ID
		WBChar szProvince_[WB_REAL_LEN(PROFILE_PROVINCE)];
		//����ID
		WBChar szCity_[ WB_REAL_LEN(PROFILE_CITY) ];
		//����
		WBChar szDescription_[WB_REAL_LEN(PROFILE_DESCRIPTION)];
	};

	/*�Զ���΢��ҳ��Ĳ�����ֻ���޸Ĳ��������*/
	struct t_wb_put_account_up_profile 
	{
		t_wb_oauth wbauth_;

		struct t_wb_profile profile_;
	};

	/** ע��۲�֧��,�ٷ���վע���
	 * �ýӿ�Ϊ���޽ӿڣ�ֻ��������ĺ�����鿪�ţ��� 
	*/
	struct t_wb_put_account_register
	{
		t_wb_oauth wbauth_;

		// nick. �ǳƣ��������.������20������ 
		WBChar szNick_[ WB_REAL_LEN(PROFILE_NAME)];

		// gender �Ա𣬱������. m,�У�f,Ů�� 
		WBChar szGender_[ WB_REAL_LEN(PROFILE_GENDER) ];

		// province ��ѡ����. �ο�ʡ�ݳ��б����  
		WBChar szProvince_[WB_REAL_LEN(PROFILE_PROVINCE)];//ʡ��ID

		// city ��ѡ����. �ο�ʡ�ݳ��б����,1000Ϊ���� 
		WBChar szCity_[ WB_REAL_LEN(PROFILE_CITY) ];//����ID

    	// email ע������ �������.
		WBChar szEmail_[WB_REAL_LEN(PROFILE_EMAIL)];

		// password ���� �������. 
		WBChar szPwd_[ WB_REAL_LEN(PWD)];

		// ip ���������ע���û��û���ǰ��ʵ��IP��
		WBChar szIP_[ WB_REAL_LEN(IP) ];//����ID
	};

    ////////////////////////////////////////////////////////////////////////////
	// �ղؽӿ� 

	// ��ȡ��ǰ�û����ղ��б� 
    struct t_wb_get_favorites
	{
		t_wb_oauth wbauth_;

		// page
		t_wb_option_params wbopt_;
	};
	
	//����ղ� 
	struct t_wb_put_favorites_create
	{
		t_wb_oauth wbauth_;

		WBChar wbId_[WB_REAL_LEN(WEIBO_ID)];
	};

	//ɾ����ǰ�û��ղص�΢����Ϣ
    #define t_wb_put_favorites_destroy t_wb_put_favorites_create

	////////////////////////////////////////////////////////////////////////////
	//��¼/OAuth�ӿ�

	// ������Ӧ��ʹ�ÿ�ʼ��request_token����ȡ�û���Ȩ����Access_token
	struct t_wb_oauth_request_token
	{
		t_wb_oauth wbauth_;
	};

	// ������Ӧ��ʹ�� request_token ����ȡ��֤���� verify code
	struct t_wb_authorize
	{
		t_wb_oauth wbauth_;

		WBChar  usrid_[ WB_REAL_LEN(UID) ];
		WBChar  usrpwd_[ WB_REAL_LEN(PWD) ];
	};



	// ������Ӧ��ʹ�ÿ�ʼ��request_token����ȡ�û���Ȩ����Access_token
	struct t_wb_oauth_access_token
	{
		t_wb_oauth wbauth_;
	};

	// ������Ӧ��ʹ�� xauth ����ȡ��ȡ�û���Ȩ����Access_token
	struct t_wb_xauth_access_token
	{
		t_wb_oauth wbauth_;

		WBChar  usrid_[ WB_REAL_LEN(UID) ];
		WBChar  usrpwd_[ WB_REAL_LEN(PWD) ];
		WBChar  authmode_[ WB_REAL_LEN(PWD) ];
	};

	// ����(�޲���)
	struct t_wb_emotions 
	{
		t_wb_oauth wbauth_;
	};

	/** �û����� 
	*/
	struct t_wb_users_search
	{
		t_wb_oauth wbauth_;

		WBChar q_[ WB_REAL_LEN(WB_INFO) ]; ///< ѡ�����. ��ѯ�Ĺؼ��֡��������url encode 
		///< ʾ��: http://api.t.sina.com.cn/users/search.json?q=api 

		WBChar snick_[WB_REAL_LEN(PROFILE_NAME)]; ///< ѡ�����. ������Χ�����ǳ� (0�ǲ�������1Ϊ����) 
		///< ʾ��: http://api.t.sina.com.cn/users/search.json?q=api&snick=0 

		int sdomain_; ///< ѡ�����. ������Χ������������ (0�ǲ�������1Ϊ����) 
		///< ʾ��: http://api.t.sina.com.cn/users/search.json?q=api&sdomain=1 

		int sintro_;///< ѡ�����. ������Χ������� (0�ǲ�������1Ϊ����) 
		///< ʾ��: http://api.t.sina.com.cn/users/search.json?q=api&sintro=1 

		int province_;///< ʡ��ID 
		///< ʾ��: http://api.t.sina.com.cn/users/search.json?province=11 

		int city_;///< ����ID 
		///< ʾ��: http://api.t.sina.com.cn/users/search.json?province=11&city=1 

		WBChar gender_[ WB_REAL_LEN(PROFILE_GENDER) ];///< �Ա�(mΪ�У�fΪŮ) 
		///< ʾ��: http://api.t.sina.com.cn/users/search.json?gender=m 

		WBChar comorsch_[ WB_REAL_LEN(WB_INFO) ];///< ��˾ѧУ���� 
		///< ʾ��: http://api.t.sina.com.cn/users/search.json?comorsch=���� 

		int sort_;///< ����ʽ(1Ϊ������ʱ�䣬2Ϊ����˿��) 
		///< ʾ��: http://api.t.sina.com.cn/users/search.json?q=api&sort=1 

		int page_;///< ҳ�� 
		///< ʾ��: http://api.t.sina.com.cn/users/search.json?q=api&sort=1&page=1 

		int count_;///<��ҳ��С(Ĭ�Ϸ���10��) 
		///< ʾ��: http://api.t.sina.com.cn/users/search.json?q=api&sort=1&page=1&count=10 
	};

	// search ����΢������ 
	struct t_wb_search
	{
		t_wb_oauth wbauth_;

		WBChar q_[WB_REAL_LEN(WB_INFO)];///< ѡ�����. ��ѯ�Ĺؼ��֡��������url encode 
		///< ʾ��: http://api.t.sina.com.cn/search.json?q=api 

		WBChar geocode_[WB_REAL_LEN(GEOCODE) ];///< ��ѡ. ����ָ����γ�ȸ�����΢����Ϣ����γ�Ȳ�����ʽ��"γ��,����,�뾶", �뾶ֻ֧��km���Ϊ�˺ͱ�׼�ӿڼ��ݣ�����Ҳ���� "mi" (miles)��������ֻ�ᵱ�� "km" ����ѯ����ʱδ��ת������ʽ��Ҫurl encode���룬���� 
		///< ʾ��: http://api.t.sina.com.cn/search.json?geocode=40.757929%2C-73.985506%2C25km 

		int sort_;

		int page_;///< ѡ�������ҳ��(��1��ʼ, Ĭ��1) 
		///< ʾ��: http://api.t.sina.com.cn/search.json?q=api&sort=1&page=1 

		int count_;///< (������rpp)��ѡ�������ÿҳ���ص�΢������Ĭ�Ϸ���10�������200 
		///< ʾ��: http://api.t.sina.com.cn/search.json?q=api&rpp=10 
	};

	// ����΢��(���������)
	struct t_wb_statuses_search 
	{
		t_wb_oauth wbauth_;

		WBChar q_[WB_REAL_LEN(WB_INFO)];///< ѡ�����. ��ѯ�Ĺؼ��֡��������url encode 
		///< ʾ��: http://api.t.sina.com.cn/statuses/search.json?q=api 

		int filter_ori_;///< ѡ�����.���������Ƿ�ԭ�� (0Ϊȫ����5Ϊԭ����4Ϊת����Ĭ��0) 
		///< ʾ��: http://api.t.sina.com.cn/statuses/search.json?q=api&filter_ori=0 

		int filter_pic_;///< ѡ�����. ���������Ƿ�ͼ (0Ϊȫ����1Ϊ��ͼ��2Ϊ����ͼ) 
		///< ʾ��: http://api.t.sina.com.cn/statuses/search.json?q=api&filter_pic=1 

		WBChar fuid_[WB_REAL_LEN(UID)];///< ѡ�����. ΢������UID 
		///< ʾ��: http://api.t.sina.com.cn/statuses/search.json?q=api&fuid=11075 

		int province_;///< ʡ��ID 
		///< ʾ��: http://api.t.sina.com.cn/statuses/search.json?province=11 

		int city_;///< ����ID 
		///< ʾ��: http://api.t.sina.com.cn/statuses/search.json?province=11&city=1 

		WBChar starttime_[WB_REAL_LEN(TIME)];///< ��ʼʱ��(unixʱ���) 
		///< ʾ��: http://api.t.sina.com.cn/statuses/search.json?q=api&starttime=1256289598 

		WBChar endtime_[WB_REAL_LEN(TIME)];///< ��ֹʱ�� (unixʱ���) 
		///< ʾ��: http://api.t.sina.com.cn/statuses/search.json?q=api&endtime=1256289598 

		int sort_;///<

		int page_;///< ѡ�������ҳ��(��1��ʼ, Ĭ��1) 
		///< ʾ��: http://api.t.sina.com.cn/statuses/search.json?q=api&sort=1&page=1 

		int count_;///<(������rpp)��ѡ�������ÿҳ���ص�΢������Ĭ�Ϸ���10�������200 
        ///< ʾ��: http://api.t.sina.com.cn/statuses/search.json?q=api&sort=1&page=1&count=10 
	};

	// ʡ�ݳ��б���� 
	struct t_wb_provinces
	{
		t_wb_oauth wbauth_;
	};

	// �ٱ�
	struct t_wb_report
	{
		t_wb_oauth wbauth_;

        WBChar ip_[WB_REAL_LEN(IP)];///< �ٱ���IP
        WBChar url_[WB_REAL_LEN(URL)];///< �ٱ�url
        WBChar content_[WB_REAL_LEN(WB_INFO)];///< �ٱ����� 
	};

	// cookie ��ʽ
	struct t_wb_cookie
	{
		t_wb_oauth wbauth_;

		WBChar  usrid_[ WB_REAL_LEN(UID) ];
		WBChar  usrpwd_[ WB_REAL_LEN(PWD) ];
	};

	// update tgt
	#define t_wb_updateTGT t_wb_cookie
	//struct t_wb_updateTGT 
	//{
	//	t_wb_oauth wbauth_;

	//	WBChar tgt_[ WB_REAL_LEN(OAUTH_VERIFIER) ]; ///< VERIFY TGT
	//	WBChar usrid_[ WB_REAL_LEN(OAUTH_TOKEN) ];///< SUE
	//	WBChar usrpwd_[ WB_REAL_LEN(OAUTH_TOKEN_SECRET) ]; ///< SUP
	//};

	/*
	��������,�Զ����
	*/
	struct t_wb_custom
	{
		t_wb_oauth wbauth_;

		WBChar szUrl_[WB_REAL_LEN(URL)];
		WBChar szPost_[WB_REAL_LEN(POST)]; ///< ����
		int    method_; ///< ���󷽷���POST�� GET
	};

	/** ����΢��/���۲�ѯ,�ڲ��ӿ�.by welbon,2011-01-07 */
	struct t_wb_hotpoint
	{
		t_wb_oauth wbauth_;

		//
		int count_; ///< ָ�����ص�����
	};

	////////////////////////////////////////////////////////////////////////////
	// ����ӿ�

	/** ��ȡĳ�˻��� */
	struct t_wb_trends
	{
		t_wb_oauth wbauth_;

		//
		WBChar  usrid_[ WB_REAL_LEN(UID) ];///< �û�id����ѡ��
		int     page_;  ///< ҳ�룬ȱʡֵΪ1
		int     count_;///< ÿҳ���صļ�¼����ȱʡֵΪ10
	};

	/** ��ȡĳһ�����µ�΢�� */
	struct t_wb_trends_statuses
	{
		t_wb_oauth wbauth_;

		WBChar  terndname_[ WB_REAL_LEN(WB_INFO) ];///< ����ؼ��֣���ѡ������
	};

	/** ��עĳһ������ */
	struct t_wb_trends_follow
	{
		t_wb_oauth wbauth_;

		WBChar  terndname_[ WB_REAL_LEN(WB_INFO) ];///< ����ؼ��֣���ѡ������
	};

	/** ȡ����עĳһ������ */
	struct t_wb_trends_destroy
	{
		t_wb_oauth wbauth_;

		WBChar  trendid_[ WB_REAL_LEN(UID) ];///< ����ID����ѡ������
	};

	/**�������һСʱ�ڵ����Ż��⡣*/
	struct t_wb_trends_hourly
	{
		t_wb_oauth wbauth_;

		int baseapp_;///< �Ƿ���ڵ�ǰӦ������ȡ���ݡ�1��ʾ���ڵ�ǰӦ������ȡ���ݡ�
	};

	/**�����ڷ������Ż��⡣����ĳһ���ڵĻ��⡣*/
	struct t_wb_trends_daily
	{
		t_wb_oauth wbauth_;

		int baseapp_;///< �Ƿ���ڵ�ǰӦ������ȡ���ݡ�1��ʾ���ڵ�ǰӦ������ȡ���ݡ�
	};

	/**���ܷ������Ż��⡣����ĳһ����֮ǰĳһ�ܵĻ��⡣*/
	struct t_wb_trends_weekly
	{
		t_wb_oauth wbauth_;

		int baseapp_;///< �Ƿ���ڵ�ǰӦ������ȡ���ݡ�1��ʾ���ڵ�ǰӦ������ȡ���ݡ�
	};

	////////////////////////////////////////////////////////////////////////////
	// �������ӿ�

	/** ��ĳ�û���������� */
	struct t_wb_blocks_create
	{
		t_wb_oauth wbauth_;

		//
		WBChar  usrid_[ WB_REAL_LEN(UID) ];///< �û�id����ѡ��
		WBChar  screenname_[ WB_REAL_LEN(SCREEN_NAME) ];///< ΢���ǳ�
	};

	/** ��ĳ�û��Ƴ������� */
	struct t_wb_blocks_destroy
	{
		t_wb_oauth wbauth_;

		//
		WBChar  usrid_[ WB_REAL_LEN(UID) ];///< �û�id����ѡ��
		WBChar  screenname_[ WB_REAL_LEN(SCREEN_NAME) ];///< ΢���ǳ�
	};

	/** ���ĳ�û��Ƿ��Ǻ������û� */
	struct t_wb_blocks_exist
	{
		t_wb_oauth wbauth_;

		//
		WBChar  usrid_[ WB_REAL_LEN(UID) ];///< �û�id����ѡ��
		WBChar  screenname_[ WB_REAL_LEN(SCREEN_NAME) ];///< ΢���ǳ�
	};

	/** �г��������û�(����û���ϸ��Ϣ) */
	struct t_wb_blocks_blocking
	{
		t_wb_oauth wbauth_;

		//
		int page_; ///< ҳ�롣
		int count_;///< ��ҳ��¼����
	};

	/** �г���ҳ�������û���ֻ���id�� */
	struct t_wb_blocks_blocking_ids
	{
		t_wb_oauth wbauth_;

		//
		int page_; ///< ҳ�롣
		int count_;///< ��ҳ��¼����
	};

	////////////////////////////////////////////////////////////////////////////
	// �û���ǩ�ӿ�

	/** ����ָ���û��ı�ǩ�б� */
	struct t_wb_tags
	{
		t_wb_oauth wbauth_;

		//
		WBChar  usrid_[ WB_REAL_LEN(UID) ];///< Ҫ��ȡ�ı�ǩ�б��������û�ID����ѡ��
		int count_;///< ��ҳ��¼����Ĭ��20�����200
		int page_; ///< ҳ�롣
	};

	/** ����ָ���û��ı�ǩ�б� */
	struct t_wb_tags_create
	{
		t_wb_oauth wbauth_;

		//
		WBChar  tags_[ WB_REAL_LEN(TAGS_INFO) ];///< Ҫ������һ���ǩ���ð�Ƕ��Ÿ���������ѡ��
	};

	/** �����û�����Ȥ�ı�ǩ */
	struct t_wb_tags_suggestions
	{
		t_wb_oauth wbauth_;

		int count_;///< ��ҳ��¼����Ĭ��10�����10��
		int page_;///<	Ĭ��1 ҳ�롣�����Ƽ���ǩ��������أ��ʴ������ݲ�֧�֡�
	};

	/**  ɾ����ǩ */
	struct t_wb_tags_destroy
	{
		t_wb_oauth wbauth_;

		WBChar tagId_[ WB_REAL_LEN(UID) ];///< Ҫɾ���ı�ǩID����ѡ��
	};

	/** ����ɾ����ǩ */
	struct t_wb_tags_destroy_batch
	{
		t_wb_oauth wbauth_;

		WBChar ids_[WB_REAL_LEN(TAGS_INFO) ];///< Ҫɾ����һ���ǩID���԰�Ƕ��Ÿ�����һ������ύ20��ID������ѡ��
	};

	////////////////////////////////////////////////////////////////////////////
	// ��ϵ����

	/** ����������ϵ�� */
	struct t_wb_invite_mailcontect
	{
		t_wb_oauth wbauth_;

		WBChar usrid_[ WB_REAL_LEN(UID) ];
		WBChar usrpwd_[ WB_REAL_LEN(PWD) ];
		int    type_;
	};

	/** ����MSN��ϵ�� */
	struct t_wb_invite_msncontect
	{
		t_wb_oauth wbauth_;

		//
		WBChar usrid_[ WB_REAL_LEN(UID) ];
		WBChar usrpwd_[ WB_REAL_LEN(PWD) ];
	};

	/** ���������ʼ� */
	struct t_wb_invite_sendmails
	{
		//
		t_wb_oauth wbauth_;

		//
		WBChar myusrid_[ WB_REAL_LEN(UID) ];
		WBChar nickname_[ WB_REAL_LEN(SCREEN_NAME) ];
		WBChar mailtype_[ 32 ];
		WBChar maillist_[ 2048 ];
	};

	/** ������ȡ�����ӵ�ַ */
	struct t_wb_media_shorturl_batch
	{
		t_wb_oauth wbauth_;

		//url ids
		WBChar urlids_ [ WB_REAL_LEN(WEIBO_IDS) ];
	};

	////////////////////////////////////////////////////////////////////////////

	/** url ֱ�����ɣ�ͨ�� loWeibo_get_url() */
	struct t_wb_URI
	{
		char uri_[ WB_REAL_LEN(URL) ];
		char post_[ WB_REAL_LEN(POST) ];
	};

	/** */
	struct t_wb_REQ_EX
	{
		// json, xml(default = json )
		int format_;

		// cookie ��ʽ | cookie= 0 ==> ouath ��ʽ
		int cookie_;

		// appkey = source
		char appkey_[256];
		char secret_[256];

		// ip��ַ
		char ip_[64];
	};

	/** �����*/
	typedef struct t_wb_task TASK_DATA , *PTASK_DATA;
	typedef void (*WBTaskCB)(const char* taskid , void*& userdata , void*& serviced);
	struct t_wb_task
	{
		char  taskid[64];
		void* userdata; ///< �û���������
		void* serviced; ///< �����ֶ�
		WBTaskCB cb;
	};
	
	/** ���� oauth �ṹ��copy ,ֻ��  struct t_wb_oauth ����������Ա��������copy 
		char oauth_token_[ WB_REAL_LEN(OAUTH_TOKEN) ];
		char oauth_token_secret_[ WB_REAL_LEN(OAUTH_TOKEN_SECRET) ];

		//  �ӹٷ���ҳ��ȡ�� ��֤���ص���֤�� ��Ϊ��oauth������ ,��ȡ��Ȩ����Access Token
		char oauth_verifier_[ WB_REAL_LEN(OAUTH_VERIFIER) ];
	*/
	WEIBO_API void loWeibo_oauth_copy(struct t_wb_oauth* pdest , const struct t_wb_oauth* psrc);

	// ���ٵ� ���� request���ڲ����������ٻ��ƣ� �� �ص� WEIBO_OPTION(REQ_DESTROY_FUNCTION)
	typedef void (*fun_req_destroy)(void* pUserdata);

	// ΢���������
	typedef void WEIBORequest;

	/** ����һ������ÿ���¼�����Ҫ ����һ������һ��Ҫͨ�� weibo_destroy_request���� �������� */
	WEIBO_API WEIBORequest* loWeibo_request_create(void);

	/** ����ֵ */
	typedef enum
	{
		ERR_createhttpobject_nil = -100,
		ERR_checkvalid_fail,
		ERR_url_set_fail,
		ERR_requestParam_nil,
		ERR_reqSessionParam_nil,
		ERR_createhttpSession_nil,
		ERR_Fail = -1,
		ERR_OK = 0,
	}WEIBOError;

	// �����optionֵ
	typedef enum
	{
		WEIBO_OPTION_DEFINE(REQ_UNK,0),
		WEIBO_OPTION(REQ_DESTROY_FUNCTION), ///< ����������ʱ��ͨ���ص������濪����

		WEIBO_OPTION(REQ_SENDDATA), ///< ���͵�����
		WEIBO_OPTION(REQ_USERDATA), ///< �������Զ�������,��������������������� �ص��������ظ�������ʹ��
		WEIBO_OPTION(REQ_PROXYDATA),///< ����Ĵ���Ŀǰ��֧��
		WEIBO_OPTION(REQ_RESULT_DATAFORMAT),///< ����ķ��ذ��ģ��ο� WEIBORequestFormat ��ʽ(E_FORMAT_JSON = json, E_FORMAT_XML = xml )
		WEIBO_OPTION(REQ_COOKIE), ///< ʹ�� cookie ��ʽ , loWeibo_request_setopt( reqobj , WEIBO_OPTION(REQ_COOKIE) ,1);

		WEIBO_OPTION(REQ_APPKEY), ///< set app key , loWeibo_request_setopt( reqobj , WEIBO_OPTION(REQ_APPKEY) ,"1122333");
		WEIBO_OPTION(REQ_SECRETKEY), ///< set app secret, loWeibo_request_setopt( reqobj , WEIBO_OPTION(REQ_SECRETKEY) ,"bbbbb");
		WEIBO_OPTION(REQ_IP), ///< set ip addre
	   WEIBO_OPTION(REQ_LAST),
	}WEIBOReqoption;

	/** ����һЩ����������д��Ȼ��ִ��(�ص�����������...) */
	WEIBO_API int  loWeibo_request_setopt(WEIBORequest* request , WEIBOReqoption option , ...);

	/** ִ��,�ȴ��ص����ݵ�ִ�У���ִ�н������ͨ�� weibo_request_setopt�������õķ�������֮������
	 *
	 * @param bAsyn = false ͬ��ִ�У��߳����� )
	 *              = true  ��ʾ�첽
	 *
	 * @return ��ο� WEIBOError
	*/
	WEIBO_API int  loWeibo_request_start(WEIBORequest* request , bool bAsyn);

	/** �ȴ�ִ�У����첽 loWeibo_request_start ִ�п��Խ�� �� �ﵽͬ������ ��*/
	WEIBO_API void loWeibo_request_wait(WEIBORequest* request);

	/** ֹͣ */
	WEIBO_API int  loWeibo_request_stop(WEIBORequest* request);

	/** ����������� */
	WEIBO_API int  loWeibo_request_destroy(WEIBORequest* request);

	/** 
	 * @brief ֱ������URL��Ϣ���ڲ�ֻ�ṩһ���㷨������Ҫ����Ϣ��֯��һ�𣬷������
	 *
	 * @param ��out] outURI,��������URL��Ϣ
	 * @param  [in] option �ο�  WEIBOoption
	 * @param  [in] t_wb   ���� WEIBOoptionֵ��Ӧ�Ĳ�ͬ�ṹ����
	 * @param  [in] req_ex һЩ���ӵ���֤��������
	 *
	 * @return 0 ,�ɹ�
	 */
	WEIBO_API int  loWeibo_get_url(struct t_wb_URI* outURI/*[out]*/ , WEIBOoption option , const void* t_wb , const struct t_wb_REQ_EX* req_ex);

#ifdef  __cplusplus
}
#endif

/**
 * @exmaple:
 *   
 *  WEIBORequest* pWBReq = loWeibo_request_create();
 *  if( pWBReq )
 *  {
 	   struct t_wb_oauth_request_token   oauth_reqtoken;
	   
	   // ʹ��json��ʽ
	   loWeibo_request_setopt(pWBReq , WEIBO_OPTION(REQ_RESULT_DATAFORMAT) , E_FORMAT_JSON);

	   // ����ʱ�����͵����������
	   loWeibo_request_setopt(pWBReq , WEIBO_OPTION(REQ_SENDDATA) ,  WEIBO_OPTION(OAUTH_REQUEST_TOKEN) , &oauth_reqtoken );

	   // ��ʼ����
	   loWeibo_request_start(pWBReq , false);

	   // ����
       loWeibo_request_destroy(pWBReq);

 *  }
 *
 *
*/

#endif /* __weibo_SDK_H__ */