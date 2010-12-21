#ifndef __BB_ICONV_H__
#define __BB_ICONV_H__

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

/**
* 字符串转化 , 通过回调函数，你可把转化的结果写入文件，写入内存...
* 可以直接使用单实例对象 Instance() , 也可自己创建...
*
* @author loach
*
* @date 2009-08-01
* 
*
*/
#ifdef __cplusplus
extern "C" {
#endif

	/** 估算[不精确] utf8 to gbk 字符串需要的长度是多少，以便分配合理的内存 */
	extern size_t bbiconv_counts_utf8_2_gbk(const char* in ,size_t in_len);

	/** utf8 to gbk 转化后，会通过 pwrite 回调用函数写到 pobject 中 */
	extern int bbiconv_utf8_2_gbk(const char* in , size_t in_len  , char* outbuffer,size_t& out_len);
	extern int bbiconv_utf8_2_gbk_s(const char* in , size_t in_len  , std::string& outbuffer);

	/** 估算[不精确] gbk to utf8 字符串需要的长度是多少，以便分配合理的内存 */
	extern size_t bbiconv_counts_gbk_2_utf8(const char* in , size_t in_len);

	/** gbk to utf8 转化后，会通过 pwrite 回调用函数写到 pobject 中 */
	extern int bbiconv_gbk_2_utf8(const char* in , size_t in_len  , char* outbuffer,size_t& out_len);
	extern int bbiconv_gbk_2_utf8_s(const char* in , size_t in_len  , std::string& outbuffer);

	/** 估算[不精确] utf8 to ucs2 字符串需要的长度是多少
	* 为了再转化前，可以动态计算出需要的长度，然后好分配内存.
	* 返回长度 = wchar_t 的长度 , char length = size_t*sizeof(wchar_t)
	*/
	extern size_t bbiconv_counts_utf8_2_wchar(const char* in , size_t in_len);

	/** utf8 to wchar_t , 转化后，会通过 pwrite 回调用函数写到 pobject 中 */
	extern int bbiconv_utf8_2_wchar(const char* in , size_t in_len, wchar_t* outbuffer,size_t& out_len);
	extern int bbiconv_utf8_2_wchar_s(const char* in , size_t in_len, std::wstring& outbuffer);


	/**  估算[不精确] ucs2 to utf8 字符串需要的长度是多少，以便分配合理的内存 */
	extern size_t bbiconv_counts_wchar_2_utf8(const wchar_t* in ,size_t in_len);

	/** wchar to utf8 转化后，会通过 pwrite 回调用函数写到 pobject 中 */
	extern int bbiconv_wchar_2_utf8(const wchar_t* in , size_t in_len, char* outbuffer,size_t& out_len);
	extern int bbiconv_wchar_2_utf8_s(const wchar_t* in , size_t in_len, std::string& outbuffer);


	/**  估算[不精确] wchar_t to mbcs 字符串需要的长度是多少，以便分配合理的内存 */
	extern size_t bbiconv_counts_wchar_2_gbk(const wchar_t* in ,size_t in_len);

	/** wchar to gbk .*/
	extern int bbiconv_wchar_2_gbk(const wchar_t* in , size_t in_len, char* outbuffer,size_t& out_len);
	extern int bbiconv_wchar_2_gbk_s(const wchar_t* in , size_t in_len, std::string& outbuffer);


	/**  估算[不精确] mbcs to wchar_t 字符串需要的长度是多少，以便分配合理的内存 */
	extern size_t bbiconv_counts_gbk_2_wchar(const char* in ,size_t in_len);

	/** gbk to wchar .*/
	extern int bbiconv_gbk_2_wchar(const char* in , size_t in_len, wchar_t* outbuffer,size_t& out_len);
	extern int bbiconv_gbk_2_wchar_s(const char* in , size_t in_len, std::wstring& outbuffer);

#ifdef __cplusplus
}
#endif

#endif /* __BB_ICONV_H__ */

