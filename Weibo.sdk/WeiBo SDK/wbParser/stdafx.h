// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"


#if defined(WIN32) || defined(_WIN32_WCE)

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <tchar.h>

// ±‡“Î∆˜∞Ê±æ∫≈≈–∂œ
#ifdef _MSC_VER
#	if _MSC_VER <= 1400  // 1400 == VC++ 8.0
#	define WEIBO_VC80
#	elif _MSC_VER <= 1500 // 1500 == VC++ 9.0
#	define WEIBO_VC90
#	endif //_MSC_VER <= 1400 
#endif //_MSC_VER

#endif //WIN32

// TODO: reference additional headers your program requires here
#include <stdio.h>
#include <stdlib.h>
