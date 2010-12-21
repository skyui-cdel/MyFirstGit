#ifndef __locomm_DLLPLUGIN_H__
#define __locomm_DLLPLUGIN_H__

#if defined(WIN32) || defined(WINCE)
#include <windows.h>
#endif

#include <tchar.h>
#include <assert.h>
#include <common/loCommon.h>

DEFINE_NAMESPACE(locom)

/** dll ²å¼þÀà */
class loCOMMON_API CloDLLPlugin
{
public:
	CloDLLPlugin()
		:hDll_(NULL)
	{		
	}
	CloDLLPlugin(const loTCHAR* szDll)
		:hDll_(NULL)
	{
		Init(szDll);
	}
	~CloDLLPlugin(void)
	{
		UnInit();
	}

	int Init(const loTCHAR* szDll)
	{
		if( !hDll_ )
		{
#if defined(WIN32) || defined(WINCE)
			hDll_ = LoadLibrary(szDll);
#else

#endif
			assert(hDll_);
		}
		return hDll_?0:-1;
	}

	int UnInit()
	{
		if( hDll_ )
		{
#if defined(WIN32) || defined(WINCE)
			::FreeLibrary(hDll_);
#else
#endif
			hDll_ = NULL;
		}
		return 0;
	}

	HMODULE_lo DLL() const
	{
		return hDll_;
	}
private:
	HMODULE_lo hDll_;
};

END_NAMESPACE(locom)

#endif //__locomm_DLLPLUGIN_H__