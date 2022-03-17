#pragma warning(disable:4786)

#include "CritSect.h"
#include <cassert>

using namespace Sexy;

////////////////////////////////////////////////////////////////////////////////

CritSect::CritSect(void)
{
#if defined(_WIN32)
	InitializeCriticalSection(&mCriticalSection);
#elif defined(_POSIX)
	pthread_mutex_lock(&mCriticalSection);
#else
	assert(false && "Critical sections are unimplemented for this platform.");
#endif
}

////////////////////////////////////////////////////////////////////////////////

CritSect::~CritSect(void)
{
#if defined(_WIN32)
	DeleteCriticalSection(&mCriticalSection);
#elif defined(_POSIX)
	pthread_mutex_unlock(&mCriticalSection);
#else
	assert(false && "Critical sections are unimplemented for this platform.");
#endif
}
