#ifndef __loOS_STRING_H__
#define __loOS_STRING_H__

#include <common/loCommon.h>
#include <string.h>

#pragma warning(disable:4996)

namespace loOS
{
	/** @name Functions from <cstring>
	*
	*  Included are the functions defined in <cstring> and their <cwchar>
	*  equivalents.
	*
	*  @todo To be complete, we should add strcoll, and strxfrm.
	*/
	//@{

	/// Finds characters in a buffer (const void version).
	INLINE_lo
		const void *memchr (const void *s, int c, size_t len);

	/// Finds characters in a buffer (void version).
	INLINE_lo
		void *memchr (void *s, int c, size_t len);

	/// Emulated memchr - Finds a character in a buffer.
	extern loCOMMON_API
		const void *memchr_emulation (const void *s, int c, size_t len);

	/// Compares two buffers.
	INLINE_lo
		int memcmp (const void *t, const void *s, size_t len);

	/// Copies one buffer to another.
	INLINE_lo
		void *memcpy (void *t, const void *s, size_t len);


	/*
	* Version of memcpy where the copy loop is unrolled.
	* On certain platforms this results in better performance.
	* This is determined and set via autoconf.
	*/
	extern loCOMMON_API
		void *fast_memcpy (void *t, const void *s, size_t len);

	/// Moves one buffer to another.
	INLINE_lo
		void *memmove (void *t, const void *s, size_t len);

	/// Fills a buffer with a character value.
	INLINE_lo
		void *memset (void *s, int c, size_t len);

	/// Appends a string to another string (char version).
	INLINE_lo
		char *strcat (char *s, const char *t);


	/// Appends a string to another string (wchar_t version).
	INLINE_lo
		wchar_t *strcat (wchar_t *s, const wchar_t *t);

	/// Finds the first occurance of a character in a string (const char
	/// version).
	INLINE_lo
		const char *strchr (const char *s, int c);


	/// Finds the first occurance of a character in a string (const wchar_t
	/// version).
	INLINE_lo
		const wchar_t *strchr (const wchar_t *s, wchar_t c);

	/// Finds the first occurance of a character in a string (char version).
	INLINE_lo
		char *strchr (char *s, int c);


	/// Finds the first occurance of a character in a string (wchar_t version).
	INLINE_lo
		wchar_t *strchr (wchar_t *s, wchar_t c);

	/// Compares two strings (char version).
	INLINE_lo
		int strcmp (const char *s, const char *t);

	/// Compares two strings (wchar_t version).
	INLINE_lo
		int strcmp (const wchar_t *s, const wchar_t *t);

	/// Copies a string (char version).
	INLINE_lo
		char *strcpy (char *s, const char *t);

	/// Copies a string (wchar_t version).
	INLINE_lo
		wchar_t *strcpy (wchar_t *s, const wchar_t *t);

	/// Searches for the first substring without any of the specified
	/// characters and returns the size of the substring (char version).
	INLINE_lo
		size_t strcspn (const char *s, const char *reject);

	/// Searches for the first substring without any of the specified
	/// characters and returns the size of the substring (wchar_t version).
	INLINE_lo
		size_t strcspn (const wchar_t *s, const wchar_t *reject);

	/// Returns a malloced duplicated string (char version).
	INLINE_lo
		char *strdup (const char *s);

	extern loCOMMON_API
		char *strdup_emulation (const char *s);

	/// Returns a malloced duplicated string (wchar_t version).
	INLINE_lo
		wchar_t *strdup (const wchar_t *s);

	extern loCOMMON_API
		wchar_t *strdup_emulation (const wchar_t *s);


	/// Copies a string, but returns a pointer to the end of the
	/// copied region (char version).
	extern loCOMMON_API
		char *strecpy (char *des, const char *src);


	/// Copies a string, but returns a pointer to the end of the
	/// copied region (wchar_t version).
	extern loCOMMON_API
		wchar_t *strecpy (wchar_t *s, const wchar_t *t);

