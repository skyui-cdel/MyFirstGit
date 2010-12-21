#ifndef __locom_STRCONV_H__
#define __locom_STRCONV_H__

///////////////////////////////////////////////////////
//	***             * * * *      ****        ****    //
//	***  	      *         *     **          **     //
//	***  	     *           *    **          **     //
//	***  	     *           *    **************     //
//	***	     **  *           *    **          **     //
//	***********   *         *     **          **     //
//	***********	    * * * *      ****        ****    // 
///////////////////////////////////////////////////////2009-08-20 @loach
/*
*
* 此库可以由读者用于学习之用,不能用于商业用途
* 如果用在商业用途,一块法律追究自己承担!
*
* 作者： loach.h ( 胡秋云)
*
* 时间： 2009-08-20
*/

#include <string>
#include <stdio.h>
#include <common/loCommon.h>

#if (defined(WIN32)) || (defined(_WIN32))

#include <tchar.h>

#endif //(defined(WIN32)) || (defined(_WIN32))


DEFINE_NAMESPACE(locom)

/** 简单字符串转化 */
class loCOMMON_API CloStrConv
{
public:
	static void unicode_2_utf8( std::string& outstr, const wchar_t* wstr );
	static void utf8_2_unicode( std::wstring& woutstr,const char* utf8 );

    /** Convert  utf8 to microsoft unicode
	*/
	std::wstring Utf8ToUnicode(const char *zFilename);

	/*
	** Convert microsoft unicode to UTF-8.  Space to hold the returned string is
	** obtained from malloc().
	*/
	std::string UnicodeToUtf8(const wchar_t *zWideFilename);


	/*
	** Convert an ansi string to microsoft unicode, based on the
	** current codepage settings for file apis.
	** 
	** Space to hold the returned string is obtained
	** from malloc.
	*/
	std::wstring MbcsToUnicode(const char *zFilename);

	/*
	** Convert microsoft unicode to multibyte character string, based on the
	** user's Ansi codepage.
	**
	** Space to hold the returned string is obtained from
	** malloc().
	*/
	std::string UnicodeToMbcs(const wchar_t *zWideFilename);


	/*
	** Convert multibyte character string to UTF-8.  Space to hold the
	** returned string is obtained from malloc().
	*/
	std::string MbcsToUtf8(const char *zFilename);

	/*
	** Convert UTF-8 to multibyte character string.  Space to hold the 
	** returned string is obtained from malloc().
	*/
	std::string Utf8ToMbcs(const char *zFilename);

	/**
	 * 把UNICODE 串输出成 转义符 " \uxxxx "
	 *
	*/
	void UicodeTransfer(std::string& out,const wchar_t* unicode , size_t buffLen );

	/**
	 * 把" \uxxxx " 转义符 串输出成  unicode
	 *
	*/
	void TransferUicode(std::string& out,const char* transfer , size_t buffLen );


	/** 是否是邮箱地址 */
	bool isemail(const char *str);

	/** 是否电话号码 */
	bool isphone(const char *str);

	/** 是否是字符串*/
	bool isalpha(const char *str);

	/** 是否是数字 */
	bool isalnum(const char* str);

	/** 是否是0-9数字 */
	bool isdigit(const char *str);

	/*
	根据分隔符来抽取字符串
	*/
	static void ExtractString( const std::wstring &seperate,const std::wstring &in,std::wstring &out);
};

END_NAMESPACE(locom)

#endif /* __locom_STRCONV_H__ */


