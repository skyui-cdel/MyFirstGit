#define CharT                              wchar_t
#define STD_TString                        std::wstring
#define _TCSLEN                            wcslen

#if !(defined(WIN32)) && !(defined(_WIN32))

int _wtoi( const wchar_t *_Str)
{
	return 0;
}

//
double _wtof(const wchar_t *_Str)
{
	return 0;
}

#endif //0

#define _TSTOI                             _wtoi
#define _TSTOF                             _wtof

#define PLineParseCBT_lo                   PLineParseCBW_lo
#define ParserT_lo                         CloParserW
#define loAssocRowT                        CloAssocRowW

