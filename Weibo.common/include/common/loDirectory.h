#ifndef __locom_DIRECTORY_H__
#define __locom_DIRECTORY_H__

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

BEGIN_EXTERN_C



/** 路径 */
loCOMMON_API void gloGetDirectoryA(HMODULE_lo handle ,char* pszPath , int& len);
loCOMMON_API void gloGetDirectoryW(HMODULE_lo handle ,wchar_t* pszPath , int& len);

/** 操作系统中我的文档目录（带 "\\") */
loCOMMON_API const loTCHAR* gloGetSystemDocumentPath(void);

/** 操作系统中APPDATA目录（带 "\\") */
loCOMMON_API const loTCHAR* gloGetSystemAppDataPath(void);

/** 应用程序的目录（带 "\\") */
loCOMMON_API const loTCHAR* gloGetApplicationPath(void);
/////////////////////////////////////////////////////////////////////////////////////////

END_EXTERN_C

END_NAMESPACE(locom)

#ifndef _UNICODE

#define gloGetDirectory gloGetDirectoryA

#else

#define gloGetDirectory gloGetDirectoryW

#endif 

#endif // __locom_DIRECTORY_H__
