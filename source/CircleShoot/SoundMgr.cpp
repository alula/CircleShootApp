#include "Zuma_Prefix.pch"

#include <SexyAppFramework/SoundManager.h>
#include <SexyAppFramework/SoundInstance.h>
#include <SexyAppFramework/SexyAppBase.h>

#include "SoundMgr.h"
#include "Res.h"

using namespace Sexy;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

SoundMgr::SoundMgr()
{
    for (int i = 0; i < LoopType_Max; i++)
    {
        mLoopingSounds[i] = LoopingSound();
    }

    mUpdateCnt = 0;
    mLoopingSounds[LoopType_RollIn].mSound = gSexyAppBase->mSoundManager->GetSoundInstance(Sexy::SOUND_ROLLING);
    mLoopingSounds[LoopType_RollOut].mSound = gSexyAppBase->mSoundManager->GetSoundInstance(Sexy::SOUND_ROLLING);
}

SoundMgr::~SoundMgr()
{
}

void SoundMgr::PlaySample(const SoundDesc &theDesc)
{
    SoundInstance *aSound = gSexyAppBase->mSoundManager->GetSoundInstance(theDesc.mSoundId);

    if (!aSound)
        return;

    if (theDesc.mPitchShift != 0.0f)
    {
        aSound->AdjustPitch(theDesc.mPitchShift);
    }

    if (theDesc.mPan != 0)
    {
        aSound->SetPan(theDesc.mPan);
    }

    aSound->Play(false, true);
}

void SoundMgr::Clear() {}

void SoundMgr::Mute(bool mute)
{
    for (int i = 0; i < LoopType_Max; i++)
    {
        mLoopingSounds[i].Mute(mute);
    }
}

void SoundMgr::PlayLoop(LoopType theSound)
{
    mLoopingSounds[(int)theSound].Play();
}

void SoundMgr::StopLoop(LoopType theSound)
{
    // wtf
    if (mLoopingSounds[(int)theSound].mVolume > 0.99f)
    {
        mLoopingSounds[(int)theSound].mVolume = 0.98f;
    }
}

void SoundMgr::StopAllLoops() {}

void SoundMgr::KillAllSounds()
{
    for (int i = 0; i < LoopType_Max; i++)
    {
        StopLoop((LoopType)i);
    }

    mSoundMap.clear();
}

void SoundMgr::AddSound(int theSound, int theDelay, int thePan, float thePitchShift)
{
    SoundDesc aDesc;
    aDesc.mSoundId = theSound;
    aDesc.mPitchShift = thePitchShift;
    aDesc.mPan = thePan;

    if (theDelay == 0)
    {
        PlaySample(aDesc);
    }
    else
    { 
        mSoundMap.insert(std::pair<int, SoundDesc>(mUpdateCnt + theDelay, aDesc));
    }
}

void SoundMgr::Update()
{
    mUpdateCnt++;
    SoundMap::iterator anItr = mSoundMap.begin();

    while (anItr != mSoundMap.end())
    {
        if (anItr->first <= mUpdateCnt)
        {
            PlaySample(anItr->second);
            mSoundMap.erase(anItr++);
        }
        else
        {
            anItr++;
        }
    }

    for (int i = 0; i < LoopType_Max; i++)
    {
        mLoopingSounds[i].Update();
    }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

LoopingSound::LoopingSound() {
    mSound = NULL;
    mVolume = 0.0f;
}

LoopingSound::~LoopingSound()
{
    if (mSound != NULL)
        mSound->Release();
}

void LoopingSound::Update()
{
    if (mSound == NULL)
        return;

    if (!mPlaying)
        return;

    if (mVolume < 1.0f)
    {
        mVolume -= 0.02f;

        if (mVolume > 0.0f)
        {
            mSound->SetVolume(mVolume);
        }
        else
        {
            mPlaying = false;
            mSound->Stop();
        }
    }
}

void LoopingSound::Play()
{
    if (mSound == NULL)
        return;

    mPlaying = true;
    mVolume = 1.0f;
    mSound->SetVolume(1.0);
    mSound->Play(true, false);
}

void LoopingSound::Stop()
{
}

void LoopingSound::Mute(bool mute)
{
    if (mSound == NULL)
        return;

    if (!mPlaying)
        return;

    if (mute)
    {
        mSound->SetVolume(0.0);
    }
    else
    {
        mSound->SetVolume(mVolume);
    }
}