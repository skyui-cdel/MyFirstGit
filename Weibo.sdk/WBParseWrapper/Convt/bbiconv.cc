//
#include "bbiconv.h"

#if !(defined(WIN32) || defined(_WIN32) )


#include <iconv.h>
#include <string>
#include <malloc/malloc.h>
/**
* 对其 iconv 库进行封装
* 
* @author loach
*
* @date 2009-08-02
* 
*/

static
int convert_(const char* tocode , const char* fromcode ,
                          char**  inbuf, size_t *inbytesleft, char* * outbuf, size_t *outbytesleft)
{
       iconv_t cd = iconv_open(tocode,fromcode);

	// create the cd
	if( (iconv_t)-1 == cd ){
                return -1;
	}

//	int arg = 1;
//	iconv_ctl(cd ,ICONV_SET_DISCARD_ILSEQ ,&arg);

	int iret = iconv(cd , inbuf , inbytesleft, outbuf, outbytesleft );

	iconv_close(cd);

        return iret;
}

extern 
size_t bbiconv_counts_utf8_2_gbk(const char* in ,size_t in_len)
{
	size_t out_len = 0;
	const char* end = in + in_len;
	const char* byte= in;
	while( byte < end )
	{
		unsigned char a = *byte ++;
		if( (  a >> 7  ) == 0 ){
			out_len ++;
		}
		else if( byte < end )
		{ // 当做汉字
			unsigned char b = *byte ++;
			if( ( a >> 5 ) == 0x6 && (b >> 6 ) == 0x2 ){
				out_len ++;
				out_len ++;
			}
			else if( byte < end )
			{
				unsigned char c = *byte ++;
				if( ( a >> 4 ) == 0xE && (b >> 6 ) == 0x2 && ( c >> 6 ) == 0x02 ){
					out_len ++;
					out_len ++;
				}
			}
		}
	}
	return out_len;
}

extern 
int bbiconv_utf8_2_gbk(const char* in , size_t in_len , char* outbuff , size_t& out_len)
{
	int ret = 0;
	try
	{	
		size_t buffbytes = out_len;
		size_t outbytes  = buffbytes;
		if( buffbytes > 0 )
		{
			char* pin = (char*)in;
			size_t inbytes  = in_len;

			char*  pout = outbuff;
			outbytes = buffbytes;

			ret = convert_("GBK" , "UTF-8",&pin ,&inbytes , &pout , &outbytes);
		        out_len = buffbytes - outbytes ;
			outbuff[out_len] = '\0';
		}
	}catch(...)
	{
		ret = -1;
	}
	return ret;
}

extern 
int bbiconv_utf8_2_gbk_s(const char* in , size_t in_len  , std::string& outbuffer)
{
	size_t outlen = bbiconv_counts_utf8_2_gbk(in , in_len);
	if( outlen <= 0 )
		return -1;

	char* out_ptr = new char[outlen+sizeof(char) ];
	if( !out_ptr )
		return -1;

	bbiconv_utf8_2_gbk(in , in_len , out_ptr , outlen);

	outbuffer = out_ptr;

	delete[] out_ptr;

	return 0;
}

extern 
size_t bbiconv_counts_gbk_2_utf8(const char* in ,size_t in_len)
{
	size_t out_len = 0;
	const char* end = in + in_len;
	const char* byte= in;
	while( byte < end )
	{
		unsigned char a = *byte ++;
		if( (  a < 0x80  ) ) {
			out_len ++;
		}
		else if( byte < end )
		{
			unsigned char b = *byte ++;
			if ((b >= 0x40 && b < 0x7f) || (b >= 0x80 && b < 0xff))
			{ //是一个汉字,三个字utf8表示
				out_len++;
				out_len++;
				out_len++;
			}
			else
			{ // 是一种无效的，可能没有办法转化，算一个
				out_len++;
			}
		}
	}
	return out_len;
}

extern 
int bbiconv_gbk_2_utf8(const char* in,size_t in_len  ,char* outbuff , size_t& out_len)
{
	int ret = 0;
	try
	{
		size_t buffbytes = out_len;
		size_t outbytes = buffbytes;

		if( buffbytes > 0 )
		{
			char* pin = (char*)in;
			size_t inbytes  = in_len;

			char*  pout = outbuff;
			outbytes = buffbytes;

			ret = convert_("UTF-8","GBK",&pin ,&inbytes ,  &pout , &outbytes);
		        out_len = buffbytes - outbytes ;
			outbuff[out_len] = '\0';
		}	
	}catch(...)
	{
		ret = -1;
	}
	return ret;
}