	/// Finds the length of a string (char version).
	INLINE_lo
		size_t strlen (const char *s);

	/// Finds the length of a string (wchar_t version).
	INLINE_lo
		size_t strlen (const wchar_t *s);

	/// Appends part of a string to another string (char version).
	INLINE_lo
		char *strncat (char *s, const char *t, size_t len);

	/// Appends part of a string to another string (wchar_t version).
	INLINE_lo
		wchar_t *strncat (wchar_t *s, const wchar_t *t, size_t len);

	/// Finds the first occurance of a character in an array (const char
	/// version).
	extern loCOMMON_API
		const char *strnchr (const char *s, int c, size_t len);

	/// Finds the first occurance of a character in an array (const wchar_t
	/// version).
	extern loCOMMON_API
		const wchar_t *strnchr (const wchar_t *s, wchar_t c, size_t len);

	/// Finds the first occurance of a character in an array (char version).
	INLINE_lo
		char *strnchr (char *s, int c, size_t len);

	/// Finds the first occurance of a character in an array (wchar_t version).
	INLINE_lo
		wchar_t *strnchr (wchar_t *s, wchar_t c, size_t len);

	/// Compares two arrays (char version).
	INLINE_lo
		int strncmp (const char *s, const char *t, size_t len);

	/// Compares two arrays (wchar_t version).
	INLINE_lo
		int strncmp (const wchar_t *s, const wchar_t *t, size_t len);

	/// Copies an array (char version)
	INLINE_lo
		char *strncpy (char *s, const char *t, size_t len);

	/// Copies an array (wchar_t version)
	INLINE_lo
		wchar_t *strncpy (wchar_t *s, const wchar_t *t, size_t len);

	/// Finds the length of a limited-length string (char version).
	/**
	* @param s       The character string to find the length of.
	* @param maxlen  The maximum number of characters that will be
	*                scanned for the terminating nul character.
	*
	* @return The length of @arg s, if the terminating nul character
	*         is located, else @arg maxlen.
	*/
	INLINE_lo
		size_t strnlen (const char *s, size_t maxlen);

	/// Finds the length of a limited-length string (wchar_t version).
	/**
	* @param s       The character string to find the length of.
	* @param maxlen  The maximum number of characters that will be
	*                scanned for the terminating nul character.
	*
	* @return The length of @arg s, if the terminating nul character
	*         is located, else @arg maxlen.
	*/
	INLINE_lo
		size_t strnlen (const wchar_t *s, size_t maxlen);

	/// Finds the first occurance of a substring in an array (const char
	/// version).
	extern loCOMMON_API
		const char *strnstr (const char *s, const char *t, size_t len);

	/// Finds the first occurance of a substring in an array (const wchar_t
	/// version).
	extern loCOMMON_API
		const wchar_t *strnstr (const wchar_t *s,const wchar_t *t,size_t len);

	/// Finds the first occurance of a substring in an array (char version).
	INLINE_lo
		char *strnstr (char *s, const char *t, size_t len);

	/// Finds the first occurance of a substring in an array (wchar_t version).
	INLINE_lo
		wchar_t *strnstr (wchar_t *s, const wchar_t *t, size_t len);

	/// Searches for characters in a string (const char version).
	INLINE_lo
		const char *strpbrk (const char *s1, const char *s2);

	/// Searches for characters in a string (const wchar_t version).
	INLINE_lo
		const wchar_t *strpbrk (const wchar_t *s1, const wchar_t *s2);

	/// Searches for characters in a string (char version).
	INLINE_lo
		char *strpbrk (char *s1, const char *s2);

	/// Searches for characters in a string (wchar_t version).
	INLINE_lo
		wchar_t *strpbrk (wchar_t *s1, const wchar_t *s2);

	/// Finds the last occurance of a character in a string (const char
	/// version).
	INLINE_lo
		const char *strrchr (const char *s, int c);

	/// Finds the last occurance of a character in a string (const wchar_t
	/// version).
	INLINE_lo
		const wchar_t *strrchr (const wchar_t *s, wchar_t c);

