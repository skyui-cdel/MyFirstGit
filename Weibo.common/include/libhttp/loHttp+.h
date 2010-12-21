#ifndef __lo__HTTP_C_H__
#define __lo__HTTP_C_H__

/**
*
* C�ӿ�
*
* @author loach
*
* @date 2010-06-07
*/

#include <libhttp/loHttpDef.h>

BEGIN_EXTERN_C

namespace lohttp
{
	//--------------------------------------------------------------------------------------------------//
	// http ֧���߳�

	// ����
	loHTTP_API LOHttp* Http_createobject(void);

	// ����һЩ����
	typedef enum 
	{
		LOHTTP_OPT_UNK = 0,
		LOHTTP_OPT_callbackOfBuffer , ///< �ο� lohttpdef.h  fun_httpbuffer_callback
		LOHTTP_OPT_callbackOfProgress ,///< �ο� lohttpdef.h  fun_httpprogress_callback
		LOHTTP_OPT_callbackOfCode ,///< �ο� lohttpdef.h  fun_httpcode_callback
		LOHTTP_OPT_callbackOfRespone,
		LOHTTP_OPT_callbackOfRelease ,///< �ο� lohttpdef.h  fun_httprelease_callback
		LOHTTP_OPT_userdata,
		LOHTTP_OPT_setmethod,
		LOHTTP_OPT_addparam, ///<  const HTTPChar* szName , const HTTPChar* szValue , unsigned long dwParamAttr
		LOHTTP_OPT_clearparam,
		LOHTTP_OPT_addheader,///< const HTTPChar* szName, const HTTPChar* szValue
		LOHTTP_OPT_clearheader,
		LOHTTP_OPT_addbinarydata, ///< const char* szBinaryData , long lLen
		LOHTTP_OPT_clearbinarydata,
		LOHTTP_OPT_setfile, ///<  const HTTPChar* szPath , const HTTPChar* szFile
		LOHTTP_OPT_setproxysrvaddr,///< int proxyType, const HTTPChar* szProxySrv, unsigned short nProxySrvPort
		LOHTTP_OPT_setproxyaccount,///< const HTTPChar* szUserName, const HTTPChar* szPassword
		LOHTTP_OPT_setuseragent,
		LOHTTP_OPT_setform,
		LOHTTP_OPT_LAST,
	}LOHttpoption;

	loHTTP_API int Http_setopt(LOHttp* phttp, LOHttpoption option , ...);
	/**
	* @brief ���������ļ�URL
	* @param szUrl Ҫ���ص��ļ�Url
	* @param szSavePath ���Ϊ�ı���·����������Ŀ¼���߾����ļ�·����
	* @param lpszExt ��չ��
	* @return true��ʾ�ɹ���ʼ�����ع�����false��ʾ����
	*/
	loHTTP_API void Http_seturl(LOHttp* phttp, const HTTPChar* szUrl);

	/*
	* @brief ��ʼ����(�첽����,�ڲ����̣߳�
	* @param method �ο� enHttpMethod
	*/
	loHTTP_API int  Http_start_asyn(LOHttp* phttp);
	loHTTP_API void Http_wait(LOHttp* phttp);

	/** ͬ��ִ�� �������߳�
	*
	* @param method �ο� enHttpMethod
	*/
	loHTTP_API int Http_start(LOHttp* phttp);

	/**
	* @brief ȡ�����ز���
	* @param msTimeout �ȴ��ĳ�ʱʱ��
	* @return 1 ��ʾ�Ѿ��ɹ�ȡ���� 0 ��ʾ����δ��ʼ��-1 ��ʾ�ȴ���ʱ��
	* @remark ������ 1 �� -1 ʱ���������ڳɹ�ȡ���󶼻�
	*/
	loHTTP_API int Http_cancel(LOHttp* phttp ,  unsigned long msTimeout = 1000/*INFINITE*/ );

	// ������󣬿��ܻ�ȡ ��Ӧ��
	loHTTP_API LOHttpResponse* Http_getRespone(LOHttp* phttp);

	// ������󣬷��� ��Ӧ��
	loHTTP_API LOHttpResponse* Http_detachRespone(LOHttp* phttp);

	// ����
	loHTTP_API void Http_destroyobject(LOHttp* phttp);

	//--------------------------------------------------------------------------------------------//
	// LOHttpRespone ,��Ҫ��� LOHttp 

	/** ��ȡhttp�루200��503��401��������*/
	loHTTP_API long Http_resp_code(const LOHttpResponse* resp);

	/** "http ͷ*/
	loHTTP_API bool Http_resp_getheaderlength(const LOHttpResponse* resp, unsigned long& dwHeaderLen );
	loHTTP_API bool Http_resp_readheader(const LOHttpResponse* resp , fun_http_resp_header_callback cb , void* pUsrData );

	/** ��ȡbody */
	loHTTP_API bool Http_resp_getbodylength(const LOHttpResponse* resp , unsigned long& dwBodyLen);
	loHTTP_API bool Http_resp_readbody(const LOHttpResponse* resp , fun_http_resp_body_callback cb , void* pUsrData );

	/** ע��ֻ�� Http_detachRespone ���ʹ�ã�������벻���ĺ�� */
	loHTTP_API void Http_resp_destroy(LOHttpResponse* resp);


	//--------------------------------------------------------------------------------------------//
	// LOHttpHeader

	loHTTP_API LOHttpHeader* Http_header_create(void);

	/** ���� */
	loHTTP_API void Http_header_parse(LOHttpHeader* header , const char* context , unsigned long len);

	/** "Content-Length" �ֶεĳ���
	* @return ���� ��Content-Length��������true
	*/
	loHTTP_API bool Http_header_getcontentlength (const LOHttpHeader* resp,unsigned long & cbContLen);

	/** http header information */
	loHTTP_API int Http_header_count(const LOHttpHeader* header , const HTTPChar* szName);
	loHTTP_API const HTTPChar*	Http_header_name(const LOHttpHeader* header , const HTTPChar* szName, unsigned long nIdx);

	/** ��ȡ������ " 200 OK ��������Ϣ "*/
	loHTTP_API const HTTPChar*	Http_header_state(const LOHttpHeader* header);

	/** ���� */
	loHTTP_API void Http_header_destroy(LOHttpHeader* header );

} // namespace lohttp

END_EXTERN_C

#endif /* __lo__HTTP_C_H__ */