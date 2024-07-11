#ifndef __SEXY_WORKER_THREAD_H__
#define __SEXY_WORKER_THREAD_H__

#include "CircleCommon.h"

#include <vector>
#include <list>

#include <windows.h>

namespace Sexy
{
	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////

	class WorkerThread
	{
	public:
		HANDLE mUnk1;
		HANDLE mUnk2;
		void (*mTaskProc)(void *);
		void *mParam;
		bool mShutdown;

		WorkerThread();

		virtual ~WorkerThread();

		void WaitForTask();
		void DoTask(void (*func)(void *), void *unk);

		static void StaticThreadProc(WorkerThread *unk);
	};
};

#endif