#ifndef __BASSLOADER_H__
#define __BASSLOADER_H__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>

#ifdef BASS2
#include "bass24.h"
#else
#include "bass18.h"
#endif

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
namespace Sexy
{

struct BASS_INSTANCE
{
	BASS_INSTANCE(const char *dllName);
	virtual ~BASS_INSTANCE();

#ifdef BASS2
	BOOL BASS_MusicSetAmplify(HMUSIC handle, DWORD amp);
	BOOL BASS_MusicPlay(HMUSIC handle);
	BOOL BASS_MusicPlayEx(HMUSIC handle, DWORD pos, int flags, BOOL reset);
	BOOL BASS_ChannelResume(DWORD handle);
	BOOL BASS_StreamPlay(HSTREAM handle, BOOL flush, DWORD flags);
	BOOL BASS_ChannelSetAttributes(DWORD handle, int freq, int volume, int pan);
	BOOL BASS_ChannelSlideAttributes(DWORD handle, int freq, int volume, int pan, DWORD time);
#endif


    HMODULE         mModule;
	bool			mVersion2;

#ifdef BASS2
	DWORD(WINAPI *BASS_GetVersion)();

	BOOL(WINAPI *BASS_Init)(int device, DWORD freq, DWORD flags, HWND win);
	BOOL(WINAPI *BASS_Init2)(int device, DWORD freq, DWORD flags, HWND win, GUID* clsid);
	void(WINAPI *BASS_Free)();
	BOOL(WINAPI *BASS_Stop)();
	BOOL(WINAPI *BASS_Start)();
	void(WINAPI *BASS_SetGlobalVolumes)(int musvol, int samvol, int strvol);
	BOOL(WINAPI *BASS_SetVolume)(DWORD volume);
	BOOL(WINAPI *BASS_GetVolume)();
	BOOL(WINAPI *BASS_GetInfo)(BASS_INFO* info);

	DWORD(WINAPI *BASS_SetConfig)(DWORD option, DWORD value); 
	DWORD(WINAPI *BASS_GetConfig)(DWORD option); 

	BOOL(WINAPI *BASS_ChannelStop)(DWORD handle);
	BOOL(WINAPI *BASS_ChannelPlay)(DWORD handle, BOOL restart);
	BOOL(WINAPI *BASS_ChannelPause)(DWORD handle);
	BOOL(WINAPI *BASS_ChannelSetAttribute)(DWORD handle, DWORD attrib, float value);
	//BOOL(WINAPI *BASS_ChannelSetAttributes)(DWORD handle, int freq, int volume, int pan);
	//BOOL(WINAPI *BASS_ChannelGetAttributes)(DWORD handle, DWORD* freq, DWORD* volume, int* pan);
	BOOL(WINAPI *BASS_ChannelSetPosition)(DWORD handle, QWORD pos, DWORD mode);
	QWORD(WINAPI *BASS_ChannelGetPosition)(DWORD handle, DWORD mode);
	BOOL (WINAPI *BASS_ChannelFlags)(DWORD handle, DWORD flags, DWORD mask);
	DWORD(WINAPI *BASS_ChannelIsActive)(DWORD handle);
	BOOL (WINAPI *BASS_ChannelSlideAttribute)(DWORD handle, DWORD attrib, float value, DWORD time);
	//BOOL (WINAPI *BASS_ChannelSlideAttributes)(DWORD handle, int freq, int volume, int pan, DWORD time);
	DWORD (WINAPI *BASS_ChannelIsSliding)(DWORD handle, DWORD attrib);
	DWORD (WINAPI *BASS_ChannelGetLevel)(DWORD handle);
	HFX (WINAPI *BASS_ChannelSetFX)(DWORD handle, DWORD theType, int priority);
	BOOL (WINAPI *BASS_ChannelRemoveFX)(DWORD handle, HFX fx);
	QWORD (WINAPI *BASS_ChannelGetLength)(DWORD handle);
	DWORD (WINAPI *BASS_ChannelGetData)(DWORD handle, void* buffer, DWORD length);
	BOOL (WINAPI *BASS_ChannelPreBuf)(DWORD handle, DWORD length);
	HSYNC (WINAPI *BASS_ChannelSetSync)(DWORD handle, DWORD theType, QWORD theParam, SYNCPROC* proc, DWORD user);
	BOOL (WINAPI *BASS_ChannelRemoveSync)(DWORD handle, HSYNC sync);
	
