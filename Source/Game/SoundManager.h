#pragma once
#include "fmod.hpp"
#include "fmod_errors.h"
#include "..\CommonUtilities\GrowingArray.h"
#include "ChannelObject.h"

typedef FMOD::Sound* SoundClass;

class SoundManager
{
public:
	SoundManager();
	~SoundManager();

	void PlaySound(SoundClass aSound, FMOD::Channel *aChannel, bool isLooping = false);
	void ReleaseSound(SoundClass aSound);
	void CreateSound(SoundClass *aSound, const char* aFile);
	void SetPan(FMOD::Channel *aChannel, float aPanFloat);
	void CreateChannel(std::string &aChannel);
	void Pause(SoundClass aSound);
	void Resume(SoundClass aSound);
	ChannelObject GetChannel(std::string &aName);
	void SetChannelAttributes(FMOD::Channel *aChannel, int aX, int aY, int aZ = 0);
private:
	FMOD::System *mySystem;
	CommonUtilities::GrowingArray<ChannelObject> myChannels;
};

