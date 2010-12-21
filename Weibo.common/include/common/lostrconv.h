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
* �˿�����ɶ�������ѧϰ֮��,����������ҵ��;
* ���������ҵ��;,һ�鷨��׷���Լ��е�!
*
* ���ߣ� loach.h ( ������)
*
* ʱ�䣺 2009-08-20
*/

#include <string>
#include <stdio.h>
#include <common/loCommon.h>

#if (defined(WIN32)) || (defined(_WIN32))

#include <tchar.h>

#endif //(defined(WIN32)) || (defined(_WIN32))


DEFINE_NAMESPACE(locom)

/** ���ַ���ת�� */
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
	 * ��UNICODE ������� ת��� " \uxxxx "
	 *
	*/
	void UicodeTransfer(std::string& out,const wchar_t* unicode , size_t buffLen );

	/**
	 * ��" \uxxxx " ת��� �������  unicode
	 *
	*/
	void TransferUicode(std::string& out,const char* transfer , size_t buffLen );


	/** �Ƿ��������ַ */
	bool isemail(const char *str);

	/** �Ƿ�绰���� */
	bool isphone(const char *str);

	/** �Ƿ����ַ���*/
	bool isalpha(const char *str);

	/** �Ƿ������� */
	bool isalnum(const char* str);

	/** �Ƿ���0-9���� */
	bool isdigit(const char *str);

	/*
	���ݷָ�������ȡ�ַ���
	*/
	static void ExtractString( const std::wstring &seperate,const std::wstring &in,std::wstring &out);
};

END_NAMESPACE(locom)

#endif /* __locom_STRCONV_H__ */


