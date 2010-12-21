/**
*  @brief String opeartion.
*  @file  strconv.cpp
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
#include "strconv.h"


#if (defined(WIN32) || defined(_WIN32) )

#include <windows.h>

#else

#include "bbiconv.h"

#endif //(defined(WIN32) || defined(_WIN32) )

#ifdef __cplusplus
extern "C" {
#endif


	/*
	** Convert an ansi string to microsoft unicode, based on the
	** current codepage settings for file apis.
	** 
	** Space to hold the returned string is obtained
	** from malloc.
	*/
	int lo_C2W(wchar_t** pout ,const char *zinname)
	{
#if (defined(WIN32) || defined(_WIN32) )
		wchar_t *zMbcsname = 0;
		int codepage = 0;
		int nByte = 0;

		if( !zinname || * zinname == '\0' )
			return 0;

#if defined(_WIN32_WCE)
		codepage =  CP_ACP ;
#else
		codepage = AreFileApisANSI() ? CP_ACP : CP_OEMCP;
#endif
		nByte = MultiByteToWideChar(codepage, 0, zinname, -1, NULL,0);
		zMbcsname = (wchar_t*)malloc( sizeof(wchar_t)*(nByte+1) );
		if( zMbcsname == 0 )
			return 0;

		nByte = MultiByteToWideChar(codepage, 0, zinname, -1, zMbcsname, nByte);
		if( nByte > 0 )
		{
			zMbcsname[nByte] = 0;
		}
		*pout = zMbcsname;
		return nByte;
#else
		const char* in  = zinname;
		size_t in_len = strlen(in);
		size_t outlen = bbiconv_counts_gbk_2_wchar(in , in_len);
		if( outlen <= 0 )
			return -1;

		wchar_t* out_ptr = (wchar_t*)malloc( (sizeof(wchar_t)) * (outlen+outlen+sizeof(wchar_t)) );
		if( !out_ptr )
			return -1;

		bbiconv_gbk_2_wchar(in , in_len , out_ptr , outlen);
		*pout = out_ptr;
		return outlen;
#endif //

	}

	/*
	** Convert microsoft unicode to multibyte character string, based on the
	** user's Ansi codepage.
	**
	** Space to hold the returned string is obtained from
	** malloc().
	*/
	int lo_W2C(char** pout ,const wchar_t *zWide)
	{
#if (defined(WIN32) || defined(_WIN32) )
		char *zname = 0;
		int  codepage = 0;
		int  nByte = 0;

		if( !zWide || *zWide == '\0' )
			return 0;

#if defined(_WIN32_WCE)
		codepage =  CP_ACP ;
#else
		codepage = AreFileApisANSI() ? CP_ACP : CP_OEMCP;
#endif
		nByte = WideCharToMultiByte(codepage, 0, zWide, -1, 0, 0, 0, 0);
		zname = (char*)malloc( nByte + 1 );
		if( zname == 0 ) return 0;

		nByte = WideCharToMultiByte(codepage, 0, zWide, -1, zname, nByte+1, 0, 0);
		if( nByte > 0 )	zname[nByte] = '\0';

		*pout = zname;
		return nByte;
#else

		const wchar_t* in  = zWide;
		size_t in_len = wcslen(in);
		size_t outlen = bbiconv_counts_wchar_2_gbk(in , in_len);
		if( outlen <= 0 )
			return -1;

		char* out_ptr = (char*)malloc( (sizeof(char)) * (outlen+1) );
		if( !out_ptr )
			return -1;

		bbiconv_wchar_2_gbk(in , in_len , out_ptr , outlen);
		*pout = out_ptr;
		return outlen;

#endif //#if (defined(WIN32) || defined(_WIN32) )

	}

	int lo_W2Utf8(char** pout , const wchar_t *zWide)
	{
#if (defined(WIN32) || defined(_WIN32) )
		int nByte = 0;
		char *zname = 0;

		if( !zWide || *zWide == '\0' )
			return 0;

		nByte = WideCharToMultiByte(CP_UTF8, 0, zWide, -1, 0, 0, 0, 0);
		zname = (char*)malloc( nByte +1 );
		if( zname == 0 )
		{
			return 0;
		}
		nByte = WideCharToMultiByte(CP_UTF8, 0, zWide, -1, zname, nByte+1, 	0, 0);
		if( nByte > 0 )
		{
			zname[nByte] = '\0';
		}
		*pout = zname ;
		return nByte;
#else

		const wchar_t* in  = zWide;
		size_t in_len = wcslen(in);
		size_t outlen = bbiconv_counts_wchar_2_utf8(in , in_len);
		if( outlen <= 0 )
			return -1;

		char* out_ptr = (char*)malloc( (sizeof(char)) * (outlen+1) );
		if( !out_ptr )
			return -1;

		bbiconv_wchar_2_utf8(in , in_len , out_ptr , outlen);
		*pout = out_ptr;
		return outlen;

#endif //#if (defined(WIN32) || defined(_WIN32) )
	}
		
	int lo_Utf82W(wchar_t** pout ,const char* utf8 ) 
	{
#if (defined(WIN32) || defined(_WIN32) )

		int nByte = 0;
		wchar_t *zwstr = 0;

		if( !utf8 )
			return 0;

		nByte = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
		zwstr = (wchar_t*)malloc( (nByte+1)*sizeof(wchar_t) );
		if( zwstr == 0 ){
			return 0;
		}
		nByte = MultiByteToWideChar(CP_UTF8, 0, utf8 , -1 , zwstr, nByte);
		if( nByte > 0 )
		{
			zwstr[nByte] = 0;
		}
		*pout = zwstr;
		return nByte;

#else
		const char* in  = utf8;
		size_t in_len = strlen(in);
		size_t outlen = bbiconv_counts_utf8_2_wchar(in , in_len);
		if( outlen <= 0 )
			return -1;

		wchar_t* out_ptr = (wchar_t*)malloc( outlen+sizeof(wchar_t) );
		if( !out_ptr )
			return -1;

		bbiconv_utf8_2_wchar(in , in_len , out_ptr , outlen);
		*pout = out_ptr;
		return outlen;

#endif //
	}

	/*
	** Convert multibyte character string to UTF-8.  Space to hold the
	** returned string is obtained from malloc().
	*/
	int lo_C2Utf8(char** pout , const char *zinname)
	{
#if (defined(WIN32) || defined(_WIN32) )
		wchar_t *zTmp = 0;
		int outlen = 0;
		
		outlen = lo_C2W(&zTmp , zinname);
		if( !zTmp || outlen <= 0 )
		{
			return 0;
		}

		outlen = lo_W2Utf8(pout , zTmp);
		free( zTmp );
		return outlen ;
#else
		const char* in  = zinname;
		size_t in_len = strlen(in);
		size_t outlen = bbiconv_counts_gbk_2_utf8(in , in_len);
		if( outlen <= 0 )
			return -1;

		char* out_ptr = (char*)malloc( outlen+sizeof(char) );
		if( !out_ptr )
			return -1;

		bbiconv_gbk_2_utf8(in , in_len , out_ptr , outlen);
		*pout = out_ptr;
		return outlen;

#endif //#if (defined(WIN32) || defined(_WIN32) )

	}

	/*
	** Convert UTF-8 to multibyte character string.  Space to hold the 
	** returned string is obtained from sqliteMalloc().
	*/
	int lo_Utf82C(char** pout ,const char *zstr)
	{
#if (defined(WIN32) || defined(_WIN32) )

		wchar_t* pwout = 0;
		int outlen = 0;
		if( 0 == lo_Utf82W(&pwout , zstr) )
			return 0;

		outlen = lo_W2C(pout , pwout );
		free( pwout );
		return outlen;

#else
		const char* in  = zstr;
		size_t in_len = strlen(in);
		size_t outlen = bbiconv_counts_utf8_2_gbk(in , in_len);
		if( outlen <= 0 )
			return -1;

		char* out_ptr = (char*)malloc( outlen+sizeof(char) );
		if( !out_ptr )
			return -1;

		bbiconv_utf8_2_gbk(in , in_len , out_ptr , outlen);
		*pout = out_ptr;
		return outlen;

#endif //
	}

#ifdef __cplusplus
}
#endif



