/* BASS 1.8 C/C++ header file, copyright (c) 1999-2003 Ian Luck.
   Please report bugs/suggestions/etc... to bass@un4seen.com

   See the BASS.CHM file for more complete documentation */

#ifndef BASS_H
#define BASS_H

#include <wtypes.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BASSDEF
#define BASSDEF(f) WINAPI f
#endif

typedef unsigned __int64 QWORD;	// 64-bit

typedef DWORD HMUSIC;		// MOD music handle
typedef DWORD HSAMPLE;		// sample handle
typedef DWORD HCHANNEL;		// playing sample's channel handle
typedef DWORD HSTREAM;		// sample stream handle
typedef DWORD HSYNC;		// synchronizer handle
typedef DWORD HDSP;			// DSP handle
typedef DWORD HFX;			// DX8 effect handle

// Error codes returned by BASS_GetErrorCode
#define BASS_OK				0	// all is OK
#define BASS_ERROR_MEM		1	// memory error
#define BASS_ERROR_FILEOPEN	2	// can't open the file
#define BASS_ERROR_DRIVER	3	// can't find a free/valid driver
#define BASS_ERROR_BUFLOST	4	// the sample buffer was lost
#define BASS_ERROR_HANDLE	5	// invalid handle
#define BASS_ERROR_FORMAT	6	// unsupported sample format
#define BASS_ERROR_POSITION	7	// invalid playback position
#define BASS_ERROR_INIT		8	// BASS_Init has not been successfully called
#define BASS_ERROR_START	9	// BASS_Start has not been successfully called
#define BASS_ERROR_INITCD	10	// can't initialize CD
#define BASS_ERROR_CDINIT	11	// BASS_CDInit has not been successfully called
#define BASS_ERROR_NOCD		12	// no CD in drive
#define BASS_ERROR_CDTRACK	13	// can't play the selected CD track
#define BASS_ERROR_ALREADY	14	// already initialized
#define BASS_ERROR_CDVOL	15	// CD has no volume control
#define BASS_ERROR_NOPAUSE	16	// not paused
#define BASS_ERROR_NOTAUDIO	17	// not an audio track
#define BASS_ERROR_NOCHAN	18	// can't get a free channel
#define BASS_ERROR_ILLTYPE	19	// an illegal type was specified
#define BASS_ERROR_ILLPARAM	20	// an illegal parameter was specified
#define BASS_ERROR_NO3D		21	// no 3D support
#define BASS_ERROR_NOEAX	22	// no EAX support
#define BASS_ERROR_DEVICE	23	// illegal device number
#define BASS_ERROR_NOPLAY	24	// not playing
#define BASS_ERROR_FREQ		25	// illegal sample rate
#define BASS_ERROR_NOTFILE	27	// the stream is not a file stream
#define BASS_ERROR_NOHW		29	// no hardware voices available
#define BASS_ERROR_EMPTY	31	// the MOD music has no sequence data
#define BASS_ERROR_NONET	32	// no internet connection could be opened
#define BASS_ERROR_CREATE	33	// couldn't create the file
#define BASS_ERROR_NOFX		34	// effects are not available
#define BASS_ERROR_PLAYING	35	// the channel is playing
#define BASS_ERROR_NOTAVAIL	37	// requested data is not available
#define BASS_ERROR_DECODE	38	// the channel is a "decoding channel"
#define BASS_ERROR_DX		39	// a sufficient DirectX version is not installed
#define BASS_ERROR_TIMEOUT	40	// connection timedout
#define BASS_ERROR_FILEFORM	41	// unsupported file format
#define BASS_ERROR_SPEAKER	42	// unavailable speaker
#define BASS_ERROR_UNKNOWN	-1	// some other mystery error

// Initialization flags
#define BASS_DEVICE_8BITS	1	// use 8 bit resolution, else 16 bit
#define BASS_DEVICE_MONO	2	// use mono, else stereo
#define BASS_DEVICE_3D		4	// enable 3D functionality
/* If the BASS_DEVICE_3D flag is not specified when initilizing BASS,
then the 3D flags (BASS_SAMPLE_3D and BASS_MUSIC_3D) are ignored when
loading/creating a sample/stream/music. */
#define BASS_DEVICE_LEAVEVOL	32	// leave the volume as it is
#define BASS_DEVICE_NOTHREAD	128	// update buffers manually (using BASS_Update)
#define BASS_DEVICE_LATENCY	256	// calculate device latency (BASS_INFO struct)
#define BASS_DEVICE_VOL1000	512	// 0-1000 volume range (else 0-100)
#define BASS_DEVICE_FLOATDSP 1024	// all DSP uses 32-bit floating-point data
#define BASS_DEVICE_SPEAKERS 2048	// force enabling of speaker assignment

// DirectSound interfaces (for use with BASS_GetDSoundObject)
#define BASS_OBJECT_DS		1	// IDirectSound
#define BASS_OBJECT_DS3DL	2	// IDirectSound3DListener

typedef struct {
	DWORD size;		// size of this struct (set this before calling the function)
	DWORD flags;	// device capabilities (DSCAPS_xxx flags)
	DWORD hwsize;	// size of total device hardware memory
	DWORD hwfree;	// size of free device hardware memory
	DWORD freesam;	// number of free sample slots in the hardware
	DWORD free3d;	// number of free 3D sample slots in the hardware
	DWORD minrate;	// min sample rate supported by the hardware
	DWORD maxrate;	// max sample rate supported by the hardware
	BOOL eax;		// device supports EAX? (always FALSE if BASS_DEVICE_3D was not used)
	DWORD minbuf;	// recommended minimum buffer length in ms (requires BASS_DEVICE_LATENCY)
	DWORD dsver;	// DirectSound version
	DWORD latency;	// delay (in ms) before start of playback (requires BASS_DEVICE_LATENCY)
	DWORD initflags;// "flags" parameter of BASS_Init call
	DWORD speakers; // number of speakers available
} BASS_INFO;

// BASS_INFO flags (from DSOUND.H)
#define DSCAPS_CONTINUOUSRATE	0x00000010
/* supports all sample rates between min/maxrate */
#define DSCAPS_EMULDRIVER		0x00000020
/* device does NOT have hardware DirectSound support */
#define DSCAPS_CERTIFIED		0x00000040
/* device driver has been certified by Microsoft */
/* The following flags tell what type of samples are supported by HARDWARE
mixing, all these formats are supported by SOFTWARE mixing */
#define DSCAPS_SECONDARYMONO	0x00000100	// mono
#define DSCAPS_SECONDARYSTEREO	0x00000200	// stereo
#define DSCAPS_SECONDARY8BIT	0x00000400	// 8 bit
#define DSCAPS_SECONDARY16BIT	0x00000800	// 16 bit

typedef struct {
	DWORD size;		// size of this struct (set this before calling the function)
	DWORD flags;	// device capabilities (DSCCAPS_xxx flags)
	DWORD formats;	// supported standard formats (WAVE_FORMAT_xxx flags)
	DWORD inputs;	// number of inputs
	BOOL singlein;	// TRUE = only 1 input can be set at a time
} BASS_RECORDINFO;

// BASS_RECORDINFO flags (from DSOUND.H)
#define DSCCAPS_EMULDRIVER	DSCAPS_EMULDRIVER
/* device does NOT have hardware DirectSound recording support */
#define DSCCAPS_CERTIFIED	DSCAPS_CERTIFIED
/* device driver has been certified by Microsoft */

// defines for formats field of BASS_RECORDINFO (from MMSYSTEM.H)
#define WAVE_FORMAT_1M08       0x00000001       /* 11.025 kHz, Mono,   8-bit  */
#define WAVE_FORMAT_1S08       0x00000002       /* 11.025 kHz, Stereo, 8-bit  */
#define WAVE_FORMAT_1M16       0x00000004       /* 11.025 kHz, Mono,   16-bit */
#define WAVE_FORMAT_1S16       0x00000008       /* 11.025 kHz, Stereo, 16-bit */
#define WAVE_FORMAT_2M08       0x00000010       /* 22.05  kHz, Mono,   8-bit  */
#define WAVE_FORMAT_2S08       0x00000020       /* 22.05  kHz, Stereo, 8-bit  */
#define WAVE_FORMAT_2M16       0x00000040       /* 22.05  kHz, Mono,   16-bit */
#define WAVE_FORMAT_2S16       0x00000080       /* 22.05  kHz, Stereo, 16-bit */
#define WAVE_FORMAT_4M08       0x00000100       /* 44.1   kHz, Mono,   8-bit  */
#define WAVE_FORMAT_4S08       0x00000200       /* 44.1   kHz, Stereo, 8-bit  */
#define WAVE_FORMAT_4M16       0x00000400       /* 44.1   kHz, Mono,   16-bit */
#define WAVE_FORMAT_4S16       0x00000800       /* 44.1   kHz, Stereo, 16-bit */

