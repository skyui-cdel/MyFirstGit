#pragma once
#ifndef __WB_PARSE_WRAPPER_H__
#define __WB_PARSE_WRAPPER_H__

#include "wbParseStruct.h"

#if (defined(WIN32) || defined(_WIN32) ) && !defined(WEIBO_PARSER_STATICLIB)
#	if defined(BUILDING_LIBWBPARSER)
#	define WBPARSER_API  __declspec(dllexport)
#	else
#	define WBPARSER_API  __declspec(dllimport)
#	endif
#else
// ����ƽ̨�ģ������Ǿ�̬����
#	define WBPARSER_API
#endif


#define WBPARSE_FUNC_NAME(interfaceType,interfaceName) Parse_##interfaceType##_##interfaceName
#define WBFREE_FUNC_NAME(interfaceType,interfaceName)  Free_##interfaceType##_##interfaceName

// Macro for Parsing
#define WBPARSE_FUNC_DECLARE(interfaceType,interfaceName)\
	WBPARSE_FUNC_NAME(interfaceType,interfaceName)(const char* sc,const int len,int &iOutCount,void **ppout,t_WBParse_Extern_info *pExternInfo,t_wbParse_Cursor **ppCursor)

#define WBPARSE_FUNC_IMPLEMENTS(interfaceType,interfaceName)\
	WBPARSE_FUNC_NAME(interfaceType,interfaceName)(const char* sc,const int len,int &iOutCount,void **ppout,t_WBParse_Extern_info *pExternInfo,t_wbParse_Cursor **ppCursor )

// Macro for Free 
#define WBFREE_FUNC_DECLARE(interfaceType,interfaceName)\
	WBFREE_FUNC_NAME(interfaceType,interfaceName)(WBPARSE_HANDLE handle)

#define WBFREE_FUNC_IMPLEMENTS(interfaceType,interfaceName)\
	WBFREE_FUNC_NAME(interfaceType,interfaceName)(WBPARSE_HANDLE handle)


// Macro for using
#define USE_WBPARSE_FUNC(interfaceType,interfaceName,sc,len,iOutCount,ppout,pExternInfo,ppCursorInfo)\
	WBPARSE_FUNC_NAME(interfaceType,interfaceName)(sc,len,iOutCount,ppout,pExternInfo,ppCursorInfo);

#define USE_WBFREE_FUNC(interfaceType,interfaceName,handle)\
	WBFREE_FUNC_NAME(interfaceType,interfaceName)(handle)

