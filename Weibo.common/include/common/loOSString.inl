// add by welbon, 2010-08-29

#if (defined (WIN32)) || (defined(_WIN32))

#define WCSCAT(s,t) 	    		::wcscat(s,t)
#define WCACHR(s,c) 	    		::wcschr(s,c)
#define WCSCMP(s,t)  	    		::wcscmp(s,t)
#define WCSCPY(s,t) 	    		::wcscpy(s,t)
#define WCSTOK(s,tokens)			::wcstok(s,tokens)
#define WCSCSPN(s,reject)			::wcscspn(s,reject)
#define WCSDUP(s) 		    		::wcsdup(s)
#define WCSLEN(s) 		    		::wcslen(s)
#define WCSNCAT(s,t,len)    		::wcsncat(s,t,len)
#define WCSNCMP(s, t, len)			::wcsncmp(s,t,len)
#define WCSNCPY(s, t, len)			::wcsncpy (s, t, len)
#define WCSNLEN(s, maxlen)			::wcsnlen (s, maxlen)
#define WCSPBRK(s, t)				::wcspbrk (s, t)
#define WCSSTR(s,t)					::wcsstr (const_cast<wchar_t*>(s), t)
#define WCSRCHR(s,c)				const_cast <const wchar_t *>(::wcsrchr (s,c))
#define WCSSPN(s,t)					::wcsspn (s,t)
#define WCSTOK_S(s,tokens, lasts)	wcstok_s(s,tokens, lasts)
#define STRTOK_S(s,tokens,lasts)    strtok_s(s,tokens,lasts)

#else

#define  WCSCAT(s,t)					0
#define  WCACHR(s,c)					0
#define  WCSCMP(s,t)					0
#define  WCSCPY(s,t) 	     				0
#define  WCSTOK(s,tokens)   				0
#define  WCSCSPN(s,reject)  				0
#define  WCSDUP(s) 		      			0
#define  WCSLEN(s) 		      				0
#define  WCSNCAT(s,t,len)     				0
#define  WCSNCMP(s,t,len)   				0
#define  WCSNCPY(s,t,len)   				0
#define  WCSNLEN(s,maxlen)	 			0
#define  WCSPBRK(s,t)				    	0
#define  WCSSTR(s,t)					0
#define  WCSRCHR(s,c)				    	0
#define  WCSSPN(s,t)					0
#define  WCSTOK_S(s,tokens, lasts)  		0
#define  STRTOK_S(s, tokens,lasts)    		0

#endif //(defined (WIN32)) && (defined(_WIN32))

INLINE_lo const void *
loOS::memchr (const void *s, int c, size_t len)
{
	return ::memchr (s, c, len);
}

INLINE_lo void *
loOS::memchr (void *s, int c, size_t len)
{
	return const_cast<void *> (loOS::memchr (static_cast<const void *> (s),c,len));
}

INLINE_lo int
loOS::memcmp (const void *t, const void *s, size_t len)
{
	return ::memcmp (t, s, len);
}

INLINE_lo void *
loOS::memcpy (void *t, const void *s, size_t len)
{
	return ::memcpy (t, s, len);
}

INLINE_lo void *
loOS::memmove (void *t, const void *s, size_t len)
{
	return ::memmove (t, s, len);
}

INLINE_lo void *
loOS::memset (void *s, int c, size_t len)
{
	return ::memset (s, c, len);
}

INLINE_lo char *
loOS::strcat (char *s, const char *t)
{
	return ::strcat(s, t);
}

INLINE_lo wchar_t *
loOS::strcat (wchar_t *s, const wchar_t *t)
{
	return WCSCAT(s, t);
}


INLINE_lo const char *
loOS::strchr (const char *s, int c)
{
	return const_cast <const char *> (::strchr (s, c));
}


INLINE_lo const wchar_t *
loOS::strchr (const wchar_t *s, wchar_t c)
{
	return WCACHR(s,c);
}


INLINE_lo char *
loOS::strchr (char *s, int c)
{
	return ::strchr(s, c);
}

INLINE_lo wchar_t *
loOS::strchr (wchar_t *s, wchar_t c)
{
	return const_cast<wchar_t *> (loOS::strchr (const_cast<const wchar_t *> (s),c));
}

INLINE_lo int
loOS::strcmp (const char *s, const char *t)
{
	return ::strcmp(s, t);
}

INLINE_lo int
loOS::strcmp (const wchar_t *s, const wchar_t *t)
{
	return WCSCMP(s,t);
}

INLINE_lo char *
loOS::strcpy (char *s, const char *t)
{
	return ::strcpy(s, t);
}

INLINE_lo wchar_t *
loOS::strcpy (wchar_t *s, const wchar_t *t)
{
	return WCSCPY(s, t);
}

INLINE_lo size_t
loOS::strcspn (const char *s, const char *reject)
{
	return ::strcspn(s, reject);
}

INLINE_lo size_t
loOS::strcspn (const wchar_t *s, const wchar_t *reject)
{
	return WCSCSPN(s, reject);
}

INLINE_lo char *
loOS::strdup (const char *s)
{
	return ::strdup (s);
}