// Music flags
#define BASS_MUSIC_RAMP		1	// normal ramping
#define BASS_MUSIC_RAMPS	2	// sensitive ramping
/* Ramping doesn't take a lot of extra processing and improves
the sound quality by removing "clicks". Sensitive ramping will
leave sharp attacked samples, unlike normal ramping. */
#define BASS_MUSIC_LOOP		4	// loop music
#define BASS_MUSIC_FT2MOD	16	// play .MOD as FastTracker 2 does
#define BASS_MUSIC_PT1MOD	32	// play .MOD as ProTracker 1 does
#define BASS_MUSIC_MONO		64	// force mono mixing (less CPU usage)
#define BASS_MUSIC_3D		128	// enable 3D functionality
#define BASS_MUSIC_POSRESET	256	// stop all notes when moving position
#define BASS_MUSIC_SURROUND	512	// surround sound
#define BASS_MUSIC_SURROUND2	1024	// surround sound (mode 2)
#define BASS_MUSIC_STOPBACK	2048	// stop the music on a backwards jump effect
#define BASS_MUSIC_FX		4096	// enable old implementation of DX8 effects
#define BASS_MUSIC_CALCLEN	8192	// calculate playback length
#define BASS_MUSIC_NONINTER	16384	// non-interpolated mixing
#define BASS_MUSIC_FLOAT	0x10000	// 32-bit floating-point
#define BASS_MUSIC_DECODE	0x200000// don't play the music, only decode (BASS_ChannelGetData)
#define BASS_MUSIC_NOSAMPLE	0x400000// don't load the samples

// Sample info structure & flags
typedef struct {
	DWORD freq;		// default playback rate
	DWORD volume;	// default volume (0-100)
	int pan;		// default pan (-100=left, 0=middle, 100=right)
	DWORD flags;	// BASS_SAMPLE_xxx flags
	DWORD length;	// length (in samples, not bytes)
	DWORD max;		// maximum simultaneous playbacks
/* The following are the sample's default 3D attributes (if the sample
is 3D, BASS_SAMPLE_3D is in flags) see BASS_ChannelSet3DAttributes */
	DWORD mode3d;	// BASS_3DMODE_xxx mode
	float mindist;	// minimum distance
	float maxdist;	// maximum distance
	DWORD iangle;	// angle of inside projection cone
	DWORD oangle;	// angle of outside projection cone
	DWORD outvol;	// delta-volume outside the projection cone
/* The following are the defaults used if the sample uses the DirectX 7
voice allocation/management features. */
	DWORD vam;		// voice allocation/management flags (BASS_VAM_xxx)
	DWORD priority;	// priority (0=lowest, 0xffffffff=highest)
} BASS_SAMPLE;

#define BASS_SAMPLE_8BITS		1	// 8 bit
#define BASS_SAMPLE_FLOAT		256	// 32-bit floating-point
#define BASS_SAMPLE_MONO		2	// mono, else stereo
#define BASS_SAMPLE_LOOP		4	// looped
#define BASS_SAMPLE_3D			8	// 3D functionality enabled
#define BASS_SAMPLE_SOFTWARE	16	// it's NOT using hardware mixing
#define BASS_SAMPLE_MUTEMAX		32	// muted at max distance (3D only)
#define BASS_SAMPLE_VAM			64	// uses the DX7 voice allocation & management
#define BASS_SAMPLE_FX			128	// old implementation of DX8 effects are enabled
#define BASS_SAMPLE_OVER_VOL	0x10000	// override lowest volume
#define BASS_SAMPLE_OVER_POS	0x20000	// override longest playing
#define BASS_SAMPLE_OVER_DIST	0x30000 // override furthest from listener (3D only)

#define BASS_MP3_SETPOS			0x20000 // enable pin-point seeking on the MP3/MP2/MP1

#define BASS_STREAM_AUTOFREE	0x40000	// automatically free the stream when it stop/ends
#define BASS_STREAM_RESTRATE	0x80000	// restrict the download rate of internet file streams
#define BASS_STREAM_BLOCK		0x100000// download/play internet file stream (MPx/OGG) in small blocks
#define BASS_STREAM_DECODE		0x200000// don't play the stream, only decode (BASS_ChannelGetData)
#define BASS_STREAM_META		0x400000// request metadata from a Shoutcast stream
#define BASS_STREAM_FILEPROC	0x800000// use a STREAMFILEPROC callback

// Speaker assignment flags
#define BASS_SPEAKER_FRONT	0x1000000	// front speakers
#define BASS_SPEAKER_REAR	0x2000000	// rear/side speakers
#define BASS_SPEAKER_CENLFE	0x3000000	// center & LFE speakers (5.1)
#define BASS_SPEAKER_REAR2	0x4000000	// rear center speakers (7.1)
#define BASS_SPEAKER_LEFT	0x10000000	// modifier: left
#define BASS_SPEAKER_RIGHT	0x20000000	// modifier: right
#define BASS_SPEAKER_FRONTLEFT	BASS_SPEAKER_FRONT|BASS_SPEAKER_LEFT
#define BASS_SPEAKER_FRONTRIGHT	BASS_SPEAKER_FRONT|BASS_SPEAKER_RIGHT
#define BASS_SPEAKER_REARLEFT	BASS_SPEAKER_REAR|BASS_SPEAKER_LEFT
#define BASS_SPEAKER_REARRIGHT	BASS_SPEAKER_REAR|BASS_SPEAKER_RIGHT
#define BASS_SPEAKER_CENTER		BASS_SPEAKER_CENLFE|BASS_SPEAKER_LEFT
#define BASS_SPEAKER_LFE		BASS_SPEAKER_CENLFE|BASS_SPEAKER_RIGHT
#define BASS_SPEAKER_REAR2LEFT	BASS_SPEAKER_REAR2|BASS_SPEAKER_LEFT
#define BASS_SPEAKER_REAR2RIGHT	BASS_SPEAKER_REAR2|BASS_SPEAKER_RIGHT


// DX7 voice allocation flags
#define BASS_VAM_HARDWARE		1
/* Play the sample in hardware. If no hardware voices are available then
the "play" call will fail */
#define BASS_VAM_SOFTWARE		2
/* Play the sample in software (ie. non-accelerated). No other VAM flags
may be used together with this flag. */

// DX7 voice management flags
/* These flags enable hardware resource stealing... if the hardware has no
available voices, a currently playing buffer will be stopped to make room for
the new buffer. NOTE: only samples loaded/created with the BASS_SAMPLE_VAM
flag are considered for termination by the DX7 voice management. */
#define BASS_VAM_TERM_TIME		4
/* If there are no free hardware voices, the buffer to be terminated will be
the one with the least time left to play. */
#define BASS_VAM_TERM_DIST		8
/* If there are no free hardware voices, the buffer to be terminated will be
one that was loaded/created with the BASS_SAMPLE_MUTEMAX flag and is beyond
it's max distance. If there are no buffers that match this criteria, then the
"play" call will fail. */
#define BASS_VAM_TERM_PRIO		16
/* If there are no free hardware voices, the buffer to be terminated will be
the one with the lowest priority. */

// 3D vector (for 3D positions/velocities/orientations)
typedef struct {
	float x;	// +=right, -=left
	float y;	// +=up, -=down
	float z;	// +=front, -=behind
} BASS_3DVECTOR;

// 3D channel modes
#define BASS_3DMODE_NORMAL		0
/* normal 3D processing */
#define BASS_3DMODE_RELATIVE	1
/* The channel's 3D position (position/velocity/orientation) are relative to
the listener. When the listener's position/velocity/orientation is changed
with BASS_Set3DPosition, the channel's position relative to the listener does
not change. */
#define BASS_3DMODE_OFF			2
/* Turn off 3D processing on the channel, the sound will be played
in the center. */

// EAX environments, use with BASS_SetEAXParameters
enum
{
    EAX_ENVIRONMENT_GENERIC,
    EAX_ENVIRONMENT_PADDEDCELL,
    EAX_ENVIRONMENT_ROOM,
    EAX_ENVIRONMENT_BATHROOM,
    EAX_ENVIRONMENT_LIVINGROOM,
    EAX_ENVIRONMENT_STONEROOM,
    EAX_ENVIRONMENT_AUDITORIUM,
    EAX_ENVIRONMENT_CONCERTHALL,
    EAX_ENVIRONMENT_CAVE,
    EAX_ENVIRONMENT_ARENA,
    EAX_ENVIRONMENT_HANGAR,
    EAX_ENVIRONMENT_CARPETEDHALLWAY,
    EAX_ENVIRONMENT_HALLWAY,
    EAX_ENVIRONMENT_STONECORRIDOR,
    EAX_ENVIRONMENT_ALLEY,
    EAX_ENVIRONMENT_FOREST,
    EAX_ENVIRONMENT_CITY,
    EAX_ENVIRONMENT_MOUNTAINS,
    EAX_ENVIRONMENT_QUARRY,
    EAX_ENVIRONMENT_PLAIN,
    EAX_ENVIRONMENT_PARKINGLOT,
    EAX_ENVIRONMENT_SEWERPIPE,
    EAX_ENVIRONMENT_UNDERWATER,
    EAX_ENVIRONMENT_DRUGGED,
    EAX_ENVIRONMENT_DIZZY,
    EAX_ENVIRONMENT_PSYCHOTIC,

