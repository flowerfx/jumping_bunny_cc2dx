#ifndef _SOUND_MANAGER_H_
#define _SOUND_MANAGER_H_

#include "LoadDataXML.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
using namespace cocos2d;
using namespace CocosDenshion;

class SoundManager
{
private:
	unsigned int p_currentIdsoundfx;
	bool p_IsInitialized;
	static SoundManager * p_Instance;
	bool p_IsEnableSound;
	std::string p_currentMusicPath;
public:
	static SoundManager * GetInstance()
	{
		if (!p_Instance)
		{
			p_Instance = new SoundManager();
		}
		return p_Instance;
	}


	SoundManager();
	~SoundManager();
	void InitSoundEngine();
	//
	//for the system interrupt
	//
	void ResumeSoundEngine();
	void PauseSoundEngine();
	//
	void PlaySoundEffect(std::string namePath, bool Loop = false);
	void PlayMusicEffect(std::string namePath);
	//
	void PlayCurrentMusic();
	bool IsMusicPlaying();
	//
	void PauseTheCurrentSound();
	void StopTheCurrentSound();
	void ResumeTheCurrentSound();

	void PauseTheCurrentMusic();
	void StopTheCurrentMusic();
	void ResumeTheCurrentMusic();
	//
	bool IsEnableSound() { return p_IsEnableSound; }
	void SetEnableSound(bool b) { p_IsEnableSound = b; }
	//
	void MuteVolume();
	void ResetVolume();
};

#define GetSimpleSound CocosDenshion::SimpleAudioEngine::sharedEngine() 
#define GetSound SoundManager::GetInstance()
#endif//_SOUND_MANAGER_H_