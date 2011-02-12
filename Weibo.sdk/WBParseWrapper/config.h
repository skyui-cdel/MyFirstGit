#pragma once

#if defined(WIN32) || defined(_WIN32_WCE)

// ±‡“Î∆˜∞Ê±æ∫≈≈–∂œ
#ifdef _MSC_VER
#	if _MSC_VER <= 1400  // 1400 == VC++ 8.0
#	define WEIBO_VC80
#	elif _MSC_VER <= 1500 // 1500 == VC++ 9.0
#	define WEIBO_VC90
#	endif //_MSC_VER <= 1400 
#endif //_MSC_VER

#endif //WIN32