    EAX_ENVIRONMENT_COUNT			// total number of environments
};

// EAX presets, usage: BASS_SetEAXParameters(EAX_PRESET_xxx)
#define EAX_PRESET_GENERIC         EAX_ENVIRONMENT_GENERIC,0.5F,1.493F,0.5F
#define EAX_PRESET_PADDEDCELL      EAX_ENVIRONMENT_PADDEDCELL,0.25F,0.1F,0.0F
#define EAX_PRESET_ROOM            EAX_ENVIRONMENT_ROOM,0.417F,0.4F,0.666F
#define EAX_PRESET_BATHROOM        EAX_ENVIRONMENT_BATHROOM,0.653F,1.499F,0.166F
#define EAX_PRESET_LIVINGROOM      EAX_ENVIRONMENT_LIVINGROOM,0.208F,0.478F,0.0F
#define EAX_PRESET_STONEROOM       EAX_ENVIRONMENT_STONEROOM,0.5F,2.309F,0.888F
#define EAX_PRESET_AUDITORIUM      EAX_ENVIRONMENT_AUDITORIUM,0.403F,4.279F,0.5F
#define EAX_PRESET_CONCERTHALL     EAX_ENVIRONMENT_CONCERTHALL,0.5F,3.961F,0.5F
#define EAX_PRESET_CAVE            EAX_ENVIRONMENT_CAVE,0.5F,2.886F,1.304F
#define EAX_PRESET_ARENA           EAX_ENVIRONMENT_ARENA,0.361F,7.284F,0.332F
#define EAX_PRESET_HANGAR          EAX_ENVIRONMENT_HANGAR,0.5F,10.0F,0.3F
#define EAX_PRESET_CARPETEDHALLWAY EAX_ENVIRONMENT_CARPETEDHALLWAY,0.153F,0.259F,2.0F
#define EAX_PRESET_HALLWAY         EAX_ENVIRONMENT_HALLWAY,0.361F,1.493F,0.0F
#define EAX_PRESET_STONECORRIDOR   EAX_ENVIRONMENT_STONECORRIDOR,0.444F,2.697F,0.638F
#define EAX_PRESET_ALLEY           EAX_ENVIRONMENT_ALLEY,0.25F,1.752F,0.776F
#define EAX_PRESET_FOREST          EAX_ENVIRONMENT_FOREST,0.111F,3.145F,0.472F
#define EAX_PRESET_CITY            EAX_ENVIRONMENT_CITY,0.111F,2.767F,0.224F
#define EAX_PRESET_MOUNTAINS       EAX_ENVIRONMENT_MOUNTAINS,0.194F,7.841F,0.472F
#define EAX_PRESET_QUARRY          EAX_ENVIRONMENT_QUARRY,1.0F,1.499F,0.5F
#define EAX_PRESET_PLAIN           EAX_ENVIRONMENT_PLAIN,0.097F,2.767F,0.224F
#define EAX_PRESET_PARKINGLOT      EAX_ENVIRONMENT_PARKINGLOT,0.208F,1.652F,1.5F
#define EAX_PRESET_SEWERPIPE       EAX_ENVIRONMENT_SEWERPIPE,0.652F,2.886F,0.25F
#define EAX_PRESET_UNDERWATER      EAX_ENVIRONMENT_UNDERWATER,1.0F,1.499F,0.0F
#define EAX_PRESET_DRUGGED         EAX_ENVIRONMENT_DRUGGED,0.875F,8.392F,1.388F
#define EAX_PRESET_DIZZY           EAX_ENVIRONMENT_DIZZY,0.139F,17.234F,0.666F
#define EAX_PRESET_PSYCHOTIC       EAX_ENVIRONMENT_PSYCHOTIC,0.486F,7.563F,0.806F

// software 3D mixing algorithm modes (used with BASS_Set3DAlgorithm)
#define BASS_3DALG_DEFAULT	0
/* default algorithm (currently translates to BASS_3DALG_OFF) */
#define BASS_3DALG_OFF		1
/* Uses normal left and right panning. The vertical axis is ignored except for
scaling of volume due to distance. Doppler shift and volume scaling are still
applied, but the 3D filtering is not performed. This is the most CPU efficient
software implementation, but provides no virtual 3D audio effect. Head Related
Transfer Function processing will not be done. Since only normal stereo panning
is used, a channel using this algorithm may be accelerated by a 2D hardware
voice if no free 3D hardware voices are available. */
#define BASS_3DALG_FULL		2
/* This algorithm gives the highest quality 3D audio effect, but uses more CPU.
Requires Windows 98 2nd Edition or Windows 2000 that uses WDM drivers, if this
mode is not available then BASS_3DALG_OFF will be used instead. */
#define BASS_3DALG_LIGHT	3
/* This algorithm gives a good 3D audio effect, and uses less CPU than the FULL
mode. Requires Windows 98 2nd Edition or Windows 2000 that uses WDM drivers, if
this mode is not available then BASS_3DALG_OFF will be used instead. */

typedef DWORD (CALLBACK STREAMPROC)(HSTREAM handle, void *buffer, DWORD length, DWORD user);
/* User stream callback function. NOTE: A stream function should obviously be as quick
as possible, other streams (and MOD musics) can't be mixed until it's finished.
handle : The stream that needs writing
buffer : Buffer to write the samples in
length : Number of bytes to write
user   : The 'user' parameter value given when calling BASS_StreamCreate
RETURN : Number of bytes written. If less than "length" then the
         stream is assumed to be at the end, and is stopped. */

// BASS_StreamGetFilePosition modes
#define BASS_FILEPOS_DECODE		0
#define BASS_FILEPOS_DOWNLOAD	1
#define BASS_FILEPOS_END		2

// STREAMFILEPROC actions
#define BASS_FILE_CLOSE		0
#define BASS_FILE_READ		1
#define BASS_FILE_QUERY		2
#define BASS_FILE_LEN		3

typedef DWORD (CALLBACK STREAMFILEPROC)(DWORD action,DWORD param1,DWORD param2,DWORD user);
/* User file stream callback function.
action : The action to perform, one of BASS_FILE_xxx values.
param1 : Depends on "action"
param2 : Depends on "action"
user   : The 'user' parameter value given when calling BASS_StreamCreate
RETURN : Depends on "action" */

/* Sync types (with BASS_ChannelSetSync "param" and SYNCPROC "data"
definitions) & flags. */
#define BASS_SYNC_MUSICPOS	0
#define BASS_SYNC_POS		0
/* Sync when a music or stream reaches a position.
if HMUSIC...
param: LOWORD=order (0=first, -1=all) HIWORD=row (0=first, -1=all)
data : LOWORD=order HIWORD=row
if HSTREAM...
param: position in bytes
data : not used */
#define BASS_SYNC_MUSICINST	1
/* Sync when an instrument (sample for the non-instrument based formats)
is played in a music (not including retrigs).
param: LOWORD=instrument (1=first) HIWORD=note (0=c0...119=b9, -1=all)
data : LOWORD=note HIWORD=volume (0-64) */
#define BASS_SYNC_END		2
/* Sync when a music or file stream reaches the end.
param: not used
data : 1 = the sync is triggered by a backward jump in a MOD music, otherwise not used */
#define BASS_SYNC_MUSICFX	3
/* Sync when the "sync" effect (XM/MTM/MOD: E8x/Wxx, IT/S3M: S2x) is used.
param: 0:data=pos, 1:data="x" value
data : param=0: LOWORD=order HIWORD=row, param=1: "x" value */
#define BASS_SYNC_META		4
/* Sync when metadata is received in a Shoutcast stream.
param: not used
data : pointer to the metadata */
#define BASS_SYNC_SLIDE		5
/* Sync when an attribute slide is completed.
param: not used
data : the type of slide completed (one of the BASS_SLIDE_xxx values) */
#define BASS_SYNC_MESSAGE	0x20000000	// FLAG: post a Windows message (instead of callback)
/* When using a window message "callback", the message to post is given in the "proc"
parameter of BASS_ChannelSetSync, and is posted to the window specified in the BASS_Init
call. The message parameters are: WPARAM = data, LPARAM = user. */
#define BASS_SYNC_MIXTIME	0x40000000	// FLAG: sync at mixtime, else at playtime
#define BASS_SYNC_ONETIME	0x80000000	// FLAG: sync only once, else continuously

typedef void (CALLBACK SYNCPROC)(HSYNC handle, DWORD channel, DWORD data, DWORD user);
/* Sync callback function. NOTE: a sync callback function should be very
quick as other syncs cannot be processed until it has finished. If the sync
is a "mixtime" sync, then other streams and MOD musics can not be mixed
until it's finished either.
handle : The sync that has occured
channel: Channel that the sync occured in
data   : Additional data associated with the sync's occurance
user   : The 'user' parameter given when calling BASS_ChannelSetSync */

