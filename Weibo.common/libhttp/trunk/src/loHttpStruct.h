#ifndef __lo_HTTPSTRUCT_H__
#define __lo_HTTPSTRUCT_H__

#include "loHttpDef.h"

namespace lohttp
{
	//
#define Http_Char_New(p,size) \
	(p) = new HTTPChar[size]; 

	//
#define Http_Char_Free(p) \
	{ if(p) delete[] (p); (p) = NULL ;}

	// add by loach �ͷŵ��ڴ� 2009-07-27
#define SAFE_DELETE_MEMORYBUFFER(mbuff)\
	{\
	   if( (mbuff)->buffer && (mbuff)->size > 0 ){\
	   delete[] (mbuff)->buffer;\
	   (mbuff)->buffer = NULL;\
	   (mbuff)->size = 0;}\
    }

	// new �ڴ�  2009-07-27
#define SAFE_NEW_MEMORYBUFFER(mbuff,buffsize,len)\
	{\
	   SAFE_DELETE_MEMORYBUFFER(mbuff);\
	   (mbuff)->buffer = new char[buffsize];\
	   (mbuff)->size = len;\
	}

	// copy memory buffer
#define COPY_MEMORYBUFFER(msrbuff,mdestbuff)\
	{\
	   if( (msrbuff)->buffer && (msrbuff)->size >= 0 && (mdestbuff)->buffer && (mdestbuff)->size >= (msrbuff)->size )\
	   memcpy( (mdestbuff)->buffer, (msrbuff)->buffer, (msrbuff)->size);\
	}

#define SAFE_FREE_BUFFER(p)    if( p ) { free(p); (p) = NULL; }

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)		   if( p ) { delete (p); (p)=NULL;}
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p)	if( p ) { delete[] (p); (p)=NULL;}
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)		   if( p ) { (p)->Release(); (p) = NULL; } 
#endif

//
#if !(defined(WIN32)) && !(defined(_WIN32))

#define strcpy_s(dst,count,src) strcpy(dst,src)

#endif //!(defined(WIN32)) && !(defined(_WIN32))


	/**
	* �������Ͷ���
	*/
	enum EHttpTransferType
	{
		E_HTTPTYPE_GET,			// GET����
		E_HTTPTYPE_DOWNLOAD,		// DOWNLOAD�������ص��ļ��ĺ�����
		E_HTTPTYPE_POST,			// ��POST����POST������Ҫָ��Ϊ���ͷǱ���
		E_HTTPTYPE_POST_FORMDATA,	// POST�����ļ���
		E_HTTPTYPE_POST_TEXT_FILE,//post���������ı�����
//		E_HTTPTYPE_UPLOAD,		// UPLOAD�����ļ���HTTP1.1��method
		E_HTTPTYPE_DELETE,		// DELETE method,HTTP 1.1
		E_HTTPTYPE_PUT,			// PUT method,HTTP 1.1
	};

