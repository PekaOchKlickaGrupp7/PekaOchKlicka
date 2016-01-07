#pragma once
//#include "fmod.hpp"
//#include "fmod_errors.h"
#include "C:\Program Files (x86)\FMOD SoundSystem\FMOD Studio API Windows\api\lowlevel\inc\fmod.hpp"
#include "C:\Program Files (x86)\FMOD SoundSystem\FMOD Studio API Windows\api\lowlevel\inc\fmod_errors.h"

typedef FMOD::Sound* SoundClass;

class SoundManager
{
public:
	SoundManager();
	~SoundManager();

	void createSound(SoundClass *pSound, const char* pFile);
	void playSound(SoundClass pSound, bool bLoop = false);
	void releaseSound(SoundClass pSound);
private:
	

	FMOD::System *m_pSystem;
};

