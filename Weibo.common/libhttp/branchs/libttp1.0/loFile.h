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
	static ULONGLONG GetHDFreeSpace(LPCTSTR lpszDisk);

	/**
	 * @brief �ļ�������
	*/
	static BOOL FileExists(LPCWSTR lpszFileName);
	static BOOL IsFileExists(LPCTSTR lpszFileName);

	/** �Ƿ��ļ�Ŀ¼���� */
	static BOOL IsDirectoryExists(LPCTSTR strFileName);

	/**
	 * @brief ���ļ�������·��
	 * @return 0 �ļ��У�-1 ������,1 �ļ�
	*/
	static int  IsPathOrFile(LPCTSTR lpszFileName);

	/** �����ļ��� */	
	static BOOL CreateFolder(LPCTSTR szFolder);
	static BOOL CreateDirectory(LPCTSTR lpPath);
};

END_NAMESPACE(locom)

#endif /* __lo_FILEIO_H__ */