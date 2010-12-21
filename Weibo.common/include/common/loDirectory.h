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
* �˿�����ɶ�������ѧϰ֮��,����������ҵ��;
* ���������ҵ��;,һ�鷨��׷���Լ��е�!
*
* ���ߣ� loach.h ( ������)
*
* ʱ�䣺 2009-08-20
*/
#include <common/loCommon.h>

DEFINE_NAMESPACE(locom)

BEGIN_EXTERN_C



/** ·�� */
loCOMMON_API void gloGetDirectoryA(HMODULE_lo handle ,char* pszPath , int& len);
loCOMMON_API void gloGetDirectoryW(HMODULE_lo handle ,wchar_t* pszPath , int& len);

/** ����ϵͳ���ҵ��ĵ�Ŀ¼���� "\\") */
loCOMMON_API const loTCHAR* gloGetSystemDocumentPath(void);

/** ����ϵͳ��APPDATAĿ¼���� "\\") */
loCOMMON_API const loTCHAR* gloGetSystemAppDataPath(void);

/** Ӧ�ó����Ŀ¼���� "\\") */
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
