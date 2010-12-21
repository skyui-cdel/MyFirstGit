#ifndef __lo_VERSION_H__
#define __lo_VERSION_H__

#include <common/loCommon.h>

DEFINE_NAMESPACE(locom)

/** �汾 */
class loCOMMON_API CloVersion
{
public:
	CloVersion();
	~CloVersion();

	/**
	* @brief ��ȡ��Դ�İ汾��Ϣ
	*/
	BOOL Read(HINSTANCE hResource);

	/**
	* @brief ��ȡһ���ļ��еİ汾��Ϣ
	*/
	BOOL Read(const loTCHAR* sFile);
	/**
	* @param nVer   == 1  �߰汾��
	*               == 2  �Ͱ汾��
	*               == -1 ��ȡ����
	*/
	const loTCHAR* GetVersionString(int nVer=-1);


	static BOOL StrVersionToDWord(const loTCHAR* lpszVer,DWORD& dwMS,DWORD& dwLS );

	enum Win32SysType
	{  
		WindowsNo = -1,///< ��Ч
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