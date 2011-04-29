#include "stdafx.h"
#include "WBParser.h"
#include "strconv.h"
#include <assert.h>
#include "config.h"


#if (defined(WIN32) || defined(_WIN32_WCE) )
#	if defined(_USE_JSON_PARSER)
#	include <Json/reader.h>
#		if defined(WEIBO_VC80)
#		pragma comment(lib,"json_VC80.lib")
#		elif defined(WEIBO_VC90)
#		pragma comment(lib,"json_VC90.lib")
#		endif
#	endif //_USE_JSON_PARSER

#	if defined(_USE_XML_PARSER)
#	include <tinyxml/tinyxml.h>
#	pragma comment(lib , "tinyxml_VC90.lib")
#	endif //_USE_XML_PARSER

#else
#	define IsBadReadPtr(p,cb) p
#	define ZeroMemory(out,len) memset((void*)out,0,len)
#	define strncpy_s( out,len,txt,txtlen ) strcpy(out, txt)
#endif //WIN32 || WINCE


#ifdef  __cplusplus
extern "C" {
#endif

	namespace wbParserNS
	{

#if defined(_USE_JSON_PARSER)
		/** 释放 */
		WBPARSER_API void Parse_free_JSON(REQOBJ *obj)
		{
			Json::Value *jvalue = (Json::Value*)obj;
			if( !jvalue || IsBadReadPtr((jvalue),sizeof(Json::Value)) )
				return ;
			delete jvalue;
		}

		/** 解析XML,UTF8 */
		WBPARSER_API REQOBJ* Parse_data_JSON(const char* data,char *err_out/*= 0*/,const int len/* = 0*/)
		{
			if( !data || *data == 0 )
				return NULL;

			char* outstr = 0;

#if defined(WIN32) || defined(_WIN32_WCE)
			if( 0 == lo_Utf82C(&outstr , data ) )
				return NULL;
#endif //WIN32||WINCE

			Json::Reader reader;
			Json::Value  *pRoot = new Json::Value();
			if( !reader.parse(outstr ,(*pRoot),false) )
			{
				//
				printf(reader.getFormatedErrorMessages().c_str());
				if( err_out){
					strncpy(err_out,reader.getFormatedErrorMessages().c_str(),len);
				}
			//	assert(0);
				delete pRoot;
				pRoot = NULL;
			}
			free( outstr );
			return (REQOBJ*)pRoot;
		}

		/** 从请求对象中获取值 */

		WBPARSER_API REQOBJ *GetObject_Key_JSON(const char *key ,REQOBJ *obj)
		{
			Json::Value *jvalue = (Json::Value*)obj;
			if( !jvalue || 
				IsBadReadPtr((jvalue),sizeof(Json::Value))||
				jvalue->isNull()  ){
					return 0;
			}
			if( !jvalue->isObject() )
				return NULL;

			Json::Value *jChildVal = (&(*jvalue)[key]);
			if( !jChildVal->isNull() )
			{
				return reinterpret_cast<REQOBJ*>(jChildVal);
			}
			return NULL;
		}

		WBPARSER_API int GetObject_JSON_SIZE(REQOBJ * obj)
		{
			Json::Value *jvalue = (Json::Value*)obj;
			if( !jvalue || 
				IsBadReadPtr((jvalue),sizeof(Json::Value))||
				jvalue->isNull()  ){
					return 0;
			}
			return jvalue->size();
		}

		WBPARSER_API REQOBJ *GetObject_Idx_JSON(const int idx ,REQOBJ *&obj )
		{
			Json::Value *jvalue = (Json::Value*)obj;
			if( !jvalue || 
				IsBadReadPtr((jvalue),sizeof(Json::Value))||
				jvalue->isNull()  ){
					return 0;
			}

			if( !jvalue->isValidIndex(idx) )
				return NULL ;

			Json::Value *jChildVal = (&(*jvalue)[idx]);
			if( jChildVal->isObject() || jChildVal->isArray() ){
				return reinterpret_cast<REQOBJ*>(jChildVal);
			}
			return NULL;
		}

		// long
		WBPARSER_API long long GetLONG_Key_JSON(const char *key,REQOBJ *obj)
		{
			Json::Value *jvalue = (Json::Value*)obj;
			if( (!jvalue || IsBadReadPtr((jvalue),sizeof(Json::Value)) ) ||
				( jvalue->isNull() || !jvalue->isObject()) ) 
			{
				return 0;
			}

			Json::Value &jChildValue = (*jvalue)[key];

			if( !jChildValue.isNull() )
			{
				//
				if( jChildValue.isInt() ){
					return (long)jChildValue.asInt();
				}
				else if( jChildValue.isUInt() ){
					return  (unsigned long)jChildValue.asUInt();
				}
				else if( jChildValue.isBool() ){
					return  (long)jChildValue.asBool();
				}
				else if( jChildValue.isDouble())
				{
					return (long long)jChildValue.asDouble();
				}
			}
			return 0;
		}

		WBPARSER_API long long GetLONG_Idx_JSON(const int idx ,REQOBJ *obj)
		{
			Json::Value *jvalue = (Json::Value*)obj;
			if( (!jvalue || IsBadReadPtr((jvalue),sizeof(Json::Value)))|| jvalue->isNull() )
			{
				return 0;
			}

			if( !jvalue->isValidIndex(idx) )
				return 0;


			Json::Value &jChildValue = jvalue[idx];

			if( !jChildValue.isNull() )
			{
				//
				if( jChildValue.isInt() ){
					return jChildValue.asInt();
				}
				else if( jChildValue.isUInt() ){
					return jChildValue.asUInt();
				}
				else if( jChildValue.isBool() ){
					return jChildValue.asBool();
				}
				else if( jChildValue.isDouble()){
					return jChildValue.asDouble();
				}
			}
			return 0;
		}




		// string 
		WBPARSER_API void GetCHAR_Key_JSON(const char *key,REQOBJ *obj,char *out,const int len)
		{
			Json::Value *jvalue = (Json::Value*)obj;
			if( (!jvalue || IsBadReadPtr((jvalue),sizeof(Json::Value)) )||
				( jvalue->isNull() || !jvalue->isObject()) ) 
			{
				return ;
			}

			Json::Value &jChildValue = (*jvalue)[key];
			if(jChildValue.isString()) {
				strncpy( out,jChildValue.asCString(),len);
			}
			else {
				if( jChildValue.isNull() ){
					return ;
				}
				long long l = 0;
				if( jChildValue.isInt() ){
					l = jChildValue.asInt();
				}
				else if( jChildValue.isUInt() ){
					l = jChildValue.asUInt();
				}
				else if( jChildValue.isBool() ){
					l = jChildValue.asBool();
				}
				else if ( jChildValue.isDouble()){
					l = jChildValue.asDouble();
				}
				_i64toa_s(l, out, len, 10); 
			}
		}
		WBPARSER_API void GetCHAR_Idx_JSON(const int idx,REQOBJ *obj,char *out,const int len)
		{
			Json::Value *jvalue = (Json::Value*)obj;
			if( (!jvalue || IsBadReadPtr((jvalue),sizeof(Json::Value)) )||
				( jvalue->isNull()) ) 
			{
				return ;
			}

			if( !jvalue->isValidIndex(idx) )
				return ;

			Json::Value &jChildValue = (*jvalue)[idx];
			if(jChildValue.isString() && !jChildValue.isNull() )  {
				strncpy( out,jChildValue.asCString(),len );
			}
			else{
				long long l = 0;
				if( jChildValue.isInt() ){
					l = jChildValue.asInt();
				}
				else if( jChildValue.isUInt() ){
					l = jChildValue.asUInt();
				}
				else if( jChildValue.isBool() ){
					l = jChildValue.asBool();
				}
				else if ( jChildValue.isDouble()){
					l = jChildValue.asDouble();
				}
				_i64toa_s(l, out, len, 10); 
			}
		}
		// string 
		WBPARSER_API const char* GetCHAR_Key_JSON_EX(const char *key,REQOBJ *obj, int& outlen)
		{
			outlen = 0;
			Json::Value *jvalue = (Json::Value*)obj;
			if( (!jvalue || IsBadReadPtr((jvalue),sizeof(Json::Value)) )||
				( jvalue->isNull() || !jvalue->isObject()) ) 
			{
				return "";
			}

			Json::Value &jChildValue = (*jvalue)[key];
			if(jChildValue.isString())  {
				outlen = strlen( jChildValue.asCString() );
				return jChildValue.asCString();
			}
			return "";
		}


		// 将OBJ转成INT 
		WBPARSER_API long long GetLong_JSON(REQOBJ *obj)
		{
			Json::Value *jvalue = (Json::Value*)obj;
			if( (!jvalue || IsBadReadPtr((jvalue),sizeof(Json::Value)) )||
				( jvalue->isNull()) ) 
			{
				return 0;
			}
			Json::Value &value = *jvalue;
			if( value.isInt() ){
				return value.asInt();
			}
			else if( value.isUInt() ){
				return value.asUInt();
			}
			else if( value.isBool() ){
				return value.asBool();
			}
			else if ( value.isDouble())
			{
				return value.asDouble();
			}
			return 0;
		}

#endif 


#if defined(_USE_XML_PARSER)

#ifdef USE_REQOBJ_XML
		struct REQOBJ_XML
		{
			TiXmlElement*  curxml; 
			TiXmlDocument  doc;
		};

#define GET_XML_OBJECT(obj) \
		struct REQOBJ_XML* xmlobj = (struct REQOBJ_XML*)obj;\
		TiXmlElement *xmlElement = xmlobj->curxml;

#define RETURN_XML_OBJECT(xml) \
	xmlobj->curxml = xml; \
	return xmlobj; 

#else

#define GET_XML_OBJECT(obj) \
	TiXmlElement *xmlElement = (TiXmlElement*)(obj);

#define RETURN_XML_OBJECT(xml) \
	return (REQOBJ*)xml;

#endif
		/** 解析XML,data format : UTF8 */
		WBPARSER_API REQOBJ* Parse_data_XML(const char* data)
		{
#ifdef USE_REQOBJ_XML
			struct REQOBJ_XML* xml = new REQOBJ_XML();
			if( !xml )
				return NULL;

			xml->doc.Parse(data,0,TIXML_ENCODING_UTF8);
			if( xml->doc.Error() )
			{
				delete xml;
				return NULL;
			}
			xml->curxml = xml->doc.RootElement();
			return xml;
#else
			TiXmlDocument  doc;
			doc.Parse(data,0,TIXML_ENCODING_UTF8);
			if( doc.Error() )
			{
				return NULL;
			}
			TiXmlElement *pXml = new TiXmlElement( *doc.RootElement() );
			return pXml;
#endif
		}


		/** 释放 */
		WBPARSER_API void Parse_free_XML(REQOBJ *obj)
		{
#ifdef USE_REQOBJ_XML
			if( obj && !IsBadReadPtr(obj,sizeof(REQOBJ_XML)) )
			{
				delete (REQOBJ_XML*)obj;
				obj = 0;
			}
#else
			TiXmlElement *pElement = reinterpret_cast<TiXmlElement*>(obj);
			if( !pElement || IsBadReadPtr(pElement,sizeof(TiXmlElement)) )
				return ;
			delete pElement;
#endif
		}

#ifdef USE_REQOBJ_XML
		// object
		WBPARSER_API REQOBJ *GetChild_XML(const char *key ,REQOBJ *obj)
		{
			TiXmlElement *pElement = reinterpret_cast<TiXmlElement*>(obj);
			if( !pElement || IsBadReadPtr(pElement,sizeof(TiXmlElement)) )
				return NULL;
			return reinterpret_cast<TiXmlElement*>(pElement->FirstChildElement(key));
		}

		WBPARSER_API REQOBJ *GetSibling_XML(const char* key ,REQOBJ *obj)
		{
			TiXmlElement *pElement = reinterpret_cast<TiXmlElement*>(obj);
			if( !pElement || IsBadReadPtr(pElement,sizeof(TiXmlElement)) )
				return NULL;

			return ( key && *key != 0 ) ? (reinterpret_cast<TiXmlElement*>(pElement->NextSiblingElement(key))) :
				(reinterpret_cast<TiXmlElement*>(pElement->NextSiblingElement())) ;
		}

		WBPARSER_API char   *GetText_XML(const char  *key ,REQOBJ *obj)
		{
			TiXmlElement *pElement = reinterpret_cast<TiXmlElement*>(obj);
			if( !pElement || IsBadReadPtr(pElement,sizeof(TiXmlElement)) )
				return NULL;

			return const_cast<char*>(pElement->GetText());
		}

		WBPARSER_API char   *GetAttribute_XML(const char  *key ,REQOBJ *obj)
		{
			TiXmlElement *pElement = reinterpret_cast<TiXmlElement*>(obj);
			if( !pElement || IsBadReadPtr(pElement,sizeof(TiXmlElement)) )
				return NULL;

			TiXmlAttribute* pAttr = pElement->FirstAttribute();
			while( pAttr)
			{
				if( strcmpi(key,pAttr->Name() ) == 0 )		// Name
				{
					return const_cast<char*>(pAttr->Value());
				}
				pAttr = pAttr->Next();
			}
			return NULL;
		}
#else 

		// object
		WBPARSER_API int     GetObject_XML_SIZE(REQOBJ *obj)
		{
			GET_XML_OBJECT(obj);
			if( !xmlElement || IsBadReadPtr((xmlElement),sizeof(TiXmlElement))  )
			{
				return 0;
			}
			//
			TiXmlElement *pChild = xmlElement->FirstChildElement();
			int i = 0;
			while(pChild)
			{
				++ i;
				pChild = pChild->NextSiblingElement();
			}
			return i;
		}

		WBPARSER_API REQOBJ *GetObject_Key_XML(const char *key ,REQOBJ *obj)
		{
			GET_XML_OBJECT(obj);
			if( !xmlElement || IsBadReadPtr((xmlElement),sizeof(TiXmlElement))  )
			{
				return 0;
			}
			TiXmlElement *xChild = xmlElement->FirstChildElement(key);
			RETURN_XML_OBJECT(xChild);
		}

		// object
		WBPARSER_API REQOBJ *GetObject_Idx_XML(const int idx ,REQOBJ *&obj )
		{
			GET_XML_OBJECT(obj);
			if( !xmlElement || IsBadReadPtr((xmlElement),sizeof(TiXmlElement))  )
			{
				return 0;
			}
			TiXmlElement *pChild = xmlElement->FirstChildElement();
			int ii = 0;
			while(pChild && ii < idx )
			{
				pChild = pChild->NextSiblingElement();
				++ ii;
			}
			return pChild;
		}

		// long
		WBPARSER_API long GetLONG_Key_XML(const char *key,REQOBJ *obj)
		{
			GET_XML_OBJECT(obj);
			if( !xmlElement || IsBadReadPtr((xmlElement),sizeof(TiXmlElement))  )
			{
				return 0;
			}
			TiXmlElement *pChild = xmlElement->FirstChildElement(key);
			if( pChild ){
				return atol( pChild->GetText() );
			}
			return 0;
		}
		WBPARSER_API long GetLONG_Idx_XML(const int idx ,REQOBJ *obj)
		{
			GET_XML_OBJECT(obj);
			if( !xmlElement || IsBadReadPtr((xmlElement),sizeof(TiXmlElement))  )
			{
				return 0;
			}
			TiXmlElement *pChild = xmlElement->FirstChildElement();
			int ii = 0;
			while(pChild && ii < idx )
			{
				pChild = pChild->NextSiblingElement();
				++ ii;
			}
			if( pChild ){
				return atol( pChild->GetText() );
			}
			return 0;
		}

		// string 
		WBPARSER_API void GetCHAR_Key_XML(const char *key,REQOBJ *obj,char *out,const int len)
		{
			GET_XML_OBJECT(obj);
			if( !xmlElement || IsBadReadPtr((xmlElement),sizeof(TiXmlElement))  )
			{
				return ;
			}

			if( out && *out != 0 )
				ZeroMemory(out,sizeof(char) * len);

			TiXmlElement *pChild = xmlElement->FirstChildElement(key);
			if( pChild && pChild->GetText() )
			{
				char* outstr = 0;

#if defined(WIN32) || defined(_WIN32_WCE)
				if( 0 == lo_Utf82C(&outstr,pChild->GetText()) )
					return ;
#endif //WIN32||_WIN32_WCE

				strncpy(out,outstr,len);
				free(outstr);
			}
		}
		// string 
		WBPARSER_API const char* GetCHAR_Key_XML_EX(const char *key,REQOBJ *obj, int& outlen)
		{
			GET_XML_OBJECT(obj);
			if( !xmlElement || IsBadReadPtr((xmlElement),sizeof(TiXmlElement))  ){
				return NULL;
			}
			TiXmlElement *pChild = xmlElement->FirstChildElement(key);
			if( pChild && pChild->GetText() )  {
				outlen = strlen(pChild->GetText());
				return pChild->GetText();
			}
			return NULL;
		}

		WBPARSER_API void GetCHAR_Idx_XML(const int idx,REQOBJ *obj,char *out,const int len)
		{
			GET_XML_OBJECT(obj);
			if( !xmlElement || IsBadReadPtr((xmlElement),sizeof(TiXmlElement))  )
			{
				return ;
			}

			if( out && *out != 0 )
				ZeroMemory(out,sizeof(char) * len);

			//
			TiXmlElement *pChild = xmlElement->FirstChildElement();
			int ii = 0;
			while(pChild && ii < idx )
			{
				pChild = pChild->NextSiblingElement();
				++ ii;
			}

			//
			if( pChild && pChild->GetText() )
			{
				char* outstr = 0;
#if defined(WIN32) || defined(_WIN32_WCE)
				if( 0 == lo_Utf82C(&outstr,pChild->GetText()) )
					return ;
#endif //WIN32||WINCE
				strncpy(out,outstr,len);
				free(outstr);
			}
		}

		// 将OBJ转成INT 
		WBPARSER_API long long GetLong_XML(REQOBJ *obj)
		{
			GET_XML_OBJECT(obj);
			if( !xmlElement || IsBadReadPtr((xmlElement),sizeof(TiXmlElement))) {
				return 0;
			}
			return atol(xmlElement->GetText());
		}

		WBPARSER_API REQOBJ *GetSibling_XML(const char* key ,REQOBJ *obj)
		{
			GET_XML_OBJECT(obj);
			if( !xmlElement || IsBadReadPtr(xmlElement,sizeof(TiXmlElement)) )
				return NULL;

			TiXmlElement* next = NULL;
			if( key && *key != 0 ) 
			{
				next = xmlElement->NextSiblingElement(key);
			}
			else
			{
				next = xmlElement->NextSiblingElement();
			}
			RETURN_XML_OBJECT(next);
		}
#endif //

#endif // #if defined(_USE_XML_PARSER)

	}
#ifdef  __cplusplus
}
#endif
