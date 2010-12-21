#include "Stdafx.h"
#include "loHttpThread.h"
#if !defined(WIN32) && !defined(_WIN32)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <errno.h>
#include <unistd.h>


/**
 * @brief  线程封装结构句柄
 * @author welbon
 * @date   2010-09-14
*/
typedef struct 
{
	pthread_t			tid_;
	pthread_mutex_t		mutex_;
	pthread_cond_t		cond_;
	
} THREAD_STR,*LPTHREAD_TAG;

typedef void*    THREAD_RETURN_TYPE;
typedef THREAD_RETURN_TYPE (*PTHREAD_START_ROUTINE)(
    void* lpThreadParameter
    );
typedef PTHREAD_START_ROUTINE LPTHREAD_START_ROUTINE;


// Thread function 
THREAD_HANDLE CreateThread(void* security,
		DWORD dwStack,
		LPTHREAD_START_ROUTINE pStartAddr,
		void* lpvThreadParam,
		DWORD fdwCreate,
		LPDWORD lpIDThread )
{
	LPTHREAD_TAG pt = (LPTHREAD_TAG)malloc(sizeof(THREAD_STR));
	memset(pt,0,sizeof(THREAD_STR));

	
	if( 0 == pthread_create(&pt->tid_,NULL,pStartAddr,lpvThreadParam) )
	{
		// create thread successed.
		//
		// 创建对应的事�?		
		pthread_mutexattr_t attr;
		pthread_mutexattr_init(&attr);
		pthread_mutex_init(&pt->mutex_, &attr);
		pthread_mutexattr_destroy(&attr);
		pthread_cond_init(&pt->cond_, NULL);

		return pt;	
	}
	return 0;
}

// close handle
BOOL CloseHandle(THREAD_HANDLE hHandle)
{
	if( !hHandle) 
		return FALSE;

	LPTHREAD_TAG pt = (LPTHREAD_TAG)hHandle;

	pthread_cond_destroy(&pt->cond_);
	pthread_mutex_destroy(&pt->mutex_);
	free(pt);

	return TRUE;
}

int WaitForSingleObject(THREAD_HANDLE hHandle,DWORD dwMilliseconds)
{
	if( !hHandle )
		return -1;

	LPTHREAD_TAG pt = (LPTHREAD_TAG)hHandle;

	if (dwMilliseconds == (uint32)-1)
	{
		//使线程阻塞在一个条件变量的互斥锁上，无条件等待
		pthread_mutex_lock(&pt->mutex_);
		pthread_cond_wait(&pt->cond_, &pt->mutex_);
		pthread_mutex_unlock(&pt->mutex_);
		return 0;
	}

	struct timeval now;      /*time when we started waiting*/ 
	struct timespec timeout; /*timeout value for the wait function */ 

	pthread_mutex_lock(&pt->mutex_);		//Lock
	//取当前时�?	gettimeofday(&now, NULL); 
	//准备时间间隔�?       
	timeout.tv_sec  = now.tv_sec    + dwMilliseconds / 1000; 
	timeout.tv_nsec = ((now.tv_usec + dwMilliseconds) % 1000) * 1000;        

	//使线程阻塞在一个条件变量的互斥锁上，计时等�?
	int ldwResult = pthread_cond_timedwait(&pt->cond_, &pt->mutex_, &timeout);
	pthread_mutex_unlock(&pt->mutex_);		//UnLock
	if(ldwResult == ETIMEDOUT)
	{
		return -1;
	}
	return WAIT_OBJECT_0;
}

BOOL TerminateThread(THREAD_HANDLE hHandle,DWORD dwExitCode)
{
	if( !hHandle )
		return FALSE;

	LPTHREAD_TAG pt = (LPTHREAD_TAG)hHandle;
	pthread_exit((void*)pt->tid_ );

	return TRUE;
}


BOOL SetEvent(THREAD_HANDLE hHandle)
{
	if( !hHandle )
		return FALSE;

	LPTHREAD_TAG pt = (LPTHREAD_TAG)hHandle;
	pthread_mutex_lock(&pt->mutex_);

	//唤醒所有被阻塞在条件变量mhCond_t上的线程�?	pthread_cond_broadcast(&pt->cond_);
	pthread_mutex_unlock(&pt->mutex_);

	return TRUE;
}

void Sleep(DWORD dwMilliseconds)
{
	usleep(dwMilliseconds);
}

#endif //#if !defined(WIN32) && !defined(_WIN32)