typedef void (CALLBACK DSPPROC)(HDSP handle, DWORD channel, void *buffer, DWORD length, DWORD user);
/* DSP callback function. NOTE: A DSP function should obviously be as quick as
possible... other DSP functions, streams and MOD musics can not be processed
until it's finished.
handle : The DSP handle
channel: Channel that the DSP is being applied to
buffer : Buffer to apply the DSP to
length : Number of bytes in the buffer
user   : The 'user' parameter given when calling BASS_ChannelSetDSP */

typedef BOOL (CALLBACK RECORDPROC)(void *buffer, DWORD length, DWORD user);
/* Recording callback function.
buffer : Buffer containing the recorded sample data
length : Number of bytes
user   : The 'user' parameter value given when calling BASS_RecordStart
RETURN : TRUE = continue recording, FALSE = stop */

// BASS_ChannelGetData flags
#define BASS_DATA_AVAILABLE	0			// query how much data is buffered
#define BASS_DATA_FFT512	0x80000000	// 512 sample FFT
#define BASS_DATA_FFT1024	0x80000001	// 1024 FFT
#define BASS_DATA_FFT2048	0x80000002	// 2048 FFT
#define BASS_DATA_FFT512S	0x80000010	// stereo 512 sample FFT
#define BASS_DATA_FFT1024S	0x80000011	// stereo 1024 FFT
#define BASS_DATA_FFT2048S	0x80000012	// stereo 2048 FFT
#define BASS_DATA_FFT_NOWINDOW	0x20	// FFT flag: no Hanning window

// BASS_StreamGetTags flags : what's returned
#define BASS_TAG_ID3		0	// ID3v1 tags : 128 byte block
#define BASS_TAG_ID3V2		1	// ID3v2 tags : variable length block
#define BASS_TAG_OGG		2	// OGG comments : array of null-terminated strings
#define BASS_TAG_HTTP		3	// HTTP headers : array of null-terminated strings
#define BASS_TAG_ICY		4	// ICY headers : array of null-terminated strings
#define BASS_TAG_META		5	// ICY metadata : null-terminated string

// DX8 effect types, use with BASS_ChannelSetFX
enum
{
	BASS_FX_CHORUS,			// GUID_DSFX_STANDARD_CHORUS
	BASS_FX_COMPRESSOR,		// GUID_DSFX_STANDARD_COMPRESSOR
	BASS_FX_DISTORTION,		// GUID_DSFX_STANDARD_DISTORTION
	BASS_FX_ECHO,			// GUID_DSFX_STANDARD_ECHO
	BASS_FX_FLANGER,		// GUID_DSFX_STANDARD_FLANGER
	BASS_FX_GARGLE,			// GUID_DSFX_STANDARD_GARGLE
	BASS_FX_I3DL2REVERB,	// GUID_DSFX_STANDARD_I3DL2REVERB
	BASS_FX_PARAMEQ,		// GUID_DSFX_STANDARD_PARAMEQ
	BASS_FX_REVERB			// GUID_DSFX_WAVES_REVERB
};

typedef struct {
    float       fWetDryMix;
    float       fDepth;
    float       fFeedback;
    float       fFrequency;
    DWORD       lWaveform;	// 0=triangle, 1=sine
    float       fDelay;
    DWORD       lPhase;		// BASS_FX_PHASE_xxx
} BASS_FXCHORUS;		// DSFXChorus

typedef struct {
    float   fGain;
    float   fAttack;
    float   fRelease;
    float   fThreshold;
    float   fRatio;
    float   fPredelay;
} BASS_FXCOMPRESSOR;	// DSFXCompressor

typedef struct {
    float   fGain;
    float   fEdge;
    float   fPostEQCenterFrequency;
    float   fPostEQBandwidth;
    float   fPreLowpassCutoff;
} BASS_FXDISTORTION;	// DSFXDistortion

typedef struct {
    float   fWetDryMix;
    float   fFeedback;
    float   fLeftDelay;
    float   fRightDelay;
    BOOL    lPanDelay;
} BASS_FXECHO;			// DSFXEcho

typedef struct {
    float       fWetDryMix;
    float       fDepth;
    float       fFeedback;
    float       fFrequency;
    DWORD       lWaveform;	// 0=triangle, 1=sine
    float       fDelay;
    DWORD       lPhase;		// BASS_FX_PHASE_xxx
} BASS_FXFLANGER;		// DSFXFlanger

typedef struct {
    DWORD       dwRateHz;               // Rate of modulation in hz
    DWORD       dwWaveShape;            // 0=triangle, 1=square
} BASS_FXGARGLE;		// DSFXGargle

typedef struct {
    int     lRoom;                  // [-10000, 0]      default: -1000 mB
    int     lRoomHF;                // [-10000, 0]      default: 0 mB
    float   flRoomRolloffFactor;    // [0.0, 10.0]      default: 0.0
    float   flDecayTime;            // [0.1, 20.0]      default: 1.49s
    float   flDecayHFRatio;         // [0.1, 2.0]       default: 0.83
    int     lReflections;           // [-10000, 1000]   default: -2602 mB
    float   flReflectionsDelay;     // [0.0, 0.3]       default: 0.007 s
    int     lReverb;                // [-10000, 2000]   default: 200 mB
    float   flReverbDelay;          // [0.0, 0.1]       default: 0.011 s
    float   flDiffusion;            // [0.0, 100.0]     default: 100.0 %
    float   flDensity;              // [0.0, 100.0]     default: 100.0 %
    float   flHFReference;          // [20.0, 20000.0]  default: 5000.0 Hz
} BASS_FXI3DL2REVERB;	// DSFXI3DL2Reverb

typedef struct {
    float   fCenter;
    float   fBandwidth;
    float   fGain;
} BASS_FXPARAMEQ;		// DSFXParamEq

typedef struct {
    float   fInGain;                // [-96.0,0.0]            default: 0.0 dB
    float   fReverbMix;             // [-96.0,0.0]            default: 0.0 db
    float   fReverbTime;            // [0.001,3000.0]         default: 1000.0 ms
    float   fHighFreqRTRatio;       // [0.001,0.999]          default: 0.001
} BASS_FXREVERB;		// DSFXWavesReverb

#define BASS_FX_PHASE_NEG_180        0
#define BASS_FX_PHASE_NEG_90         1
#define BASS_FX_PHASE_ZERO           2
#define BASS_FX_PHASE_90             3
#define BASS_FX_PHASE_180            4

#define CDCHANNEL	0	// CD channel, for use with BASS_Channel functions
#define RECORDCHAN	1	// Recording channel, for use with BASS_Channel functions

// BASS_ChannelIsActive return values
#define BASS_ACTIVE_STOPPED	0
#define BASS_ACTIVE_PLAYING	1
#define BASS_ACTIVE_STALLED	2
#define BASS_ACTIVE_PAUSED	3

// BASS_ChannelIsSliding return flags
#define BASS_SLIDE_FREQ	1
#define BASS_SLIDE_VOL	2
#define BASS_SLIDE_PAN	4

// BASS_CDGetID flags
#define BASS_CDID_IDENTITY	0
#define BASS_CDID_UPC		1
#define BASS_CDID_CDDB		2
#define BASS_CDID_CDDB2		3

// BASS_RecordSetInput flags
#define BASS_INPUT_OFF		0x10000
#define BASS_INPUT_ON		0x20000
#define BASS_INPUT_LEVEL	0x40000

#define BASS_INPUT_TYPE_MASK		0xff000000
#define BASS_INPUT_TYPE_UNDEF		0x00000000
#define BASS_INPUT_TYPE_DIGITAL		0x01000000
#define BASS_INPUT_TYPE_LINE		0x02000000
#define BASS_INPUT_TYPE_MIC			0x03000000
#define BASS_INPUT_TYPE_SYNTH		0x04000000
#define BASS_INPUT_TYPE_CD			0x05000000
#define BASS_INPUT_TYPE_PHONE		0x06000000
#define BASS_INPUT_TYPE_SPEAKER		0x07000000
#define BASS_INPUT_TYPE_WAVE		0x08000000
#define BASS_INPUT_TYPE_AUX			0x09000000
#define BASS_INPUT_TYPE_ANALOG		0x0a000000

// BASS_SetNetConfig flags
#define BASS_NET_TIMEOUT	0
#define BASS_NET_BUFFER		1


DWORD BASSDEF(BASS_GetVersion)();
/* Retrieve the version number of BASS that is loaded.
RETURN : The BASS version (LOWORD.HIWORD) */

char *BASSDEF(BASS_GetDeviceDescription)(DWORD devnum);
/* Get the text description of a device. This function can be used to
enumerate the available devices.
devnum : The device (0=first)
RETURN : The text description of the device (NULL=error) */

float BASSDEF(BASS_SetBufferLength)(float length);
/* Set the amount that BASS mixes ahead new musics/streams. Changing
this setting does not affect musics/streams that have already been
loaded/created. Increasing the buffer length, decreases the chance of
the sound possibly breaking-up on slower computers, but increase DSP
latency. The default length is 0.5 secs.
length : The buffer length in seconds
RETURN : The actual new buffer length */

