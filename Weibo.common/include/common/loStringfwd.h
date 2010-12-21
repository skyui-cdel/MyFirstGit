#ifndef __loSSTRINGFWD_H__
#define __loSSTRINGFWD_H__

#include <common/loCommon.h>

DEFINE_NAMESPACE(locom)

class CloAString;

class CloWString;

#if defined (_UNICODE)
typedef CloWString  CloTString;
#else
typedef CloAString  CloTString;
#endif

END_NAMESPACE(locom)

#endif  /* __loSSTRINGFWD_H__ */
