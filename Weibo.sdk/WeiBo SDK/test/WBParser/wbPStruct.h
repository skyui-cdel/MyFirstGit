/**
*  @brief struct define,for Unit test.
*  @file  WeiboParseStruct.h
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


#pragma once
#ifndef __WB_PARSER_STRUCT_DEFINE_H__
#define __WB_PARSER_STRUCT_DEFINE_H__

#include <string.h>
#include <stdlib.h>

/**
* @brief һЩ�ṹ�Ķ���
* 
* @author welbon
*
*/

#define WTRUE  true
#define WFALSE false

#ifndef BUILD_CHARACTER_SET
	typedef char WBParseCHAR;
	//
#	define _WBC(c) c
#	define WBPARSER_COPY(dst,size,source,srcsize) strcpy_s(dst,size,source)

#else

	typedef TCHAR WBParseCHAR;
	//
#	define _WBC(c) _T(c)
#	define WBPARSER_COPY(dst,size,source,srcsize) _tcsncpy_s(dst,size,source,srcsize)

#endif // BUILD_CHARACTER_SET
	typedef int   WBParseINT;
	typedef long  WBParseLONG;
	typedef bool  WBParseBOOL;


namespace wbParserNS
{

#define WBPARSER_UINT_DEFINE(name,val)     const unsigned int WBPARSER_##name##_LEN = val
#define WBPARSER_USE_LEN(name)             WBPARSER_##name##_LEN
#define WBPARSER_REAL_LEN(name)            WBPARSER_##name##_LEN + 1 ///< ʵ�ʵĳ���
#define WBPARSER_OPTION_TYPE(name)         WBPT_OPTION_TYPE_##name   ///< ����������
#define WBPARSER_INFOR_TYPE(name)		   WBPT_INFOR_TYPE_##name    ///< �ṹ������


	// ������Ϣ
	WBPARSER_UINT_DEFINE(create_time,255);		    	 ///< ����ʱ��
	WBPARSER_UINT_DEFINE(id,64);				    	 ///< ΢��ID
	WBPARSER_UINT_DEFINE(sender_id,64);			    	 ///< ������UID
	WBPARSER_UINT_DEFINE(recipient_id,64);				 ///< ������UID
	WBPARSER_UINT_DEFINE(text,2048);					 ///< ΢����Ϣ����
	WBPARSER_UINT_DEFINE(source,255);				     ///< ΢����Դ
	WBPARSER_UINT_DEFINE(in_reply_to_status_id,64);     ///< �ظ�ID
	WBPARSER_UINT_DEFINE(in_reply_to_user_id  ,64);     ///< �ظ���UID
	WBPARSER_UINT_DEFINE(in_reply_to_screen_name,255);   ///< �ظ����ǳ�
	WBPARSER_UINT_DEFINE(thumbnail_pic,1024);            ///< ����ͼ
	WBPARSER_UINT_DEFINE(bmiddle_pic,1024);              ///< ����ͼƬ
	WBPARSER_UINT_DEFINE(original_pic,1024);             ///< ԭʼͼƬ

	WBPARSER_UINT_DEFINE(screen_name,255);             ///< ΢���ǳ�
	WBPARSER_UINT_DEFINE(name,255);                    ///< �Ѻ���ʾ����


	WBPARSER_UINT_DEFINE(province,128);
	WBPARSER_UINT_DEFINE(city,128);
	WBPARSER_UINT_DEFINE(location,128);
	WBPARSER_UINT_DEFINE(description,1024);
	WBPARSER_UINT_DEFINE(url,1024);
	WBPARSER_UINT_DEFINE(profile_image_url,1024);
	WBPARSER_UINT_DEFINE(domain,1024);
	WBPARSER_UINT_DEFINE(gender,32);

	WBPARSER_UINT_DEFINE(created_at,255);
	WBPARSER_UINT_DEFINE(following,32);
	WBPARSER_UINT_DEFINE(verified,32);