DWORD BASSDEF(BASS_SetNetConfig)(DWORD option, DWORD value);
/* Set an internet config option.
option : The option to set (one of the BASS_NET_xxx flags)
value  : The new value
RETURN : The old value (-1=error) */

void BASSDEF(BASS_SetGlobalVolumes)(int musvol, int samvol, int strvol);
/* Set the global music/sample/stream volume levels.
musvol : MOD music global volume level (0-100, -1=leave current)
samvol : Sample global volume level (0-100, -1=leave current)
strvol : Stream global volume level (0-100, -1=leave current) */

void BASSDEF(BASS_GetGlobalVolumes)(DWORD *musvol, DWORD *samvol, DWORD *strvol);
/* Retrieve the global music/sample/stream volume levels.
musvol : MOD music global volume level (NULL=don't retrieve it)
samvol : Sample global volume level (NULL=don't retrieve it)
strvol : Stream global volume level (NULL=don't retrieve it) */

void BASSDEF(BASS_SetLogCurves)(BOOL volume, BOOL pan);
/* Make the volume/panning values translate to a logarithmic curve,
or a linear "curve" (the default).
volume : volume curve (FALSE=linear, TRUE=log)
pan    : panning curve (FALSE=linear, TRUE=log) */

void BASSDEF(BASS_Set3DAlgorithm)(DWORD algo);
/* Set the 3D algorithm for software mixed 3D channels (does not affect
hardware mixed channels). Changing the mode only affects subsequently
created or loaded samples/streams/musics, not those that already exist.
Requires DirectX 7 or above.
algo   : algorithm flag (BASS_3DALG_xxx) */

DWORD BASSDEF(BASS_ErrorGetCode)();
/* Get the BASS_ERROR_xxx error code. Use this function to get the
reason for an error. */

void BASSDEF(BASS_SetCLSID)(GUID *clsid);
/* Set the class identifier of the object to create, that will be used
to initialize DirectSound.
clsid  : Class identifier of the object to create (NULL=use default) */

BOOL BASSDEF(BASS_Init)(int device, DWORD freq, DWORD flags, HWND win);
/* Initialize the digital output. This must be called before all following
BASS functions (except CD and recording functions).
device : Device to use (0=first, -1=default, -2=no sound)
freq   : Output sample rate
flags  : BASS_DEVICE_xxx flags (optional HIWORD=update period)
win    : Owner window (0=current foreground window)

NOTE: The "no sound" device (device=-2), allows loading and "playing"
of MOD musics only (all sample/stream functions and most other functions
fail). This is so that you can still use the MOD musics as synchronizers
when there is no soundcard present. When using device -2, you should still
set the other arguments as you would do normally. */

void BASSDEF(BASS_Free)();
/* Free all resources used by the digital output, including all musics
and samples. */

void *BASSDEF(BASS_GetDSoundObject)(DWORD object);
/* Retrieve a pointer to a DirectSound interface. This can be used by
advanced users to "plugin" external functionality.
object : The interface to retrieve (BASS_OBJECT_xxx)
RETURN : A pointer to the requested interface (NULL=error) */

void BASSDEF(BASS_GetInfo)(BASS_INFO *info);
/* Retrieve some information on the device being used.
info   : Pointer to store info at */

BOOL BASSDEF(BASS_Update)();
/* Update the HMUSIC/HSTREAM channel buffers. */

float BASSDEF(BASS_GetCPU)();
/* Get the current CPU usage of BASS. This includes the time taken to mix
the MOD musics and sample streams, and also the time taken by any user
DSP functions. It does not include plain sample mixing which is done by
the output device (hardware accelerated) or DirectSound (emulated). Audio
CD playback requires no CPU usage.
RETURN : The CPU usage percentage (floating-point) */

BOOL BASSDEF(BASS_Start)();
/* Start the digital output. */

BOOL BASSDEF(BASS_Stop)();
/* Stop the digital output, stopping all musics/samples/streams. */

BOOL BASSDEF(BASS_Pause)();
/* Stop the digital output, pausing all musics/samples/streams. Use
BASS_Start to resume the digital output. */

BOOL BASSDEF(BASS_SetVolume)(DWORD volume);
/* Set the digital output master volume.
volume : Desired volume level (0-100) */

int BASSDEF(BASS_GetVolume)();
/* Get the digital output master volume.
RETURN : The volume level (0-100, -1=error) */

BOOL BASSDEF(BASS_Set3DFactors)(float distf, float rollf, float doppf);
/* Set the factors that affect the calculations of 3D sound.
distf  : Distance factor (0.0-10.0, 1.0=use meters, 0.3=use feet, <0.0=leave current)
         By default BASS measures distances in meters, you can change this
         setting if you are using a different unit of measurement.
roolf  : Rolloff factor, how fast the sound quietens with distance
         (0.0=no rolloff, 1.0=real world, 2.0=2x real... 10.0=max, <0.0=leave current)
doppf  : Doppler factor (0.0=no doppler, 1.0=real world, 2.0=2x real... 10.0=max, <0.0=leave current)
         The doppler effect is the way a sound appears to change frequency when it is
         moving towards or away from you. The listener and sound velocity settings are
         used to calculate this effect, this "doppf" value can be used to lessen or
         exaggerate the effect. */

BOOL BASSDEF(BASS_Get3DFactors)(float *distf, float *rollf, float *doppf);
/* Get the factors that affect the calculations of 3D sound.
distf  : Distance factor (NULL=don't get it)
roolf  : Rolloff factor (NULL=don't get it)
doppf  : Doppler factor (NULL=don't get it) */

BOOL BASSDEF(BASS_Set3DPosition)(BASS_3DVECTOR *pos, BASS_3DVECTOR *vel, BASS_3DVECTOR *front, BASS_3DVECTOR *top);
/* Set the position/velocity/orientation of the listener (ie. the player/viewer).
pos    : Position of the listener (NULL=leave current)
vel    : Listener's velocity, used to calculate doppler effect (NULL=leave current)
front  : Direction that listener's front is pointing (NULL=leave current)
top    : Direction that listener's top is pointing (NULL=leave current)
         NOTE: front & top must both be set in a single call */

BOOL BASSDEF(BASS_Get3DPosition)(BASS_3DVECTOR *pos, BASS_3DVECTOR *vel, BASS_3DVECTOR *front, BASS_3DVECTOR *top);
/* Get the position/velocity/orientation of the listener.
pos    : Position of the listener (NULL=don't get it)
vel    : Listener's velocity (NULL=don't get it)
front  : Direction that listener's front is pointing (NULL=don't get it)
top    : Direction that listener's top is pointing (NULL=don't get it)
         NOTE: front & top must both be retrieved in a single call */

BOOL BASSDEF(BASS_Apply3D)();
/* Apply changes made to the 3D system. This must be called to apply any changes
made with BASS_Set3DFactors, BASS_Set3DPosition, BASS_ChannelSet3DAttributes or
BASS_ChannelSet3DPosition. It improves performance to have DirectSound do all the
required recalculating at the same time like this, rather than recalculating after
every little change is made. NOTE: This is automatically called when starting a 3D
sample with BASS_SamplePlay3D/Ex. */

BOOL BASSDEF(BASS_SetEAXParameters)(int env, float vol, float decay, float damp);
/* Set the type of EAX environment and it's parameters. Obviously, EAX functions
have no effect if no EAX supporting device (ie. SB Live) is used.
env    : Reverb environment (EAX_ENVIRONMENT_xxx, -1=leave current)
vol    : Volume of the reverb (0.0=off, 1.0=max, <0.0=leave current)
decay  : Time in seconds it takes the reverb to diminish by 60dB (0.1-20.0, <0.0=leave current)
damp   : The damping, high or low frequencies decay faster (0.0=high decays quickest,
         1.0=low/high decay equally, 2.0=low decays quickest, <0.0=leave current) */

BOOL BASSDEF(BASS_GetEAXParameters)(DWORD *env, float *vol, float *decay, float *damp);
/* Get the current EAX parameters.
env    : Reverb environment (NULL=don't get it)
vol    : Reverb volume (NULL=don't get it)
decay  : Decay duration (NULL=don't get it)
damp   : The damping (NULL=don't get it) */


HMUSIC BASSDEF(BASS_MusicLoad)(BOOL mem, void *file, DWORD offset, DWORD length, DWORD flags);
/* Load a music (MO3/XM/MOD/S3M/IT/MTM). The amplification and pan
seperation are initially set to 50, use BASS_MusicSetAmplify
and BASS_MusicSetPanSep to adjust them.
mem    : TRUE = Load music from memory
file   : Filename (mem=FALSE) or memory location (mem=TRUE)
offset : File offset to load the music from (only used if mem=FALSE)
length : Data length (only used if mem=FALSE, 0=use to end of file)
flags  : BASS_MUSIC_xxx flags
RETURN : The loaded music's handle (NULL=error) */

void BASSDEF(BASS_MusicFree)(HMUSIC handle);
/* Free a music's resources.
handle : Music handle */

char *BASSDEF(BASS_MusicGetName)(HMUSIC handle);
/* Retrieves a music's name.
handle : Music handle
RETURN : The music's name (NULL=error) */

