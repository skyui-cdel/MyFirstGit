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
* �˿�����ɶ�������ѧϰ֮��,����������ҵ��;
* ���������ҵ��;,һ�鷨��׷���Լ��е�!
*
* ���ߣ� loach.h ( ������)
*
* ʱ�䣺 2009-08-20
*/

#include <common/loCommon.h>

DEFINE_NAMESPACE(locom)

/**
 * @brief �ļ�/�ļ��в���io
 *
 * @author loach
 *
 * @date 2007-09-10
*/
class loCOMMON_API CloFile
{
public:
	/** ��ȡӲ�̿ռ� */
	static __int64 GetHDFreeSpace(const loTCHAR* lpszDisk);

	/**
	 * @brief �ļ�������
	*/
	static bool FileExists(const wchar_t* lpszFileName);
	static bool IsFileExists(const loTCHAR* lpszFileName);

	/** �Ƿ��ļ�Ŀ¼���� */
	static bool IsDirectoryExists(const loTCHAR* strFileName);

	/**
	 * @brief ���ļ�������·��
	 * @return 0 �ļ��У�-1 ������,1 �ļ�
	*/
	static int  IsPathOrFile(const loTCHAR* lpszFileName);

	/** �����ļ��� */	
	static bool CreateFolder(const loTCHAR* szFolder);
	static bool CreateDirectory(const loTCHAR* lpPath);

	/** �ļ���С */
	static long long GetFileSize(const loTCHAR* szFileName);
};

END_NAMESPACE(locom)

#endif /* __lo_FILEIO_H__ */