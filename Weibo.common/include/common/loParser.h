#ifndef __loPARSER_H__
#define __loPARSER_H__

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
* ʱ�䣺 2009-09-21
*/

#include <common/loCommon.h>

DEFINE_NAMESPACE(locom)


// declare CParserW_lo, et. al.
#include "parser-template-def-unichar.h"
#include "loparsert.h"
#include "parser-template-undef.h"

  // declare CParserA_lo, et. al.
#include "parser-template-def-char.h"
#include "loparsert.h"
#include "parser-template-undef.h"


#ifdef UNICODE
  typedef CloParserW CloParser;
#else
  typedef CloParserA CloParser;
#endif

END_NAMESPACE(locom)



#endif /* __loPARSER_H__ */
