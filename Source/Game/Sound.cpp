#include "stdafx.h"
#include "Sound.h"
#include "SoundManager.h"

Sound::Sound()
{
	myPosition = { 0.0f, 0.0f };
}


Sound::~Sound()
{
}

void Sound::Destroy()
{
	myChannel->stop();
	mySFX->release();
}

void Sound::Create(const char* aFile)
{
	mySFX = SoundManager::GetInstance()->CreateSound(aFile);
}
void Sound::Create3D(const char* aFile)
{
	mySFX = SoundManager::GetInstance()->CreateSound3D(aFile);
}

void Sound::Play(DX2D::Vector2f aPosition)
{
	myPosition = aPosition;
	myChannel = SoundManager::GetInstance()->PlaySound(this->mySFX, myPosition, myIsLooping);
}

void Sound::SetPosition(float aX, float aY, float aZ)
{
	myPosition.x = aX;
	myPosition.y = aY;
	SoundManager::GetInstance()->SetChannelAttributes(myChannel, myPosition.x, myPosition.y, aZ);
}

void Sound::SetPosition(DX2D::Vector2f aPosition)
{
	myPosition = aPosition;
	SoundManager::GetInstance()->SetChannelAttributes(myChannel, myPosition);
}

void Sound::SetPan(float aPan)
{
	myChannel->setPan(aPan);
}

void Sound::Pause()
{
	myChannel->setPaused(true);
}

void Sound::Resume()
{
	myChannel->setPaused(false);
}

void Sound::Stop()
{
	myChannel->stop();
}

void Sound::SetVolume(float aVolume)
{
	myChannel->setVolume(aVolume);
}

float Sound::GetVolume()
{
	float tempFloat = 0;
	myChannel->getVolume(&tempFloat);
	return tempFloat;
}