#include "Zuma_Prefix.pch"

#include "WorkerThread.h"

#ifdef _WIN32
#include <windows.h>
#include <process.h>
#else
#error WorkerThread is unimplemented on non-win32 platforms.
#endif

using namespace Sexy;

WorkerThread::WorkerThread()
{
	mShutdown = false;
	mTaskProc = NULL;
	mUnk1 = CreateEventA(0, FALSE, FALSE, NULL);
	mUnk2 = CreateEventA(0, FALSE, FALSE, NULL);

	_beginthread((_beginthread_proc_type) WorkerThread::StaticThreadProc, 0, this);
}

WorkerThread::~WorkerThread()
{
}

void WorkerThread::WaitForTask()
{
	if (mTaskProc)
	{
		WaitForSingleObject(mUnk2, 1000);
	}

	ResetEvent(mUnk2);
}

void WorkerThread::DoTask(void (*theTaskProc)(void*), void* theParam)
{
	WaitForTask();

	mTaskProc = theTaskProc;
	mParam = theParam;

	SetEvent(mUnk1);
}

void WorkerThread::StaticThreadProc(WorkerThread* thr) 
{
	HANDLE ct = GetCurrentThread();
	SetThreadPriority(ct, THREAD_PRIORITY_LOWEST);
	WaitForSingleObject(thr->mUnk1, 1000);
	while (!thr->mShutdown)
	{
		if (thr->mTaskProc) {
			thr->mTaskProc(thr->mParam);
			thr->mTaskProc = NULL;

			SetEvent(thr->mUnk2);
		}

		WaitForSingleObject(thr->mUnk1, 1000);
	}

	SetEvent(thr->mUnk2);
}