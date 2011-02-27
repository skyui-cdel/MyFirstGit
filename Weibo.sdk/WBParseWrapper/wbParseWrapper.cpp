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


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �ڲ����ͺ���

// �ڲ����ͽ��

inline t_wbParse_Ret* Internal_ParseRet(const char *body)
{
	t_wbParse_Ret* pRet = 0;
	REQOBJ *pObjRoot = wbParserNS::Parse_data_JSON(body);
	if( pObjRoot )
	{
		pRet = CWBJsonParser::parse_Ret(pObjRoot);
		wbParserNS::Parse_free_JSON(pObjRoot);
	}
	return pRet;
}

// trend hot
inline t_wbParse_TrendHotQuery* Internal_Parse_TrendHotQuery(const char *body )
{
	t_wbParse_TrendHotQuery *pTrendHotQuery = 0;
	REQOBJ *pObjRoot = wbParserNS::Parse_data_JSON(body);
	if( pObjRoot )
	{
		REQOBJ *pObjTrends = wbParserNS::GetObject_Key_JSON("trends",pObjRoot);
		if( !pObjTrends )
		{
			wbParserNS::Parse_free_JSON(pObjRoot);
			return 0;
		}
		pTrendHotQuery = (t_wbParse_TrendHotQuery*)wbParse_Malloc_TrendHotQuery(1);
		if( !pTrendHotQuery )
		{
			wbParserNS::Parse_free_JSON(pObjRoot);
			return 0;
		}
		// get time
		Json::Value *pvalTrends  = (Json::Value*)(pObjTrends);
		if(pvalTrends->size())
		{
			Json::Value::iterator it = pvalTrends->begin() ;
			WBPARSER_COPY( pTrendHotQuery->time_,WBPARSER_REAL_LEN(TREND_ID),it.memberName(),strlen(it.memberName()) );
		}
		// get as_of
		char buf [128] = {0};
		GET_LONG_TO_STR(JSON,pObjRoot,"as_of",buf,128);
		//
		pTrendHotQuery->trend_counts_ = GetObject_JSON_SIZE(pObjRoot);
		if( pTrendHotQuery->trend_counts_ <= 0 )
		{
			wbParse_Free_TrendHotQuery(pTrendHotQuery,1);
			wbParserNS::Parse_free_JSON(pObjRoot);
			return 0;
		}
		pTrendHotQuery->trends_ = (t_wbParse_TrendHotQuery::TrendItem*)wbParse_Malloc_TrendHotQuery(pTrendHotQuery->trend_counts_);
		//
		t_wbParse_TrendHotQuery::TrendItem *pItemTrendList = pTrendHotQuery->trends_;
		for( int i = 0; i < pTrendHotQuery->trend_counts_; ++ i ){
			t_wbParse_TrendHotQuery::TrendItem *pItemTrend = (pItemTrendList + i) ;
			CWBJsonParser::parse_Trends_getHot( GetObject_Idx_JSON(i,pObjRoot),pItemTrend);
		}
		//
		wbParserNS::Parse_free_JSON(pObjRoot);
	}
	return pTrendHotQuery;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifdef __cplusplus
extern "C"{
#endif //__cplusplus


	namespace wbParserNS
	{
		//�������ݼ�

		/** 2.���µĹ���΢����Ϣ */
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



		/** 3.��ǰ��¼�û���������ע�û�������΢����Ϣ */
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


		/** 4.��ǰ��¼�û�������΢����Ϣ�б� */
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


		/** 5.@��ǰ�û���΢���б� */
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


		/** 6.��ǰ�û����ͼ��յ��������б� */
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

		/** 7.��ȡ��ǰ�û����������� */
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


		/** 8.��ȡ��ǰ�û��յ������� */
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


		/** 9.����΢����ϢID����ĳ��΢����Ϣ�������б� */
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


		/** 10.������ȡһ��΢������������ת���� */
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

		/** 11.��ȡ��ǰ�û�δ����Ϣ�� */
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

		/** δ����Ϣ������ӿ� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Statuses,ResetCount)
		{
			iOutCount = 1;
			void *pout = Internal_ParseRet(sc);
			ppout ? *ppout = pout : 0;
			return (WBPARSE_HANDLE)pout;
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Statuses,ResetCount)
		{
			wbParse_Free_Ret((void*)handle,0 );
		}

		/** ����ӿڣ���ȡ�����б� */
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
		//΢�����ʽӿ�


		/** ����ID��ȡ����΢����Ϣ���� */
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


		/** ����һ��΢����Ϣ */
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

		/** �ϴ�ͼƬ������һ��΢����Ϣ */
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

		/** �ϴ�ͼƬ */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS( Statuses,Upload_Pic )
		{
			iOutCount = 1;
			CParseUploadPicSingleT* pParseObj = new CONSTRUCT_UPLOADPIC_MULTIPL(sc,len);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			return ((WBPARSE_HANDLE)pParseObj) ;
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS( Statuses,Upload_Pic)
		{
			handle ? (delete ((CParseUploadPicSingleT*)handle)) : 0;
		}

		/** ɾ��һ��΢����Ϣ */
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

		/** ��һ��΢����Ϣ�������� */
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

		/** ɾ����ǰ�û���΢��������Ϣ */
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

		/** ����ɾ����ǰ�û���΢��������Ϣ */
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

		/** �ظ�΢��������Ϣ */
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

		/** ת��һ��΢�� */
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
		// �û��ӿ�

		/** �����û�ID��ȡ�û����ϣ���Ȩ�û��� */
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

		/** ��ȡ�û���ע�б�ÿ����ע�û�����һ��΢��  */
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

		/** ��ȡ�û���˿�б���ÿ����˿�û�����һ��΢�� */
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

		/** ��ȡϵͳ�Ƽ����û� */
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

		/** ����Ȥ���û� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Users,Suggestions)
		{
			CParseUserMultiplT* pParseObj = new CONSTRUCT_USER_MULTIPL(sc,len);
			pParseObj->ParseBody();
			//
			ppout    ? *ppout    = pParseObj->get() : NULL;
			iOutCount = pParseObj->GetCounts();
			return ((WBPARSE_HANDLE)pParseObj) ;
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Users,Suggestions)
		{
			handle ? (delete ((CParseUserMultiplT*)handle)) : 0;
		}


		/** ���µ�ǰ��¼�û�����ע��ĳ�����ѵı�ע��Ϣ */
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
		// ˽�Žӿ�

		/*��ȡ��ǰ�û�����˽���б�*/
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

		/** ��ȡ��ǰ�û����͵�����˽���б� */
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

		/** ����һ��˽�� */
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

		/** ɾ��һ��˽�� */
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

		/** ��ǰ����ѵ�����˽���б� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(DirectMessages,With)
		{
			CParseDirectMessageMultiplT* pParseObj = new CONSTRUCT_DIRECTMESSAGE_MULTIPL(sc,len);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			iOutCount = pParseObj->GetCounts();
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(DirectMessages,With)
		{
			handle ? (delete ((CParseDirectMessageMultiplT*)handle)) : 0;
		}


		/** ����ɾ��˽�� */
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

		/*��ǰ��½�û���˽���������û��б�*/
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
		//��ע�ӿ�

		/** ��עĳ�û� */
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

		/** ������ע�û� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Friendship,CreateBatch)
		{
			CParseUserMultiplT* pParseObj = new CONSTRUCT_USER_MULTIPL(sc,len);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			iOutCount = pParseObj->GetCounts();
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Friendship,CreateBatch)
		{
			handle ? (delete ((CParseUserMultiplT*)handle)) : 0;
		}

		/** ȡ����ע */
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


		/** �Ƿ��עĳ�û�(�Ƽ�ʹ��friendships/show) */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Friendship,Exist)
		{
			iOutCount = 1;
			void *pout = Internal_ParseRet(sc);
			ppout ? *ppout = pout : 0;
			return (WBPARSE_HANDLE)pout;
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Friendship,Exist)
		{
			wbParse_Free_Ret((void*)handle,0 );
		}



		/** ��ȡ�����û���ϵ����ϸ��� */
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
		//Social Graph�ӿ�
		//
		/*��ȡ�û���ע����uid�б�*/
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



		/* ��ȡ�û���˿����uid�б�*/
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
		//��˽���ýӿ�

		/* ������˽��Ϣ*/
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

		/* ��ȡ��˽��Ϣ*/
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
		//�������ӿ�

		/** ��ĳ�û���������� */
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

		/** ��ĳ�û��Ƴ������� */
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

		/** ���ĳ�û��Ƿ��Ǻ������û� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Block,Exist)
		{
			iOutCount = 1;
			void *pout = Internal_ParseRet(sc);
			ppout ? *ppout = pout : 0;
			return (WBPARSE_HANDLE)pout;
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Block,Exist)
		{
			wbParse_Free_Ret((void*)handle,0 );
		}

		/* �г��������û�(����û���ϸ��Ϣ) */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Block,Blocking )
		{
			CParseUserMultiplT* pParseObj = new CONSTRUCT_USER_MULTIPL(sc,len);
			pParseObj->ParseBody();
			ppout ? *ppout = pParseObj->get() : NULL;
			iOutCount = pParseObj->GetCounts();
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Block,Blocking)
		{
			handle ? (delete ((CParseUserMultiplT*)handle)) : 0;
		}


		/** �г���ҳ�������û���ֻ���id��*/
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Block,Blocking_IDs)
		{
			CParseIDs *pParseObj = new CParseIDs(sc,len);
			pParseObj->ParseBody(NULL);
			ppout ? *ppout = pParseObj->get() : NULL;
			iOutCount = pParseObj->GetCounts();
			return ((WBPARSE_HANDLE)pParseObj);
		}

		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Block,Blocking_IDs)
		{
			handle ? (delete ((CParseIDs*)handle)) : 0;
		}


		////////////////////////////////////////////////////////////////////////////////////////////////
		//�û���ǩ�ӿ�

		/** �����û�ָ���ı�ǩ�б� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Tags,List)
		{
#ifdef _USE_JSON_PARSER
			REQOBJ *pObjRoot = Parse_data_JSON(sc);
			iOutCount = GetObject_JSON_SIZE( pObjRoot);
			if( iOutCount <= 0 ){
				Parse_free_JSON(pObjRoot);
				return NULL;
			}
			t_wbParse_Tag *pTaglist = (t_wbParse_Tag *)wbParse_Malloc_Tag(iOutCount);
			for( int i = 0; i < iOutCount; ++ i ){
				t_wbParse_Tag *pTag = (pTaglist + i) ;
				CWBJsonParser::parse_Tags( GetObject_Idx_JSON(i,pObjRoot),pTag,NULL);
			}
			Parse_free_JSON(pObjRoot);
			return ((WBPARSE_HANDLE)pTaglist);
#else
			//TODO:
			assert(0);
			return NULL;
#endif //
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Tags,List)
		{
			wbParse_Free_Tag((void*)handle,0);
		}

		/** �û������ı�ǩ */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Tags,Create)
		{
#ifdef _USE_JSON_PARSER
			REQOBJ *pObjRoot = Parse_data_JSON(sc);
			iOutCount = GetObject_JSON_SIZE( pObjRoot);
			if( iOutCount <= 0 )
			{
				Parse_free_JSON(pObjRoot);
				return 0;
			}
			t_wbParse_Tag *pTaglist = (t_wbParse_Tag *)wbParse_Malloc_Tag(iOutCount);
			for( int i = 0; i < iOutCount; ++ i )
			{
				t_wbParse_Tag *pTag = (pTaglist + i) ;
				CWBJsonParser::parse_Tags( GetObject_Idx_JSON(i,pObjRoot),pTag,_WBC("tagid") );
			}
			Parse_free_JSON(pObjRoot);
			return ((WBPARSE_HANDLE)pTaglist);
#else
			//TODO:
			assert(0);
			return NULL;
#endif //
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Tags,Create)
		{
			wbParse_Free_Tag((void*)handle,0);
		}

		/** �����û�����Ȥ�ı�ǩ */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Tags,Suggestions)
		{
#ifdef _USE_JSON_PARSER
			REQOBJ *pObjRoot = Parse_data_JSON(sc);
			iOutCount = GetObject_JSON_SIZE( pObjRoot);
			if( iOutCount <= 0 )
			{
				Parse_free_JSON(pObjRoot);
				return 0;
			}
			t_wbParse_Tag *pTaglist = (t_wbParse_Tag *)wbParse_Malloc_Tag(iOutCount);
			for( int i = 0; i < iOutCount; ++ i )
			{
				t_wbParse_Tag *pTag = (pTaglist + i) ;
				CWBJsonParser::parse_Tags( GetObject_Idx_JSON(i,pObjRoot),pTag,_WBC("id") );
			}
			Parse_free_JSON(pObjRoot);
			return ((WBPARSE_HANDLE)pTaglist);
#else
			//TODO:
			assert(0);
			return NULL;
#endif //
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Tags,Suggestions)
		{
			wbParse_Free_Tag((void*)handle,0 );
		}

		/** ɾ����ǩ */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Tags,Destroy)
		{
			iOutCount = 1;
			void *pout = Internal_ParseRet(sc);
			ppout ? *ppout = pout : 0;
			return (WBPARSE_HANDLE)pout;
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Tags,Destroy)
		{
			wbParse_Free_Ret((void*)handle,0 ) ;
		}

		/** ����ɾ����ǩ */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Tags,Destroy_Batch)
		{
#ifdef _USE_JSON_PARSER
			REQOBJ *pObjRoot = Parse_data_JSON(sc);
			iOutCount = GetObject_JSON_SIZE( pObjRoot);
			if( iOutCount <= 0 )
			{
				Parse_free_JSON(pObjRoot);
				return 0;
			}
			t_wbParse_Tag *pTaglist = (t_wbParse_Tag *)wbParse_Malloc_Tag(iOutCount);
			for( int i = 0; i < iOutCount; ++ i )
			{
				t_wbParse_Tag *pTag = (pTaglist + i) ;
				CWBJsonParser::parse_Tags( GetObject_Idx_JSON(i,pObjRoot),pTag,_WBC("tagid") );
			}
			Parse_free_JSON(pObjRoot);
			return ((WBPARSE_HANDLE)pTaglist);
#else
			//TODO:
			assert(0);
			return NULL;
#endif //
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Tags,Destroy_Batch)
		{
			wbParse_Free_Tag((void*)handle,0 );
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// ����ӿ� ,by welbon,2011-01-10

		/** ��ȡĳ�˵Ļ��� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Trend,Trends)
		{
#ifdef _USE_JSON_PARSER
			REQOBJ *pObjRoot = Parse_data_JSON(sc);
			iOutCount = GetObject_JSON_SIZE( pObjRoot);
			if( iOutCount <= 0 )
			{
				Parse_free_JSON(pObjRoot);
				return 0;
			}
			t_wbParse_Trend *pTrendlist = (t_wbParse_Trend*)wbParse_Malloc_Trend(iOutCount);
			for( int i = 0; i < iOutCount; ++ i )
			{
				t_wbParse_Trend *pTrend = (pTrendlist + i) ;
				CWBJsonParser::parse_Trends_getData( GetObject_Idx_JSON(i,pObjRoot),pTrend );
			}
			Parse_free_JSON(pObjRoot);
			return ((WBPARSE_HANDLE)pTrendlist);
#else
			//TODO:
			assert(0);
#endif //
			return NULL;
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Trend,Trends)
		{
			wbParse_Free_Trend((void*)handle,0 );
		}

		/** ��ȡĳһ�����µ�΢�� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Trend,Statuses)
		{
			CParseStatusesMultiplT* pParseObj = new CONSTRUCT_STATUSES_MULTIPL(sc,len,pExternInfo);
			pParseObj->ParseBody();
			iOutCount = pParseObj->GetCounts();
			ppout ? *ppout = pParseObj->get() : NULL;
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Trend,Statuses)
		{
			handle ? (delete ((CParseStatusesMultiplT*)handle)) : 0;
		}

		/** ��עĳһ������ */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Trend,Follow)
		{
#ifdef _USE_JSON_PARSER
			REQOBJ *pObjRoot = Parse_data_JSON(sc);
			if( pObjRoot )
			{
				t_wbParse_Trend *pTrend = (t_wbParse_Trend*)wbParse_Malloc_Trend(iOutCount);
				CWBJsonParser::parse_Trends_Follow( pObjRoot,pTrend );
				Parse_free_JSON(pObjRoot);
				return ((WBPARSE_HANDLE)pTrend);
			}
			return 0;
#else
			//TODO:
			assert(0);
#endif //
			return NULL;
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Trend,Follow)
		{
			wbParse_Free_Trend((void*)handle,0);
		}

		/** ȡ����ע��ĳһ������ */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Trend,Destroy)
		{
			iOutCount = 1;
			void *pout = Internal_ParseRet(sc);
			ppout ? *ppout = pout : 0;
			return (WBPARSE_HANDLE)pout;
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Trend,Destroy)
		{
			wbParse_Free_Ret((void*)handle,0);
		}

		/** ��Сʱ�������Ż��� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Trend,Hourly)
		{
			iOutCount = 1;
			void *pout = Internal_Parse_TrendHotQuery(sc);
			ppout ? *ppout = pout : 0;
			return (WBPARSE_HANDLE)pout;
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Trend,Hourly)
		{
			wbParse_Free_TrendHotQuery((void*)handle,0);
		}

		/** �����ڷ������Ż��⡣����ĳһ���ڵĻ��� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Trend,Daily)
		{
			iOutCount = 1;
			void *pout = Internal_Parse_TrendHotQuery(sc);
			ppout ? *ppout = pout : 0;
			return (WBPARSE_HANDLE)pout;
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Trend,Daily)
		{
			wbParse_Free_TrendHotQuery((void*)handle,0);
		}

		/** ���ܷ������Ż��⡣����ĳһ����֮ǰĳһ�ܵĻ��� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Trend,Weekly)
		{
			iOutCount = 1;
			void *pout = Internal_Parse_TrendHotQuery(sc);
			ppout ? *ppout = pout : 0;
			return (WBPARSE_HANDLE)pout;
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Trend,Weekly)
		{
			wbParse_Free_TrendHotQuery((void*)handle,0);
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// ����ӿ�

		//
		void ParseInviteUsrs(wbParserNS::REQOBJ *pObjInAtt,t_wbParse_InviteContact::ItemUsr **ppUsrArray,int &outsize,bool bobject )
		{
			if( !pObjInAtt)
				return;

			outsize = wbParserNS::GetObject_JSON_SIZE(pObjInAtt);
			if( outsize <= 0 ){
				return ;
			}
			t_wbParse_InviteContact::ItemUsr *pInviteContactUsrs = (t_wbParse_InviteContact::ItemUsr*)wbParse_Malloc_InviteContact_Usr(outsize);
			if( !bobject )
			{//ֻ��email�ֶ�
				for( int i = 0 ; i < outsize; i ++ ) {
					t_wbParse_InviteContact::ItemUsr *pUsr = (pInviteContactUsrs + i);
					wbParserNS::GetCHAR_Idx_JSON( i,pObjInAtt,pUsr->email_,WBPARSER_USE_LEN(email) );
				}
			}
			else {
				for( int i = 0 ; i < outsize; i ++ )
				{
					wbParserNS::REQOBJ* pObjInAttUsr = wbParserNS::GetObject_Idx_JSON( i,pObjInAtt);
					t_wbParse_InviteContact::ItemUsr *pUsr = (pInviteContactUsrs + i);
					wbParserNS::CWBJsonParser::parse_invite_contact_usr(pObjInAttUsr,pUsr);
				}
			}
			(ppUsrArray) ? (*ppUsrArray = pInviteContactUsrs) : 0 ;
		}


		t_wbParse_InviteContact* ParseInviteContact(const char* sc,int &outsize,bool bgroup )
		{
			outsize = 0;
#ifdef _USE_JSON_PARSER
			wbParserNS::REQOBJ *pObjRoot = wbParserNS::Parse_data_JSON(sc);
			if( !pObjRoot )
			{
				return 0;
			}
			//
			t_wbParse_InviteContact* pInviteContact = (t_wbParse_InviteContact*)wbParse_Malloc_InviteContact(1);
			if( !pInviteContact )
			{
				wbParserNS::Parse_free_JSON(pObjRoot);
				return 0;
			}
			outsize = 1;

			// error code
			wbParserNS::GetCHAR_Key_JSON("code", pObjRoot , pInviteContact->error_code_ , WBPARSER_REAL_LEN(error_code) );

			//data
			wbParserNS::REQOBJ* pObjData = wbParserNS::GetObject_Key_JSON("data",pObjRoot);
			if( pObjData) 
			{
				// in attend
				ParseInviteUsrs(wbParserNS::GetObject_Key_JSON("in_att",pObjData) ,
					&pInviteContact->in_attend_usr_,pInviteContact->in_attend_usr_count_ ,true );

				wbParserNS::REQOBJ *pObjUsrArray = NULL;

				// in not attend
				pObjUsrArray = wbParserNS::GetObject_Key_JSON("in_no_att",pObjData);

				if( pObjUsrArray )
				{
					if( bgroup  ) 
					{
						// group name
						Json::Value *val = (Json::Value *)(pObjUsrArray);
						pInviteContact->not_attend_group_count_ = val->size();
						pInviteContact->not_attend_group_ = (t_wbParse_InviteContact::ItemGroup*)wbParse_Malloc_InviteContact_Group(val->size());
						Json::Value::iterator it = val->begin();
						int i = 0;
						while( it != val->end())
						{
							t_wbParse_InviteContact::ItemGroup *gr = (pInviteContact->not_attend_group_ + i);

							// UTF8 convert
							char* outstr = NULL;
							if(  lo_Utf82C(&outstr , it.memberName()) ) {
								strcpy( gr->groupname_,outstr );
								free(outstr);
							}
							else{
								strcpy( gr->groupname_,it.memberName() );
							}

							ParseInviteUsrs((wbParserNS::REQOBJ*)&(*it),&(gr->usrs_),(gr->count_),true );
							++ i;
							++ it ;
						}

					}
					else{
						pInviteContact->not_attend_group_count_ = 1;
						pInviteContact->not_attend_group_ = (t_wbParse_InviteContact::ItemGroup*)wbParse_Malloc_InviteContact_Group(1);
						ParseInviteUsrs( pObjUsrArray,&pInviteContact->not_attend_group_->usrs_,
							pInviteContact->not_attend_group_->count_,true );
					}
				}
				// not weibo
				pObjUsrArray = wbParserNS::GetObject_Key_JSON("out",pObjData);

				if( pObjUsrArray )
				{
					if( bgroup)  {
						// group name
						Json::Value *val = (Json::Value *)(pObjUsrArray);
						pInviteContact->out_attend_group_count_ = val->size();
						pInviteContact->out_attend_group_ = (t_wbParse_InviteContact::ItemGroup*)wbParse_Malloc_InviteContact_Group(val->size());
						Json::Value::iterator it = val->begin();
						int i = 0;
						while( it != val->end())
						{
							t_wbParse_InviteContact::ItemGroup *gr = (pInviteContact->out_attend_group_ + i);

							// UTF8 convert
							char* outstr = NULL;
							if(  lo_Utf82C(&outstr , it.memberName()) ) {
								strcpy( gr->groupname_,outstr );
								free(outstr);
							}
							else{
								strcpy( gr->groupname_,it.memberName() );
							}

							ParseInviteUsrs((wbParserNS::REQOBJ*)&(*it),&(gr->usrs_),(gr->count_),true );
							++ i;
							++ it ;
						}
					}
					else{
						pInviteContact->out_attend_group_count_ = 1;
						pInviteContact->out_attend_group_ = (t_wbParse_InviteContact::ItemGroup*)wbParse_Malloc_InviteContact_Group(1);
						ParseInviteUsrs( pObjUsrArray,&pInviteContact->out_attend_group_->usrs_,
							pInviteContact->out_attend_group_->count_,false );
					}
				}
			}
			//Free
			wbParserNS::Parse_free_JSON(pObjRoot);
#endif //_USE_JSON_PARSER

			return pInviteContact;
		}


		/** ����������ϵ�� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(InviteContact,Mail)
		{
			t_wbParse_InviteContact *pInviteContact = ParseInviteContact(sc,iOutCount,false);
			ppout ? *ppout = pInviteContact : NULL;
			return (WBPARSE_HANDLE)pInviteContact; 
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(InviteContact,Mail)
		{
			handle ? wbParserNS::wbParse_Free_InviteContact((t_wbParse_InviteContact*)handle,1) : 0;
		}

		/** ����MSN��ϵ�� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(InviteContact,MSN)
		{
			t_wbParse_InviteContact *pInviteContact = ParseInviteContact(sc,iOutCount,true);
			ppout ? *ppout = pInviteContact : NULL;
			return (WBPARSE_HANDLE)pInviteContact;  
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(InviteContact,MSN)
		{
			handle ? wbParserNS::wbParse_Free_InviteContact((t_wbParse_InviteContact*)handle,1) : 0;
		}

		/** ���������ʼ� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(InviteContact,SendMail)
		{
#ifdef _USE_JSON_PARSER
			t_wbParse_Ret* pRet = 0;
			REQOBJ *pObjRoot = wbParserNS::Parse_data_JSON(sc);
			if( pObjRoot )
			{
				pRet = CWBJsonParser::parse_Ret(pObjRoot);
				wbParserNS::Parse_free_JSON(pObjRoot);
			}
			ppout ? *ppout = pRet : NULL;
			return (WBPARSE_HANDLE)pRet ;
#endif //_USE_XML_PARSER
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(InviteContact,SendMail)
		{
			wbParse_Free_Ret((void*)handle,0 ) ;
		}


		////////////////////////////////////////////////////////////////////////////////////////////////
		//�˺Žӿ�

		/** ��֤��ǰ�û�����Ƿ�Ϸ� */
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

		/** ��ȡ��ǰ�û�API����Ƶ������ */
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


		/** ��ǰ�û��˳���¼ */
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

		/**  ����ͷ�� */
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

		/**��������*/
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

		/**ע������΢���ʺ�*/
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Account,Register)
		{
			iOutCount = 0;
			t_wbParse_Register_Ret *pRet = 0;
			REQOBJ *pObjRoot = wbParserNS::Parse_data_JSON(sc);
			if( pObjRoot )
			{				
				pRet = (t_wbParse_Register_Ret*)malloc(sizeof(t_wbParse_Register_Ret));
				if( pRet )
				{
					memset(pRet ,0 ,sizeof(t_wbParse_Register_Ret));
					iOutCount = 1;
					GET_LONG_TO_STR( JSON,pObjRoot,"uid",pRet->uid_,WBPARSER_USE_LEN(id) );
				}
				wbParserNS::Parse_free_JSON(pObjRoot);
			}
			ppout ? *ppout = pRet : 0 ;
			return (WBPARSE_HANDLE)pRet;
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Account,Register)
		{
			handle ? free(handle) : 0;
		}

		/**����ע��΢���Ľӿ�*/
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Account,Activate)
		{
			t_wbParse_Register_Ret *pRet = 0;
			REQOBJ *pObjRoot = wbParserNS::Parse_data_JSON(sc);
			if( pObjRoot )
			{
				pRet = (t_wbParse_Register_Ret*)malloc(sizeof(t_wbParse_Register_Ret));
				if( pRet )
				{
					memset(pRet ,0 , sizeof(t_wbParse_Register_Ret));
					iOutCount = 1;
					GET_LONG_TO_STR( JSON,pObjRoot,"uid",pRet->uid_,WBPARSER_USE_LEN(id) );
				}
				wbParserNS::Parse_free_JSON(pObjRoot);
			}
			ppout ? *ppout = pRet : 0 ;
			return (WBPARSE_HANDLE)pRet;
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS(Account,Activate)
		{
			handle ? free(handle) : 0;
		}


		////////////////////////////////////////////////////////////////////////////////////////////////
		// �ղؽӿ�


		/** ��ȡ��ǰ�û����ղ��б� */
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

		/** ����ղ� */
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

		/** ɾ���ղ���Ϣ */
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

		/** ����ɾ���ղص�΢����Ϣ */
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
		// ��¼/OAuth�ӿ�
		//

		/** ��ȡδ��Ȩ��Request Token */
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
			struct t_wbParse_Oauth* oa = (struct t_wbParse_Oauth*)handle;
			if( oa){
				free(oa);
			}
		}

		/** �����û���ȨToken */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(Oauth,Oauthorize)
		{
			iOutCount = 0;
			struct t_wbParse_Oauth* oa = (struct t_wbParse_Oauth*)malloc(sizeof(struct t_wbParse_Oauth));
			if( oa )
			{
				iOutCount = 1;
				memset(oa , 0 , sizeof(struct t_wbParse_Oauth) );
#ifdef _USE_JSON_PARSER
				wbParserNS::REQOBJ *pRoot = wbParserNS::Parse_data_JSON(sc);
				if( pRoot )
				{
					wbParserNS::GetCHAR_Key_JSON("oauth_verifier", pRoot , oa->oauth_verifier , 64);
					wbParserNS::Parse_free_JSON(pRoot);
				}
				else
				{
					free(oa);
					oa = 0;
					iOutCount = 0;
				}
#elif _USE_XML_PARSER
				wbParserNS::REQOBJ *pRoot = wbParserNS::Parse_data_XML(sc);
				if( pRoot )
				{
					wbParserNS::GetCHAR_Key_XML("oauth_verifier", pRoot , oa->oauth_verifier , 64);
					wbParserNS::Parse_free_XML(pRoot);
				}
				else
				{
					free(oa);
					oa = 0;
					iOutCount = 0;
				}
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

		/**��ȡ��Ȩ����Access Token*/
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
		// ΢������ :Search API�������˺��������߿��ţ���������ϵ @΢������ƽ̨
		//

		/** ����΢�� */
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

		/** �����û� */
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
		// �ȵ���Ϣ :HOT API�������˺��������߿��ţ���������ϵ @΢������ƽ̨,by welbon,2011-01-07
		//
		/** ��ȡ�ȵ����۵�΢�������� */
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

		/** ��ȡ�ȵ����۵�΢�������� */
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

		/** ��ȡ�ȵ�ת����΢�������� */
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

		/** ��ȡ�ȵ�ת����΢�������� */
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

#ifdef _USE_GET_SHORTURL_BATCH

		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS( MEDIA,SHORTURL )
		{
			CParseMediaShortMultiplT* pParseObj = new CONSTRUCT_MEDIA_SHORTURL_MULTIPL(sc,len,NULL);
			pParseObj->ParseBody();
			iOutCount = pParseObj->GetCounts();
			ppout ? *ppout = pParseObj->get() : NULL;
			return ((WBPARSE_HANDLE)pParseObj);
		}
		WBPARSER_API void WBFREE_FUNC_IMPLEMENTS( MEDIA,SHORTURL )
		{
			handle ? (delete ((CParseMediaShortMultiplT*)handle)) : 0;
		}
#endif //#ifdef _USE_GET_SHORTURL_BATCH

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// һ�����

		/** ������� */
		WBPARSER_API WBPARSE_HANDLE WBPARSE_FUNC_IMPLEMENTS(WEIBO_ERR,ITEM)
		{
			struct t_wbParse_Error* pError = 0;
			iOutCount = 0;

#ifdef _USE_JSON_PARSER

			REQOBJ *pObjRoot = Parse_data_JSON(sc);
			if( pObjRoot )
			{
				pError = (struct t_wbParse_Error*)wbParserNS::wbParse_Malloc_Error(1);
				if( !pError )
				{
					ppout ? *ppout = pError : NULL;
					Parse_free_JSON(pObjRoot);
					return 0;
				}
				iOutCount = 1;
				Json::Value *jRoot = (Json::Value*)(pObjRoot);

				//error code
				Json::Value &jErrorCode = (*jRoot)["error_code"] ; 
				if( jErrorCode.isString()){
					strncpy( pError->error_code,jErrorCode.asString().c_str(),jErrorCode.asString().length());
				}
				//error description
				Json::Value &jError = (*jRoot)["error"] ; 
				if( jError.isString() )
				{
					std::string strError = jError.asString();
					if( !strError.empty()) 
					{
						const char *cErrorDesc =  strchr(strError.c_str(),':' );
						*( (char*) cErrorDesc) = '\0';
						pError->error_sub_code = atoi(strError.c_str());

						const char *c = cErrorDesc + 1;
						if( c && *c != '\0'){
							strncpy( pError->error,c,255 );
						}
					}
				}
				//request
				Json::Value &jRequest = (*jRoot)["request"] ; 
				if( jRequest.isString() ){
					strncpy( pError->request,jRequest.asString().c_str(),jRequest.asString().length());
				}
				Parse_free_JSON(pObjRoot);
			}
#endif //0
			
			//CURLDecodeA c_url(sc , len );
			//CUTF82C	c_err(c_url.c_str() );
			//wb_parse_error( c_err.c_str() , pErr->request , pErr->error , pErr->error_code );
			//ppout ? *ppout = pErr : NULL;
			//pErr->error_sub_code = atoi( pErr->error);
			ppout ? *ppout = pError : NULL;
			return (WBPARSE_HANDLE)pError;
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
				// json ����
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
			{// �ɹ���
				strncpy(_cookie->uid , uid.c_str() , 128);
				strncpy(_cookie->tgt , tgt.c_str() , 256 );
				strncpy(_cookie->publickey , publickey.c_str() , 512 );
				strncpy(_cookie->keyversion, keyversion.c_str(),64);
				strncpy(_cookie->ticket , ticket.c_str() , 256 );
				_cookie->lservertime = lservertime;
				_cookie->llocaltime  = GetTickCount();
			}
			else
			{// ʧ�ܵ�
				_cookie->error = error;
			}
		}

		/** ����cookie,����ָ���Ĵ� */
		WBPARSER_API int Parse_CookieStr(const char *sc,char *token,char *secret)
		{
			return wb_parse_cookie(sc,token,secret);
		}
	}



#ifdef __cplusplus
}
#endif //__cplusplus
