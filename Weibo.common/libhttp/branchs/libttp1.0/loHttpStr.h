
#pragma once

#include <windows.h>			// for generic types, .. etc
#include <wininet.h>			// for the windows internet functions
#include <stdio.h>
#include <stdlib.h>
#include "loHttpDef.h"

#pragma warning (push)
#pragma warning (disable: 4290)	// avoids 'C++ Exception Specification ignored' message
#pragma warning (disable: 4996)	// avoids 'This function or variable may be unsafe' message

/*!
* \brief	The namespace of the lohttp's library
*
* This is the namespace for source codes written by Jo Hyeong-ryeol.
*/
namespace lohttp
{

#ifndef SAFEFREE
#define SAFEFREE(x) if(x){ ::free((void *)x); x = NULL; }
#endif

	class CloHttpStrA
	{
	public:
		typedef char	CharType ;
		typedef LPSTR	PSZ ;
		typedef LPCSTR	PCSZ ;

		static LPCSTR	szEmptyString ;

		static inline BOOL IsAnsi (void) throw ()
		{
			return TRUE ;
		}

		// Wrapper methods for CRT string functions
		static inline size_t StringLen (LPCSTR szStr) throw ()
		{
			if( szStr == NULL )
				return 0;
			return ::strlen (szStr) ;
		}

		static inline LPSTR StringCopy (LPSTR szDest, LPCSTR szSrc) throw ()
		{
			return ::strcpy (szDest, szSrc) ;
		}

		static inline LPSTR StringCat (LPSTR szDest, LPCSTR szSrc) throw ()
		{
			return ::strcat (szDest, szSrc) ;
		}

		static inline LPSTR StringNCopy (LPSTR szDest, LPCSTR szSrc, size_t count) throw ()
		{
			return ::strncpy (szDest, szSrc, count) ;
		}

		static inline int StringNICmp (LPCSTR szStr1, LPCSTR szStr2, size_t count) throw ()
		{
			return ::_strnicmp (szStr1, szStr2, count) ;
		}

		static inline int StringCmp (LPCSTR szStr1, LPCSTR szStr2) throw ()
		{
			return ::strcmp (szStr1, szStr2) ;
		}

		static inline LPCSTR StringChr (LPCSTR szStr, int Chr) throw ()
		{
			return ::strchr (szStr, Chr) ;
		}

		static inline LPCSTR StringRChr (LPCSTR szStr, int Chr) throw ()
		{
			return ::strrchr (szStr, Chr) ;
		}

		static int SNPrintf (LPSTR buffer, size_t count, LPCSTR format, ...) throw ()
		{
			int			cchWritten ;
			va_list		args ;

			va_start (args, format) ;
			cchWritten = ::_vsnprintf (buffer, count, format, args) ;
			va_end (args) ;
			return cchWritten ;
		}

		static PCSTR StringStr (PCSTR szString, PCSTR szSearch) throw ()
		{
			return ::strstr (szString, szSearch) ;
		}

		static unsigned long StringToUL (PCSTR szString, PSTR * pEndPtr, int base) throw ()
		{
			return ::strtoul (szString, pEndPtr, base) ;
		}

		// Conversion methods
		static LPSTR Unicode2Ansi (LPCWSTR szStr, unsigned int CodePage = CP_ACP) throw () ;
		static LPWSTR Ansi2Unicode (LPCSTR szStr, unsigned int CodePage = CP_ACP) throw () ;

		// Methods for Ansi character set
		static BOOL IsDBCSLeadByteEx (unsigned int CodePage, char TestChar) throw ()
		{
			return ::IsDBCSLeadByteEx (CodePage, TestChar) ;
		}
		bool operator () (LPCSTR szKey1, LPCSTR szKey2) const throw ()
		{
			// return true if the two strings are null
			if ( szKey1 == NULL && szKey2 == NULL )
				return true ;

			if ( szKey1 == NULL )
				return true ;

			if ( szKey2 == NULL )
				return false ;

			// case insensitive
			return ::stricmp (szKey1, szKey2) < 0 ;
		}
		
	};

	class CloHttpStrW
	{
	public:
		typedef WCHAR	CharType ;
		typedef LPWSTR	PSZ ;
		typedef LPCWSTR	PCSZ ;

		static LPCWSTR	szEmptyString ;

		static inline BOOL IsAnsi (void) throw ()
		{
			return FALSE ;
		}

		static inline size_t StringLen (LPCWSTR szStr) throw ()
		{
			return ::wcslen (szStr) ;
		}

		static inline LPWSTR StringCopy (LPWSTR szDest, LPCWSTR szSrc) throw ()
		{
			return ::wcscpy (szDest, szSrc) ;
		}

		static inline LPWSTR StringCat (LPWSTR szDest, LPCWSTR szSrc) throw ()
		{
			return ::wcscat (szDest, szSrc) ;
		}

		static inline LPWSTR StringNCopy (LPWSTR szDest, LPCWSTR szSrc, size_t count) throw ()
		{
			return ::wcsncpy (szDest, szSrc, count) ;
		}

		static inline int StringNICmp (LPCWSTR szStr1, LPCWSTR szStr2, size_t count) throw ()
		{
			return ::_wcsnicmp (szStr1, szStr2, count) ;
		}

		static inline int StringCmp (LPCWSTR szStr1, LPCWSTR szStr2) throw ()
		{
			return ::wcscmp (szStr1, szStr2) ;
		}

		static inline LPCWSTR StringChr (LPCWSTR szStr, WCHAR Chr) throw ()
		{
			return ::wcschr (szStr, Chr) ;
		}

		static inline LPCWSTR StringRChr (LPCWSTR szStr, WCHAR Chr) throw ()
		{
			return ::wcsrchr (szStr, Chr) ;
		}

		static int SNPrintf (LPWSTR buffer, size_t count, LPCWSTR format, ...) throw ()
		{
			int			cchWritten ;
			va_list		args ;

			va_start (args, format) ;
			cchWritten = ::_vsnwprintf (buffer, count, format, args) ;
			va_end (args) ;
			return cchWritten ;
		}

		static PCWSTR StringStr (PCWSTR szString, PCWSTR szSearch) throw ()
		{
			return ::wcsstr (szString, szSearch) ;
		}

		static unsigned long StringToUL (PCWSTR szString, PWSTR * pEndPtr, int base) throw ()
		{
			return ::wcstoul (szString, pEndPtr, base) ;
		}

		// Conversion methods
		static LPSTR Unicode2Ansi (LPCWSTR szStr, unsigned int CodePage = CP_ACP) throw () ;
		static LPWSTR Ansi2Unicode (LPCSTR szStr, unsigned int CodePage = CP_ACP) throw () ;

		// Methods for Ansi character set
		static BOOL IsDBCSLeadByteEx (unsigned int /* CodePage */, WCHAR /* TestChar */) throw ()
		{
			return FALSE ;		// always returns FALSE
		}

		// comparison function (used by STL multimap)
		bool operator () (LPCWSTR szKey1, LPCWSTR szKey2) const 	throw ()
		{
			// return true if the two strings are null
			if ( szKey1 == NULL && szKey2 == NULL )
				return true ;

			if ( szKey1 == NULL )
				return true ;

			if ( szKey2 == NULL )
				return false ;

			// case insensitive
			return ::wcsicmp (szKey1, szKey2) < 0 ;
		}
	};
}

#pragma warning (pop)	// restores the default
