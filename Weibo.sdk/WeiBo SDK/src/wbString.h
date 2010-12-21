/**
*  @brief String define.
*  @file  wbString.h
*  @author loach 
*  @Email < loachmr@sina.com >
*
* Copyright (C) 1996-2010 SINA Corporation, All Rights Reserved
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
*/



#ifndef __wb_String_H__
#define __wb_String_H__

//

#if defined(WIN32) || defined(_WIN32)

#if defined(UNICODE) || defined(_UNICODE)

const char* UnicodeToMbcs(const wchar_t *zWide , char** outstr);
const char* UnicodeToUTF8(const wchar_t *zWide , char** outstr );

#endif // unicode
 
const wchar_t* MbcsToUnicode(const char *zinname,wchar_t** outstr);
const char* MbcsToUTF8(const char *zz , char** outstr);

#else // ! WIN32
// 其它平台

#endif

const char* MbcsToUTF8(const char *zz , char** outstr);


#ifndef BUILD_CHARACTER_SET
// 只使用 char字体集

#define HTTP_TCSCPY_S strcpy
#define HTTP_TCSCAT_S strcat
#define HTTP_SPRINTF  sprintf
#define HTTP_STRCHR   strchr

#define _TCSCPY_S     strcpy
#define _TCSCAT_S     strcat
#define _TSPRINTF     sprintf
#define _ITOT         itoa

#define Char_2HTTPChar(S)   S
#define Char_2WBChar(S)     S

#define HTTPChar_2Char(S)   S
#define HTTPChar_2UTF8(S)   MbcsToUTF8(S , &outstr)

#define WBChar_2HTTPChar(S) S
#define WBChar_2Char(S)     S
#define WBChar_2UTF8(S)     MbcsToUTF8(S , &outstr)
#define WBChar_2WChar(S)    MbcsToUnicode(S,&outstrW)
#define WBCHAR_T(S)         S

#else // !BUILD_CHARACTER_SET

// 使用 编译环境的字符集，需要正解的 相应的字符集支持
#if defined(UNICODE) || defined(_UNICODE)

#define HTTP_TCSCPY_S wcscpy
#define HTTP_TCSCAT_S wcscat
#define HTTP_SPRINTF  wprintf
#define HTTP_STRCHR   wcschr

#define _TCSCPY_S     wcscpy
#define _TCSCAT_S     wcscat
#define _TSPRINTF     wprintf
#define _ITOT         _itow

#define Char_2HTTPChar(S)   MbcsToUnicode(S,&outstrW)
#define Char_2WBChar(S)     MbcsToUnicode(S,&outstrW)

#define HTTPChar_2Char(S)   UnicodeToMbcs(S,&outstr)
#define HTTPChar_2UTF8(S)   UnicodeToUTF8(S,&outstr)

#define WBChar_2HTTPChar(S) S
#define WBChar_2Char(S)     UnicodeToMbcs(S,&outstr)
#define WBChar_2UTF8(S)     UnicodeToUTF8(S,&outstr)
#define WBChar_2WChar(S)    S
#define WBCHAR_T(S)         L##S


#else // !UNICODE OR !WIN32

#define HTTP_TCSCPY_S strcpy
#define HTTP_TCSCAT_S strcat
#define HTTP_SPRINTF  sprintf
#define HTTP_STRCHR   strchr

#define _TCSCPY_S     strcpy
#define _TCSCAT_S     strcat
#define _TSPRINTF     sprintf
#define _ITOT         itoa

#define Char_2HTTPChar(S)   S
#define Char_2WBChar(S)     S

#define HTTPChar_2Char(s)   S
#define HTTPChar_2UTF8(s)   MbcsToUTF8(S , &outstr)

#define WBChar_2HTTPChar(S) S
#define WBChar_2Char(S)     S
#define WBChar_2UTF8(S)     MbcsToUTF8(S , &outstr)
#define WBChar_2WChar(S)    MbcsToUnicode(S,&outstrW)
#define WBCHAR_T(S)         S

#endif

#endif // BUILD_CHARACTER_SET


#endif // __wb_String_H__