DWORD BASSDEF(BASS_MusicGetLength)(HMUSIC handle, BOOL playlen);
/* Retrieves the length of a music in patterns (how many "orders" there are)
or in output bytes (requires BASS_MUSIC_CALCLEN was used with BASS_MusicLoad).
handle : Music handle
playlen: TRUE=get the playback length, FALSE=get the pattern length
RETURN : The length of the music (-1=error) */

BOOL BASSDEF(BASS_MusicPreBuf)(HMUSIC handle);
/* Pre-buffer initial sample data ready for playback.
handle : Handle of music */

BOOL BASSDEF(BASS_MusicPlay)(HMUSIC handle);
/* Play a music. Playback continues from where it was last stopped/paused.
Multiple musics may be played simultaneously.
handle : Handle of music to play */

BOOL BASSDEF(BASS_MusicPlayEx)(HMUSIC handle, DWORD pos, int flags, BOOL reset);
/* Play a music, specifying start position and playback flags.
handle : Handle of music to play
pos    : Position to start playback from, LOWORD=order HIWORD=row
flags  : BASS_MUSIC_xxx flags. These flags overwrite the defaults
         specified when the music was loaded. (-1=use current flags)
reset  : TRUE = Stop all current playing notes and reset bpm/etc... */

BOOL BASSDEF(BASS_MusicSetAmplify)(HMUSIC handle, DWORD amp);
/* Set a music's amplification level.
handle : Music handle
amp    : Amplification level (0-100) */

BOOL BASSDEF(BASS_MusicSetPanSep)(HMUSIC handle, DWORD pan);
/* Set a music's pan seperation.
handle : Music handle
pan    : Pan seperation (0-100, 50=linear) */

BOOL BASSDEF(BASS_MusicSetPositionScaler)(HMUSIC handle, DWORD scale);
/* Set a music's "GetPosition" scaler
When you call BASS_ChannelGetPosition, the "row" (HIWORD) will be
scaled by this value. By using a higher scaler, you can get a more
precise position indication.
handle : Music handle
scale  : The scaler (1-256) */

BOOL BASSDEF(BASS_MusicSetChannelVol)(HMUSIC handle, DWORD channel, DWORD volume);
/* Set the volume level of a channel in a music
handle : Music handle
channel: Channel number (0=first)
volume : Volume level (0-100) */

int BASSDEF(BASS_MusicGetChannelVol)(HMUSIC handle, DWORD channel);
/* Get the volume level of a channel in a music
handle : Music handle
channel: Channel number (0=first)
RETURN : The channel's volume (-1=error) */


HSAMPLE BASSDEF(BASS_SampleLoad)(BOOL mem, void *file, DWORD offset, DWORD length, DWORD max, DWORD flags);
/* Load a WAV/MP3/MP2/MP1 sample. If you're loading a sample with 3D
functionality, then you should use BASS_GetInfo and BASS_SetInfo to set
the default 3D parameters. You can also use these two functions to set
the sample's default frequency/volume/pan/looping.
mem    : TRUE = Load sample from memory
file   : Filename (mem=FALSE) or memory location (mem=TRUE)
offset : File offset to load the sample from (only used if mem=FALSE)
length : Data length (only used if mem=FALSE, 0=use to end of file)
max    : Maximum number of simultaneous playbacks (1-65535)
flags  : BASS_SAMPLE_xxx flags (only the LOOP/3D/SOFTWARE/VAM/MUTEMAX/OVER_xxx flags are used)
RETURN : The loaded sample's handle (NULL=error) */

void* BASSDEF(BASS_SampleCreate)(DWORD length, DWORD freq, DWORD max, DWORD flags);
/* Create a sample. This function allows you to generate custom samples, or
load samples that are not in the WAV format. A pointer is returned to the
memory location at which you should write the sample's data. After writing
the data, call BASS_SampleCreateDone to get the new sample's handle.
length : The sample's length (in samples, NOT bytes)
freq   : default sample rate
max    : Maximum number of simultaneous playbacks (1-65535)
flags  : BASS_SAMPLE_xxx flags
RETURN : Memory location to write the sample's data (NULL=error) */

HSAMPLE BASSDEF(BASS_SampleCreateDone)();
/* Finished creating a new sample.
RETURN : The new sample's handle (NULL=error) */

void BASSDEF(BASS_SampleFree)(HSAMPLE handle);
/* Free a sample's resources.
handle : Sample handle */

BOOL BASSDEF(BASS_SampleGetInfo)(HSAMPLE handle, BASS_SAMPLE *info);
/* Retrieve a sample's current default attributes.
handle : Sample handle
info   : Pointer to store sample info */

BOOL BASSDEF(BASS_SampleSetInfo)(HSAMPLE handle, BASS_SAMPLE *info);
/* Set a sample's default attributes.
handle : Sample handle
info   : Sample info, only the freq/volume/pan/3D attributes and
         looping/override method flags are used */

HCHANNEL BASSDEF(BASS_SamplePlay)(HSAMPLE handle);
/* Play a sample, using the sample's default attributes.
handle : Handle of sample to play
RETURN : Handle of channel used to play the sample (NULL=error) */

HCHANNEL BASSDEF(BASS_SamplePlayEx)(HSAMPLE handle, DWORD start, int freq, int volume, int pan, BOOL loop);
/* Play a sample, using specified attributes.
handle : Handle of sample to play
start  : Playback start position (in samples, not bytes)
freq   : Playback rate (-1=default)
volume : Volume (-1=default, 0=silent, 100=max)
pan    : Pan position (-101=default, -100=left, 0=middle, 100=right)
loop   : TRUE = Loop sample (-1=default)
RETURN : Handle of channel used to play the sample (NULL=error) */

HCHANNEL BASSDEF(BASS_SamplePlay3D)(HSAMPLE handle, BASS_3DVECTOR *pos, BASS_3DVECTOR *orient, BASS_3DVECTOR *vel);
/* Play a 3D sample, setting it's 3D position, orientation and velocity.
handle : Handle of sample to play
pos    : position of the sound (NULL = x/y/z=0.0)
orient : orientation of the sound, this is irrelevant if it's an
         omnidirectional sound source (NULL = x/y/z=0.0)
vel    : velocity of the sound (NULL = x/y/z=0.0)
RETURN : Handle of channel used to play the sample (NULL=error) */

HCHANNEL BASSDEF(BASS_SamplePlay3DEx)(HSAMPLE handle, BASS_3DVECTOR *pos, BASS_3DVECTOR *orient, BASS_3DVECTOR *vel, DWORD start, int freq, int volume, BOOL loop);
/* Play a 3D sample, using specified attributes.
handle : Handle of sample to play
pos    : position of the sound (NULL = x/y/z=0.0)
orient : orientation of the sound, this is irrelevant if it's an
         omnidirectional sound source (NULL = x/y/z=0.0)
vel    : velocity of the sound (NULL = x/y/z=0.0)
start  : Playback start position (in samples, not bytes)
freq   : Playback rate (-1=default)
volume : Volume (-1=default, 0=silent, 100=max)
loop   : TRUE = Loop sample (-1=default)
RETURN : Handle of channel used to play the sample (NULL=error) */

BOOL BASSDEF(BASS_SampleStop)(HSAMPLE handle);
/* Stops all instances of a sample. For example, if a sample is playing
simultaneously 3 times, calling this function will stop all 3 of them,
which is obviously simpler than calling BASS_ChannelStop 3 times.
handle : Handle of sample to stop */


HSTREAM BASSDEF(BASS_StreamCreate)(DWORD freq, DWORD flags, void *proc, DWORD user);
/* Create a user sample stream.
freq   : Stream playback rate (100-100000)
flags  : BASS_SAMPLE_xxx flags (only the 8BITS/FLOAT/MONO/3D flags are used)
proc   : User defined stream writing function
user   : The 'user' value passed to the callback function
RETURN : The created stream's handle (NULL=error) */

HSTREAM BASSDEF(BASS_StreamCreateFile)(BOOL mem, void *file, DWORD offset, DWORD length, DWORD flags);
/* Create a sample stream from an MP3/MP2/MP1/OGG or WAV file.
mem    : TRUE = Stream file from memory
file   : Filename (mem=FALSE) or memory location (mem=TRUE)
offset : File offset of the stream data
length : File length (0=use whole file if mem=FALSE)
flags  : Flags
RETURN : The created stream's handle (NULL=error) */

HSTREAM BASSDEF(BASS_StreamCreateURL)(char *url, DWORD offset, DWORD flags, char *save);
/* Create a sample stream from an MP3/MP2/MP1/OGG or WAV file on the internet,
optionally saving a local copy to disk.
url    : The URL (beginning with "http://" or "ftp://")
offset : File offset of start streaming from
flags  : Flags
save   : Filename to save the streamed file as locally (NULL=don't save)
RETURN : The created stream's handle (NULL=error) */

void BASSDEF(BASS_StreamFree)(HSTREAM handle);
/* Free a sample stream's resources.
handle : Stream handle */