	HMUSIC(WINAPI *BASS_MusicLoad)(BOOL mem, const void* file, QWORD offset, DWORD length, DWORD flags, DWORD freq);
	void(WINAPI *BASS_MusicFree)(HMUSIC handle);

	HSTREAM(WINAPI *BASS_StreamCreateFile)(BOOL mem, void *file, DWORD offset, DWORD length, DWORD flags); 
	void(WINAPI *BASS_StreamFree)(HSTREAM handle);

	BOOL (WINAPI *BASS_FXSetParameters)(HFX handle, void *par);
	BOOL (WINAPI *BASS_FXGetParameters)(HFX handle, void *par);

	//DWORD(WINAPI *BASS_MusicGetAttribute)(DWORD handle, DWORD attrib);
	//void(WINAPI *BASS_MusicSetAttribute)(DWORD handle, DWORD attrib,DWORD value);

	//DWORD (WINAPI *BASS_MusicGetOrders)(HMUSIC handle);
	//DWORD (WINAPI *BASS_MusicGetOrderPosition)(HMUSIC handle);

	HPLUGIN (WINAPI *BASS_PluginLoad)(char *file, DWORD flags);

	HSAMPLE (WINAPI *BASS_SampleLoad)(BOOL mem, void *file, DWORD offset, DWORD length, DWORD max, DWORD flags);
	BOOL (WINAPI *BASS_SampleFree)(HSAMPLE handle);
	BOOL (WINAPI *BASS_SampleSetInfo)(HSAMPLE handle, BASS_SAMPLE *info);
	BOOL (WINAPI *BASS_SampleGetInfo)(HSAMPLE handle, BASS_SAMPLE *info);
	HCHANNEL (WINAPI *BASS_SampleGetChannel)(HSAMPLE handle, BOOL onlynew);
	BOOL (WINAPI *BASS_SampleStop)(HSAMPLE handle);	

	int (WINAPI *BASS_ErrorGetCode)();
#else
	DWORD (WINAPI *BASS_GetVersion)();

