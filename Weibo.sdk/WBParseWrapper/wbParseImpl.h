#pragma once

#include "wbParseWrapper.h"
#include "wbParser.h"
#include "wbParseMalloc.h"
#include "wbParseObj.h"
#include "wbParseT.hpp"
#include <assert.h>
#include <string>
#include "wbParseMacro.h"


/** 解析部分扩展信息 */
class CParseExtraInfo
{
public:
	CParseExtraInfo(t_WBParse_Extern_info *pExternInfo)
	{
		externInfo_ = pExternInfo;
	}

	/** 检查短链接 */
	void CheckShortLink(t_wbParse_Status *pStatus)
	{
		if( !pStatus )
			return ;

		if( externInfo_ ){
			//短链接
			wbParserNS::CheckShortLink( pStatus, 0 , &externInfo_->_Media.pMedia,externInfo_->_Media.iMediaSize,externInfo_->_Media.nCmd);
		}
	}

	/** MMI Type */
	void CompareMMId(t_wbParse_Status *pStatus)
	{
		if( !pStatus )
			return ;

		if( externInfo_ && externInfo_->_MMId.iMMIDtype > t_wbParser_MMId::EType_Unknow && externInfo_->_MMId._pMMId)
		{
			MMIdCompare(externInfo_->_MMId._pMMId,(wbParserNS::t_wbParser_MMId::eType)externInfo_->_MMId.iMMIDtype,pStatus->id ,pStatus->created_at );
		}
	}
protected:
	t_WBParse_Extern_info *externInfo_;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
/** 
* @brief 微博解析特化处理
*
* @author welbon
*
* @date 2010-12-27
*/
class CParseStatusesMultipl: public CParseGenericMultiplT<t_wbParse_Status>, public CParseExtraInfo
{
public:
	CParseStatusesMultipl(const char* sc,const int len,t_WBParse_Extern_info *pExternInfo)
		:CParseGenericMultiplT(sc,len,wbParserNS::wbParse_Malloc_Status,wbParserNS::wbParse_Free_Status,PARSE_T(parse_status) )
		,CParseExtraInfo(pExternInfo)
	{
	}
	virtual void OnParseObject(t_wbParse_Status *pStatus) 
	{
		CheckShortLink(pStatus);
		CompareMMId(pStatus);
	}
};
class CParseStatusesSingle: public CParseGenericSingleT<t_wbParse_Status>, public CParseExtraInfo
{
public:
	CParseStatusesSingle(const char* sc,const int len,t_WBParse_Extern_info *pExternInfo)
		:CParseGenericSingleT(sc,len,wbParserNS::wbParse_Malloc_Status,wbParserNS::wbParse_Free_Status,PARSE_T(parse_status) )
		,CParseExtraInfo(pExternInfo)
	{
	}
	virtual void OnParseObject(t_wbParse_Status *pStatus) 
	{
		CheckShortLink(pStatus);
		CompareMMId(pStatus);
	}
};

class CParseStatusesList: public CParseGenericByCursorT<t_wbParse_Status>, public CParseExtraInfo
{
public:
	CParseStatusesList(const char* sc,const int len,t_WBParse_Extern_info *pExternInfo)
		:CParseGenericByCursorT(sc,len,wbParserNS::wbParse_Malloc_Status,wbParserNS::wbParse_Free_Status,PARSE_T(parse_status) )
		,CParseExtraInfo(pExternInfo)
	{
	}
	virtual void OnParseObject(t_wbParse_Status *pStatus) 
	{
		CheckShortLink(pStatus);
		CompareMMId(pStatus);
	}
};


/**
* @brief IDs解析特化处理
*
* @author welbon
*
* @date 2010-12-29
*/
class CParseIDs : public CParseGenericByCursorT<t_wbParse_IDs>
{
public:
	CParseIDs(const char* sc,const int len)
		:CParseGenericByCursorT(sc,len,wbParserNS::wbParse_Malloc_IDs,wbParserNS::wbParse_Free_IDs,NULL)
	{
	}

