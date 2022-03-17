#ifndef _H_CritSect
#define _H_CritSect

#include "Common.h"
#if defined(_WIN32)
#include <windows.h>
#elif defined(_POSIX)
#include <pthread.h>
#endif

class CritSync;

namespace Sexy
{

class CritSect 
{
private:
#if defined(_WIN32)
	CRITICAL_SECTION mCriticalSection;
#elif defined(_POSIX)
	pthread_mutex_t mCriticalSection = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
#endif
	friend class AutoCrit;

public:
	CritSect(void);
	~CritSect(void);
};

}

#endif // _H_CritSect