extern 
int bbiconv_gbk_2_utf8_s(const char* in , size_t in_len  , std::string& outbuffer)
{
	size_t outlen = bbiconv_counts_gbk_2_utf8(in , in_len);
	if( outlen <= 0 )
		return -1;

	char* out_ptr = new char[outlen+sizeof(char) ];
	if( !out_ptr )
		return -1;

	bbiconv_gbk_2_utf8(in , in_len , out_ptr , outlen);

	outbuffer = out_ptr;

	delete[] out_ptr;

	return 0;
}

extern
size_t bbiconv_counts_utf8_2_wchar(const char* in ,size_t in_len)
{
	size_t out_len = 0;
	const char* end = in + in_len;
	const char* byte= in;
	while( byte < end )
	{
		unsigned char a = *byte ++;
		if( (  a >> 7  ) == 0 ){
			out_len ++;
		}
		else if( byte < end )
		{
			unsigned char b = *byte ++;
			if( ( a >> 5 ) == 0x6 && (b >> 6 ) == 0x2 )
				out_len ++;
			else if( byte < end )
			{
				unsigned char c = *byte ++;
				if( ( a >> 4 ) == 0xE && (b >> 6 ) == 0x2 && ( c >> 6 ) == 0x02 )
					out_len ++;
			}
		}
	}
	return out_len;
}

extern
int bbiconv_utf8_2_wchar(const char* in,size_t in_len , wchar_t* outbuff , size_t& out_len )
{
	// test exmaple,for 不规则性的字符串
	/*
	{
	char src[8] = {0};
	src[0] = 0x9A;
	src[1] = 0x2D;
	src[2] = 0xEC;
	src[3] = 0xEB;

	gbk_2_utf8(src,strlen(src),NULL,NULL);
	}
	*/

	int ret = 0;
	try
	{
		size_t buffbytes = out_len*sizeof(wchar_t);
		size_t outbytes = buffbytes;
		if( buffbytes > 0 )
		{ // 动态分配内存

			char* pin = (char*)in;
			size_t inbytes  = in_len;

			char*  pout = (char*)outbuff;
			outbytes = buffbytes;

			ret = convert_("UCS-2LE" , "UTF-8",&pin ,&inbytes ,  &pout , &outbytes);
			out_len = (buffbytes - outbytes)/sizeof(wchar_t);
			outbuff[out_len] = 0;
		}

	}catch(...)
	{
		ret = -1;
	}
	return ret;
}

extern
int bbiconv_utf8_2_wchar_s(const char* in , size_t in_len, std::wstring& outbuffer)
{
	size_t outlen = bbiconv_counts_utf8_2_wchar(in , in_len);
	if( outlen <= 0 )
		return -1;

	wchar_t* out_ptr = new wchar_t[outlen+sizeof(wchar_t) ];
	if( !out_ptr )
		return -1;

	bbiconv_utf8_2_wchar(in , in_len , out_ptr , outlen);

	outbuffer = out_ptr;

	delete[] out_ptr;

	return 0;
}

extern
size_t bbiconv_counts_wchar_2_utf8(const wchar_t* in ,size_t in_len)
{
	size_t out_len = 0;
	const wchar_t* end = in + in_len;
	const wchar_t* byte= in;
	while( byte < end )
	{
		wchar_t a = *byte ++;
		if( (  a <= 0x7F  ) ){
			out_len ++;
		}
		else if( a >= 0x80 && a <= 0x07FF )
		{
			out_len ++;
			out_len ++;
		}
		else if( a >= 0x800 && a <= 0xFFFF ){
			out_len ++;
			out_len ++;
			out_len ++;
		}
	}
	return out_len;
}

extern
int bbiconv_wchar_2_utf8(const wchar_t* in , size_t in_len , char* outbuff , size_t& out_len)
{
	int ret = 0;
	try 
	{
		size_t buffbytes = out_len;
		size_t outbytes  = buffbytes;
		if( buffbytes > 0 )
		{
			char* pin = (char*)in;
			size_t inbytes  = in_len * sizeof(wchar_t);
			char*  pout = outbuff;

			ret = convert_("UTF-8" , "UCS-2LE",&pin ,(size_t*)&inbytes ,  &pout , (size_t*)&outbytes);
			out_len = buffbytes - outbytes;
			outbuff[out_len] = '\0';
		}       
	}catch(...)
	{
		ret = -1;
	}
	return ret;
}

