// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"
#include <stdio.h>


#if defined(WIN32) || defined(_WIN32_WCE)

//
#include <tchar.h>
#include <Windows.h>

// 编译器版本号判断
#ifdef _MSC_VER
#	if _MSC_VER <= 1400  // 1400 == VC++ 8.0
#	define WEIBO_VC80
#	elif _MSC_VER <= 1500 // 1500 == VC++ 9.0
#	define WEIBO_VC90
#	endif //_MSC_VER <= 1400 
#endif //_MSC_VER

#endif //WIN32



// TODO: 在此处引用程序需要的其他头文件
