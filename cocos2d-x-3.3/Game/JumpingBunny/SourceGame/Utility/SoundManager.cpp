#include "SoundManager.h"
#include "FileManager.h"
#include "Init/DisplayView.h"
SoundManager * SoundManager::p_Instance = NULL;

SoundManager::SoundManager()
{
	p_IsInitialized = false;
	p_currentIdsoundfx = 0;
	p_IsEnableSound = true;
	//
	p_currentMusicPath = "";
}
SoundManager::~SoundManager()
{

}
void SoundManager::InitSoundEngine()
{
	p_IsInitialized = true;

	p_IsEnableSound = (SaveMgr->isSoundEnable() == "1") ? true : false;

	if (p_IsEnableSound)
		ResetVolume();
	else
		MuteVolume();

}
//
//for the system interrupt
//
void SoundManager::ResumeSoundEngine()
{
	if (!p_IsInitialized)
		return;
	/*if (!p_IsEnableSound)
		return;*/
	GetSimpleSound->resumeAllEffects();
	GetSimpleSound->resumeBackgroundMusic();
}
void SoundManager::PauseSoundEngine()
{
	if (!p_IsInitialized)
		return;
	GetSimpleSound->pauseAllEffects();
	GetSimpleSound->pauseBackgroundMusic();
}
//
void SoundManager::PlaySoundEffect(std::string namePath, bool loop)
{
	if (!p_IsInitialized)
		return;
	/*if (!p_IsEnableSound)
		return;*/
	p_currentIdsoundfx = GetSimpleSound->playEffect(namePath.c_str(), loop);
}
void SoundManager::PlayMusicEffect(std::string namePath)
{
	p_currentMusicPath = namePath;
	if (!p_IsInitialized)
		return;
	/*if (!p_IsEnableSound)
		return;*/
	GetSimpleSound->playBackgroundMusic(namePath.c_str(), true);
}

void SoundManager::PlayCurrentMusic()
{
	if (!p_IsInitialized)
		return;
	/*if (!p_IsEnableSound)
		return;*/
	if (p_currentMusicPath == "")
		return;
	GetSimpleSound->rewindBackgroundMusic();
}
//
void SoundManager::PauseTheCurrentSound()
{
	if (!p_IsInitialized)
		return;
	GetSimpleSound->pauseEffect(p_currentIdsoundfx);
}
void SoundManager::StopTheCurrentSound()
{
	if (!p_IsInitialized)
		return; 
	GetSimpleSound->stopEffect(p_currentIdsoundfx);
}
void SoundManager::ResumeTheCurrentSound()
{
	if (!p_IsInitialized)
		return;
	GetSimpleSound->resumeEffect(p_currentIdsoundfx);
}

void SoundManager::PauseTheCurrentMusic()
{
	if (!p_IsInitialized)
		return;
	GetSimpleSound->pauseBackgroundMusic();
}

void SoundManager::StopTheCurrentMusic()
{
	if (!p_IsInitialized)
		return;
	GetSimpleSound->stopBackgroundMusic(true);
}

void SoundManager::ResumeTheCurrentMusic()
{
	if (!p_IsInitialized)
		return;
	GetSimpleSound->resumeBackgroundMusic();
}

bool SoundManager::IsMusicPlaying()
{
	if (!p_IsInitialized)
		return false;
	/*if (!p_IsEnableSound)
		return false;*/
	return GetSimpleSound->isBackgroundMusicPlaying();
}

void SoundManager::MuteVolume()
{
	if (!p_IsInitialized)
		return ;
	GetSimpleSound->setBackgroundMusicVolume(0.f);
	GetSimpleSound->setEffectsVolume(0.f);
}
void SoundManager::ResetVolume()
{
	if (!p_IsInitialized)
		return;
	GetSimpleSound->setBackgroundMusicVolume(0.7f);
	GetSimpleSound->setEffectsVolume(1.f);
}