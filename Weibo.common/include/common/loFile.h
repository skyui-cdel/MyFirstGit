#ifndef __lo_FILEIO_H__
#define __lo_FILEIO_H__

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

#include <common/loCommon.h>

DEFINE_NAMESPACE(locom)

/**
 * @brief 文件/文件夹操作io
 *
 * @author loach
 *
 * @date 2007-09-10
*/
class loCOMMON_API CloFile
{
public:
	/** 获取硬盘空间 */
	static __int64 GetHDFreeSpace(const loTCHAR* lpszDisk);

	/**
	 * @brief 文件虽否存在
	*/
	static bool FileExists(const wchar_t* lpszFileName);
	static bool IsFileExists(const loTCHAR* lpszFileName);

	/** 是否文件目录存在 */
	static bool IsDirectoryExists(const loTCHAR* strFileName);

	/**
	 * @brief 是文件，还是路径
	 * @return 0 文件夹，-1 不存在,1 文件
	*/
	static int  IsPathOrFile(const loTCHAR* lpszFileName);

	/** 创建文件夹 */	
	static bool CreateFolder(const loTCHAR* szFolder);
	static bool CreateDirectory(const loTCHAR* lpPath);

	/** 文件大小 */
	static long long GetFileSize(const loTCHAR* szFileName);
};

END_NAMESPACE(locom)

#endif /* __lo_FILEIO_H__ */