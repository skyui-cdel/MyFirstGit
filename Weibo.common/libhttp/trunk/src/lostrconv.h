#ifndef __strConv_H__
#define __strConv_H__

#include <string>
/*
** Convert an ansi string to microsoft unicode, based on the
** current codepage settings for file apis.
** 
** Space to hold the returned string is obtained
** from sqliteMalloc.
*/
std::wstring MbcsToUnicode(const char *zinname);


std::string UnicodeToMbcs(const wchar_t *zWide);


std::string UnicodeToUtf8(const wchar_t *zWide);

/*
** Convert multibyte character string to UTF-8.  Space to hold the
** returned string is obtained from sqliteMalloc().
*/
std::string MbcsToUtf8(const char *zinname);


/*
** Other platform not suport "strlwr"
*/
#if !defined(WIN32) && !defined(WINCE)

char* _strlwr(char *a);

#endif //#ifndef WIN32


// Ϊ�˴�atl �ĵ� ���õ� atl��ͳһ����, by loach  2010-06-07
#if (defined(_UNICODE) || defined(UNICODE) )
#define STR_TO_UTF8(s) UnicodeToUtf8(s)

#define CT2A(s)  UnicodeToMbcs(s).c_str()
#define CA2T(s)  MbcsToUnicode(s).c_str()
#else

#define STR_TO_UTF8(s) MbcsToUtf8(s)
#define CT2A(s)  (s)
#define CA2T(s)  (s)

#endif

#define CA2W(s)  MbcsToUnicode(s).c_str()

#endif //__strConv_H__
