#include "lofile.h"
#include <windows.h>
#include <mmsystem.h>
#include <tchar.h>
#include <atlstr.h>

DEFINE_NAMESPACE(locom)

ULONGLONG CloFile::GetHDFreeSpace(LPCTSTR lpszDisk)
{
	CString strDisk = lpszDisk;
	strDisk = strDisk.Left(3);

	DWORD dwSectorsPerCluster;
	DWORD dwBytesPerSector;
	DWORD dwNumberOfFreeClusters;
	DWORD dwTotalNumberOfClusters;

	if( !::GetDiskFreeSpace(strDisk, &dwSectorsPerCluster, &dwBytesPerSector,
		&dwNumberOfFreeClusters, &dwTotalNumberOfClusters) )
		return 0;

	return ((ULONGLONG)dwNumberOfFreeClusters * (ULONGLONG)dwSectorsPerCluster * (ULONGLONG)dwBytesPerSector);
}


BOOL CloFile::IsFileExists(LPCTSTR lpszFileName)
{
	return ( 1 != IsPathOrFile(lpszFileName) ) ? FALSE : TRUE;
}

BOOL CloFile::IsDirectoryExists(LPCTSTR lpszFileName)
{
	return ( 0 != IsPathOrFile(lpszFileName) ) ? FALSE: TRUE;
}

BOOL CloFile::FileExists(LPCWSTR lpszFileName)
{
	DWORD  dwFlag = GetFileAttributesW(lpszFileName); 
	if ( 0xFFFFFFFF == dwFlag ) 
		return FALSE;//不存在
	if (  FILE_ATTRIBUTE_DIRECTORY & dwFlag ) 
		return FALSE;//文件夹
	return TRUE; //文件
}

int CloFile::IsPathOrFile(LPCTSTR lpszFileName)
{
	DWORD  dwFlag = GetFileAttributes(lpszFileName); 
	if ( 0xFFFFFFFF == dwFlag ) 
		return -1;//不存在
	if (  FILE_ATTRIBUTE_DIRECTORY & dwFlag ) 
		return 0;//文件夹
	return 1; //文件
}

BOOL CloFile::CreateDirectory(LPCTSTR lpPath)
{
	BOOL bRet = TRUE;
	CString szPath = lpPath;
	if( szPath.Right(1) != _T("\\") )
		szPath += _T("\\");

	BOOL bRes = FALSE;
	for(int nPos = 0; ((nPos < szPath.GetLength())); ++nPos)
	{		
		nPos = szPath.Find(_T("\\"), nPos);
		if(nPos < 0)
		{
			break;
		}
		CString subpath = szPath.Left(nPos);
		bRes = ::CreateDirectory( subpath , NULL);		
	}
	bRes |= (GetLastError() == ERROR_ALREADY_EXISTS );
	return bRet;
}

BOOL CloFile::CreateFolder(LPCTSTR szFolder)
{
	if (!szFolder || !lstrlen(szFolder))
		return FALSE;

	DWORD dwAttrib = GetFileAttributes(szFolder);

	// already exists ?
	if (dwAttrib != 0xffffffff)
		return ((dwAttrib & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY);

	// if can't create parent
	if(::CreateDirectory(szFolder,NULL)){
		dwAttrib = 0;			
	}

	return (0==dwAttrib);
}

END_NAMESPACE(locom)