#ifndef __lo_VERSION_H__
#define __lo_VERSION_H__

#include <common/loCommon.h>

DEFINE_NAMESPACE(locom)

/** 版本 */
class loCOMMON_API CloVersion
{
public:
	CloVersion();
	~CloVersion();

	/**
	* @brief 读取资源的版本信息
	*/
	BOOL Read(HINSTANCE hResource);

	/**
	* @brief 读取一个文件夹的版本信息
	*/
	BOOL Read(const loTCHAR* sFile);
	/**
	* @param nVer   == 1  高版本号
	*               == 2  低版本号
	*               == -1 获取所有
	*/
	const loTCHAR* GetVersionString(int nVer=-1);


	static BOOL StrVersionToDWord(const loTCHAR* lpszVer,DWORD& dwMS,DWORD& dwLS );

	enum Win32SysType
	{  
		WindowsNo = -1,///< 无效
		Windows32s,  
		WindowsNT3,  
		Windows95,  
		Windows98,  
		WindowsME,  
		WindowsNT4,  
		Windows2000,  
		WindowsXP,
		WindowsVista,
	};
	static Win32SysType GetShellType();

private:
	friend class CloVersionAssoc;
	CloVersionAssoc* _pAssoc;
};

END_NAMESPACE(locom)

#endif //__lo_VERSION_H__