	WBPARSER_UINT_DEFINE(geo,255);
	WBPARSER_UINT_DEFINE(type,255);
	WBPARSER_UINT_DEFINE(coordinates,255);

	//
	WBPARSER_UINT_DEFINE(status,255);
	WBPARSER_UINT_DEFINE(reset_time,255);

	// using in emotion
	WBPARSER_UINT_DEFINE(phrase,255);
	WBPARSER_UINT_DEFINE(category,255);


	//
	WBPARSER_UINT_DEFINE(request,255);
	WBPARSER_UINT_DEFINE(error,255);
	WBPARSER_UINT_DEFINE(error_code,64);
	

	//
	struct t_wbParse_Status;
	struct t_wbParse_User;
	struct t_wbParse_Geo;
	struct t_wbParse_Comment;
	struct t_wbParse_CommentCounts ;
	struct t_wbParse_ShipshowBase ;
	struct t_wbParse_Shipshow ;
	struct t_wbParse_LimitStatus ;
	struct t_wbParse_Error;
	struct t_wbParse_Unread ;
	struct t_wbParse_DirectMessage ;

	/// ��֤������Ϣ
	struct t_wbParse_Oauth
	{
		char oauth_token[ 257 ];
		char oauth_token_secret[ 257 ];

		char uid_[128];

		//  �ӹٷ���ҳ��ȡ�� ��֤���ص���֤�� ��Ϊ��oauth������ ,��ȡ��Ȩ����Access Token
		char oauth_verifier[ 65 ];
	};

	/** cookie ��Ϣ */
	struct t_wbParse_Cookie
	{
		// cookie
		char SUE[1024];
		char SUP[2048];
		// 
		char uid[128];
		char tgt[256];
		char ticket[256];
		char publickey[512];
		char keyversion[16];
		//
		int  error;
	};


	struct t_wbParse_Cursor
	{
		long long nextcur;
		long long prevcur;
	};

	/** �û���Ϣ */
	struct t_wbParse_User 
	{
		WBParseCHAR id[WBPARSER_REAL_LEN(id)];						///< �û�UID
		WBParseCHAR screen_name[WBPARSER_REAL_LEN(screen_name)];	///< ΢���ǳ�
		WBParseCHAR name[WBPARSER_REAL_LEN(name)];					///< �Ѻ���ʾ���ƣ���Tim Yang(�������ݲ�֧��)
		WBParseCHAR province[WBPARSER_REAL_LEN(province)];			///< ʡ�ݱ��루�ο�ʡ�ݱ����
		WBParseCHAR city[WBPARSER_REAL_LEN(city)];					///< ���б��루�ο����б����
		WBParseCHAR location[WBPARSER_REAL_LEN(location)];			///< ��ַ
		WBParseCHAR description[WBPARSER_REAL_LEN(description)];	///< ��������
		WBParseCHAR url[WBPARSER_REAL_LEN(url)];					///< �û����͵�ַ
		WBParseCHAR domain[WBPARSER_REAL_LEN(domain)];				///< �û����Ի�����
		WBParseCHAR gender[WBPARSER_REAL_LEN(gender)];				///< �Ա�,m--�У�f--Ů,n--δ֪
		WBParseCHAR profile_image_url[WBPARSER_REAL_LEN(profile_image_url)];///< �Զ���ͼ��

		WBParseINT followers_count;    ///< ��˿��
		WBParseINT friends_count;      ///< ��ע��
		WBParseINT statuses_count;     ///< ΢����
		WBParseINT favourites_count;   ///< �ղ���


		WBParseBOOL geo_enabled; ///< ����λ�ÿ���
		WBParseBOOL allow_all_act_msg;
		WBParseBOOL following;  ///< �Ƿ��ѹ�ע(�������ݲ�֧��)
		WBParseBOOL verified;   ///< ��V��ʾ

		WBParseCHAR created_at[WBPARSER_REAL_LEN(created_at)];///< ����ʱ��

		t_wbParse_Status *last_status; ///< ���һ����Ϣ
	};

