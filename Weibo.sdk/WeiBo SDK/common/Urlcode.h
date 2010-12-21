/**
*  @brief URL Encode
*  @file Urlcode.h
*  @author loach <loachmr@sina.com>
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


#ifndef __urlcode_H__
#define __urlcode_H__

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

	int	 lo_UrlDecode(wchar_t** out,const char* pin,  int len);
	int  lo_UrlDecodeA(char** out,const char* pin, int len);

	int	 lo_UrlEncode(wchar_t** out , const char* pin ,  int len);	
	int  lo_UrlEncodeA(char** out, const char* pin ,  int len);

	class CURLDecodeW
	{
	public:
		CURLDecodeW(const char* pin,  int len)
		{
			_out = 0;
			lo_UrlDecode(&_out,pin,len);
		}
		~CURLDecodeW()
		{
			free(_out);
		}
		wchar_t* c_str() const{ return _out;}
	private:
		wchar_t* _out;
	};

	class CURLDecodeA
	{
	public:
		CURLDecodeA(const char* pin,  int len)
		{
			_out = 0;
			lo_UrlDecodeA(&_out,pin,len);
		}
		~CURLDecodeA()
		{
			free(_out);
		}
		char* c_str() const{ return _out;}
	private:
		char* _out;
	};


	class CURLEncodeW
	{
	public:
		CURLEncodeW(const char* pin,  int len)
		{
			_out = 0;
			lo_UrlEncode(&_out,pin,len);
		}
		~CURLEncodeW()
		{
			free(_out);
		}
		wchar_t* c_str() const{ return _out;}
	private:
		wchar_t* _out;
	};

	class CURLEncodeA
	{
	public:
		CURLEncodeA(const char* pin,  int len)
		{
			_out = 0;
			lo_UrlEncodeA(&_out,pin,len);
		}
		~CURLEncodeA()
		{
			free(_out);
		}
		char* c_str() const{ return _out;}
	private:
		char* _out;
	};

#ifdef __cplusplus
}
#endif

#endif //__urlcode_H__