#pragma pack(push)
#pragma pack(4)

	/**
	* @˵�����Զ����ڴ�Buffer�ṹ�塣
	*/
	typedef struct _tagHttpMemoryBuffer
	{
		char *	buffer;				// Buffer�ڴ��ַ
		size_t	size;				// Buffer��С
		_tagHttpMemoryBuffer()
		{
			memset(this , 0 , sizeof(*this));
		}
	}THttpMemoryBuffer;

	/**
	* @�����ļ�ʱ�õ��Ĳ����ṹ�壬�ⲿ�����ù��ġ�
	*/
	typedef struct _tagHttpFileBuffer
	{
		char	fileName[1024];		// ��Զ�̻�ȡ���ļ�������
		size_t	fileSize;			// ��Զ�̻�ȡ���ļ���С
		FILE *	fp;					// File point
		size_t	offset;				// �����ļ���ƫ����
		char *	buffer;				// ���ݻ��壬��һ��Ҫ�õ�
		size_t	size;				// ���ݻ����С
		_tagHttpFileBuffer()
		{
			memset(this , 0 , sizeof(*this));
		}
	}THttpFileBuffer;

	const int ConstProxyServerLength = 256;
	const int ConstProxyUserLength = 256;
	const int ConstProxyPwdLength = 128;
	typedef struct _tagHttpProxyInfo	// ������Ϣ�ṹ��
	{
		int		nProxyType;
		unsigned short	wPort;				// ����������˿�
		char	cServer[ConstProxyServerLength];		// �����������ַ
		char	cUser[ConstProxyUserLength];			// ����������û�
		char	cPassword[ConstProxyPwdLength];		// ����������û�����

		_tagHttpProxyInfo()
		{
			memset(this , 0 , sizeof(*this));
			wPort = 808;
		}
	}THttpProxyInfo;


	const int ConstLocalFileLength = 1024;
	const int ConstFormNameLength = 256;
	const int ConstFormContentLength = 1024;
	const int ConstHeadLength = 4096;
	const int ConstURLLength = 2048;
	const int ConstUploadParamLength = 1024;


	/**
	* @˵����HttpClient�������������ԵĽṹ�塣
	* @��ע��TTaskParam�ṹ��������Ĺ��캯���������������ⲿ������Ҫ���ֶ�
	* 		delete pDataֵ�������ⲿ��������ǲ�Ҫʹ��memset��������ʹ����ĳ�ʼ������
	* 		Ч��
	*		TTaskParam���С�operator =�����ܡ�
	*/
	typedef struct _tagTaskParam
	{
		char	cURL[ConstURLLength];			// ���ʵ�ַ
		char	cHead[ConstHeadLength];		// Httpͷ

		// �ϴ��ļ��Ĳ���
//		char	cLocalFile[ConstLocalFileLength];	// �����ļ����Ե�ַ��POST�ļ���UPLOAD�ļ���

//		char    cFormName[ConstFormNameLength];
//		char    cFormContents[ConstFormContentLength];        //�����ļ��������������ı����ŵ�post����

		// ���������õĲ���		
		char	cSavePath[ConstLocalFileLength];		// ���ر����ļ�·��
		char	cNewFileName[ConstLocalFileLength];		// ���ر�����ļ���
		BOOL	bRename;				// �Ƿ�ʹ��ָ�������ļ���
        BOOL	bToBuffer;				// ֻ���ص�Buffer��

		// POST,DELETE,PUT��
		unsigned int uDataLength;		// POST���ݳ���
		char*	pData;				// POST����
		BOOL	bFormData;		// ʹ�ñ���ʽPOST

		THttpForm form;
		/**
		* @�ṹ��ʼ��
		*/
		_tagTaskParam()
		{
			memset(this , 0 , sizeof(*this));
			bFormData = TRUE;
		}

		~_tagTaskParam()
		{
			if( pData )
			{
				delete[] pData;
				pData = NULL;
			}
		}

		/**
		* @��ֵ����
		*/
		void operator = (const _tagTaskParam& value) throw()
		{
			strcpy_s(cURL, ConstURLLength, value.cURL);
			strcpy_s(cHead,ConstHeadLength,value.cHead);

//			strcpy_s(cFormContents ,value.cFormContents);
//			strcpy_s(cFormName,value.cFormName);

//			strcpy_s(cLocalFile, value.cLocalFile);

			// POST,DELETE,PUT��
			if( value.uDataLength > 0 && value.pData )
			{
				uDataLength = value.uDataLength;
				pData = new char[value.uDataLength+1];
				memcpy(pData, value.pData, value.uDataLength);
				pData[uDataLength] = '\0';
			}
			bFormData = value.bFormData;

			// ���������õĲ���
			bToBuffer = value.bToBuffer;

			strcpy_s(cSavePath ,  ConstLocalFileLength, value.cSavePath);
			strcpy_s(cNewFileName,ConstLocalFileLength, value.cNewFileName);
			bRename = value.bRename;

		    memcpy(&form , &value.form, sizeof(THttpForm) );
		};
	}TTaskParam;

	// http ����
	typedef struct _tagHttpURLParam
	{
		HTTPChar*  pszName;
		HTTPChar*  pszValue;
		unsigned long	dwAttrib;
		_tagHttpURLParam()
		{
			pszName = NULL;
			pszValue = NULL;
			dwAttrib = 0;
		};
		~_tagHttpURLParam()
		{
			Http_Char_Free( pszName );
			Http_Char_Free( pszValue );
		};
	}THttpURLParam;

	typedef struct _tagHttpHeader
	{
		HTTPChar* pszName;
		HTTPChar* pszValue;

		_tagHttpHeader()
		{
			pszName = NULL;
			pszValue = NULL;
		};
		~_tagHttpHeader()
		{
			Http_Char_Free( pszName );
			Http_Char_Free( pszValue );
		};
	}THttpHeader;
    
#pragma pack(pop)

} // namespace lohttp


// ���û�ж��� �ַ��������� �����ַ���
#ifndef BUILD_CHARACTER_SET

#define HTTP_TCSCPY_S       strcpy_s
#define HTTP_TCSLEN         strlen
#define HTTP_TTOL           atol
#define HTTP_TCSCHR         strchr
#define HTTP_CT2A(s)        s 
#define HTTP_CA2T(s)        s
#define HTTP_STR_TO_UTF8(s) MbcsToUtf8(s)
#define HTTP_HTString       std::string

#else

#define HTTP_TCSCPY_S       _tcscpy_s
#define HTTP_TCSLEN         _tcslen
#define HTTP_TTOL           _ttol
#define HTTP_TCSCHR         _tcschr
#define HTTP_CT2A(s)        CT2A(s) 
#define HTTP_CA2T(s)        CA2T(s)
#define HTTP_STR_TO_UTF8(s) STR_TO_UTF8(s)
#define HTTP_HTString       HTString

#endif

#endif // __lo_HTTPSTRUCT_H__
