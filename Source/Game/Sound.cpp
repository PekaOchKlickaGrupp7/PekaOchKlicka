#include "stdafx.h"
#include "Sound.h"
#include "SoundManager.h"

Sound::Sound()
{
	myPosition = { 0.0f, 0.0f };
}


Sound::~Sound()
{
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

void Sound::SetPosition(int aX, int aY, int aZ)
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