extern
int bbiconv_wchar_2_utf8_s(const wchar_t* in , size_t in_len, std::string& outbuffer)
{
	size_t outlen = bbiconv_counts_wchar_2_utf8(in , in_len);
	if( outlen <= 0 )
		return -1;

	char* out_ptr = new char[outlen+1];
	if( !out_ptr )
		return -1;

	bbiconv_wchar_2_utf8(in , in_len , out_ptr , outlen);

	outbuffer = out_ptr;

	delete[] out_ptr;

	return 0;
}


//////////////////////////////////////////////////////////////////////////////
//



// caculate WCHAR_T to GBK COUNTS
extern
size_t bbiconv_counts_wchar_2_gbk(const wchar_t* in ,size_t in_len)
{
	size_t out_len = 0;
	const wchar_t* end = in + in_len;
	const wchar_t* byte= in;
	while( byte < end )
	{
		wchar_t a = *byte ++;
		if( (  a <= 0x7F  ) ){
			out_len ++;
		}
		else 
		{
			out_len ++;
			out_len ++;
		}
	}
	return out_len;
}


extern 
int bbiconv_wchar_2_gbk(const wchar_t* in , size_t in_len, char* outbuffer,size_t& out_len)
{
	int ret = 0;
	try 
	{
		size_t buffbytes = out_len;
		size_t outbytes  = buffbytes;
		if( buffbytes > 0 )
		{
			char* pin = (char*)in;
			size_t inbytes  = in_len * sizeof(wchar_t);
			char*  pout = outbuffer;

			ret = convert_("GBK" , "UCS-2LE",&pin ,(size_t*)&inbytes ,  &pout , (size_t*)&outbytes);
			out_len = buffbytes - outbytes;
			outbuffer[out_len] = '\0';
		}       
	}
	catch(...)
	{
		ret = -1;
	}
	return ret;
}

extern 
int bbiconv_wchar_2_gbk_s(const wchar_t* in , size_t in_len, std::string& outbuffer)
{
	size_t outlen = bbiconv_counts_wchar_2_gbk(in , in_len);
	if( outlen <= 0 )
		return -1;

	char* out_ptr = new char[outlen+1];
	if( !out_ptr )
		return -1;

	bbiconv_wchar_2_gbk(in , in_len , out_ptr , outlen);

	outbuffer = out_ptr;

	delete[] out_ptr;

	return 0;
}


// caculate GBK to WCHAR_T COUNTS
extern
size_t bbiconv_counts_gbk_2_wchar(const char* in ,size_t in_len)
{
	return 	(sizeof(wchar_t)*in_len);
}

extern
int bbiconv_gbk_2_wchar(const char* in , size_t in_len, wchar_t* outbuffer,size_t& out_len)
{
	int ret = 0;
	try 
	{
		size_t buffbytes = out_len;
		size_t outbytes  = buffbytes;
		if( buffbytes > 0 )
		{
			char* pin = (char*)in;
			size_t inbytes  = in_len * sizeof(wchar_t);
			char*  pout = (char*)outbuffer;

			ret = convert_("UCS-2LE" , "GBK",&pin ,(size_t*)&inbytes ,  &pout , (size_t*)&outbytes);
			out_len = buffbytes - outbytes;
			outbuffer[out_len] = '\0';
		}       
	}catch(...)
	{
		ret = -1;
	}
	return ret;
}

extern
int bbiconv_gbk_2_wchar_s(const char* in , size_t in_len, std::wstring& outbuffer)
{
	size_t outlen = bbiconv_counts_gbk_2_wchar(in , in_len);
	if( outlen <= 0 )
		return -1;

	wchar_t* out_ptr = new wchar_t[outlen+outlen+sizeof(wchar_t)];
	if( !out_ptr )
		return -1;

	bbiconv_gbk_2_wchar(in , in_len , out_ptr , outlen);

	outbuffer = out_ptr;

	delete[] out_ptr;

	return 0;
}



#endif // #if !(defined(WIN32) || defined(_WIN32) )

