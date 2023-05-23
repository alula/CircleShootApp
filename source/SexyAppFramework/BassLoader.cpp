#include "BassLoader.h"
#include <stdlib.h>

using namespace Sexy;

BASS_INSTANCE* Sexy::gBass = NULL;
static long gBassLoadCount = 0;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void CheckBassFunction(unsigned int theFunc, const char *theName)
{
	if (theFunc==0)
	{
		char aBuf[1024];
		sprintf(aBuf,"%s function not found in bass.dll",theName);
		MessageBoxA(NULL,aBuf,"Error",MB_OK | MB_ICONERROR);
		exit(0);
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
BASS_INSTANCE::BASS_INSTANCE(const char *dllName)
{
    mModule = LoadLibrary(dllName);
	if (!mModule)
		return;

#define GETPROC(_x) CheckBassFunction(*((uintptr_t*)&_x) = (uintptr_t)GetProcAddress(mModule, #_x),#_x)    

#ifdef BASS2
	GETPROC(BASS_Init);
	GETPROC(BASS_Free);
	GETPROC(BASS_Stop);
	GETPROC(BASS_Start);
	
	*((uintptr_t*) &BASS_SetGlobalVolumes) = (uintptr_t) GetProcAddress(mModule, "BASS_SetGlobalVolumes");
	*((uintptr_t*) &BASS_SetVolume) = (uintptr_t) GetProcAddress(mModule, "BASS_SetVolume");

	if ((BASS_SetVolume == NULL) && (BASS_SetGlobalVolumes == NULL))
	{
		MessageBoxA(NULL,"Neither BASS_SetGlobalVolumes or BASS_SetVolume found in bass.dll","Error",MB_OK | MB_ICONERROR);
		exit(0);
	}	

	*((uintptr_t*) &BASS_SetConfig) = (uintptr_t) GetProcAddress(mModule, "BASS_SetConfig");
	*((uintptr_t*) &BASS_GetConfig) = (uintptr_t) GetProcAddress(mModule, "BASS_GetConfig");

	GETPROC(BASS_GetVolume);
	GETPROC(BASS_GetInfo);

	GETPROC(BASS_GetVersion);
	GETPROC(BASS_ChannelStop);
	GETPROC(BASS_ChannelPlay);
	GETPROC(BASS_ChannelPause);
	GETPROC(BASS_ChannelSetAttribute);
	//GETPROC(BASS_ChannelSetAttributes);
	//GETPROC(BASS_ChannelGetAttributes);
	GETPROC(BASS_ChannelSetPosition);
	GETPROC(BASS_ChannelGetPosition);
	GETPROC(BASS_ChannelIsActive);
	GETPROC(BASS_ChannelFlags);
	GETPROC(BASS_ChannelSlideAttribute);
	//GETPROC(BASS_ChannelSlideAttributes);
	GETPROC(BASS_ChannelIsSliding);
	GETPROC(BASS_ChannelGetLevel);	
	GETPROC(BASS_ChannelSetSync);
	GETPROC(BASS_ChannelRemoveSync);
	GETPROC(BASS_ChannelGetData);

	// supported by BASS 1.1 and higher. Only work if the user has DX8 or higher though.
	GETPROC(BASS_FXSetParameters);
	GETPROC(BASS_FXGetParameters);
	GETPROC(BASS_ChannelSetFX);
	GETPROC(BASS_ChannelRemoveFX);

	GETPROC(BASS_MusicLoad);
	GETPROC(BASS_MusicFree);
	//GETPROC(BASS_MusicGetAttribute);
	//GETPROC(BASS_MusicSetAttribute);

	GETPROC(BASS_StreamCreateFile);
	GETPROC(BASS_StreamFree);

	//GETPROC(BASS_MusicGetOrders);
	//GETPROC(BASS_MusicGetOrderPosition);

	GETPROC(BASS_SampleLoad);
	GETPROC(BASS_SampleFree);
	GETPROC(BASS_SampleSetInfo);
	GETPROC(BASS_SampleGetInfo);
	GETPROC(BASS_SampleGetChannel);
	GETPROC(BASS_SampleStop);

	GETPROC(BASS_ErrorGetCode);

	mVersion2 = BASS_SetConfig != NULL;
	if (mVersion2)
	{
		// Version 2 has different BASS_Init params
		*((uintptr_t*) &BASS_Init2) = (uintptr_t) BASS_Init;
		BASS_Init = NULL;

		// The following are only supported in 2.2 and higher
		//*((unsigned int*) &BASS_PluginLoad) = (unsigned int) GetProcAddress(mModule, "BASS_PluginLoad");
		*((uintptr_t*) &BASS_ChannelGetLength) = (uintptr_t) GetProcAddress(mModule, "BASS_ChannelGetLength");

		// 2.1 and higher only
		*((uintptr_t*) &BASS_ChannelPreBuf) = (uintptr_t) GetProcAddress(mModule, "BASS_ChannelPreBuf");
	}
	else
	{
		BASS_PluginLoad = NULL;
		BASS_ChannelPreBuf = NULL;
	}
#else
	GETPROC(BASS_GetVersion);
    GETPROC(BASS_GetDeviceDescription);
    GETPROC(BASS_SetBufferLength);
    GETPROC(BASS_SetNetConfig);
    GETPROC(BASS_SetGlobalVolumes);
    GETPROC(BASS_GetGlobalVolumes);
    GETPROC(BASS_SetLogCurves);
    GETPROC(BASS_Set3DAlgorithm);
    GETPROC(BASS_ErrorGetCode);
    GETPROC(BASS_SetCLSID);
    GETPROC(BASS_Init);
    GETPROC(BASS_Free);
    GETPROC(BASS_GetDSoundObject);
    GETPROC(BASS_GetInfo);
    GETPROC(BASS_Update);
    GETPROC(BASS_GetCPU);
    GETPROC(BASS_Start);
    GETPROC(BASS_Stop);
    GETPROC(BASS_Pause);
    GETPROC(BASS_SetVolume);
    GETPROC(BASS_GetVolume);
    GETPROC(BASS_MusicLoad);
    GETPROC(BASS_MusicFree);
    GETPROC(BASS_MusicGetName);
    GETPROC(BASS_MusicGetLength);
    GETPROC(BASS_MusicPreBuf);
    GETPROC(BASS_MusicPlay);
    GETPROC(BASS_MusicPlayEx);
    GETPROC(BASS_MusicSetAmplify);
    GETPROC(BASS_MusicSetPanSep);
    GETPROC(BASS_MusicSetPositionScaler);
    GETPROC(BASS_MusicSetChannelVol);
    GETPROC(BASS_MusicGetChannelVol);
    GETPROC(BASS_SampleLoad);
    GETPROC(BASS_SampleCreate);
    GETPROC(BASS_SampleCreateDone);
    GETPROC(BASS_SampleFree);
    GETPROC(BASS_SampleGetInfo);
    GETPROC(BASS_SampleSetInfo);
    GETPROC(BASS_SamplePlay);
    GETPROC(BASS_SamplePlayEx);
    GETPROC(BASS_SamplePlay3D);
    GETPROC(BASS_SamplePlay3DEx);
    GETPROC(BASS_SampleStop);
    GETPROC(BASS_StreamCreate);
    GETPROC(BASS_StreamCreateFile);
    GETPROC(BASS_StreamCreateURL);
    GETPROC(BASS_StreamFree);
    GETPROC(BASS_StreamGetLength);
    GETPROC(BASS_StreamGetTags);
    GETPROC(BASS_StreamPreBuf);
    GETPROC(BASS_StreamPlay);
    GETPROC(BASS_StreamGetFilePosition);
    GETPROC(BASS_ChannelBytes2Seconds);
    GETPROC(BASS_ChannelSeconds2Bytes);
    GETPROC(BASS_ChannelIsActive);
    GETPROC(BASS_ChannelGetFlags);
    GETPROC(BASS_ChannelStop);
    GETPROC(BASS_ChannelPause);
    GETPROC(BASS_ChannelResume);
    GETPROC(BASS_ChannelSetAttributes);
    GETPROC(BASS_ChannelGetAttributes);
    GETPROC(BASS_ChannelSlideAttributes);
    GETPROC(BASS_ChannelIsSliding);
    GETPROC(BASS_ChannelSet3DAttributes);
    GETPROC(BASS_ChannelGet3DAttributes);
    GETPROC(BASS_ChannelSet3DPosition);
    GETPROC(BASS_ChannelGet3DPosition);
    GETPROC(BASS_ChannelSetPosition);
    GETPROC(BASS_ChannelGetPosition);
    GETPROC(BASS_ChannelGetLevel);
    GETPROC(BASS_ChannelGetData);
    GETPROC(BASS_ChannelSetSync);
    GETPROC(BASS_ChannelRemoveSync);
    GETPROC(BASS_ChannelSetDSP);
    GETPROC(BASS_ChannelRemoveDSP);
    GETPROC(BASS_ChannelSetFX);
    GETPROC(BASS_ChannelRemoveFX);
    GETPROC(BASS_ChannelSetEAXMix);
    GETPROC(BASS_ChannelGetEAXMix);
    GETPROC(BASS_ChannelSetLink);
    GETPROC(BASS_ChannelRemoveLink);
    GETPROC(BASS_FXSetParameters);
    GETPROC(BASS_FXGetParameters);
#endif

#undef GETPROC
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
BASS_INSTANCE::~BASS_INSTANCE()
{
    if (mModule)
        FreeLibrary(mModule);
}

#ifdef BASS2
BOOL BASS_INSTANCE::BASS_MusicSetAmplify(HMUSIC handle, DWORD amp)
{
#ifdef BASS2
	BASS_ChannelSetAttribute(handle, BASS_ATTRIB_MUSIC_AMPLIFY, amp);
#else
	BASS_MusicSetAttribute(handle, BASS_MUSIC_ATTRIB_AMPLIFY, amp);
#endif
	return true;
}


BOOL BASS_INSTANCE::BASS_MusicPlay(HMUSIC handle)
{
	return BASS_ChannelPlay(handle, true);
}


BOOL BASS_INSTANCE::BASS_MusicPlayEx(HMUSIC handle, DWORD pos, int flags, BOOL reset)
{
#ifdef BASS2
	BASS_ChannelStop(handle);
	BASS_ChannelSetPosition(handle, pos, BASS_POS_MUSIC_ORDER);
	BASS_ChannelFlags(handle, flags, -1);

	return BASS_ChannelPlay(handle, false/*reset*/);
#else
	int anOffset = MAKEMUSICPOS(pos,0);

	BASS_ChannelStop(handle);
	BASS_ChannelSetPosition(handle, anOffset);
	BASS_ChannelSetFlags(handle, flags);

	return BASS_ChannelPlay(handle, false/*reset*/);
#endif
}


BOOL BASS_INSTANCE::BASS_ChannelResume(DWORD handle)
{
	return BASS_ChannelPlay(handle, false);
}

BOOL BASS_INSTANCE::BASS_StreamPlay(HSTREAM handle, BOOL flush, DWORD flags)
{
#ifdef BASS2
	BASS_ChannelFlags(handle, flags, -1);
#else
	BASS_ChannelSetFlags(handle, flags);
#endif
	return BASS_ChannelPlay(handle, flush);
}

BOOL BASS_INSTANCE::BASS_ChannelSetAttributes(DWORD handle, int freq, int volume, int pan)
{
#define RETIFFALSE(exp) if (!exp) return FALSE;

	if (freq != -1) RETIFFALSE(BASS_ChannelSetAttribute(handle, BASS_ATTRIB_FREQ, freq));
	if (volume != -1) RETIFFALSE(BASS_ChannelSetAttribute(handle, BASS_ATTRIB_VOL, (float)volume * 0.01f));
	if (pan != -101) RETIFFALSE(BASS_ChannelSetAttribute(handle, BASS_ATTRIB_PAN, (float)pan * 0.01f));

#undef RETIFFALSE
	return TRUE;
}

BOOL BASS_INSTANCE::BASS_ChannelSlideAttributes(DWORD handle, int freq, int volume, int pan, DWORD time)
{
#define RETIFFALSE(exp) if (!exp) return FALSE;

	if (freq != -1) RETIFFALSE(BASS_ChannelSlideAttribute(handle, BASS_ATTRIB_FREQ, freq, time));
	if (volume != -1) RETIFFALSE(BASS_ChannelSlideAttribute(handle, BASS_ATTRIB_VOL, (float)volume * 0.01f, time));
	if (pan != -101) RETIFFALSE(BASS_ChannelSlideAttribute(handle, BASS_ATTRIB_PAN, (float)pan * 0.01f, time));

#undef RETIFFALSE
	return TRUE;
}
#endif


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void Sexy::LoadBassDLL()
{
	InterlockedIncrement(&gBassLoadCount);
	if (gBass!=NULL)
		return;

	gBass = new BASS_INSTANCE(".\\bass.dll");
	if (gBass->mModule==NULL)
	{
		MessageBoxA(NULL,"Can't find bass.dll." ,"Error",MB_OK | MB_ICONERROR);
		exit(0);
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void Sexy::FreeBassDLL()
{
	if (gBass!=NULL)
	{
		if (InterlockedDecrement(&gBassLoadCount) <= 0)
		{
			delete gBass;
			gBass = NULL;
		}
	}
}