QWORD BASSDEF(BASS_StreamGetLength)(HSTREAM handle);
/* Retrieves the playback length (in bytes) of a file stream. It's not always
possible to 100% accurately guess the length of a stream, so the length returned
may be only an approximation when using some WAV codecs.
handle : Stream handle 
RETURN : The length (0=streaming in blocks, -1=error) */

char *BASSDEF(BASS_StreamGetTags)(HSTREAM handle, DWORD tags);
/* Retrieves the requested tags/headers, if available.
handle : Stream handle
tags   : A BASS_TAG_xxx flag
RETURN : Pointer to the tags (NULL=error) */

BOOL BASSDEF(BASS_StreamPreBuf)(HSTREAM handle);
/* Pre-buffer initial sample data ready for playback.
handle : Handle of stream */

BOOL BASSDEF(BASS_StreamPlay)(HSTREAM handle, BOOL flush, DWORD flags);
/* Play a sample stream, optionally flushing the buffer first.
handle : Handle of stream to play
flush  : Flush buffer contents. If you stop a stream and then want to
         continue it from where it stopped, don't flush it. Flushing
         a file stream causes it to restart from the beginning.
flags  : BASS_SAMPLE_LOOP flag (only affects file streams) */

DWORD BASSDEF(BASS_StreamGetFilePosition)(HSTREAM handle, DWORD mode);
/* Retrieves the file position of the decoding, the download (if streaming from
the internet), or the end (total length). Obviously only works with file streams.
handle : Stream handle
mode   : The position to retrieve (0=decoding, 1=download, 2=end)
RETURN : The position (-1=error) */


BOOL BASSDEF(BASS_CDInit)(char *drive, DWORD flags);
/* Initialize the CD functions, must be called before any other CD
functions. The volume is initially set to 100 (the maximum), use
BASS_ChannelSetAttributes to adjust it.
drive  : The CD drive, for example: "d:" (NULL=use default drive)
flags  : BASS_DEVICE_LEAVEVOL=leave the volume as it is */

void BASSDEF(BASS_CDFree)();
/* Free resources used by the CD. */

BOOL BASSDEF(BASS_CDInDrive)();
/* Check if there is a CD in the drive. */

BOOL BASSDEF(BASS_CDDoor)(BOOL open);
/* Opens or closes the CD door.
open   : TRUE=open the door */

char *BASSDEF(BASS_CDGetID)(DWORD id);
/* Retrieves identification info from the CD in the drive.
id     : One of the BASS_CDID_xxx flags
RETURN : ID string (NULL=error) */

DWORD BASSDEF(BASS_CDGetTracks)();
/* Retrieves the number of tracks on the CD
RETURN : The number of tracks (-1=error) */

BOOL BASSDEF(BASS_CDPlay)(DWORD track, BOOL loop, BOOL wait);
/* Play a CD track.
track  : Track number to play (1=first)
loop   : TRUE = Loop the track
wait   : TRUE = don't return until playback has started (some drives
                will always wait anyway) */

DWORD BASSDEF(BASS_CDGetTrackLength)(DWORD track);
/* Retrieves the playback length (in milliseconds) of a cd track.
track  : The CD track (1=first)
RETURN : The length (-1=error) */


char *BASSDEF(BASS_RecordGetDeviceDescription)(DWORD devnum);
/* Get the text description of a recording device. This function can be
used to enumerate the available devices.
devnum : The device (0=first)
RETURN : The text description of the device (NULL=error) */

BOOL BASSDEF(BASS_RecordInit)(int device);
/* Initialize a recording device.
device : Device to use (0=first, -1=default) */

void BASSDEF(BASS_RecordFree)();
/* Free all resources used by the recording device. */

void BASSDEF(BASS_RecordGetInfo)(BASS_RECORDINFO *info);
/* Retrieve some information on the recording device being used.
info   : Pointer to store info at */

char *BASSDEF(BASS_RecordGetInputName)(DWORD input);
/* Get the text description of a recording input.
input  : Input number (0=first)
RETURN : The text description (NULL=error) */

BOOL BASSDEF(BASS_RecordSetInput)(DWORD input, DWORD setting);
/* Adjust the setting of a recording input.
input  : Input number (0=first)
setting: BASS_INPUT flags (if BASS_INPUT_LEVEL used, LOWORD=volume) */

DWORD BASSDEF(BASS_RecordGetInput)(DWORD input);
/* Retrieve the setting of a recording input.
input  : Input number (0=first)
RETURN : The setting (LOWORD=volume, with BASS_INPUT_OFF flag if off, -1=error) */

BOOL BASSDEF(BASS_RecordStart)(DWORD freq, DWORD flags, RECORDPROC *proc, DWORD user);
/* Start recording. Use BASS_ChannelStop to stop recording.
freq   : Sampling rate
flags  : BASS_SAMPLE_8BITS/FLOAT/MONO flags (optional HIWORD=update period)
proc   : User defined function to receive the recorded data
user   : The 'user' value passed to the callback function */


/* A "channel" can be a playing sample (HCHANNEL), a MOD music (HMUSIC), a
sample stream (HSTREAM), the CD (CDCHANNEL), or recording (RECORDCHAN). The
following functions can be used with one or more of these channel types. */

float BASSDEF(BASS_ChannelBytes2Seconds)(DWORD handle, QWORD pos);
/* Translate a byte position into time (seconds)
handle : Handle of channel (HCHANNEL/HMUSIC/HSTREAM, or RECORDCHAN, also HSAMPLE)
pos    : The position to translate
RETURN : The millisecond position (<0=error) */

QWORD BASSDEF(BASS_ChannelSeconds2Bytes)(DWORD handle, float pos);
/* Translate a time (seconds) position into bytes
handle : Handle of channel (HCHANNEL/HMUSIC/HSTREAM, or RECORDCHAN, also HSAMPLE)
pos    : The position to translate
RETURN : The byte position (-1=error) */

DWORD BASSDEF(BASS_ChannelIsActive)(DWORD handle);
/* Check if a channel is active (playing) or stalled.
handle : Channel handle (HCHANNEL/HMUSIC/HSTREAM, or CDCHANNEL/RECORDCHAN)
RETURN : One of the BASS_ACTIVE_xxx values */

DWORD BASSDEF(BASS_ChannelGetFlags)(DWORD handle);
/* Get some info about a channel.
handle : Channel handle (HCHANNEL/HMUSIC/HSTREAM)
RETURN : BASS_SAMPLE_xxx flags (-1=error) */

BOOL BASSDEF(BASS_ChannelStop)(DWORD handle);
/* Stop a channel.
handle : Channel handle (HCHANNEL/HMUSIC/HSTREAM, or CDCHANNEL/RECORDCHAN) */

BOOL BASSDEF(BASS_ChannelPause)(DWORD handle);
/* Pause a channel.
handle : Channel handle (HCHANNEL/HMUSIC/HSTREAM, or CDCHANNEL/RECORDCHAN) */

BOOL BASSDEF(BASS_ChannelResume)(DWORD handle);
/* Resume a paused channel.
handle : Channel handle (HCHANNEL/HMUSIC/HSTREAM, or CDCHANNEL/RECORDCHAN) */

BOOL BASSDEF(BASS_ChannelSetAttributes)(DWORD handle, int freq, int volume, int pan);
/* Update a channel's attributes. The actual setting may not be exactly
as specified, depending on the accuracy of the device and drivers.
NOTE: Only the volume can be adjusted for the CD "channel", but not all
soundcards allow controlling of the CD volume level.
handle : Channel handle (HCHANNEL/HMUSIC/HSTREAM, or CDCHANNEL)
freq   : Playback rate (100-100000, 0=original, -1=leave current)
volume : Volume (-1=leave current, 0=silent, 100=max)
pan    : Pan position (-101=current, -100=left, 0=middle, 100=right)
         panning has no effect on 3D channels */

BOOL BASSDEF(BASS_ChannelGetAttributes)(DWORD handle, DWORD *freq, DWORD *volume, int *pan);
/* Retrieve a channel's attributes. Only the volume is available for
the CD "channel" (if allowed by the soundcard/drivers).
handle : Channel handle (HCHANNEL/HMUSIC/HSTREAM, or CDCHANNEL)
freq   : Pointer to store playback rate (NULL=don't retrieve it)
volume : Pointer to store volume (NULL=don't retrieve it)
pan    : Pointer to store pan position (NULL=don't retrieve it) */

BOOL BASSDEF(BASS_ChannelSlideAttributes)(DWORD handle, int freq, int volume, int pan, DWORD time);
/* Slide a channel's attributes over a period of time.
handle : Channel handle (HCHANNEL/HMUSIC/HSTREAM, or CDCHANNEL)
freq   : Playback rate (100-100000, -1=leave current)
volume : Volume (-1=leave current, 0=silent, 100=max, -2 = fadeout and stop channel)
pan    : Pan position (-101=current, -100=left, 0=middle, 100=right)
time   : Duration of the slide (in milliseconds) */