#ifdef __cplusplus
extern "C"{
#endif //__cplusplus


	namespace wbParserNS
	{

		typedef void* WBPARSER_API WBPARSE_HANDLE;

		/** �ش�����չ��Ϣ */
		struct t_WBParse_Extern_info
		{
			struct t_Media
			{
				int				 nCmd;		///< Command information
				int 			 iMediaSize;///< media size
				t_wbParse_Media* pMedia;	///< media��Ϣ
			} _Media;

			struct t_MMId
			{
				int              iMMIDtype; ///< MMID type
				t_wbParser_MMId* _pMMId;    ///< MMID
			} _MMId;
		};
		//�������ݼ�


		/** 2.���µĹ���΢����Ϣ */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Statuses,Public_Timeline);
		WBPARSER_API void WBFREE_FUNC_DECLARE(Statuses,Public_Timeline);

		/** 3.��ǰ��¼�û���������ע�û�������΢����Ϣ */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE( Statuses,Friends_Timeline);
		WBPARSER_API void WBFREE_FUNC_DECLARE( Statuses,Friends_Timeline);

		/** 4.��ǰ��¼�û�������΢����Ϣ�б� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Statuses,User_Timeline);
		WBPARSER_API void WBFREE_FUNC_DECLARE(Statuses,User_Timeline);

		/** 5.@��ǰ�û���΢���б� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Statuses,Metions);
		WBPARSER_API void WBFREE_FUNC_DECLARE(Statuses,Metions);


		/** 6.��ǰ�û����ͼ��յ��������б� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Statuses,CommentsTimeLine);
		WBPARSER_API void WBFREE_FUNC_DECLARE(Statuses,CommentsTimeLine);

		/** 7.��ȡ��ǰ�û����������� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Statuses,Comments_BYME);
		WBPARSER_API void WBFREE_FUNC_DECLARE(Statuses,Comments_BYME);

		/** 8.��ȡ��ǰ�û��յ������� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Statuses,Comments_TOME);
		WBPARSER_API void WBFREE_FUNC_DECLARE(Statuses,Comments_TOME);

		/** 9.����΢����ϢID����ĳ��΢����Ϣ�������б� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Statuses,Comments);
		WBPARSER_API void WBFREE_FUNC_DECLARE(Statuses,Comments);

		/** 10.������ȡһ��΢������������ת���� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Statuses,CommentCounts);
		WBPARSER_API void WBFREE_FUNC_DECLARE(Statuses,CommentCounts);

		/** 11.��ȡ��ǰ�û�δ����Ϣ�� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Statuses,Unread);
		WBPARSER_API void WBFREE_FUNC_DECLARE(Statuses,Unread);

		/** δ����Ϣ������ӿ� */
		WBPARSER_API bool WBPARSE_FUNC_DECLARE(Statuses,ResetCount);

		/** ����ӿڣ���ȡ�����б� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Emotions,Timeline);
		WBPARSER_API void WBFREE_FUNC_DECLARE(Emotions,Timeline);

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//΢�����ʽӿ�


		/** ����ID��ȡ����΢����Ϣ���� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE( Statuses,Show);
		WBPARSER_API void WBFREE_FUNC_DECLARE( Statuses,Show);

		/** ����һ��΢����Ϣ */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE( Statuses,Update);
		WBPARSER_API void WBFREE_FUNC_DECLARE( Statuses,Update);

		/** �ϴ�ͼƬ������һ��΢����Ϣ */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE( Statuses,Upload);
		WBPARSER_API void WBFREE_FUNC_DECLARE( Statuses,Upload);

		/** ɾ��һ��΢����Ϣ */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE( Statuses,Destroy);
		WBPARSER_API void WBFREE_FUNC_DECLARE( Statuses,Destroy);

		/** ��һ��΢����Ϣ�������� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE( Statuses,Comment);
		WBPARSER_API void WBFREE_FUNC_DECLARE( Statuses,Comment);

		/** ɾ����ǰ�û���΢��������Ϣ */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE( Statuses,Comment_Destroy);
		WBPARSER_API void WBFREE_FUNC_DECLARE( Statuses,Comment_Destroy);

		/** ����ɾ����ǰ�û���΢��������Ϣ */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE( Statuses,Comment_DestroyBatch);
		WBPARSER_API void WBFREE_FUNC_DECLARE( Statuses,Comment_DestroyBatch);

		/** �ظ�΢��������Ϣ */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Statuses,Comment_Reply);
		WBPARSER_API void WBFREE_FUNC_DECLARE(Statuses,Comment_Reply);

		/** ת��һ��΢�� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Statuses,Repost);
		WBPARSER_API void WBFREE_FUNC_DECLARE(Statuses,Repost);

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// �û��ӿ�

		/** �����û�ID��ȡ�û����ϣ���Ȩ�û��� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE( User,Show);
		WBPARSER_API void WBFREE_FUNC_DECLARE( User,Show);

		/** ��ȡ�û���ע�б�ÿ����ע�û�����һ��΢��  */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Statuses,Friends);
		WBPARSER_API void WBFREE_FUNC_DECLARE(Statuses,Friends);

		/** ��ȡ�û���˿�б���ÿ����˿�û�����һ��΢�� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Statuses,FollowerList);
		WBPARSER_API void WBFREE_FUNC_DECLARE(Statuses,FollowerList);

		/** ��ȡϵͳ�Ƽ����û� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Users,HotUser);
		WBPARSER_API void WBFREE_FUNC_DECLARE(Users,HotUser);

		/** ���µ�ǰ��¼�û�����ע��ĳ�����ѵı�ע��Ϣ */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Users,Friends_UpdateRemark);
		WBPARSER_API void WBFREE_FUNC_DECLARE(Users,Friends_UpdateRemark);

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// ˽�Žӿ�

		/*��ȡ��ǰ�û�����˽���б�*/
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(DirectMessages,Timeline);
		WBPARSER_API void WBFREE_FUNC_DECLARE(DirectMessages,Timeline);

		/** ��ȡ��ǰ�û����͵�����˽���б� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(DirectMessages,Sent);
		WBPARSER_API void WBFREE_FUNC_DECLARE(DirectMessages,Sent);

		/** ����һ��˽�� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(DirectMessages,New);
		WBPARSER_API void WBFREE_FUNC_DECLARE(DirectMessages,New);

		/** ɾ��һ��˽�� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(DirectMessages,Destroy);
		WBPARSER_API void WBFREE_FUNC_DECLARE(DirectMessages,Destroy);

		/** ����ɾ��˽�� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(DirectMessages,Destroy_Batch);
		WBPARSER_API void WBFREE_FUNC_DECLARE(DirectMessages,Destroy_Batch);

		/*��ǰ��½�û���˽���������û��б�*/
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(DirectMessages,UserList );
		WBPARSER_API void WBFREE_FUNC_DECLARE(DirectMessages,UserList);

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//��ע�ӿ�

		/** ��עĳ�û� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Friendship,Create);
		WBPARSER_API void WBFREE_FUNC_DECLARE(Friendship,Create);

		/** ȡ����ע */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Friendship,Destroy);
		WBPARSER_API void WBFREE_FUNC_DECLARE(Friendship,Destroy);

		/** �Ƿ��עĳ�û�(�Ƽ�ʹ��friendships/show) */
		bool WBPARSE_FUNC_DECLARE(Friendship,Exist);

		/** ��ȡ�����û���ϵ����ϸ��� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Friendship,Show);
		WBPARSER_API void WBFREE_FUNC_DECLARE(Friendship,Show);

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Social Graph�ӿ�
		//
		/*��ȡ�û���ע����uid�б�*/
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Friends,ids);
		WBPARSER_API void WBFREE_FUNC_DECLARE(Friends,ids);


		/* ��ȡ�û���˿����uid�б�*/
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Follower,ids);
		WBPARSER_API void WBFREE_FUNC_DECLARE(Follower,ids);

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//��˽���ýӿ�

		/* ������˽��Ϣ*/
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Account,UpdatePrivacy);
		WBPARSER_API void WBFREE_FUNC_DECLARE(Account,UpdatePrivacy);

		/* ��ȡ��˽��Ϣ*/
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Account,GetPrivacy);
		WBPARSER_API void WBFREE_FUNC_DECLARE(Account,GetPrivacy);

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//�������ӿ�

		/** ��ĳ�û���������� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Block,Create);
		WBPARSER_API void WBFREE_FUNC_DECLARE(Block,Create);

		/** ��ĳ�û��Ƴ������� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Block,Destroy);
		WBPARSER_API void WBFREE_FUNC_DECLARE(Block,Destroy);

		/** ���ĳ�û��Ƿ��Ǻ������û� */
		bool WBPARSE_FUNC_DECLARE(Blocks,Exist);

		/* �г��������û�(����û���ϸ��Ϣ) */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Blocks,Blocking );
		WBPARSER_API void WBFREE_FUNC_DECLARE(Blocks,Blocking);

		/** �г���ҳ�������û���ֻ���id��*/
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Blocks,Blocking_IDs);
		WBPARSER_API void WBFREE_FUNC_DECLARE(Blocks,Blocking_IDs);


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//�û���ǩ�ӿ�

		/** �����û�ָ���ı�ǩ�б� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Tags,List);
		WBPARSER_API void WBFREE_FUNC_DECLARE(Tags,List);

		/** �����û�ָ���ı�ǩ�б� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Tags,Create);
		WBPARSER_API void WBFREE_FUNC_DECLARE(Tags,Create);

		/** �����û�����Ȥ�ı�ǩ */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Tags,Suggestions);
		WBPARSER_API void WBFREE_FUNC_DECLARE(Tags,Suggestions);

		/** ɾ����ǩ */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Tags,Destroy);
		WBPARSER_API void WBFREE_FUNC_DECLARE(Tags,Destroy);

		/** ����ɾ����ǩ */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Tags,Destroy_Batch);
		WBPARSER_API void WBFREE_FUNC_DECLARE(Tags,Destroy_Batch);

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//�˺Žӿ�

		/** ��֤��ǰ�û�����Ƿ�Ϸ� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Account,Verify_Credentials);
		WBPARSER_API void WBFREE_FUNC_DECLARE(Account,Verify_Credentials);

		/** ��ȡ��ǰ�û�API����Ƶ������ */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Account,Rate_Limit);
		WBPARSER_API void WBFREE_FUNC_DECLARE(Account,Rate_Limit);


		/** ��ǰ�û��˳���¼ */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Account,EndSession);
		WBPARSER_API void WBFREE_FUNC_DECLARE(Account,EndSession);

		/**  ����ͷ�� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Account,Update_Profile_Image);
		WBPARSER_API void WBFREE_FUNC_DECLARE( Account,Update_Profile_Image);

		/**��������*/
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Account,Update_Profile);
		WBPARSER_API void WBFREE_FUNC_DECLARE( Account,Update_Profile);

		/**ע������΢���ʺ�*/
		bool WBPARSE_FUNC_DECLARE(Account,Register);

		/**����ע��΢���Ľӿ�*/
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Account,Activate);
		WBPARSER_API void WBFREE_FUNC_DECLARE( Account,Activate);


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// �ղؽӿ�


		/** ��ȡ��ǰ�û����ղ��б� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Favorites,List);
		WBPARSER_API void WBFREE_FUNC_DECLARE(Favorites,List);

		/** ����ղ� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Favorites,Create);
		WBPARSER_API void WBFREE_FUNC_DECLARE(Favorites,Create);

		/** ɾ���ղ���Ϣ */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Favorites,Destroy);
		WBPARSER_API void WBFREE_FUNC_DECLARE(Favorites,Destroy);

		/** ����ɾ���ղص�΢����Ϣ */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Favorites,Destroy_batch);
		WBPARSER_API void WBFREE_FUNC_DECLARE(Favorites,Destroy_batch);

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// ��¼/OAuth�ӿ�
		//

		/** ��ȡδ��Ȩ��Request Token */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Oauth,RequestToken);
		WBPARSER_API void WBFREE_FUNC_DECLARE(Oauth,RequestToken);

		/** �����û���ȨToken */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Oauth,Oauthorize);
		WBPARSER_API void WBFREE_FUNC_DECLARE(Oauth,Oauthorize);

		/**��ȡ��Ȩ����Access Token*/
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Oauth,AccessToken) ;
		WBPARSER_API void WBFREE_FUNC_DECLARE(Oauth,AccessToken);



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// ΢������ :Search API�������˺��������߿��ţ���������ϵ @΢������ƽ̨
		//

		/** ����΢�� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Search,Statuses);
		WBPARSER_API void WBFREE_FUNC_DECLARE(Search,Statuses);

		/** �����û� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(Search,Users);
		WBPARSER_API void WBFREE_FUNC_DECLARE(Search,Users);


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// �ȵ���Ϣ :HOT API�������˺��������߿��ţ���������ϵ @΢������ƽ̨,by welbon,2011-01-07
		//

		/** ��ȡ�ȵ����۵�΢�������� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(HotPoint,Comment_Dayliy);
		WBPARSER_API void WBFREE_FUNC_DECLARE(HotPoint,Comment_Dayliy);

		/** ��ȡ�ȵ����۵�΢�������� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(HotPoint,Comment_Weekliy);
		WBPARSER_API void WBFREE_FUNC_DECLARE(HotPoint,Comment_Weekliy);

		/** ��ȡ�ȵ�ת����΢�������� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(HotPoint,Repost_Dayliy);
		WBPARSER_API void WBFREE_FUNC_DECLARE(HotPoint,Repost_Dayliy);

		/** ��ȡ�ȵ�ת����΢�������� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(HotPoint,Repost_Weekliy);
		WBPARSER_API void WBFREE_FUNC_DECLARE(HotPoint,Repost_Weekliy);

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// һ�����

		/** ������� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_DECLARE(WEIBO_ERR,ITEM);
		WBPARSER_API void WBFREE_FUNC_DECLARE(WEIBO_ERR,ITEM);

		/** cookie���� */
		WBPARSER_API void WBPARSE_FUNC_DECLARE(Cookie,BASE);

		/** ����cookie,����ָ���Ĵ� */
		WBPARSER_API int Parse_CookieStr(const char *sc,char *token,char *secret);
	}


#ifdef __cplusplus
}
#endif //__cplusplus


#endif //__WB_PARSE_WRAPPER_H__