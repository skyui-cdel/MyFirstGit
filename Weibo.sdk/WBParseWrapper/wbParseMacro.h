#pragma once

//#include "wbParser.h"


// 一些宏定义 
#define PARSE_DATA(type,source,outobj)		outobj  = wbParserNS::Parse_data_##type(source)
#define PARSE_DATA_FREE(type,obj)			wbParserNS::Parse_free_##type(obj)
//
#define	GET_OBJECT_SIZE(type,obj,outsize)	outsize = wbParserNS::GetObject_##type##_SIZE(obj)
//
#define GET_LONG(type,obj,key,out)			out = wbParserNS::GetLONG_Key_##type(key,obj);
#define GET_LONG_IDX(type,obj,idx,out)		out = wbParserNS::GetLONG_Idx_##type(idx,obj);
//
#define GET_BOOL(type,obj,key,out)			out = (wbParserNS::GetLONG_Key_##type( key,obj) == 1 )?(WTRUE):(WFALSE);
//
#define GET_STR(type,obj,key,out,outlen)	wbParserNS::GetCHAR_Key_##type(key,obj,out,outlen)
#define GET_STR_EX(type,obj,key,out,outlen) out = wbParserNS::GetCHAR_Key_##type##_EX(key,obj,outlen)
#define GET_STR_IDX(type,obj,idx,out,outlen) wbParserNS::GetCHAR_Idx_##type(idx,obj,out,outlen)


#if defined(WIN32) || defined(WINCE)
#define GET_LONG_TO_STR(type,obj,key,out,outlen) _ltoa( wbParserNS::GetLONG_Key_##type( key,obj ),out,10 );
#else
#define GET_LONG_TO_STR(type,obj,key,out,outlen) sprintf( out,"%llu",GetLONG_Key_##type( key,obj ) );
#endif //


#define USE_WEIBOPARSE_OBJ_PTR wbParserNS::REQOBJ *pObj = NULL;
#define GET_OBJECT(type,obj,key,out,parse)\
	{\
		pObj = wbParserNS::GetObject_Key_##type(key,obj) ;\
		if( pObj ){ out = parse(pObj);}\
	}
//
#define GET_OBJECT_IDX(type,obj,idx,outobj) outobj = GetObject_Idx_##type(idx,obj)



#ifdef _USE_JSON_PARSER
#	define PARSE_T(func) CWBJsonParser::func
#elif _USE_XML_PARSER
#	define PARSE_T(func) CWBXmlParser::func
#endif