	virtual void OnParseREQ(wbParserNS::REQOBJ *reqObj) 
	{
		if( 0 == counts_){
			return ;
		}
		for( int i = 0 ; i < counts_ ; i ++ ) 
		{
			t_wbParse_IDs *pIDs = (object_ + i);

#ifdef _USE_XML_PARSER
			GET_LONG_IDX(XML,reqObj,i,pIDs->id);
#else
			GET_LONG_IDX(JSON,reqObj,i,pIDs->id);
#endif //_USE_XML_PARSER
		}
	}
};

///

//
#ifdef _USE_JSON_PARSER 
#	define WBParserImplNS CWBJsonParser
#elif _USE_XML_PARSER
#	define WBParserImplNS CWBXmlParser
#endif //


// Status 
typedef CParseStatusesMultipl	CParseStatusesMultiplT;
typedef CParseStatusesSingle	CParseStatusesSingleT;
typedef CParseStatusesList		CParseStatusesListT;

#define CONSTRUCT_STATUSES_MULTIPL(sc,len,pExternInfo)\
	CParseStatusesMultiplT(sc,len,pExternInfo)

#define CONSTRUCT_STATUSES_SINGLE(sc,len,pExternInfo)\
	CParseStatusesSingleT(sc,len,pExternInfo)

#define CONSTRUCT_STATUSES_LIST(sc,len,pExternInfo)\
	CParseStatusesListT(sc,len,pExternInfo)


// Comment
typedef CParseGenericMultiplT< t_wbParse_Comment >  CParseCommentMultiplT;
typedef CParseGenericSingleT< t_wbParse_Comment >   CParseCommentSingleT;
typedef CParseGenericByCursorT< t_wbParse_Comment > CParseCommentListT;

#define CONSTRUCT_COMMENTS_MULTIPL(sc,len)\
	CParseCommentMultiplT(sc,len,wbParserNS::wbParse_Malloc_Comment,wbParserNS::wbParse_Free_Comment,PARSE_T(parse_comment) )

#define CONSTRUCT_COMMENTS_SINGLE(sc,len)\
	CParseCommentSingleT(sc,len,wbParserNS::wbParse_Malloc_Comment,wbParserNS::wbParse_Free_Comment,PARSE_T(parse_comment) )

#define CONSTRUCT_COMMENTS_LIST(sc,len)\
	CParseCommentListT(sc,len,wbParserNS::wbParse_Malloc_Comment,wbParserNS::wbParse_Free_Comment,PARSE_T(parse_comment) )

// User
typedef CParseGenericMultiplT< t_wbParse_User >  CParseUserMultiplT;
typedef CParseGenericSingleT< t_wbParse_User >   CParseUserSingleT;
typedef CParseGenericByCursorT< t_wbParse_User > CParseUserListT;

#define CONSTRUCT_USER_MULTIPL(sc,len)\
	CParseUserMultiplT(sc,len,wbParserNS::wbParse_Malloc_User,wbParserNS::wbParse_Free_User,PARSE_T(parse_user) )

#define CONSTRUCT_USER_SINGLE(sc,len)\
	CParseUserSingleT(sc,len,wbParserNS::wbParse_Malloc_User,wbParserNS::wbParse_Free_User,PARSE_T(parse_user) )

#define CONSTRUCT_USER_LIST(sc,len)\
	CParseUserListT(sc,len,wbParserNS::wbParse_Malloc_User,wbParserNS::wbParse_Free_User,PARSE_T(parse_user) )


// Counts
typedef CParseGenericMultiplT< t_wbParse_CommentCounts >  CParseCommentCountMultiplT;
typedef CParseGenericSingleT< t_wbParse_CommentCounts >   CParseCommentCountSingleT;
typedef CParseGenericByCursorT< t_wbParse_CommentCounts > CParseCommentCountListT;

#define CONSTRUCT_COMMENTCOUNTS_MULTIPL(sc,len)\
	CParseCommentCountMultiplT(sc,len,wbParserNS::wbParse_Malloc_Commentcount,wbParserNS::wbParse_Free_Commentcount,PARSE_T(parse_commentcounts) )

#define CONSTRUCT_COMMENTCOUNTS_SINGLE(sc,len)\
	CParseCommentCountSingleT(sc,len,wbParserNS::wbParse_Malloc_Commentcount,wbParserNS::wbParse_Free_Commentcount,PARSE_T(parse_commentcounts) )

#define CONSTRUCT_COMMENTCOUNTS_LIST(sc,len)\
	CParseCommentCountListT(sc,len,wbParserNS::wbParse_Malloc_Commentcount,wbParserNS::wbParse_Free_Commentcount,PARSE_T(parse_commentcounts) )

// Counts
typedef CParseGenericMultiplT< t_wbParse_Unread >  CParseUnreadMultiplT;
typedef CParseGenericSingleT< t_wbParse_Unread >   CParseUnreadCountSingleT;
typedef CParseGenericByCursorT< t_wbParse_Unread > CParseUnreadCountListT;

#define CONSTRUCT_UNREAD_MULTIPL(sc,len)\
	CParseUnreadMultiplT(sc,len,wbParserNS::wbParse_Malloc_Unread,wbParserNS::wbParse_Free_Unread,PARSE_T(parse_unread) )

#define CONSTRUCT_UNREAD_SINGLE(sc,len)\
	CParseUnreadCountSingleT(sc,len,wbParserNS::wbParse_Malloc_Unread,wbParserNS::wbParse_Free_Unread,PARSE_T(parse_unread))

#define CONSTRUCT_UNREAD_LIST(sc,len)\
	CParseUnreadCountListT(sc,len,wbParserNS::wbParse_Malloc_Unread,wbParserNS::wbParse_Free_Unread,PARSE_T(parse_unread))


//Direct messages
typedef CParseGenericMultiplT< t_wbParse_DirectMessage >  CParseDirectMessageMultiplT;
typedef CParseGenericSingleT< t_wbParse_DirectMessage >   CParseDirectMessageSingleT;
typedef CParseGenericByCursorT< t_wbParse_DirectMessage > CParseDirectMessageListT;

#define CONSTRUCT_DIRECTMESSAGE_MULTIPL(sc,len)\
	CParseDirectMessageMultiplT(sc,len,wbParserNS::wbParse_Malloc_Directmessage,wbParserNS::wbParse_Free_Directmessage,PARSE_T(parse_directmessage))

#define CONSTRUCT_DIRECTMESSAGE_SINGLE(sc,len)\
	CParseDirectMessageSingleT(sc,len,wbParserNS::wbParse_Malloc_Directmessage,wbParserNS::wbParse_Free_Directmessage,PARSE_T(parse_directmessage))

#define CONSTRUCT_DIRECTMESSAGE_LIST(sc,len)\
	CParseDirectMessageListT(sc,len,wbParserNS::wbParse_Malloc_Directmessage,wbParserNS::wbParse_Free_Directmessage,PARSE_T(parse_directmessage))


//Ship show
typedef CParseGenericMultiplT< t_wbParse_Shipshow >  CParseShipShowMultiplT;
typedef CParseGenericSingleT< t_wbParse_Shipshow >   CParseShipShowSingleT;
typedef CParseGenericByCursorT< t_wbParse_DirectMessage > CParseShipShowListT;

#define CONSTRUCT_FRIENDSHIP_MULTIPL(sc,len)\
	CParseShipShowMultiplT(sc,len,wbParserNS::wbParse_Malloc_Friendship,wbParserNS::wbParse_Free_Friendship,PARSE_T(parse_shipshow))

#define CONSTRUCT_FRIENDSHIP_SINGLE(sc,len)\
	CParseShipShowSingleT(sc,len,wbParserNS::wbParse_Malloc_Friendship,wbParserNS::wbParse_Free_Friendship,PARSE_T(parse_shipshow))

#define CONSTRUCT_FRIENDSHIP_LIST(sc,len)\
	CParseShipShowListT(sc,len,wbParserNS::wbParse_Malloc_Friendship,wbParserNS::wbParse_Free_Friendship,PARSE_T(parse_shipshow))


//Limite
typedef CParseGenericMultiplT<  t_wbParse_LimitStatus >  CParseLimitesMultiplT;
typedef CParseGenericSingleT<   t_wbParse_LimitStatus >  CParseLimitesSingleT;
typedef CParseGenericByCursorT< t_wbParse_LimitStatus >  CParseLimitesListT;

#define CONSTRUCT_LIMITES_MULTIPL(sc,len)\
	CParseLimitesMultiplT(sc,len,wbParserNS::wbParse_Malloc_Limits,wbParserNS::wbParse_Free_Limits,PARSE_T(parse_limite_status))

#define CONSTRUCT_LIMITES_SINGLE(sc,len)\
	CParseLimitesSingleT(sc,len,wbParserNS::wbParse_Malloc_Limits,wbParserNS::wbParse_Free_Limits,PARSE_T(parse_limite_status))

#define CONSTRUCT_LIMITES_LIST(sc,len)\
	CParseLimitesListT(sc,len,wbParserNS::wbParse_Malloc_Limits,wbParserNS::wbParse_Free_Limits,PARSE_T(parse_limite_status))



//Emotion
typedef CParseGenericMultiplT<  t_wbParse_Emotion >  CParseEmotionMultiplT;
typedef CParseGenericSingleT<   t_wbParse_Emotion >  CParseEmotionSingleT;
typedef CParseGenericByCursorT< t_wbParse_Emotion >  CParseEmotionListT;

#define CONSTRUCT_EMOTION_MULTIPL(sc,len)\
	CParseEmotionMultiplT(sc,len,wbParserNS::wbParse_Malloc_Emotion,wbParserNS::wbParse_Free_Emotion,PARSE_T(parse_emotion))

#define CONSTRUCT_EMOTION_SINGLE(sc,len)\
	CParseEmotionSingleT(sc,len,wbParserNS::wbParse_Malloc_Emotion,wbParserNS::wbParse_Free_Emotion,PARSE_T(parse_emotion))

#define CONSTRUCT_EMOTION_LIST(sc,len)\
	CParseEmotionListT(sc,len,wbParserNS::wbParse_Malloc_Emotion,wbParserNS::wbParse_Free_Emotion,PARSE_T(parse_emotion))

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

#if 0
//#define WBPARSE_OBJECT_NAME(name,type) CParse##name##type##T
//
//#define WBPARSE_CONSTRUCT_OBJECT(sc,len,name,type,mallofunc,freefunc,parsefunc)\
//	WBPARSE_OBJECT_NAME(name,type)(sc,len,mallofunc,freefunc,parsefunc )\
//
//#define WBPARSE_DECLARE_TEMPLATE(structtype,name,memfuncname,parsefuncname)\
//	{\
//		typedef CParseGenericMultiplT<  structtype >  WBPARSE_OBJECT_NAME(name,Multipl);\
//		typedef CParseGenericSingleT<   structtype >  WBPARSE_OBJECT_NAME(name,Single);\
//		typedef CParseGenericByCursorT< structtype >  WBPARSE_OBJECT_NAME(name,List);\
//	}\

//#define CONSTRUCT_##name##_MULTIPL(sc,len)\
//	CParse##name##MultiplT(sc,len,wbParserNS::wbParse_Malloc_##memfuncname,wbParserNS::wbParse_Free_##memfuncname,WBParserImplNS::parse_##parsefuncname )\
//#define CONSTRUCT_##name##_SINGLE(sc,len)\
//	CParse##name##SingleT(sc,len,wbParserNS::wbParse_Malloc_##memfuncname,wbParserNS::wbParse_Free_##memfuncname,WBParserImplNS::parse_##parsefuncname )\
//#define CONSTRUCT_##name##_LIST(sc,len)\
//	CParse##name##ListT(sc,len,wbParserNS::wbParse_Malloc_##memfuncname,wbParserNS::wbParse_Free_##memfuncname,WBParserImplNS::parse_##parsefuncname )\

#endif //0