DWORD BASSDEF(BASS_ChannelIsSliding)(DWORD handle);
/* Check if a channel's attributes (freq/vol/pan) are sliding.
handle : Channel handle (HCHANNEL/HMUSIC/HSTREAM, or CDCHANNEL)
RETURN : Combination of the BASS_SLIDE_xxx flags */

BOOL BASSDEF(BASS_ChannelSet3DAttributes)(DWORD handle, int mode, float min, float max, int iangle, int oangle, int outvol);
/* Set a channel's 3D attributes.
handle : Channel handle (HCHANNEL/HSTREAM/HMUSIC)
mode   : BASS_3DMODE_xxx mode (-1=leave current setting)
min    : minimum distance, volume stops increasing within this distance (<0.0=leave current)
max    : maximum distance, volume stops decreasing past this distance (<0.0=leave current)
iangle : angle of inside projection cone in degrees (360=omnidirectional, -1=leave current)
oangle : angle of outside projection cone in degrees (-1=leave current)
         NOTE: iangle & oangle must both be set in a single call
outvol : delta-volume outside the projection cone (0=silent, 100=same as inside)
The iangle/oangle angles decide how wide the sound is projected around the
orientation angle. Within the inside angle the volume level is the channel
level as set with BASS_ChannelSetAttributes, from the inside to the outside
angles the volume gradually changes by the "outvol" setting. */

BOOL BASSDEF(BASS_ChannelGet3DAttributes)(DWORD handle, DWORD *mode, float *min, float *max, DWORD *iangle, DWORD *oangle, DWORD *outvol);
/* Retrieve a channel's 3D attributes.
handle : Channel handle (HCHANNEL/HSTREAM/HMUSIC)
mode   : BASS_3DMODE_xxx mode (NULL=don't retrieve it)
min    : minumum distance (NULL=don't retrieve it)
max    : maximum distance (NULL=don't retrieve it)
iangle : angle of inside projection cone (NULL=don't retrieve it)
oangle : angle of outside projection cone (NULL=don't retrieve it)
         NOTE: iangle & oangle must both be retrieved in a single call
outvol : delta-volume outside the projection cone (NULL=don't retrieve it) */

BOOL BASSDEF(BASS_ChannelSet3DPosition)(DWORD handle, BASS_3DVECTOR *pos, BASS_3DVECTOR *orient, BASS_3DVECTOR *vel);
/* Update a channel's 3D position, orientation and velocity. The velocity
is only used to calculate the doppler effect.
handle : Channel handle (HCHANNEL/HSTREAM/HMUSIC)
pos    : position of the sound (NULL=leave current)
orient : orientation of the sound, this is irrelevant if it's an
         omnidirectional sound source (NULL=leave current)
vel    : velocity of the sound (NULL=leave current) */

BOOL BASSDEF(BASS_ChannelGet3DPosition)(DWORD handle, BASS_3DVECTOR *pos, BASS_3DVECTOR *orient, BASS_3DVECTOR *vel);
/* Retrieve a channel's current 3D position, orientation and velocity.
handle : Channel handle (HCHANNEL/HSTREAM/HMUSIC)
pos    : position of the sound (NULL=don't retrieve it)
orient : orientation of the sound, this is irrelevant if it's an
         omnidirectional sound source (NULL=don't retrieve it)
vel    : velocity of the sound (NULL=don't retrieve it) */

BOOL BASSDEF(BASS_ChannelSetPosition)(DWORD handle, QWORD pos);
/* Set the current playback position of a channel.
handle : Channel handle (HCHANNEL/HMUSIC/HSTREAM, or CDCHANNEL)
pos    : the position
    if HCHANNEL: position in bytes
    if HMUSIC: LOWORD=order HIWORD=row ... use MAKELONG(order,row)
    if HSTREAM: position in bytes, file streams only
    if CDCHANNEL: position in milliseconds from start of track */

QWORD BASSDEF(BASS_ChannelGetPosition)(DWORD handle);
/* Get the current playback position of a channel.
handle : Channel handle (HCHANNEL/HMUSIC/HSTREAM, or CDCHANNEL)
RETURN : the position (-1=error)
	if HCHANNEL: position in bytes
	if HMUSIC: LOWORD=order HIWORD=row (see BASS_MusicSetPositionScaler)
	if HSTREAM: total bytes played since the stream was last flushed
	if CDCHANNEL: position in milliseconds from start of track */

DWORD BASSDEF(BASS_ChannelGetLevel)(DWORD handle);
/* Calculate a channel's current output level.
handle : Channel handle (HMUSIC/HSTREAM, or RECORDCHAN)
RETURN : LOWORD=left level (0-128) HIWORD=right level (0-128) (-1=error) */

DWORD BASSDEF(BASS_ChannelGetData)(DWORD handle, void *buffer, DWORD length);
/* Retrieves upto "length" bytes of the channel's current sample data. This is
useful if you wish to "visualize" the sound.
handle : Channel handle (HMUSIC/HSTREAM, or RECORDCHAN)
buffer : Location to write the data
length : Number of bytes of wanted, or a BASS_DATA_xxx flag
RETURN : Number of bytes actually written to the buffer (-1=error) */

HSYNC BASSDEF(BASS_ChannelSetSync)(DWORD handle, DWORD type, QWORD param, SYNCPROC *proc, DWORD user);
/* Setup a sync on a channel. Multiple syncs may be used per channel.
handle : Channel handle
type   : Sync type (BASS_SYNC_xxx type & flags)
param  : Sync parameters (see the BASS_SYNC_xxx type description)
proc   : User defined callback function (window message if using BASS_SYNC_MESSAGE flag)
user   : The 'user' value passed to the callback function
RETURN : Sync handle (NULL=error) */

BOOL BASSDEF(BASS_ChannelRemoveSync)(DWORD handle, HSYNC sync);
/* Remove a sync from a channel
handle : Channel handle
sync   : Handle of sync to remove */

HDSP BASSDEF(BASS_ChannelSetDSP)(DWORD handle, DSPPROC *proc, DWORD user);
/* Setup a user DSP function on a channel. When multiple DSP functions
are used on a channel, they are called in the order that they were added.
handle : Channel handle (HMUSIC/HSTREAM)
proc   : User defined callback function
user   : The 'user' value passed to the callback function
RETURN : DSP handle (NULL=error) */

BOOL BASSDEF(BASS_ChannelRemoveDSP)(DWORD handle, HDSP dsp);
/* Remove a DSP function from a channel
handle : Channel handle (HMUSIC/HSTREAM)
dsp    : Handle of DSP to remove */

HFX BASSDEF(BASS_ChannelSetFX)(DWORD handle, DWORD type);
/* Setup a DX8 effect on a channel. Use BASS_FXSetParameters
to set the effect parameters. Obviously requires DX8.
handle : Channel handle (HMUSIC/HSTREAM)
type   : Type of effect to setup (BASS_FX_xxx)
RETURN : FX handle (NULL=error) */

BOOL BASSDEF(BASS_ChannelRemoveFX)(DWORD handle, HFX fx);
/* Remove a DX8 effect from a channel.
handle : Channel handle (HMUSIC/HSTREAM)
fx     : Handle of FX to remove */

BOOL BASSDEF(BASS_ChannelSetEAXMix)(DWORD handle, float mix);
/* Set the wet(reverb)/dry(no reverb) mix ratio on the channel. By default
the distance of the sound from the listener is used to calculate the mix.
NOTE: The channel must have 3D functionality enabled for the EAX environment
to have any affect on it.
handle : Channel handle (HCHANNEL/HSTREAM/HMUSIC)
mix    : The ratio (0.0=reverb off, 1.0=max reverb, -1.0=let EAX calculate
         the reverb mix based on the distance) */

BOOL BASSDEF(BASS_ChannelGetEAXMix)(DWORD handle, float *mix);
/* Get the wet(reverb)/dry(no reverb) mix ratio on the channel.
handle : Channel handle (HCHANNEL/HSTREAM/HMUSIC)
mix    : Pointer to store the ratio at */

BOOL BASSDEF(BASS_ChannelSetLink)(DWORD handle, DWORD chan);
/* Set a link between 2 channels. When the 1st is played/stopped/paused/resumed
the 2nd channel is also played/stopped/paused/resumed.
handle : Handle of channel to link "chan" to (HMUSIC/HSTREAM)
chan   : Handle of channel to link to "handle" (HMUSIC/HSTREAM) */

BOOL BASSDEF(BASS_ChannelRemoveLink)(DWORD handle, DWORD chan);
/* Remove a link from a channel.
handle : Handle of channel to unlink with "chan" (HMUSIC/HSTREAM)
chan   : Handle of channel to unlink from "handle" (HMUSIC/HSTREAM) */


BOOL BASSDEF(BASS_FXSetParameters)(HFX handle, void *par);
/* Set the parameters of a DX8 effect.
handle : FX handle
par    : Pointer to the parameter structure */

BOOL BASSDEF(BASS_FXGetParameters)(HFX handle, void *par);
/* Retrieve the parameters of a DX8 effect.
handle : FX handle
par    : Pointer to the parameter structure */

#ifdef __cplusplus
}
#endif

#endif