	/// Finds the last occurance of a character in a string (char version).
	INLINE_lo
		char *strrchr (char *s, int c);

	/// Finds the last occurance of a character in a string (wchar_t version).
	INLINE_lo
		wchar_t *strrchr (wchar_t *s, wchar_t c);

	/// Emulated strrchr (char version) - Finds the last occurance of a
	/// character in a string.
	extern loCOMMON_API
		char *strrchr_emulation (char *s, int c);

	/// Emulated strrchr (const char version) - Finds the last occurance of a
	/// character in a string.
	extern loCOMMON_API
		const char *strrchr_emulation (const char *s, int c);

	/// This is a "safe" c string copy function (char version).
	/**
	* Unlike strncpy() this function will always add a terminating '\0'
	* char if maxlen > 0.  So the user doesn't has to provide an extra
	* '\0' if the user wants a '\0' terminated dst.  The function
	* doesn't check for a 0 @a dst, because this will give problems
	* anyway.  When @a src is 0 an empty string is made.  We do not
	* "touch" * @a dst if maxlen is 0.  Returns @a dst.  Care should be
	* taken when replacing strncpy() calls, because in some cases a
	* strncpy() user is using the "not '\0' terminating" feature from
	* strncpy().  This happens most when the call to strncpy() was
	* optimized by using a maxlen which is 1 smaller than the size
	* because there's always written a '\0' inside this last position.
	* Very seldom it's possible that the '\0' padding feature from
	* strncpy() is needed.
	*/
	extern loCOMMON_API
		char *strsncpy (char *dst,const char *src,size_t maxlen);

	/// This is a "safe" c string copy function (wchar_t version).
	/**
	* Unlike strncpy() this function will always add a terminating '\0'
	* char if maxlen > 0.  So the user doesn't has to provide an extra
	* '\0' if the user wants a '\0' terminated dst.  The function
	* doesn't check for a 0 @a dst, because this will give problems
	* anyway.  When @a src is 0 an empty string is made.  We do not
	* "touch" * @a dst if maxlen is 0.  Returns @a dst.  Care should be
	* taken when replacing strncpy() calls, because in some cases a
	* strncpy() user is using the "not '\0' terminating" feature from
	* strncpy().  This happens most when the call to strncpy() was
	* optimized by using a maxlen which is 1 smaller than the size
	* because there's always written a '\0' inside this last position.
	* Very seldom it's possible that the '\0' padding feature from
	* strncpy() is needed.
	*/
	extern loCOMMON_API
		wchar_t *strsncpy (wchar_t *dst,const wchar_t *src,size_t maxlen);

	/// Searches for the first substring containing only the specified
	/// characters and returns the size of the substring (char version).
	INLINE_lo
		size_t strspn (const char *s1, const char *s2);


	/// Searches for the first substring containing only the specified
	/// characters and returns the size of the substring (wchar_t version).
	INLINE_lo
		size_t strspn (const wchar_t *s1, const wchar_t *s2);

	/// Finds the first occurance of a substring in a string (const char
	/// version).
	INLINE_lo
		const char *strstr (const char *s, const char *t);

	/// Finds the first occurance of a substring in a string (const wchar_t
	/// version).
	INLINE_lo
		const wchar_t *strstr (const wchar_t *s, const wchar_t *t);

	/// Finds the first occurance of a substring in a string (char version).
	INLINE_lo
		char *strstr (char *s, const char *t);

	/// Finds the first occurance of a substring in a string (wchar_t version).
	INLINE_lo
		wchar_t *strstr (wchar_t *s, const wchar_t *t);

	/// Finds the next token in a string (char version).
	INLINE_lo
		char *strtok (char *s, const char *tokens);

	/// Finds the next token in a string (wchar_t version).
	INLINE_lo
		wchar_t *strtok (wchar_t *s, const wchar_t *tokens);

	//@}

	/// Finds the next token in a string (safe char version).
	INLINE_lo
		char *strtok_r (char *s, const char *tokens, char **lasts);

