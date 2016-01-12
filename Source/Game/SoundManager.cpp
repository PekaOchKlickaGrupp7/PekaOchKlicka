#include "stdafx.h"
#include "SoundManager.h"

SoundManager::SoundManager()
{
	myChannels.Init(1);

	if (FMOD::System_Create(&mySystem) != FMOD_OK)
	{
		// report error
		return;
	}

	int driverCount = 0;
	mySystem->getNumDrivers(&driverCount);

	if (driverCount == 0)
	{
		// report error
		return;
	}

	mySystem->init(36, FMOD_INIT_NORMAL, nullptr);
}

void SoundManager::CreateSound(SoundClass *aSound, const char* aFile)
{
	// for stream use FMOD_CREATESTREAM
	// FMOD_CREATESAMPLE compresses and streams. FMOD_DEFAULT is higher qual-ish
	mySystem->createSound(aFile, FMOD_CREATESAMPLE /*| FMOD_2D*/, 0, aSound);
}

void SoundManager::SetPan(FMOD::Channel *aChannel, float aPanFloat)
{
	aChannel->setPan(aPanFloat);
}

void SoundManager::CreateChannel(std::string &aName) // choose group to attach to instead
{
	//mySystem->setOutput(FMOD_OUTPUTTYPE_WAVWRITER);
	//mySystem->setSoftwareFormat(48000, FMOD_SPEAKERMODE_STEREO, 2);

	FMOD::Channel *aChannel = nullptr;
	aChannel->setPaused(true); // start paused

	ChannelObject aChannelObject(aName, aChannel);
	myChannels.Add(aChannelObject);
}

ChannelObject SoundManager::GetChannel(std::string &aName)
{
	for (size_t i = 0; i < myChannels.Size(); i++)
	{
		if (myChannels[i].GetName() == aName)
		{
			return myChannels[i];
		}
	}
	// If nothing found, do this.
	ChannelObject nullObject;
	return nullObject;
}

void SoundManager::SetChannelAttributes(FMOD::Channel *aChannel, int aX, int aY, int aZ)
{
	FMOD_VECTOR positionVector;
	FMOD_VECTOR velocityVector; // default 0
	FMOD_VECTOR alt_pan_posVector;

	positionVector.x = aX;
	positionVector.y = aY;
	positionVector.z = aZ;

	aChannel->set3DAttributes(&positionVector, &velocityVector, &alt_pan_posVector);
}

void SoundManager::Pause(SoundClass aSound)
{

}
void SoundManager::Resume(SoundClass aSound)
{

}

void SoundManager::PlaySound(SoundClass aSound, FMOD::Channel *aChannel, bool isLooping)
{
	if (!isLooping)
	{
		aSound->setMode(FMOD_LOOP_OFF);
	}
	else
	{
		aSound->setMode(FMOD_LOOP_NORMAL);
		aSound->setLoopCount(-1);
	}

	mySystem->playSound(aSound, nullptr, false, &aChannel);
}

void SoundManager::ReleaseSound(SoundClass aSound)
{
	aSound->release();
}


SoundManager::~SoundManager()
{
}
