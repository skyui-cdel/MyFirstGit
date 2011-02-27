#pragma once
#include <string>
#include "wbParseMacro.h"

using namespace wbParserNS;


#ifdef _USE_JSON_PARSER

/** 
* @brief 解析模块模板实现
*
* @author welbon
*
* @date 2010-12-27
*/
template< typename T>
class CParseGenericT
{
public:
	typedef void*(*wbParseMallocT)(int size);
	typedef void (*wbParseFreeT)(void *pvoid,int size);
	typedef void (*wbParseDataT)(wbParserNS::REQOBJ *obj,T* p);

	CParseGenericT(const char* sc,const int len,wbParseMallocT malloT,wbParseFreeT freeT,wbParseDataT parseT)
	{
		source_  = sc;
		mallocT_ = malloT;
		freeT_   = freeT;
		parseT_  = parseT;
		object_  = 0;
		counts_  = 0;
	}

	virtual ~CParseGenericT() 
	{
		if( freeT_&&object_)
			freeT_(object_,counts_);
	}

	T* get()const {
		return object_;
	}
	int GetCounts() const {
		return counts_;
	}
protected:
	int counts_;
	T *object_;  ///< 解析的目标
	std::string    source_; ///< 原始串
	wbParseMallocT mallocT_;
	wbParseFreeT   freeT_;
	wbParseDataT   parseT_;
};

/**
* @brief 多个处理的模板
*
* @author welbon
*
* @date 2010-12-29
*/
template<typename T>
class CParseGenericMultiplT : public CParseGenericT<T>
{
public:
	CParseGenericMultiplT(const char* sc,const int len,wbParseMallocT malloT,wbParseFreeT freeT,wbParseDataT parseT)
		:CParseGenericT(sc,len,malloT,freeT,parseT)
	{
		
	}


	virtual void ParseBody()
	{
		if( !parseT_)
			return ;

		int idx  = 0 ;
		wbParserNS::REQOBJ* pRoot = NULL;
		PARSE_DATA(JSON,source_.c_str(),pRoot);
		//
		GET_OBJECT_SIZE(JSON,pRoot,counts_);
		if( counts_) 
		{
			object_ = (T*)mallocT_(counts_);
			if(object_) {
				wbParserNS::REQOBJ *pChild = wbParserNS::GetObject_Idx_JSON(idx,pRoot);
				while( pChild ) {
					parseT_(pChild,object_ + idx);
					OnParseObject(object_ + idx);
					++idx;
					pChild = wbParserNS::GetObject_Idx_JSON(idx,pRoot);
				}
			}
		}
		wbParserNS::Parse_free_JSON(pRoot);	
	}

	// 事件回调
	virtual void OnParseObject(T* pObject){
	}
};


/**
* @brief 单条处理的模板
*
* @author welbon
*
* @date 2010-12-29
*/
template<typename T>
class CParseGenericSingleT : public CParseGenericT<T>
{
public:
	CParseGenericSingleT(const char* sc,const int len, wbParseMallocT malloT,wbParseFreeT freeT,wbParseDataT parseT)
		:CParseGenericT(sc,len,malloT,freeT,parseT)
	{
	}

	//virtual ~CParseGenericSingleT() 
	//{
	//	if( freeT_ && object_ )
	//		freeT_(object_,1);
	//}

	virtual void ParseBody() 
	{
		if( !parseT_)
			return ;

		wbParserNS::REQOBJ* pRoot = wbParserNS::Parse_data_JSON(source_.c_str());
		if( !pRoot ){
			return ;
		}
		counts_ = 1;
		object_ = (T*)mallocT_(1);
		parseT_(pRoot,object_);
		OnParseObject(object_);
		wbParserNS::Parse_free_JSON(pRoot);
	}

	// 事件回调
	virtual void OnParseObject(T* pObject){
	}
};


/**
* @brief LIST列表处理模板
*
* @author welbon
*
* @date 2010-12-29
*/
template<typename T>
class CParseGenericByCursorT : public CParseGenericT<T>
{
public:
	CParseGenericByCursorT(const char* sc,const int len,wbParseMallocT malloT,wbParseFreeT freeT,wbParseDataT parseT)
		:CParseGenericT(sc,len,malloT,freeT,parseT)
	{
		pcur_   = NULL;
	}

	virtual ~CParseGenericByCursorT()
	{
		// 由外面去删除，有点特别
		//if( pcur_ )
			//wbParserNS::wbParse_Free_Cursor(pcur_,1);
	}

