#include "stdafx.h"
#include "swalibsoundmanager.h"
#include "core.h"


SwalibSoundManager::SwalibSoundManager() {}

// *************************************************
//
// *************************************************

SwalibSoundManager::~SwalibSoundManager()
{
}

// *************************************************
//
// *************************************************

bool SwalibSoundManager::InitSound()
{
	return CORE_InitSound();
}

// *************************************************
//
// *************************************************

void SwalibSoundManager::EndSound()
{
	CORE_EndSound();
}

// *************************************************
//
// *************************************************

unsigned int SwalibSoundManager::LoadWav(const char filename[])
{
	return CORE_LoadWav(filename);
}

// *************************************************
//
// *************************************************

void SwalibSoundManager::PlaySound(unsigned int snd, float volume, float pitch)
{
	CORE_PlaySound(snd, volume, pitch);
}

// *************************************************
//
// *************************************************

void SwalibSoundManager::UnloadWav(unsigned int snd)
{
	CORE_UnloadWav(snd);
}

// *************************************************
//
// *************************************************

void SwalibSoundManager::PlayMusic(unsigned int snd, float volume)
{
	CORE_PlayMusic(snd, volume);
}

// *************************************************
//
// *************************************************

void SwalibSoundManager::StopMusic()
{
	CORE_StopMusic();
}

// *************************************************
//
// *************************************************

void SwalibSoundManager::SetVolume(float volume)
{
	CORE_SetVolume(volume / 100.f);
}