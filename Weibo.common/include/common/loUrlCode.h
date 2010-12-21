#ifndef __locom_URLENCODE_H__
#define __locom_URLENCODE_H__

///////////////////////////////////////////////////////
//	***             * * * *      ****        ****    //
//	***  	      *         *     **          **     //
//	***  	     *           *    **          **     //
//	***  	     *           *    **************     //
//	***	     **  *           *    **          **     //
//	***********   *         *     **          **     //
//	***********	    * * * *      ****        ****    // 
///////////////////////////////////////////////////////2009-08-20 @loach
/*
*
* �˿�����ɶ�������ѧϰ֮��,����������ҵ��;
* ���������ҵ��;,һ�鷨��׷���Լ��е�!
*
* ���ߣ� loach.h ( ������)
*
* ʱ�䣺 2009-08-20
*/

#include <common/loCommon.h>

DEFINE_NAMESPACE(locom)

#ifdef __cplusplus
extern "C"{
#endif

loCOMMON_API int gloURLDecodeW(const wchar_t *source, wchar_t *dest);
loCOMMON_API int gloURLEncodeW(const wchar_t *source, wchar_t *dest, unsigned max);

loCOMMON_API int gloURLDecodeA(const char *source, char *dest);
loCOMMON_API int gloURLEncodeA(const char *source, char *dest, unsigned max);


#ifdef __cplusplus
}
#endif


#ifdef _UNICODE

#define gloURLDecode gloURLDecodeW
#define gloURLEncode gloURLEncodeW

#else

#define gloURLDecode gloURLDecodeA
#define gloURLEncode gloURLEncodeA

#endif

END_NAMESPACE(locom)

#endif //__locom_URLENCODE_H__