	virtual void ParseBody(const char *key) 
	{
		wbParserNS::REQOBJ* pRQRoot  = wbParserNS::Parse_data_JSON(source_.c_str());
		if( !pRQRoot )
		{
			return ;
		}
		wbParserNS::REQOBJ* pRQChild = wbParserNS::GetObject_Key_JSON(key,pRQRoot);
		//
		if( !pRQChild){
			pRQChild = pRQRoot;
		}
		wbParserNS::REQOBJ* pCursorNext = wbParserNS::GetObject_Key_JSON("next_cursor",pRQRoot);
		wbParserNS::REQOBJ* pCursorPrev = wbParserNS::GetObject_Key_JSON("previous_cursor",pRQRoot);

		counts_ = wbParserNS::GetObject_JSON_SIZE(pRQChild);
		if(counts_)
		{
			object_ = (T*)mallocT_(counts_);
			if(object_){
				OnParseREQ(pRQChild);
			}
		}
		if( pCursorPrev || pCursorNext ) 
		{
			pcur_ = (struct t_wbParse_Cursor*)wbParserNS::wbParse_Malloc_Cursor(1);
			if( pcur_ ){
				pcur_->prevcur = wbParserNS::GetLong_JSON(pCursorPrev);
				pcur_->nextcur = wbParserNS::GetLong_JSON(pCursorNext);
			}
		}
		wbParserNS::Parse_free_JSON(pRQRoot);
	}

	t_wbParse_Cursor* GetCursor(){
		return pcur_;
	}

	virtual void OnParseREQ(wbParserNS::REQOBJ *reqObj)
	{
		int  idx = 0;
		wbParserNS::REQOBJ *pChild = wbParserNS::GetObject_Idx_JSON(idx,reqObj);
		while( pChild ) 
		{
			T *tmp = object_ + idx;
			parseT_(pChild,tmp );
			OnParseObject(tmp);
			++ idx;
			pChild = wbParserNS::GetObject_Idx_JSON(idx,reqObj);
		}
	}

	// 事件回调
	virtual void OnParseObject(T* pObject){
	}

protected:
	t_wbParse_Cursor* pcur_;
};

#elif _USE_XML_PARSER


/** 
* @brief 解析模块模板实现
*
* @author welbon
*
* @date 2010-12-27
*/
template< typename T>
class CParseGenericT
{
public:
	typedef void*(*wbParseMallocT)(int size);
	typedef void (*wbParseFreeT)(void *pvoid,int size);
	typedef void (*wbParseDataT)(wbParserNS::REQOBJ *obj,T* p);

	CParseGenericT(const char* sc,const int len,wbParseMallocT malloT,wbParseFreeT freeT,wbParseDataT parseT)
	{
		source_  = sc;
		mallocT_ = malloT;
		freeT_   = freeT;
		parseT_  = parseT;
		object_  = NULL;
	}


	T* get()const {
		return object_;
	}

protected:
	T *object_;  ///< 解析的目标
	std::string    source_; ///< 原始串
	wbParseMallocT mallocT_;
	wbParseFreeT   freeT_;
	wbParseDataT   parseT_;
};

/**
* @brief 多个处理的模板
*
* @author welbon
*
* @date 2010-12-29
*/
template<typename T>
class CParseGenericMultiplT : public CParseGenericT<T>
{
public:
	CParseGenericMultiplT(const char* sc,const int len,wbParseMallocT malloT,wbParseFreeT freeT,wbParseDataT parseT)
		:CParseGenericT(sc,len,malloT,freeT,parseT)
	{
		counts_     = 0;
	}

	virtual ~CParseGenericMultiplT() 
	{
		if( freeT_)
			freeT_(object_,counts_);
	}

	virtual void ParseBody()
	{
		if( !parseT_)
			return ;

		int idx  = 0 ;
		wbParserNS::REQOBJ* pRoot = NULL;
		PARSE_DATA(XML,source_.c_str(),pRoot);
		//
		GET_OBJECT_SIZE(XML,pRoot,counts_);
		if( counts_) 
		{
			object_ = (T*)mallocT_(counts_);
			if(object_) {
				wbParserNS::REQOBJ *pChild = wbParserNS::GetObject_Idx_XML(idx,pRoot);
				while( pChild ) {
					parseT_(pChild,object_ + idx);
					OnParseObject(object_ + idx);
					++idx;
					pChild = wbParserNS::GetObject_Idx_XML(idx,pRoot);
				}
			}
		}
		wbParserNS::Parse_free_XML(pRoot);	
	}

