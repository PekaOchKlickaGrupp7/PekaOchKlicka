
#include "stdafx.h"
#include "SoundManager.h"
#include "..\CommonUtilities\DL_Debug.h"
#include <iostream>

SoundManager* SoundManager::mySoundManager = nullptr;

const float DISTANCEFACTOR = 1.0f;          // Units per meter.  I.e feet would = 3.28.  centimeters would = 100.

SoundManager::SoundManager()
{
	myListenerPosition = { 5.0f, 5.0f, -1.0f * DISTANCEFACTOR };

	if (FMOD::System_Create(&mySystem) != FMOD_OK)
	{
		DL_PRINT("Could not create a sound device.");
		return;
	}

	int driverCount = 0;
	mySystem->getNumDrivers(&driverCount);

	if (driverCount == 0)
	{
		DL_PRINT("No sound drivers detected.");
		return;
	}

	mySystem->init(36, FMOD_INIT_NORMAL, nullptr);
	DL_PRINT("Created a sound device");

	mySystem->set3DSettings(1.0, DISTANCEFACTOR, 1.0f);
}

SoundClass SoundManager::CreateSound3D(const char* aFile)
{
	SoundClass tempSoundClass = nullptr;
	mySystem->createSound(aFile, FMOD_3D, 0, &tempSoundClass);
	tempSoundClass->set3DMinMaxDistance(0.5f * DISTANCEFACTOR, 5000.0f * DISTANCEFACTOR);
	return tempSoundClass;
}

SoundClass SoundManager::CreateSound(const char* aFile)
{
	// for stream use FMOD_CREATESTREAM
	// FMOD_CREATESAMPLE compresses and streams. FMOD_DEFAULT is higher qual-ish
	SoundClass tempSoundClass = nullptr;
	FMOD_RESULT aRes;
	aRes = mySystem->createSound(aFile, FMOD_2D, 0, &tempSoundClass);
	if (aRes == 18)
	{
		std::string outputstring = "Sound file could not be found. File name was: '" + std::string(aFile) + "'";
		DL_DEBUG(outputstring.c_str());
		std::cout << "Sound file could not be found! File name was: " + std::string(aFile) + " Check empty playsound events" << std::endl;
	}
	return tempSoundClass;
}

void SoundManager::SetPan(FMOD::Channel *aChannel, float aPanFloat)
{
	aChannel->setPan(aPanFloat);
}

void SoundManager::SetChannelAttributes(FMOD::Channel *aChannel, float aX, float aY, float aZ)
{
	FMOD_VECTOR positionVector;

	positionVector.x = aX;
	positionVector.y = aY;
	positionVector.z = aZ;

	aChannel->set3DAttributes(&positionVector, 0, nullptr);
}

void SoundManager::SetChannelAttributes(FMOD::Channel *aChannel, DX2D::Vector2f aPosition)
{
	FMOD_VECTOR positionVector;

	positionVector.x = aPosition.x;
	positionVector.y = aPosition.y;
	positionVector.z = 0;

	aChannel->set3DAttributes(&positionVector, 0, nullptr);
}

void SoundManager::Update(float aUpdateTimer)
{
	static float t = 0;
	static FMOD_VECTOR lastpos = { 0.0f, 0.0f, 0.0f };
	FMOD_VECTOR forward = { 0.0f, 0.0f, 1.0f };
	FMOD_VECTOR up = { 0.0f, 1.0f, 0.0f };
	FMOD_VECTOR vel;

	vel.x = (myListenerPosition.x - lastpos.x) * (1000 / aUpdateTimer); //
	vel.y = (myListenerPosition.y - lastpos.y) * (1000 / aUpdateTimer); //
	vel.z = (myListenerPosition.z - lastpos.z) * (1000 / aUpdateTimer); //

	//myListenerPosition.x = (float)sin(t * 0.05f) * 24.0f * DISTANCEFACTOR;

	lastpos = myListenerPosition;

	mySystem->set3DListenerAttributes(0, &myListenerPosition, &vel, &forward, &up);
	mySystem->update();
}

FMOD::Channel* SoundManager::PlaySound(SoundClass aSound, DX2D::Vector2f aPosition, bool isLooping)
{
	// If 2D, all 3D commands will be ignored by FMOD. No error check required.
	FMOD_VECTOR pos = { 0.0f, 0.0f, 0.0f };
	FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };
	pos.x = aPosition.x * DISTANCEFACTOR;
	pos.y = aPosition.y * DISTANCEFACTOR;
	pos.z = 0;

	if (!isLooping)
	{
		aSound->setMode(FMOD_LOOP_OFF);
	}
	else
	{
		aSound->setMode(FMOD_LOOP_NORMAL);
		aSound->setLoopCount(-1);
	}
	FMOD::Channel* tempChannel = nullptr;

	mySystem->playSound(aSound, nullptr, false, &tempChannel);

	tempChannel->set3DAttributes(&pos, &vel);

	return tempChannel;
}

void SoundManager::ReleaseSound(SoundClass aSound)
{
	aSound->release();
}

void SoundManager::Destroy()
{
	mySystem->close();
	mySystem->release();
}

SoundManager::~SoundManager()
{
	//Destroy();
}
