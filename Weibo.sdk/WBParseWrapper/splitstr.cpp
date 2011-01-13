/**
*  @brief weibo call back
*  @file  wbcallback.cpp
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

#include "splitstr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>

#ifdef  __cplusplus
extern "C" {
#endif

int split_url_param(const char*& next , const char*& key , const char*& qe , const char*& split ,const char endflag )
{
	if( !next || *next == '\0' )
		return -1;

	key = next; 
	// ǰ��Ŀո����
	do
	{
		if( *key != 0x20 || *key == '\0' ) break;
		key++;
	}while(1);

	qe = strchr(key , '=' );
	if( !qe )
		return -1;

	split = strchr( (qe+1) , endflag);

	if(split) next = (split + 1 );
	else next = 0;

	return 0;
}

int split_url_copy_keyval(char* val , const char* start , const char* end)
{
	if( !val || !start || *start == '\0' )
		return -1;

	if( end && end <= start )
		return -1;

	if( !end || *end == '\0' )
	{
		strcpy(val , start );
	}
	else
	{
		strncpy(val , start , end - start );		
	}
	return 0;
}

/**
* split and parse URL parameters replied by the test-server
* into <em>oauth_token</em> and <em>oauth_token_secret</em>.
*/
int wb_parse_oauth(const char *reply,
						char *token,
						char *secret,char* userid )
{
	int tokenok = 0 ,secretok = 0; 
	const char* key = 0;
	const char* qe  = 0;
	const char* split = 0;
	const char* s = reply;
	// ��һ������

	while( 0 == split_url_param(s , key , qe , split ,'&') )
	{
		if( _strnicmp(key,"oauth_token_secret=",18 ) == 0 )
		{
			if (secret) { split_url_copy_keyval(secret ,(qe+1) , split); secretok=1; }
		}
		else if( _strnicmp(key,"oauth_token=",11 ) == 0 )
		{
			if (token) { split_url_copy_keyval(token , (qe+1) , split ) ; tokenok = 1; }
		}
		else if( _strnicmp(key,"user_id=",7 ) == 0 )
		{
			if (userid) { split_url_copy_keyval(userid , (qe+1) , split ) ; }
		}
	}
	return (tokenok || secretok);
}

void wb_parse_error(const char *reply,
						char *request , char* error ,  char *error_code )
{//request=/oauth/request_token&error_code=500&error=50001:Error:+system+error!&error_CN=����:ϵͳ����!
	const char* key = 0;
	const char* qe  = 0;
	const char* split = 0;
	const char* s = reply;
	// ��һ������

	while( 0 == split_url_param(s , key , qe , split ,'&') )
	{
		if( _strnicmp(key,"request=",7 ) == 0 )
		{
			if (request) { split_url_copy_keyval(request ,(qe+1) , split);  }
		}
		else if( _strnicmp(key,"error_code=",10 ) == 0 )
		{
			if (error_code) { split_url_copy_keyval(error_code , (qe+1) , split ) ; }
		}
		else if( _strnicmp(key,"error=",6 ) == 0 )
		{
			if (error) { split_url_copy_keyval(error , (qe+1) , split ) ; }
		}
	}
}


int	wb_get_cookie(const char* reply , char *token, char *secret )
{
	const char* key = 0;
	const char* qe  = 0;
	const char* split = 0;
	const char* s = reply;

	while( 0 == split_url_param(s , key , qe , split , ';') )
	{
		if( _strnicmp(key,"sup=", 4 ) == 0 )
		{
			if (secret) { split_url_copy_keyval(secret ,(qe+1) , split);  }
			return 2;
		}
		else if( _strnicmp(key,"sue=",4 ) == 0 )
		{
			if (token) { split_url_copy_keyval(token , (qe+1) , split ) ; }
			return 1;
		}
	}
	return 0;
}

bool wb_readline( const char *reply , const char* &linestart, const char* &lineend )
{
	const char* s = reply;
	linestart = 0;
	lineend = 0;

	do
	{
		if( *s == '\0' )
		{
			lineend = s;
			break;
		}

		if( linestart == 0 )
		{
			linestart = s;
		}

		if( *s == 0x0d && *(s+1) == 0x0a )
		{
			lineend = s;
			break;
		}
		s++;
	}while(1);

	return (linestart && lineend );
}

int wb_parse_cookie( const char *reply , char *token, char *secret )
{
	int tokenok = 0 ,secretok = 0; 
	const char* key = 0;
	const char* qe  = 0;
	const char* split = 0;
	const char* s = reply;
    const char* linestart, *lineend ;

	while( wb_readline(s , linestart , lineend ) )
	{
		//Set-Cookie:
		if( _strnicmp("Set-Cookie:" , linestart , 11 ) == 0 )
		{
			linestart+= 11;
			int ii = wb_get_cookie( linestart , token , secret );
			if( 1 == ii  ) tokenok = 1;
			if( 2 == ii )  secretok= 1;
		}

		if( tokenok && secretok )
			return 0;

		s = lineend;
		if( *s == '\0' )
			break;

		// ���� 0x0d , 0x0a
		s = ++lineend ;
		if( *s == '\0' )
			break;

		s = ++lineend ;
		if( *s == '\0' )
			break;
	}
	return -1;
}

int wb_split_shortlink(const char* txt , const char* szEnd , const char*& start ,const char*& end , const char*& shortID)
{
	if( txt >= szEnd )
		return -1;
	static const char* zMatch = "http://sinaurl.cn/";
	static const int   iLen =  strlen(zMatch);

	const char* zFind = strstr(txt ,zMatch);
	if( !zFind )
	{
		return -1;
	}
	shortID = zFind + iLen;
	start   = zFind;
	end     = start;
	while( end < szEnd )
	{
		unsigned char c = (unsigned char)*end;
		if( c == 0x20 )
			break;

		if( c < 0x80 )
		{
			end++;
		}
		else
		{// �Ǻ��־ͽ�����
			break;
		}
	}
	return 0;
}

#ifdef  __cplusplus
}
#endif