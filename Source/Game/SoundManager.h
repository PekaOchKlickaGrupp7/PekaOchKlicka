#pragma once
#include "..\FMOD\api\inc\fmod.hpp"
#include "..\FMOD\api\inc\fmod_errors.h"

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