	int GetCounts() const {
		return counts_;
	}

	// 事件回调
	virtual void OnParseObject(T* pObject){
	}

protected:
	int counts_; ///< 解析目标的个数
};


/**
* @brief 单条处理的模板
*
* @author welbon
*
* @date 2010-12-29
*/
template<typename T>
class CParseGenericSingleT : public CParseGenericT<T>
{
public:
	CParseGenericSingleT(const char* sc,const int len, wbParseMallocT malloT,wbParseFreeT freeT,wbParseDataT parseT)
		:CParseGenericT(sc,len,malloT,freeT,parseT)
	{
	}

	virtual ~CParseGenericSingleT() 
	{
		if( freeT_)
			freeT_(object_,1);
	}

	virtual void ParseBody() 
	{
		if( !parseT_)
			return ;

		wbParserNS::REQOBJ* pRoot = wbParserNS::Parse_data_XML(source_.c_str());
		if( !pRoot ){
			return ;
		}
		//
		object_ = (T*)mallocT_(1);
		parseT_(pRoot,object_);
		OnParseObject(object_);
		wbParserNS::Parse_free_XML(pRoot);
	}

	// 事件回调
	virtual void OnParseObject(T* pObject){
	}
};


/**
* @brief LIST列表处理模板
*
* @author welbon
*
* @date 2010-12-29
*/
template<typename T>
class CParseGenericByCursorT : public CParseGenericT<T>
{
public:
	CParseGenericByCursorT(const char* sc,const int len,wbParseMallocT malloT,wbParseFreeT freeT,wbParseDataT parseT)
		:CParseGenericT(sc,len,malloT,freeT,parseT)
	{
		counts_ = 0;
		pcur_   = NULL;
	}

	virtual ~CParseGenericByCursorT()
	{
		if( pcur_ )
			wbParserNS::wbParse_Free_Cursor(pcur_,1);

		if( freeT_)
			freeT_(object_,counts_);
	}

	virtual void ParseBody(const char *key) 
	{
		wbParserNS::REQOBJ* pRQRoot  = wbParserNS::Parse_data_XML(source_.c_str());
		wbParserNS::REQOBJ* pRQChild = wbParserNS::GetObject_Key_XML(key,pRQRoot);
		//
		if( !pRQChild){
			pRQChild = pRQRoot;
		}
		wbParserNS::REQOBJ* pCursorNext = wbParserNS::GetObject_Key_XML("next_cursor",pRQRoot);
		wbParserNS::REQOBJ* pCursorPrev = wbParserNS::GetObject_Key_XML("previous_cursor",pRQRoot);

		counts_ = wbParserNS::GetObject_XML_SIZE(pRQChild);
		if(counts_)
		{
			object_ = (T*)mallocT_(counts_);
			if(object_)
			{
				OnParseREQ(pRQChild);
				if( pCursorPrev || pCursorNext )
				{
					pcur_ = (struct t_wbParse_Cursor*)wbParserNS::wbParse_Malloc_Cursor(1);
					if( pcur_ )
					{
						pcur_->prevcur = wbParserNS::GetLong_XML(pCursorPrev);
						pcur_->nextcur = wbParserNS::GetLong_XML(pCursorNext);
					}
				}
			}
		}

		if( pCursorPrev || pCursorNext ) 
		{
			pcur_ = (struct t_wbParse_Cursor*)wbParserNS::wbParse_Malloc_Cursor(1);
			if( pcur_ )
			{
				pcur_->prevcur = wbParserNS::GetLong_XML(pCursorPrev);
				pcur_->nextcur = wbParserNS::GetLong_XML(pCursorNext);
			}
		}
		wbParserNS::Parse_free_XML(pRQRoot);
	}

	t_wbParse_Cursor* GetCursor(){
		return pcur_;
	}

	int GetCounts() const {
		return counts_;
	}

	virtual void OnParseREQ(wbParserNS::REQOBJ *reqObj)
	{
		int  idx = 0;
		wbParserNS::REQOBJ *pChild = wbParserNS::GetObject_Idx_XML(idx,reqObj);
		while( pChild ) 
		{
			parseT_(reqObj,object_ + idx);
			OnParseObject(object_ + idx);
			++ idx;
			pChild = wbParserNS::GetObject_Idx_XML(idx,reqObj);
		}
	}

	// 事件回调
	virtual void OnParseObject(T* pObject){
	}

protected:
	int               counts_;
	t_wbParse_Cursor* pcur_;
};


#endif //