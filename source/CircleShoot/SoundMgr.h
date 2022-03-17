#ifndef __SOUNDMGR_H__
#define __SOUNDMGR_H__
#include "CircleCommon.h"

#include <map>

namespace Sexy
{
	
class SoundInstance;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
enum LoopType
{
	LoopType_RollIn,
	LoopType_RollOut,
	LoopType_Max
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
struct LoopingSound
{
public:
	SoundInstance *mSound;
	float mVolume;
	bool mPlaying;

	LoopingSound();
	virtual ~LoopingSound();
	
	void Update();
	void Play();
	void Stop();
	void Mute(bool mute);
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class SoundMgr
{
protected:
	int mUpdateCnt;

	struct SoundDesc
	{
		int mSoundId;
		int mPan;
		float mPitchShift;
	};

	typedef std::multimap<int,SoundDesc> SoundMap;
	SoundMap mSoundMap;

	LoopingSound mLoopingSounds[LoopType_Max];
	void PlaySample(const SoundDesc &theDesc);


public:
	SoundMgr();
	virtual ~SoundMgr();

	void Update();
	void Clear();
	void PlayLoop(LoopType theSound);
	void StopLoop(LoopType theSound);
	void StopAllLoops();
	void Mute(bool mute);
	void KillAllSounds();

	void AddSound(int theSound, int theDelay = 0, int thePan = 0, float thePitchShift = 0);
};


}
#endif