INLINE_lo wchar_t *
loOS::strdup (const wchar_t *s)
{
	return WCSDUP(s);

}

INLINE_lo size_t
loOS::strlen (const char *s)
{
	return ::strlen(s);
}

INLINE_lo size_t
loOS::strlen (const wchar_t *s)
{
	return WCSLEN(s);
}

INLINE_lo char *
loOS::strncat (char *s, const char *t, size_t len)
{
	return ::strncat(s, t, len);

}

INLINE_lo wchar_t *
loOS::strncat (wchar_t *s, const wchar_t *t, size_t len)
{
	return WCSNCAT(s, t, len);
}

INLINE_lo char *
loOS::strnchr(char *s, int c, size_t len)
{
	return const_cast<char *> (loOS::strnchr (static_cast<const char *> (s),c,len));
}

INLINE_lo wchar_t *
loOS::strnchr(wchar_t *s, wchar_t c, size_t len)
{
	return
		const_cast<wchar_t *> (loOS::strnchr (const_cast<const wchar_t *> (s),c,len));
}

INLINE_lo int
loOS::strncmp (const char *s, const char *t, size_t len)
{
	return ::strncmp(s, t, len);
}

INLINE_lo int
loOS::strncmp (const wchar_t *s, const wchar_t *t, size_t len)
{
	return WCSNCMP (s, t, len);
}

INLINE_lo char *
loOS::strncpy (char *s, const char *t, size_t len)
{
	return ::strncpy(s, t, len);
}

INLINE_lo wchar_t *
loOS::strncpy (wchar_t *s, const wchar_t *t, size_t len)
{
	return WCSNCPY(s, t, len);
}

INLINE_lo size_t
loOS::strnlen (const char *s, size_t maxlen)
{
	return ::strnlen(s, maxlen);
}

INLINE_lo size_t
loOS::strnlen (const wchar_t *s, size_t maxlen)
{
	return WCSNLEN(s, maxlen);
}

INLINE_lo char *
loOS::strnstr (char *s, const char *t, size_t len)
{
	return
		const_cast <char *> (loOS::strnstr (const_cast <const char *> (s), t, len));
}

INLINE_lo wchar_t *
loOS::strnstr (wchar_t *s, const wchar_t *t, size_t len)
{
	return
		const_cast<wchar_t *> (loOS::strnstr (static_cast<const wchar_t *> (s),t,len));
}

INLINE_lo const char *
loOS::strpbrk (const char *s1, const char *s2)
{
	return const_cast <const char *> (::strpbrk (s1, s2));
}

INLINE_lo const wchar_t *
loOS::strpbrk (const wchar_t *s, const wchar_t *t)
{
	return WCSPBRK(s, t);
}

INLINE_lo char *
loOS::strpbrk (char *s1, const char *s2)
{
	return ::strpbrk(s1, s2);
}

INLINE_lo wchar_t *
loOS::strpbrk (wchar_t *s, const wchar_t *t)
{
	return const_cast<wchar_t *> (loOS::strpbrk (const_cast<const wchar_t *> (s), t));
}

INLINE_lo const char *
loOS::strrchr (const char *s, int c)
{
	return (const char *) ::strrchr (s, c);
}

INLINE_lo const wchar_t *
loOS::strrchr (const wchar_t *s, wchar_t c)
{
	return WCSRCHR(s, c);

}

INLINE_lo char *
loOS::strrchr (char *s, int c)
{
	return ::strrchr (s, c);

}

INLINE_lo wchar_t *
loOS::strrchr (wchar_t *s, wchar_t c)
{
	return const_cast<wchar_t *> (loOS::strrchr (const_cast<const wchar_t *> (s), c));
}

INLINE_lo size_t
loOS::strspn (const char *s, const char *t)
{
	return ::strspn (s, t);
}

INLINE_lo size_t
loOS::strspn (const wchar_t *s, const wchar_t *t)
{
	return WCSSPN(s, t);

}

INLINE_lo const char *
loOS::strstr (const char *s, const char *t)
{
	return (const char *) ::strstr (s, t);
}

INLINE_lo const wchar_t *
loOS::strstr (const wchar_t *s, const wchar_t *t)
{
	return WCSSTR(s,t);
}

INLINE_lo char *
loOS::strstr (char *s, const char *t)
{
	return ::strstr (s, t);
}

INLINE_lo wchar_t *
loOS::strstr (wchar_t *s, const wchar_t *t)
{
	return WCSSTR (s, t);

}

INLINE_lo char *
loOS::strtok (char *s, const char *tokens)
{
	return ::strtok (s, tokens);
}

INLINE_lo wchar_t *
loOS::strtok (wchar_t *s, const wchar_t *tokens)
{
	return WCSTOK(s, tokens);
}

INLINE_lo char *
loOS::strtok_r (char *s, const char *tokens, char **lasts)
{
	return STRTOK_S(s, tokens, lasts);
}

INLINE_lo wchar_t*
loOS::strtok_r (wchar_t *s, const wchar_t *tokens, wchar_t **lasts)
{
	return WCSTOK_S(s, tokens, lasts);
}
