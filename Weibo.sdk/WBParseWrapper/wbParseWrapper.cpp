#include "stdafx.h"

//
#include "splitstr.h"
#include "WBParseWrapper.h"
#include "wbParseImpl.h"
#include "Urlcode.h"
#include "strconv.h"
#include <json/reader.h>

//
#define MMID_TYPE(p,type) if( pExternInfo){ pExternInfo->_MMId.iMMIDtype = type; }


#ifdef __cplusplus
extern "C"{
#endif //__cplusplus


	namespace wbParserNS
	{
		//下行数据集

		/** 2.最新的公共微博消息 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Statuses,Public_Timeline)
		{
			MMID_TYPE(pExternInfo,t_wbParser_MMId::EType_Homes);
			//
			CParseStatusesMultiplT* pParseObj = new CONSTRUCT_STATUSES_MULTIPL(sc,len,pExternInfo);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			iOutCount = pParseObj->GetCounts();
			return ((WBPARSE_HANDLE)pParseObj) ;
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Statuses,Public_Timeline)
		{
			handle ? (delete ((CParseStatusesMultiplT*)handle)) : 0;
		}



		/** 3.当前登录用户及其所关注用户的最新微博消息 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS( Statuses,Friends_Timeline)
		{
			MMID_TYPE(pExternInfo,t_wbParser_MMId::EType_Homes);
			//
			CParseStatusesMultiplT* pParseObj = new CONSTRUCT_STATUSES_MULTIPL(sc,len,pExternInfo);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			iOutCount = pParseObj->GetCounts();
			return ((WBPARSE_HANDLE)pParseObj) ;
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS( Statuses,Friends_Timeline)
		{
			handle ? (delete ((CParseStatusesMultiplT*)handle)) : 0;
		}


		/** 4.当前登录用户发布的微博消息列表 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Statuses,User_Timeline)
		{
			MMID_TYPE(pExternInfo,t_wbParser_MMId::EType_Homes);
			//
			CParseStatusesMultiplT* pParseObj = new CONSTRUCT_STATUSES_MULTIPL(sc,len,pExternInfo);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			iOutCount = pParseObj->GetCounts();
			return ((WBPARSE_HANDLE)pParseObj) ;
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Statuses,User_Timeline)
		{
			handle ? (delete ((CParseStatusesMultiplT*)handle)) : 0;
		}


		/** 5.@当前用户的微博列表 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Statuses,Metions)
		{
			MMID_TYPE(pExternInfo,t_wbParser_MMId::EType_Homes);
			//
			CParseStatusesMultiplT* pParseObj = new CONSTRUCT_STATUSES_MULTIPL(sc,len,pExternInfo);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			iOutCount = pParseObj->GetCounts();
			return ((WBPARSE_HANDLE)pParseObj) ;
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Statuses,Metions)
		{
			handle ? (delete ((CParseStatusesMultiplT*)handle)) : 0;
		}


		/** 6.当前用户发送及收到的评论列表 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Statuses,CommentsTimeLine)
		{
			MMID_TYPE(pExternInfo,t_wbParser_MMId::EType_CommentsTimeline);
			//
			CParseCommentMultiplT* pParseObj = new CONSTRUCT_COMMENTS_MULTIPL(sc,len);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			iOutCount = pParseObj->GetCounts();
			return ((WBPARSE_HANDLE)pParseObj) ;
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Statuses,CommentsTimeLine)
		{
			handle ? (delete ((CParseCommentMultiplT*)handle)) : 0;
		}

		/** 7.获取当前用户发出的评论 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Statuses,Comments_BYME)
		{
			MMID_TYPE(pExternInfo,t_wbParser_MMId::EType_CommentsByMe);
			//
			CParseCommentMultiplT* pParseObj = new CONSTRUCT_COMMENTS_MULTIPL(sc,len);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			iOutCount = pParseObj->GetCounts();
			return ((WBPARSE_HANDLE)pParseObj) ;

		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Statuses,Comments_BYME)
		{
			handle ? (delete ((CParseCommentMultiplT*)handle)) : 0;
		}


		/** 8.获取当前用户收到的评论 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Statuses,Comments_TOME)
		{
			MMID_TYPE(pExternInfo,t_wbParser_MMId::EType_CommentsToMe);
			//
			CParseCommentMultiplT* pParseObj = new CONSTRUCT_COMMENTS_MULTIPL(sc,len);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			iOutCount = pParseObj->GetCounts();
			return ((WBPARSE_HANDLE)pParseObj) ;
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Statuses,Comments_TOME)
		{
			handle ? (delete ((CParseCommentMultiplT*)handle)) : 0;
		}


		/** 9.根据微博消息ID返回某条微博消息的评论列表 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Statuses,Comments)
		{
			MMID_TYPE(pExternInfo,t_wbParser_MMId::EType_Unknow);
			//
			CParseCommentMultiplT* pParseObj = new CONSTRUCT_COMMENTS_MULTIPL(sc,len);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			iOutCount = pParseObj->GetCounts();
			return ((WBPARSE_HANDLE)pParseObj) ;
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Statuses,Comments)
		{
			handle ? (delete ((CParseCommentMultiplT*)handle)) : 0;
		}


		/** 10.批量获取一组微博的评论数及转发数 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Statuses,CommentCounts)
		{
			CParseCommentCountMultiplT* pParseObj = new CONSTRUCT_COMMENTCOUNTS_MULTIPL(sc,len);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			iOutCount = pParseObj->GetCounts();
			return ((WBPARSE_HANDLE)pParseObj) ;
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Statuses,CommentCounts)
		{
			handle ? (delete ((CParseCommentCountMultiplT*)handle)) : 0;
		}

		/** 11.获取当前用户未读消息数 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Statuses,Unread)
		{
			CParseUnreadMultiplT* pParseObj = new CONSTRUCT_UNREAD_MULTIPL(sc,len);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			iOutCount = pParseObj->GetCounts();
			return ((WBPARSE_HANDLE)pParseObj) ;
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Statuses,Unread)
		{
			handle ? (delete ((CParseUnreadMultiplT*)handle)) : 0;
		}

		/** 未读消息数清零接口 */
		bool WBPARSE_FUNC_IMPLEMENTS(Statuses,ResetCount)
		{
			//TODO:未实现
			return false;
		}


		/** 表情接口，获取表情列表 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Emotions,Timeline)
		{
			CParseEmotionMultiplT* pParseObj = new CONSTRUCT_EMOTION_MULTIPL(sc,len);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			iOutCount = pParseObj->GetCounts();
			return ((WBPARSE_HANDLE)pParseObj) ;
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Emotions,Timeline)
		{
			handle ? (delete ((CParseEmotionMultiplT*)handle)) : 0;
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//微博访问接口


		/** 根据ID获取单条微博信息内容 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS( Statuses,Show)
		{
			iOutCount = 1;
			CParseStatusesSingleT* pParseObj = new CONSTRUCT_STATUSES_SINGLE(sc,len,pExternInfo);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			return ((WBPARSE_HANDLE)pParseObj) ;
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS( Statuses,Show)
		{
			handle ? (delete ((CParseStatusesSingleT*)handle)) : 0;
		}


		/** 发布一条微博信息 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS( Statuses,Update)
		{
			iOutCount = 1;
			CParseStatusesSingleT* pParseObj = new CONSTRUCT_STATUSES_SINGLE(sc,len,pExternInfo);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			return ((WBPARSE_HANDLE)pParseObj) ;
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS( Statuses,Update)
		{
			handle ? (delete ((CParseStatusesSingleT*)handle)) : 0;
		}

		/** 上传图片并发布一条微博信息 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS( Statuses,Upload)
		{
			iOutCount = 1;
			CParseStatusesSingleT* pParseObj = new CONSTRUCT_STATUSES_SINGLE(sc,len,pExternInfo);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			return ((WBPARSE_HANDLE)pParseObj) ;
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS( Statuses,Upload)
		{
			handle ? (delete ((CParseStatusesSingleT*)handle)) : 0;
		}

		/** 删除一条微博信息 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS( Statuses,Destroy)
		{
			iOutCount = 1;
			CParseStatusesSingleT* pParseObj = new CONSTRUCT_STATUSES_SINGLE(sc,len,pExternInfo);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS( Statuses,Destroy)
		{
			handle ? (delete ((CParseStatusesSingleT*)handle)) : 0;
		}

		/** 对一条微博信息进行评论 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS( Statuses,Comment)
		{
			iOutCount = 1;
			CParseCommentSingleT* pParseObj = new CONSTRUCT_COMMENTS_SINGLE(sc,len);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS( Statuses,Comment)
		{
			handle ? (delete ((CParseCommentSingleT*)handle)) : 0;
		}

		/** 删除当前用户的微博评论信息 */
		//WBPARSE_HANDLE Parse_Statuses_Comment_Destroy(const char* sc,const int len,
		//	/*[IN/OUT]*/t_WBParse_Extern_info *pExternInfo,void **ppout)
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS( Statuses,Comment_Destroy)
		{
			iOutCount = 1;
			CParseCommentSingleT* pParseObj = new CONSTRUCT_COMMENTS_SINGLE(sc,len);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS( Statuses,Comment_Destroy)
		{
			handle ? (delete ((CParseCommentSingleT*)handle)) : 0;
		}

		/** 批量删除当前用户的微博评论信息 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS( Statuses,Comment_DestroyBatch)
		{
			CParseCommentMultiplT* pParseObj = new CONSTRUCT_COMMENTS_MULTIPL(sc,len);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			iOutCount = pParseObj->GetCounts();
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS( Statuses,Comment_DestroyBatch)
		{
			handle ? (delete ((CParseCommentMultiplT*)handle)) : 0;
		}

		/** 回复微博评论信息 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Statuses,Comment_Reply)
		{
			iOutCount = 1;
			CParseCommentSingleT* pParseObj = new CONSTRUCT_COMMENTS_SINGLE(sc,len);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Statuses,Comment_Reply)
		{
			handle ? (delete ((CParseCommentSingleT*)handle)) : 0;
		}

		/** 转发一条微博 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Statuses,Repost)
		{
			iOutCount = 1;
			CParseStatusesSingleT* pParseObj = new CONSTRUCT_STATUSES_SINGLE(sc,len,pExternInfo);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Statuses,Repost)
		{
			handle ? (delete ((CParseStatusesSingleT*)handle)) : 0;
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 用户接口

		/** 根据用户ID获取用户资料（授权用户） */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS( User,Show)
		{
			iOutCount = 1;
			CParseUserSingleT* pParseObj = new CONSTRUCT_USER_SINGLE(sc,len);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS( User,Show)
		{
			handle ? (delete ((CParseUserSingleT*)handle)) : 0;
		}

		/** 获取用户关注列表及每个关注用户最新一条微博  */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Statuses,Friends)
		{
			CParseUserListT* pParseObj = new CONSTRUCT_USER_LIST(sc,len);
			pParseObj->ParseBody("users");
			//
			ppout    ? *ppout    = pParseObj->get() : NULL;
			ppCursor ? *ppCursor = pParseObj->GetCursor() : NULL;
			iOutCount = pParseObj->GetCounts();
			return ((WBPARSE_HANDLE)pParseObj) ;
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Statuses,Friends)
		{
			handle ? (delete ((CParseUserListT*)handle)) : 0;
		}

		/** 获取用户粉丝列表及及每个粉丝用户最新一条微博 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Statuses,FollowerList)
		{
			CParseUserListT* pParseObj = new CONSTRUCT_USER_LIST(sc,len);
			pParseObj->ParseBody("users");
			//
			ppout    ? *ppout    = pParseObj->get() : NULL;
			ppCursor ? *ppCursor = pParseObj->GetCursor() : NULL;
			iOutCount = pParseObj->GetCounts();
			return ((WBPARSE_HANDLE)pParseObj) ;
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Statuses,FollowerList)
		{
			handle ? (delete ((CParseUserListT*)handle)) : 0;
		}

		/** 获取系统推荐的用户 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Users,HotUser)
		{
			CParseUserMultiplT* pParseObj = new CONSTRUCT_USER_MULTIPL(sc,len);
			pParseObj->ParseBody();
			//
			ppout    ? *ppout    = pParseObj->get() : NULL;
			iOutCount = pParseObj->GetCounts();
			return ((WBPARSE_HANDLE)pParseObj) ;
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Users,HotUser)
		{
			handle ? (delete ((CParseUserMultiplT*)handle)) : 0;
		}


		/** 更新当前登录用户所关注的某个好友的备注信息 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Users,Friends_UpdateRemark)
		{
			iOutCount = 1;
			CParseUserSingleT* pParseObj = new CONSTRUCT_USER_SINGLE(sc,len);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Users,Friends_UpdateRemark)
		{
			handle ? (delete ((CParseUserSingleT*)handle)) : 0;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////
		// 私信接口

		/*获取当前用户最新私信列表*/
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(DirectMessages,Timeline)
		{
			CParseDirectMessageMultiplT* pParseObj = new CONSTRUCT_DIRECTMESSAGE_MULTIPL(sc,len);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			iOutCount = pParseObj->GetCounts();
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(DirectMessages,Timeline)
		{
			handle ? (delete ((CParseDirectMessageMultiplT*)handle)) : 0;
		}

		/** 获取当前用户发送的最新私信列表 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(DirectMessages,Sent)
		{
			CParseDirectMessageMultiplT* pParseObj = new CONSTRUCT_DIRECTMESSAGE_MULTIPL(sc,len);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			iOutCount = pParseObj->GetCounts();
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(DirectMessages,Sent)
		{
			handle ? (delete ((CParseDirectMessageMultiplT*)handle)) : 0;
		}

		/** 发送一条私信 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(DirectMessages,New)
		{
			iOutCount = 1;
			CParseDirectMessageSingleT* pParseObj = new CONSTRUCT_DIRECTMESSAGE_SINGLE(sc,len);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(DirectMessages,New)
		{
			handle ? (delete ((CParseDirectMessageSingleT*)handle)) : 0;
		}

		/** 删除一条私信 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(DirectMessages,Destroy)
		{
			iOutCount = 1;
			CParseDirectMessageSingleT* pParseObj = new CONSTRUCT_DIRECTMESSAGE_SINGLE(sc,len);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(DirectMessages,Destroy)
		{
			handle ? (delete ((CParseDirectMessageSingleT*)handle)) : 0;
		}

		/** 批量删除私信 */
		//WBPARSE_HANDLE Parse_DirectMessages_Destroy_Batch(const char* sc,const int len,/*[OUT]*/int &iOutCount,void **ppout)
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(DirectMessages,Destroy_Batch)
		{
			CParseDirectMessageMultiplT* pParseObj = new CONSTRUCT_DIRECTMESSAGE_MULTIPL(sc,len);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			iOutCount = pParseObj->GetCounts();
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(DirectMessages,Destroy_Batch)
		{
			handle ? (delete ((CParseDirectMessageMultiplT*)handle)) : 0;
		}

		/*当前登陆用户有私信往来的用户列表*/
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(DirectMessages,UserList )
		{
			CParseUserMultiplT* pParseObj = new CONSTRUCT_USER_MULTIPL(sc,len);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			iOutCount = pParseObj->GetCounts();
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(DirectMessages,UserList)
		{
			handle ? (delete ((CParseUserMultiplT*)handle)) : 0;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////
		//关注接口

		/** 关注某用户 */
		//WBPARSE_HANDLE Parse_Friendship_Create(const char* sc,const int len,void **ppout)
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Friendship,Create)
		{
			iOutCount = 1;
			CParseUserSingleT* pParseObj = new CONSTRUCT_USER_SINGLE(sc,len);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Friendship,Create)
		{
			handle ? (delete ((CParseUserSingleT*)handle)) : 0;
		}

		/** 取消关注 */
		//WBPARSE_HANDLE Parse_Friendship_Destroy(const char* sc,const int len,void **ppout)
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Friendship,Destroy)
		{
			iOutCount = 1;
			CParseUserSingleT* pParseObj = new CONSTRUCT_USER_SINGLE(sc,len);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Friendship,Destroy)
		{
			handle ? (delete ((CParseUserSingleT*)handle)) : 0;
		}


		/** 是否关注某用户(推荐使用friendships/show) */
		bool WBPARSE_FUNC_IMPLEMENTS(Friendship,Exist)
		{
			// TODO:
			return false;
		}

		/** 获取两个用户关系的详细情况 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Friendship,Show)
		{
			CParseShipShowMultiplT* pParseObj = new CONSTRUCT_FRIENDSHIP_MULTIPL(sc,len);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Friendship,Show)
		{
			handle ? (delete ((CParseUserSingleT*)handle)) : 0;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////
		//Social Graph接口
		//
		/*获取用户关注对象uid列表*/
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Friends,ids)
		{
			CParseIDs *pParseObj = new CParseIDs(sc,len);
			pParseObj->ParseBody("ids");
			ppout ? *ppout = pParseObj->get() : NULL;
			iOutCount = pParseObj->GetCounts();
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Friends,ids)
		{
			handle ? (delete ((CParseUserSingleT*)handle)) : 0;
		}



		/* 获取用户粉丝对象uid列表*/
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Follower,ids)
		{
			CParseIDs *pParseObj = new CParseIDs(sc,len);
			pParseObj->ParseBody("ids");
			ppout ? *ppout = pParseObj->get() : NULL;
			iOutCount = pParseObj->GetCounts();
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Follower,ids)
		{
			handle ? (delete ((CParseUserSingleT*)handle)) : 0;
		}


		////////////////////////////////////////////////////////////////////////////////////////////////
		//隐私设置接口

		/* 设置隐私信息*/
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Account,UpdatePrivacy)
		{
			iOutCount = 1;
			CParseUserSingleT* pParseObj = new CONSTRUCT_USER_SINGLE(sc,len);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Account,UpdatePrivacy)
		{
			handle ? (delete ((CParseUserSingleT*)handle)) : 0;
		}

		/* 获取隐私信息*/
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Account,GetPrivacy)
		{
			iOutCount = 1;
			CParseUserSingleT* pParseObj = new CONSTRUCT_USER_SINGLE(sc,len);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Account,GetPrivacy)
		{
			handle ? (delete ((CParseUserSingleT*)handle)) : 0;
		}


		////////////////////////////////////////////////////////////////////////////////////////////////
		//黑名单接口

		/** 将某用户加入黑名单 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Block,Create)
		{
			iOutCount = 1;
			CParseUserSingleT* pParseObj = new CONSTRUCT_USER_SINGLE(sc,len);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Block,Create)
		{
			handle ? (delete ((CParseUserSingleT*)handle)) : 0;
		}

		/** 将某用户移出黑名单 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Block,Destroy)
		{
			iOutCount = 1;
			CParseUserSingleT* pParseObj = new CONSTRUCT_USER_SINGLE(sc,len);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Block,Destroy)
		{
			handle ? (delete ((CParseUserSingleT*)handle)) : 0;
		}

		/** 检测某用户是否是黑名单用户 */
		bool WBPARSE_FUNC_IMPLEMENTS(Blocks,Exist)
		{
			return false;
		}

		/* 列出黑名单用户(输出用户详细信息) */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Blocks,Blocking )
		{
			CParseUserMultiplT* pParseObj = new CONSTRUCT_USER_MULTIPL(sc,len);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			iOutCount = pParseObj->GetCounts();
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Blocks,Blocking)
		{
			handle ? (delete ((CParseUserMultiplT*)handle)) : 0;
		}


		/** 列出分页黑名单用户（只输出id）*/
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Blocks,Blocking_IDs)
		{
			CParseIDs *pParseObj = new CParseIDs(sc,len);
			pParseObj->ParseBody(NULL);
			ppout ? *ppout = pParseObj->get() : NULL;
			iOutCount = pParseObj->GetCounts();
			return ((WBPARSE_HANDLE)pParseObj);
		}

		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Blocks,Blocking_IDs)
		{
			handle ? (delete ((CParseIDs*)handle)) : 0;
		}


		////////////////////////////////////////////////////////////////////////////////////////////////
		//用户标签接口

		/** 返回用户指定的标签列表 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Tags,List)
		{
			//TODO:
			return NULL;
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Tags,List)
		{
			//TODO:
		}

		/** 返回用户指定的标签列表 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Tags,Create)
		{
			//TODO:
			return NULL;
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Tags,Create)
		{
			//TODO:
		}

		/** 返回用户感兴趣的标签 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Tags,Suggestions)
		{
			//TODO:
			return NULL;
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Tags,Suggestions)
		{
			//TODO:
		}

		/** 删除标签 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Tags,Destroy)
		{
			//TODO:
			return NULL;
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Tags,Destroy)
		{
			//TODO:
		}

		/** 批量删除标签 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Tags,Destroy_Batch)
		{
			//TODO:
			return NULL;
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Tags,Destroy_Batch)
		{
			//TODO:
		}


		////////////////////////////////////////////////////////////////////////////////////////////////
		//账号接口

		/** 验证当前用户身份是否合法 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Account,Verify_Credentials)
		{
			iOutCount = 1;
			CParseUserSingleT* pParseObj = new CONSTRUCT_USER_SINGLE(sc,len);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Account,Verify_Credentials)
		{
			handle ? (delete ((CParseUserMultiplT*)handle)) : 0;
		}

		/** 获取当前用户API访问频率限制 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Account,Rate_Limit)
		{
			iOutCount = 1;
			CParseLimitesSingleT* pParseObj = new CONSTRUCT_LIMITES_SINGLE(sc,len);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Account,Rate_Limit)
		{
			handle ? (delete ((CParseLimitesSingleT*)handle)) : 0;
		}


		/** 当前用户退出登录 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Account,EndSession)
		{
			iOutCount = 1;
			CParseUserSingleT* pParseObj = new CONSTRUCT_USER_SINGLE(sc,len);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Account,EndSession)
		{
			handle ? (delete ((CParseUserSingleT*)handle)) : 0;
		}

		/**  更改头像 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Account,Update_Profile_Image)
		{
			iOutCount = 1;
			CParseUserSingleT* pParseObj = new CONSTRUCT_USER_SINGLE(sc,len);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS( Account,Update_Profile_Image)
		{
			handle ? (delete ((CParseUserSingleT*)handle)) : 0;
		}

		/**更改资料*/
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Account,Update_Profile)
		{
			iOutCount = 1;
			CParseUserSingleT* pParseObj = new CONSTRUCT_USER_SINGLE(sc,len);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS( Account,Update_Profile)
		{
			handle ? (delete ((CParseUserSingleT*)handle)) : 0;
		}

		/**注册新浪微博帐号*/
		bool WBPARSE_FUNC_IMPLEMENTS(Account,Register)
		{
			//TODO:
			return false;
		}

		/**二次注册微博的接口*/
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Account,Activate)
		{
			//TODO:
			return NULL;
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS( Account,Activate)
		{
			//TODO:
		}


		////////////////////////////////////////////////////////////////////////////////////////////////
		// 收藏接口


		/** 获取当前用户的收藏列表 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Favorites,List)
		{
			CParseStatusesMultiplT* pParseObj = new CONSTRUCT_STATUSES_MULTIPL(sc,len,pExternInfo);
			pParseObj->ParseBody();
			iOutCount = pParseObj->GetCounts();
			ppout ? *ppout = pParseObj->get() : NULL;
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Favorites,List)
		{
			handle ? (delete ((CParseStatusesMultiplT*)handle)) : 0;
		}

		/** 添加收藏 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Favorites,Create)
		{
			iOutCount = 1;
			CParseStatusesSingleT* pParseObj = new CONSTRUCT_STATUSES_SINGLE(sc,len,pExternInfo);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Favorites,Create)
		{
			handle ? (delete ((CParseStatusesSingleT*)handle)) : 0;
		}

		/** 删除收藏信息 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Favorites,Destroy)
		{
			iOutCount = 1;
			CParseStatusesSingleT* pParseObj = new CONSTRUCT_STATUSES_SINGLE(sc,len,pExternInfo);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Favorites,Destroy)
		{
			handle ? (delete ((CParseStatusesSingleT*)handle)) : 0;
		}

		/** 批量删除收藏的微博信息 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Favorites,Destroy_batch)
		{
			CParseStatusesMultiplT* pParseObj = new CONSTRUCT_STATUSES_MULTIPL(sc,len,pExternInfo);
			pParseObj->ParseBody();
			iOutCount = pParseObj->GetCounts();
			ppout ? *ppout = pParseObj->get() : NULL;
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Favorites,Destroy_batch)
		{
			handle ? (delete ((CParseStatusesMultiplT*)handle)) : 0;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////
		// 登录/OAuth接口
		//

		/** 获取未授权的Request Token */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Oauth,RequestToken)
		{
			struct t_wbParse_Oauth* oa = (struct t_wbParse_Oauth*)malloc(sizeof(struct t_wbParse_Oauth));
			if( oa )
			{
				memset(oa , 0 , sizeof(struct t_wbParse_Oauth) );
				wb_parse_oauth(sc , oa->oauth_token, oa->oauth_token_secret , 0 );
				ppout ? *ppout = oa : NULL;
			}
			return (WBPARSE_HANDLE)oa;
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Oauth,RequestToken)
		{
			struct t_wbParse_Oauth* oa = (struct t_wbParse_Oauth*)malloc(sizeof(struct t_wbParse_Oauth));
			if( oa) {
				free(oa);
			}
		}

		/** 请求用户授权Token */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Oauth,Oauthorize)
		{
			struct t_wbParse_Oauth* oa = (struct t_wbParse_Oauth*)malloc(sizeof(struct t_wbParse_Oauth));
			if( oa ) {
				memset(oa , 0 , sizeof(struct t_wbParse_Oauth) );
#ifdef _USE_JSON_PARSER
				wbParserNS::REQOBJ *pRoot = wbParserNS::Parse_data_JSON(sc);
				wbParserNS::GetCHAR_Key_JSON("oauth_verifier", pRoot , oa->oauth_verifier , 64);
				wbParserNS::Parse_free_JSON(pRoot);
#elif _USE_XML_PARSER
				wbParserNS::REQOBJ *pRoot = wbParserNS::Parse_data_XML(sc);
				wbParserNS::GetCHAR_Key_XML("oauth_verifier", pRoot , oa->oauth_verifier , 64);
				wbParserNS::Parse_free_XML(pRoot);
#endif //_USE_XML_PARSER
				ppout ? *ppout = oa : NULL;
			}
			return (WBPARSE_HANDLE)oa;
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Oauth,Oauthorize)
		{
			struct t_wbParse_Oauth* oa = (struct t_wbParse_Oauth*)handle;
			if( oa){
				free(oa);
			}
		}

		/**获取授权过的Access Token*/
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Oauth,AccessToken) 
		{
			struct t_wbParse_Oauth* oa = (struct t_wbParse_Oauth*)malloc(sizeof(struct t_wbParse_Oauth));
			if( oa )
			{
				memset(oa , 0 , sizeof(struct t_wbParse_Oauth) );
				wb_parse_oauth(sc , oa->oauth_token , oa->oauth_token_secret ,oa->uid_  );
				ppout ? *ppout = oa : NULL;
			}
			return (WBPARSE_HANDLE)oa;
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Oauth,AccessToken)
		{
			struct t_wbParse_Oauth* oa = (struct t_wbParse_Oauth*)handle;
			if( oa){
				free(oa);
			}
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 微博搜索 :Search API仅对新浪合作开发者开放，申请请联系 @微博开放平台
		//

		/** 搜索微博 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Search,Statuses)
		{
			CParseStatusesMultiplT* pParseObj = new CONSTRUCT_STATUSES_MULTIPL(sc,len,pExternInfo);
			pParseObj->ParseBody();
			iOutCount = pParseObj->GetCounts();
			ppout ? *ppout = pParseObj->get() : NULL;
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Search,Statuses)
		{
			handle ? (delete ((CParseStatusesMultiplT*)handle)) : 0;
		}

		/** 搜索用户 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Search,Users)
		{
			CParseUserMultiplT* pParseObj = new CONSTRUCT_USER_MULTIPL(sc,len);
			pParseObj->ParseBody();
			iOutCount = pParseObj->GetCounts();
			ppout ? *ppout = pParseObj->get() : NULL;
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Search,Users)
		{
			handle ? (delete ((CParseUserMultiplT*)handle)) : 0;
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 热点信息 :HOT API仅对新浪合作开发者开放，申请请联系 @微博开放平台,by welbon,2011-01-07
		//
		/** 获取热点评论的微博按当天 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(HotPoint,Comment_Dayliy)
		{
			CParseStatusesMultiplT* pParseObj = new CONSTRUCT_STATUSES_MULTIPL(sc,len,pExternInfo);
			pParseObj->ParseBody();
			iOutCount = pParseObj->GetCounts();
			ppout ? *ppout = pParseObj->get() : NULL;
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(HotPoint,Comment_Dayliy)
		{
			handle ? (delete ((CParseStatusesMultiplT*)handle)) : 0;
		}

		/** 获取热点评论的微博按本周 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(HotPoint,Comment_Weekliy)
		{
			CParseStatusesMultiplT* pParseObj = new CONSTRUCT_STATUSES_MULTIPL(sc,len,pExternInfo);
			pParseObj->ParseBody();
			iOutCount = pParseObj->GetCounts();
			ppout ? *ppout = pParseObj->get() : NULL;
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(HotPoint,Comment_Weekliy)
		{
			handle ? (delete ((CParseStatusesMultiplT*)handle)) : 0;
		}

		/** 获取热点转发的微博按当天 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(HotPoint,Repost_Dayliy)
		{
			CParseStatusesMultiplT* pParseObj = new CONSTRUCT_STATUSES_MULTIPL(sc,len,pExternInfo);
			pParseObj->ParseBody();
			iOutCount = pParseObj->GetCounts();
			ppout ? *ppout = pParseObj->get() : NULL;
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(HotPoint,Repost_Dayliy)
		{
			handle ? (delete ((CParseStatusesMultiplT*)handle)) : 0;
		}

		/** 获取热点转发的微博按本周 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(HotPoint,Repost_Weekliy)
		{
			CParseStatusesMultiplT* pParseObj = new CONSTRUCT_STATUSES_MULTIPL(sc,len,pExternInfo);
			pParseObj->ParseBody();
			iOutCount = pParseObj->GetCounts();
			ppout ? *ppout = pParseObj->get() : NULL;
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(HotPoint,Repost_Weekliy)
		{
			handle ? (delete ((CParseStatusesMultiplT*)handle)) : 0;
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 一般解析

		/** 错误解析 */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(WEIBO_ERR,ITEM)
		{
			struct t_wbParse_Error* pErr = (struct t_wbParse_Error*)wbParserNS::wbParse_Malloc_Error(1);
			if( pErr ) {
				memset(pErr , 0, sizeof(t_wbParse_Error));
				CURLDecodeA c_url(sc , len );
				CUTF82C	c_err(c_url.c_str() );
				wb_parse_error( c_err.c_str() , pErr->request , pErr->error , pErr->error_code );
				ppout ? *ppout = pErr : NULL;
				pErr->error_sub_code = atoi( pErr->error);
				return (WBPARSE_HANDLE)pErr;
			}
			else {
				assert(0);
			}
			return NULL;
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(WEIBO_ERR,ITEM)
		{
			struct t_wbParse_Error* pErr = (struct t_wbParse_Error*)handle;
			if( pErr){
				free(pErr);
			}
		}

		/** Cookie base */
		WBPARSER_API void WBPARSE_FUNC_IMPLEMENTS(Cookie,BASE) 
		{
			t_wbParse_Cookie* _cookie = ppout ? (t_wbParse_Cookie*)*ppout : NULL;
			std::string uid ,tgt , ticket , publickey , keyversion;
			if( !_cookie )
			{
				return ;
			}

			int     error = 0;
			__int64 lservertime = 0;

			if( !sc || len <= 0 )
			{
				error = -1;
			}
			else
			{
				// json 解释
				Json::Reader jReader;
				Json::Value  jValue;
				if( !jReader.parse( sc ,jValue ,false) )
				{
					// to do
					error = -2;
				}

				Json::Value::UInt i = 0;
				while( i < jValue.size() ) 
				{			
					Json::Value* jo = NULL;
					if( jValue.type() != Json::arrayValue ){
						jo = &jValue;
					}
					else{
						jo = &jValue[i];
					}
					// object
					if( jo->type() == Json::objectValue )
					{
						Json::Value::Members members( jo->getMemberNames() );
						Json::Value::Members::iterator it = members.begin();
						Json::Value::Members::iterator end= members.end();			
						while( it != end )
						{
							const std::string &name    = *it;
							const Json::Value& jovalue = (*jo)[name];
							if( stricmp(name.c_str(),"ticket") == 0 )
							{// ticket
								if( jovalue.type() == Json::stringValue)
								{
									const char* szTicket = jovalue.asCString();
									if( strnicmp(szTicket,"ST-",3) == 0 )
									{
										ticket = szTicket;
									}
									else if( strnicmp(szTicket,"ERR-",4) == 0 )
									{
										std::string error = szTicket;
										int pos = error.find('-');
										if( pos > 0 )
										{
											std::string cerr;
											std::string er = error.substr(pos+1);
											pos = er.find('-');
											if( pos )
											{
												cerr = er.substr(pos+1);
												er   = er.substr(0,pos);
											}
											int i_error = atoi( er.c_str() );
											int i_cerror= atoi( cerr.c_str() );
										}
										else
										{
											error =  -1;
										}
									}
								}
							}					
							else if( stricmp(name.c_str(),"uid") == 0 )
							{// uid
								if( jovalue.type() == Json::stringValue)
								{
									uid = jovalue.asCString();
								}
							}
							else if( stricmp(name.c_str(),"tgt") == 0 )
							{
								if( jovalue.type() == Json::stringValue)
								{
									tgt = jovalue.asCString();
								}
							}
							else if( stricmp(name.c_str(),"retcode") == 0 )
							{
								if( jovalue.type() == Json::stringValue)
								{
									error = atoi( jovalue.asCString() );
								}
								else if( jovalue.type() == Json::intValue)
								{
									error = jovalue.asInt();
								}

							}
							else if( stricmp(name.c_str(),"servertime") == 0 )
							{
								if( jovalue.type() == Json::stringValue )
									lservertime = _atoi64(jovalue.asString().c_str() );
							}
							else if( stricmp(name.c_str(),"publickey") == 0 )
							{
								if( jovalue.type() == Json::stringValue )
									publickey = jovalue.asString();
							}
							else if( stricmp(name.c_str(),"keyversion") == 0 )
							{
								if( jovalue.type() == Json::stringValue )
									keyversion = jovalue.asString();
							}
							it++;
						}//while( it != end )
					}
					i++;
				} //while( i < jValue.size() ) 
			}

			//
			if( _cookie && uid.size() && tgt.size() && ticket.size() )
			{// 成功的
				strncpy(_cookie->uid , uid.c_str() , 128);
				strncpy(_cookie->tgt , tgt.c_str() , 256 );
				strncpy(_cookie->publickey , publickey.c_str() , 512 );
				strncpy(_cookie->keyversion, keyversion.c_str(),16);
				strncpy(_cookie->ticket , ticket.c_str() , 256 );
			}
			else
			{// 失败的
				_cookie->error = error;
			}
		}

		/** 解析cookie,返回指定的串 */
		WBPARSER_API int Parse_CookieStr(const char *sc,char *token,char *secret)
		{
			return wb_parse_cookie(sc,token,secret);
		}
	}



#ifdef __cplusplus
}
#endif //__cplusplus
