/**
*  @brief URL Encode
*  @file  Urlcode.cpp
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


#include "Urlcode.h"
#include "strconv.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#ifdef __cplusplus
extern "C" {
#endif

	int lo_UrlDecodeA(char** pout,const char* pin , int len)
	{
		char sca[4];
		char* buff = 0 , *pstr = 0;
		const char* pin_end = 0;
		int  c = 0;

		if( !pin )
			return 0;

		if( len <= 0 ){
			len = strlen( pin );
			if( len <= 0 )
				return 0;
		}
		pin_end = pin + len;

		buff = (char*)malloc( len+1 );
		memset(buff , 0 , len+1 );
		pstr= buff;

		while( pin != '\0' && (pin < pin_end) )
		{
			if( *(pin) == '%' )
			{
				if( (pin + 2) < pin_end )
				{				
					sca[0] = *(pin+1);
					sca[1] = *(pin+2);
					sca[2] = '\0';
					sscanf( sca , "%02X", &c );
					*pstr += (char)c;

					pin ++;
					pin ++;
					pin ++;
					pstr++;
				}
				else
				{
					free(buff);			
					return -1;
				}
			}
			else
			{
				*pstr += *pin;
				pin++;
				pstr++;
			}
		}
		*pstr = '\0';
		*pout = buff;
		return (pstr-buff);
	}

	int	 lo_UrlDecode(wchar_t** pout,const char* pin,  int len)
	{		
		char* poutA = 0;
		int outlen = 0;

		outlen = lo_UrlDecodeA(&poutA , pin , len );
		if( outlen > 0 )
		{//
			outlen = lo_C2W(pout , poutA);
		}
		
		if( poutA ) free(poutA);

		return outlen;
	}

	int lo_UrlEncodeA(char** pout, const char* pin ,  int len)
	{
		//�������ַ�ת��Ϊ16���Ƹ�ʽ.
		// according to RFC 2396
		char sca[4] = "%";
		unsigned char c;
		char* buff = 0 , *pstr = 0;
		int i = 0 , bufflen = 0;

		if( !pin )
			return 0;

		if( len <= 0 ){
			//
			len = strlen( pin );
			if( len <= 0 )
				return 0;
		}
		bufflen = (len * 3) + 2;

		buff = (char*)malloc( bufflen );
		memset(buff , 0 , bufflen );
		pstr = buff;

		while( *pin != '\0' && i < len )
		{
			c = *pin;
			if( isalpha( c ) || isdigit( c ) ||
				c == '-' ||	c == '_' || 	c == '.' ||	c == '!' ||	c == '~' ||	c == '*' ||	c == '\'' || c == '(' || c == ')' )
			{
				// found an unreserved character
				strncat(pstr, pin , 1);
				pstr++;
			}
			else if( c == ' ' )
			{
				strcat(pstr , "%20");
				pstr += 3;
			}
			else
			{
				sprintf( &(sca[1]) , "%02X", c );
				sca[3] = '\0';
				strcat(pstr , sca );
				pstr += 3;
			}
			pin++;
			i++;
		}
		*pstr ='\0';
		*pout = buff;
		return (pstr-buff);
	}

	int lo_UrlEncode(wchar_t** pout, const char* pin ,  int len)
	{
		char* poutA = 0;
		int outlen = 0;

		outlen = lo_UrlEncodeA(&poutA , pin , len);
		if( outlen )
		{
			outlen = lo_C2W(pout , poutA);
		}

		if( poutA ) free( poutA  );

		return 0;
	}


#ifdef __cplusplus
}
#endif
