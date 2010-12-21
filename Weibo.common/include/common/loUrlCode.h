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
* 此库可以由读者用于学习之用,不能用于商业用途
* 如果用在商业用途,一块法律追究自己承担!
*
* 作者： loach.h ( 胡秋云)
*
* 时间： 2009-08-20
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