	/** ��ַλ����Ϣ */
	struct t_wbParse_Geo
	{
		WBParseCHAR type[WBPARSER_REAL_LEN(type)]; //
		WBParseCHAR coordinates[WBPARSER_REAL_LEN(coordinates)];
	};

	/** ״̬��Ϣ */
	struct t_wbParse_Status
	{
		WBParseCHAR create_time[WBPARSER_REAL_LEN(create_time)];   ///<����ʱ��
		WBParseCHAR id[WBPARSER_REAL_LEN(id)];					   ///<΢��ID
		WBParseCHAR text[WBPARSER_REAL_LEN(text)];				   ///<΢����Ϣ����
		WBParseCHAR source[WBPARSER_REAL_LEN(source)];			   ///<΢����Դ

		WBParseBOOL favorited;									   ///<�Ƿ����ղ�
		WBParseBOOL truncated;                                     ///<�Ƿ񱻽ض�


		WBParseCHAR in_reply_to_status_id[WBPARSER_REAL_LEN(in_reply_to_status_id)];   ///<�ظ�ID
		WBParseCHAR in_reply_to_user_id[WBPARSER_REAL_LEN(in_reply_to_user_id)];     ///<�ظ���UID
		WBParseCHAR in_reply_to_screen_name[WBPARSER_REAL_LEN(in_reply_to_screen_name)]; ///<�ظ����ǳ�

		t_wbParse_Geo    *geo; ///< ����λ��

		WBParseCHAR thumbnail_pic[WBPARSER_REAL_LEN(thumbnail_pic)];         ///<����ͼ
		WBParseCHAR bmiddle_pic[WBPARSER_REAL_LEN(bmiddle_pic)];             ///<����ͼƬ
		WBParseCHAR original_pic[WBPARSER_REAL_LEN(original_pic)];           ///<ԭʼͼƬ

		t_wbParse_User   *user;//������Ϣ
		t_wbParse_Status *retweeted_status ; //ת���Ĳ��ģ�����Ϊstatus���������ת������û�д��ֶ�
	};

	/** ������Ϣ */
	struct t_wbParse_Comment
	{

		WBParseCHAR id[WBPARSER_REAL_LEN(id)];///< ����ID
		WBParseCHAR text[WBPARSER_REAL_LEN(text)];///< ��������
		WBParseCHAR source[WBPARSER_REAL_LEN(source)];///<������Դ

		//
		WBParseBOOL favorited;///<�Ƿ��ղ�
		WBParseBOOL truncated;///<�Ƿ񱻽ض�

		WBParseCHAR created_at[WBPARSER_REAL_LEN(created_at)];///<����ʱ��

		t_wbParse_User    *user;     ///<��������Ϣ,�ṹ�ο�user
		t_wbParse_Status  *status;   ///<���۵�΢��,�ṹ�ο�status
		t_wbParse_Comment *reply_comment;///<������Դ�����ݽṹ��commentһ��
	};

	/** ���ۼ��� */
	struct t_wbParse_CommentCounts
	{
		WBParseCHAR id[WBPARSER_REAL_LEN(id)];///< ����ID
		WBParseINT  comments;  ///<������
		WBParseINT  rt;        ///<ת����
	};

	/** δ����Ϣ�� */
	struct t_wbParse_Unread
	{
		WBParseINT comments; ///<δ������
		WBParseINT mentions; ///<@�ҵ�
		WBParseINT dm;       ///<˽��
		WBParseINT followers;///<//�·�˿��
		WBParseINT new_status;///< new weibo
	};

	/** ˽����Ϣ */
	struct t_wbParse_DirectMessage
	{
		WBParseCHAR id[WBPARSER_REAL_LEN(id)];///<˽��ID
		WBParseCHAR text[WBPARSER_REAL_LEN(text)];///< ˽������
		WBParseCHAR sender_id[WBPARSER_REAL_LEN(sender_id)];///<������UID
		WBParseCHAR recipient_id[WBPARSER_REAL_LEN(recipient_id)];///< ������UID
		WBParseCHAR created_at[WBPARSER_REAL_LEN(created_at)];///<����ʱ��
		WBParseCHAR sender_screen_name[WBPARSER_REAL_LEN(screen_name)];///<�������ǳ�
		WBParseCHAR recipient_screen_name[WBPARSER_REAL_LEN(screen_name)];///<�������ǳ�

