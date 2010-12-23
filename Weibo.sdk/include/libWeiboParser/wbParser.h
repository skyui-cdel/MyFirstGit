#ifndef __WBPARSER_H__
#define __WBPARSER_H__

#ifdef  __cplusplus
extern "C" {
#endif

#if (defined(WIN32) || defined(_WIN32) ) && !defined(WEIBO_PARSER_STATICLIB)

#if defined(BUILDING_LIBWBPARSER)
#define WBPARSER_API  __declspec(dllexport)
#else
#define WBPARSER_API  __declspec(dllimport)
#endif

#else
	// ����ƽ̨�ģ������Ǿ�̬����
#define WBPARSER_API
#endif

	namespace wbParserNS
	{
		/*
		*
		* ������˵Ŀ���ƽ̨�Ļص����ַ�������
		* ���㿪���ߵ���
		*
		* ��Ȩ���� �����й�
		*
		* ���ߣ�welbon
		*
		* ʹ�����̣� 
		* 
		REQOBJ *pObjRoot = Parse_data_JSON(data);
		REQOBJ *pChild   = GetObject_Key_JSON(key,pObjRoot);
		if( pChild ){
		long l = GetLONG_Key_JSON(pChild);
		}
		*
		*
		* ʱ�䣺2010-07-01
		*/
		typedef void REQOBJ;

#if defined(_USE_JSON_PARSER)

		/** ����JSON,data format : UTF8 */
		WBPARSER_API REQOBJ* Parse_data_JSON(const char* data);

		/** �ͷ� */
		WBPARSER_API void Parse_free_JSON(REQOBJ *obj);

		/** ����������л�ȡֵ */

		// object
		WBPARSER_API int     GetObject_JSON_SIZE(REQOBJ *obj);
		WBPARSER_API REQOBJ *GetObject_Key_JSON(const char *key ,REQOBJ *obj);
		WBPARSER_API REQOBJ *GetObject_Idx_JSON(const int idx ,REQOBJ *&obj );
		// long
		WBPARSER_API long long GetLONG_Key_JSON(const char *key,REQOBJ *obj);
		WBPARSER_API long long GetLONG_Idx_JSON(const int idx ,REQOBJ *obj);
		// string 
		WBPARSER_API void GetCHAR_Key_JSON(const char *key,REQOBJ *obj,char *out,const int len);
		WBPARSER_API void GetCHAR_Idx_JSON(const int idx,REQOBJ *obj,char *out,const int len);
		WBPARSER_API const char* GetCHAR_Key_JSON_EX(const char *key,REQOBJ *obj, int& outlen);
		// ��OBJת��INT 
		WBPARSER_API long long GetLong_JSON(REQOBJ *obj);

#endif //_USE_JSON_PARSER

		////////////////////////////////////////////////////////////////////////////////
#if defined(_USE_XML_PARSER)
		/** ����XML,data format : UTF8 */
		WBPARSER_API REQOBJ* Parse_data_XML(const char* data);

		/** �ͷ� */
		WBPARSER_API void Parse_free_XML(REQOBJ *obj);

		// object
		WBPARSER_API REQOBJ *GetObject_Key_XML(const char *key ,REQOBJ *obj);
		WBPARSER_API REQOBJ *GetObject_Idx_XML(const int idx ,REQOBJ *&obj );
		// long
		WBPARSER_API long GetLONG_Key_XML(const char *key,REQOBJ *obj);
		WBPARSER_API long GetLONG_Idx_XML(const int idx ,REQOBJ *obj);
		// string 
		WBPARSER_API void GetCHAR_Key_XML(const char *key,REQOBJ *obj,char *out,const int len);
		WBPARSER_API void GetCHAR_Idx_XML(const int idx,REQOBJ *obj,char *out,const int len);

		/** ��ȡ�ֵܽڵ� */
		WBPARSER_API REQOBJ *GetSibling_XML(const char* key ,REQOBJ *obj);
#endif //_USE_XML_PARSER

	} // namespace

#ifdef  __cplusplus
}
#endif


#endif //#define __WEIBO_PARSER_H__