	/// Finds the next token in a string (wchar_t version).
	INLINE_lo
		wchar_t *strtok_r (wchar_t *s, const wchar_t *tokens, wchar_t **lasts);

	/// Emulated strtok_r.
	extern loCOMMON_API
		char *strtok_r_emulation (char *s, const char *tokens, char **lasts);

	/// Emulated strtok_r (wchar_t version).
	extern loCOMMON_API
		wchar_t *strtok_r_emulation (wchar_t *s, const wchar_t *tokens, wchar_t **lasts);


	//! Returns a character converted to lower case
	INLINE_lo
		unsigned int lower ( unsigned int x )
	{
		// ansi
		return x >= 'A' && x <= 'Z' ? x + 0x20 : x;
	}

	//! Returns a character converted to upper case
	INLINE_lo
		unsigned int upper ( unsigned int x )
	{
		// ansi
		return x >= 'a' && x <= 'z' ? x + ( 'A' - 'a' ) : x;
	}

	
	/** 
	 * finds first occurrence of character in string
	@param c: Character to search for.
	@return Position where the character has been found,
	or -1 if not found. */
	template<typename T>
	size_t findFirst(const T* str , size_t len , T c , size_t start = 0)
	{
		if( start >= len )
			return -1;
		for(size_t i=start; i<len; ++i)
			if( str[i] == c)
				return i;

		return -1;
	}
	
	/**
	* finds last occurrence of character in string
	@param c: Character to search for.
	@param start: start to search reverse ( default = -1, on end )
	@return Position where the character has been found,
	or -1 if not found. */
	template<typename T>
	int findLast(const T* str , size_t len , T c , size_t start = 0)
	{
		if( start >= len )
			return start = len - 1;

		for(size_t i=start; i>= 0; --i)
			if( str[i] == c)
				return i;

		return -1;
	}	

	/**
	 * finds first occurrence of a character of a list in string
	 @param c: List of characters to find. For example if the method
	should find the first occurrence of 'a' or 'b', this parameter should be "ab".
	@param count: Amount of characters in the list. Usually,
	this should be strlen(c)
	@return Position where one of the characters has been found,
	or -1 if not found. */
	template<typename T>
	int findFirstChar(const T* str , size_t len , const T* const c, size_t count)
	{
		if (!c)
			return -1;

		for (size_t i=0; i<len; ++i)
			for (size_t j=0; j<count; ++j)
				if (str[i] == c[j])
					return i;
		return -1;
	}

	//! Finds first position of a character not in a given list.
	/** \param c: List of characters not to find. For example if the method
	should find the first occurrence of a character not 'a' or 'b', this parameter should be "ab".
	\param count: Amount of characters in the list. Usually,
	this should be strlen(c)
	\return Position where the character has been found,
	or -1 if not found. */
	template <typename T>
	int findFirstCharNotInList(const T* str , size_t len , const T* const c, size_t count)
	{
		for(size_t i=0; i<len-1; ++i)
		{
			size_t j;
			for (j=0; j<count; ++j)
				if (str[i] == c[j])
					break;

			if (j==count)
				return i;
		}

		return -1;
	}

	//! Finds last position of a character not in a given list.
	/** \param c: List of characters not to find. For example if the method
	should find the first occurrence of a character not 'a' or 'b', this parameter should be "ab".
	\param count: Amount of characters in the list. Usually,
	this should be strlen(c)
	\return Position where the character has been found,
	or -1 if not found. */
	template <typename T>
	int findLastCharNotInList(const T* str , size_t len , const T* const c, size_t count)
	{
		for(int i= (int)len-2; i >= 0; --i)
		{
			size_t j;
			for (j=0; j<count; ++j)
				if (str[i] == c[j])
					break;

			if (j==count)
				return i;
		}
		return -1;
	}

} /* namespace loOS */

# if defined (__INLINE_lo__)
#include "loOSString.inl"
# endif /* __INLINE_lo__ */

#endif /* __loOS_STRING_H__ */