		t_wbParse_User *sender;///< ��������Ϣ���ο�user˵��
		t_wbParse_User *recipient;///<��������Ϣ���ο�user˵��
	};

	/** ���ѹ�ϵ */
	struct t_wbParse_Shipshow
	{
		struct Item
		{
			WBParseCHAR id[WBPARSER_REAL_LEN(id)];
			WBParseCHAR screen_name[WBPARSER_REAL_LEN(screen_name)];

			WBParseBOOL following ;
			WBParseBOOL followed_by ;
			WBParseBOOL notifications_enabled ;
		};
		Item *source;
		Item *target;
	};

	/** Ƶ������ */
	struct t_wbParse_LimitStatus
	{
		WBParseINT  hourly_limit;
		WBParseINT  reset_time_in_seconds;
		WBParseINT  remaining_hits;
		WBParseCHAR reset_time[WBPARSER_REAL_LEN(reset_time)];
	};

	/** ���� */
	struct t_wbParse_Emotion
	{
		WBParseCHAR phrase[WBPARSER_REAL_LEN(phrase)];
		WBParseCHAR type[WBPARSER_REAL_LEN(type)];
		WBParseCHAR url	[WBPARSER_REAL_LEN(url)];
		WBParseCHAR category[WBPARSER_REAL_LEN(category)];

		//
		WBParseBOOL is_hot;
		WBParseBOOL is_common;
		WBParseINT	order_number;
	};

	/** ���� */
	struct t_wbParse_Error 
	{
		WBParseCHAR request[WBPARSER_REAL_LEN(request)];
		WBParseCHAR error[WBPARSER_REAL_LEN(error)];
		WBParseCHAR error_code[WBPARSER_REAL_LEN(error_code)];
		unsigned int error_sub_code; ///< �Ӵ�����Ϣ
	};


	/** Ϊ��ý����� */
	struct t_wbParse_media_item
	{
		WBParseCHAR id[WBPARSER_REAL_LEN(id)];///<mediaid
		WBParseCHAR url[WBPARSER_REAL_LEN(url)];///<media- url
		WBParseCHAR screen[WBPARSER_REAL_LEN(url)];///<media- url
		WBParseCHAR type[WBPARSER_REAL_LEN(type)];///<media- type:video/audio
		WBParseCHAR title[WBPARSER_REAL_LEN(name)];///<media- title
		WBParseCHAR flash[WBPARSER_REAL_LEN(url)];///<media- flash
	};

	/** ���΢������ */
	struct t_wbParse_Media
	{
		int cmd;
		WBParseCHAR parentid[WBPARSER_REAL_LEN(id)];///<��΢��ID����������΢���ĸ�΢��id)
		WBParseCHAR id[WBPARSER_REAL_LEN(id)];///< ��ǰ΢��ID

		t_wbParse_media_item mitem;
	};


	const long long  llMAX = 0xFFFFFFFFFFFFFFFF;
	/** �򵥵� */
	struct t_wbParser_MMId
	{
		enum eType
		{
			/** �ҵ����� */
			EType_CommentsToMe = 0,

			/** �ҷ���ȥ�� */
			EType_CommentsByMe,

			/** �ҷ����յ� */
			EType_CommentsTimeline,

			/** @�ҵ� */
			EType_AtMe,

			/** ��ҳ */
			EType_Homes,

			/** ˽�� */
			EType_DirectMsgs,

			/** ��˿*/
			EType_Follower,

			EType_End,
		};
		struct IdValue
		{
			long long maxId;
			long long minId;
		};

		struct IdValue _idVal[EType_End];

		/** �Ƿ���Ҫ��������ֹ�Ƿ����ʣ�*/
	};
}

#endif //__WB_PARSER_STRUCT_DEFINE_H__
