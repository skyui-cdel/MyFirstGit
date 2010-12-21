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
* 此库可以由读者用于学习之用,不能用于商业用途
* 如果用在商业用途,一块法律追究自己承担!
*
* 作者： loach.h ( 胡秋云)
*
* 时间： 2009-09-21
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
