#ifndef __lo_HTTP_THREAD_H__
#define __lo_HTTP_THREAD_H__

#include <common/lo/lodefine.h>

#if defined(WIN32) || defined(_WIN32)

#define  STDCALLTYPE    __stdcall
typedef HANDLE 		    THREAD_HANDLE;
typedef unsigned long  	THREAD_RETURN_TYPE;

#define HTTPTHREAD_SET_EVENT(h)

#else

#define  STDCALLTYPE 

typedef void*    THREAD_HANDLE;
typedef void*    THREAD_RETURN_TYPE;

typedef THREAD_RETURN_TYPE (*PTHREAD_START_ROUTINE)(
    void* lpThreadParameter
    );
typedef PTHREAD_START_ROUTINE LPTHREAD_START_ROUTINE;


#define WAIT_OBJECT_0		0
#define INFINITE                -1

// Thread function 
THREAD_HANDLE CreateThread(void* security,DWORD dwStack,LPTHREAD_START_ROUTINE pStartAddr,
		void* lpvThreadParam,DWORD fdwCreate,LPDWORD lpIDThread );


// close handle
BOOL CloseHandle(THREAD_HANDLE hHandle);

//
int WaitForSingleObject(THREAD_HANDLE hHandle,DWORD dwMilliseconds);

// terminate
BOOL TerminateThread(THREAD_HANDLE hHandle,DWORD dwExitCode);

// 
void Sleep(DWORD dwMilliseconds);


// set the event ,
BOOL SetEvent(THREAD_HANDLE hHandle);


#define HTTPTHREAD_SET_EVENT(h) SetEvent(h)



#endif //defined(WIN32) || defined(_WIN32)



#endif //__lo_HTTP_THREAD_H__