	char *(WINAPI *BASS_GetDeviceDescription)(DWORD devnum);
	float (WINAPI *BASS_SetBufferLength)(float length);
	DWORD (WINAPI *BASS_SetNetConfig)(DWORD option, DWORD value);
	void (WINAPI *BASS_SetGlobalVolumes)(int musvol, int samvol, int strvol);
	void (WINAPI *BASS_GetGlobalVolumes)(DWORD *musvol, DWORD *samvol, DWORD *strvol);
	void (WINAPI *BASS_SetLogCurves)(BOOL volume, BOOL pan);
	void (WINAPI *BASS_Set3DAlgorithm)(DWORD algo);
	DWORD (WINAPI *BASS_ErrorGetCode)();
	void (WINAPI *BASS_SetCLSID)(GUID *clsid);
	BOOL (WINAPI *BASS_Init)(int device, DWORD freq, DWORD flags, HWND win);
	void (WINAPI *BASS_Free)();
	void *(WINAPI *BASS_GetDSoundObject)(DWORD object);
	void (WINAPI *BASS_GetInfo)(BASS_INFO *info);
	BOOL (WINAPI *BASS_Update)();
	float (WINAPI *BASS_GetCPU)();
	BOOL (WINAPI *BASS_Start)();
	BOOL (WINAPI *BASS_Stop)();
	BOOL (WINAPI *BASS_Pause)();
	BOOL (WINAPI *BASS_SetVolume)(DWORD volume);
	int (WINAPI *BASS_GetVolume)();
	HMUSIC (WINAPI *BASS_MusicLoad)(BOOL mem, void *file, DWORD offset, DWORD length, DWORD flags);
	void (WINAPI *BASS_MusicFree)(HMUSIC handle);
	char *(WINAPI *BASS_MusicGetName)(HMUSIC handle);
	DWORD (WINAPI *BASS_MusicGetLength)(HMUSIC handle, BOOL playlen);
	BOOL (WINAPI *BASS_MusicPreBuf)(HMUSIC handle);
	BOOL (WINAPI *BASS_MusicPlay)(HMUSIC handle);
	BOOL (WINAPI *BASS_MusicPlayEx)(HMUSIC handle, DWORD pos, int flags, BOOL reset);
	BOOL (WINAPI *BASS_MusicSetAmplify)(HMUSIC handle, DWORD amp);
	BOOL (WINAPI *BASS_MusicSetPanSep)(HMUSIC handle, DWORD pan);
	BOOL (WINAPI *BASS_MusicSetPositionScaler)(HMUSIC handle, DWORD scale);
	BOOL (WINAPI *BASS_MusicSetChannelVol)(HMUSIC handle, DWORD channel, DWORD volume);
	int (WINAPI *BASS_MusicGetChannelVol)(HMUSIC handle, DWORD channel);
	HSAMPLE (WINAPI *BASS_SampleLoad)(BOOL mem, void *file, DWORD offset, DWORD length, DWORD max, DWORD flags);
	void* (WINAPI *BASS_SampleCreate)(DWORD length, DWORD freq, DWORD max, DWORD flags);
	HSAMPLE (WINAPI *BASS_SampleCreateDone)();
	void (WINAPI *BASS_SampleFree)(HSAMPLE handle);
	BOOL (WINAPI *BASS_SampleGetInfo)(HSAMPLE handle, BASS_SAMPLE *info);
	BOOL (WINAPI *BASS_SampleSetInfo)(HSAMPLE handle, BASS_SAMPLE *info);
	HCHANNEL (WINAPI *BASS_SamplePlay)(HSAMPLE handle);
	HCHANNEL (WINAPI *BASS_SamplePlayEx)(HSAMPLE handle, DWORD start, int freq, int volume, int pan, BOOL loop);
	HCHANNEL (WINAPI *BASS_SamplePlay3D)(HSAMPLE handle, BASS_3DVECTOR *pos, BASS_3DVECTOR *orient, BASS_3DVECTOR *vel);
	HCHANNEL (WINAPI *BASS_SamplePlay3DEx)(HSAMPLE handle, BASS_3DVECTOR *pos, BASS_3DVECTOR *orient, BASS_3DVECTOR *vel, DWORD start, int freq, int volume, BOOL loop);
	BOOL (WINAPI *BASS_SampleStop)(HSAMPLE handle);
	HSTREAM (WINAPI *BASS_StreamCreate)(DWORD freq, DWORD flags, void *proc, DWORD user);
	HSTREAM (WINAPI *BASS_StreamCreateFile)(BOOL mem, void *file, DWORD offset, DWORD length, DWORD flags);
	HSTREAM (WINAPI *BASS_StreamCreateURL)(char *url, DWORD offset, DWORD flags, char *save);
	void (WINAPI *BASS_StreamFree)(HSTREAM handle);
	QWORD (WINAPI *BASS_StreamGetLength)(HSTREAM handle);
	char *(WINAPI *BASS_StreamGetTags)(HSTREAM handle, DWORD tags);
	BOOL (WINAPI *BASS_StreamPreBuf)(HSTREAM handle);
	BOOL (WINAPI *BASS_StreamPlay)(HSTREAM handle, BOOL flush, DWORD flags);
	DWORD (WINAPI *BASS_StreamGetFilePosition)(HSTREAM handle, DWORD mode);
	float (WINAPI *BASS_ChannelBytes2Seconds)(DWORD handle, QWORD pos);
	QWORD (WINAPI *BASS_ChannelSeconds2Bytes)(DWORD handle, float pos);
	DWORD (WINAPI *BASS_ChannelIsActive)(DWORD handle);
	DWORD (WINAPI *BASS_ChannelGetFlags)(DWORD handle);
	BOOL (WINAPI *BASS_ChannelStop)(DWORD handle);
	BOOL (WINAPI *BASS_ChannelPause)(DWORD handle);
	BOOL (WINAPI *BASS_ChannelResume)(DWORD handle);
	BOOL (WINAPI *BASS_ChannelSetAttributes)(DWORD handle, int freq, int volume, int pan);
	BOOL (WINAPI *BASS_ChannelGetAttributes)(DWORD handle, DWORD *freq, DWORD *volume, int *pan);
	BOOL (WINAPI *BASS_ChannelSlideAttributes)(DWORD handle, int freq, int volume, int pan, DWORD time);
	DWORD (WINAPI *BASS_ChannelIsSliding)(DWORD handle);
	BOOL (WINAPI *BASS_ChannelSet3DAttributes)(DWORD handle, int mode, float min, float max, int iangle, int oangle, int outvol);
	BOOL (WINAPI *BASS_ChannelGet3DAttributes)(DWORD handle, DWORD *mode, float *min, float *max, DWORD *iangle, DWORD *oangle, DWORD *outvol);
	BOOL (WINAPI *BASS_ChannelSet3DPosition)(DWORD handle, BASS_3DVECTOR *pos, BASS_3DVECTOR *orient, BASS_3DVECTOR *vel);
	BOOL (WINAPI *BASS_ChannelGet3DPosition)(DWORD handle, BASS_3DVECTOR *pos, BASS_3DVECTOR *orient, BASS_3DVECTOR *vel);
	BOOL (WINAPI *BASS_ChannelSetPosition)(DWORD handle, QWORD pos);
	QWORD (WINAPI *BASS_ChannelGetPosition)(DWORD handle);
	DWORD (WINAPI *BASS_ChannelGetLevel)(DWORD handle);
	DWORD (WINAPI *BASS_ChannelGetData)(DWORD handle, void *buffer, DWORD length);
	HSYNC (WINAPI *BASS_ChannelSetSync)(DWORD handle, DWORD type, QWORD param, SYNCPROC *proc, DWORD user);
	BOOL (WINAPI *BASS_ChannelRemoveSync)(DWORD handle, HSYNC sync);
	HDSP (WINAPI *BASS_ChannelSetDSP)(DWORD handle, DSPPROC *proc, DWORD user);
	BOOL (WINAPI *BASS_ChannelRemoveDSP)(DWORD handle, HDSP dsp);
	HFX (WINAPI *BASS_ChannelSetFX)(DWORD handle, DWORD type);
	BOOL (WINAPI *BASS_ChannelRemoveFX)(DWORD handle, HFX fx);
	BOOL (WINAPI *BASS_ChannelSetEAXMix)(DWORD handle, float mix);
	BOOL (WINAPI *BASS_ChannelGetEAXMix)(DWORD handle, float *mix);
	BOOL (WINAPI *BASS_ChannelSetLink)(DWORD handle, DWORD chan);
	BOOL (WINAPI *BASS_ChannelRemoveLink)(DWORD handle, DWORD chan);
	BOOL (WINAPI *BASS_FXSetParameters)(HFX handle, void *par);
	BOOL (WINAPI *BASS_FXGetParameters)(HFX handle, void *par);
#endif
};


extern BASS_INSTANCE *gBass;

BASS_INSTANCE* BASS_CreateInstance(char *dllName);
void BASS_FreeInstance(BASS_INSTANCE *instance);
void LoadBassDLL(); // exits on failure
void FreeBassDLL();

} // namespace Sexy

#endif