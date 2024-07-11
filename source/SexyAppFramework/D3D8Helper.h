#ifndef __D3D8HELPER_H__
#define __D3D8HELPER_H__

#include <string>

#ifdef _WIN32
#include <guiddef.h>
#else
typedef struct _GUID {
	unsigned long  Data1;
	unsigned short Data2;
	unsigned short Data3;
	unsigned char  Data4[8];
} GUID;
#endif

namespace Sexy
{
	bool GetD3D8AdapterInfo(GUID &theGUID, std::string &theDriver, std::string &theDescription);
}

#endif