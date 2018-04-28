#ifndef __SWALIB_SOUND_MANAGER_H__
#define __SWALIB_SOUND_MANAGER_H__


#include "soundmanager.h"


class SwalibSoundManager : public ISoundManager
{

public:

	SwalibSoundManager();
	~SwalibSoundManager();

	virtual bool         InitSound ();
	virtual void         EndSound  ();
	virtual unsigned int LoadWav   (const char filename[]);
	virtual void         PlaySound (unsigned int snd, float volume, float pitch);
	virtual void         UnloadWav (unsigned int snd);
	virtual void         PlayMusic (unsigned int snd, float volume = 1.f);
	virtual void         StopMusic ();

};

#endif