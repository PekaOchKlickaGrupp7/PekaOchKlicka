#include "stdafx.h"
#include "Sound.h"
#include "SoundManager.h"

Sound::Sound()
{
	myPosition = { 0.0f, 0.0f };
	mySFX = nullptr;
	myIdentifier = "";

	myIsFadingDown = false;
	myIsFadingUp = false;

	myChannel = nullptr;
}


Sound::~Sound()
{
}

void Sound::Destroy()
{
	//myChannel->stop(); // Apparently releasing a sound will release the channel.
	mySFX->release();
}

void Sound::Create(const char* aFile, const std::string &anIdentifier)
{
	myIdentifier = anIdentifier;
	mySFX = SoundManager::GetInstance()->CreateSound(aFile);
}
void Sound::Create3D(const char* aFile, const std::string &anIdentifier)
{
	myIdentifier = anIdentifier;
	mySFX = SoundManager::GetInstance()->CreateSound3D(aFile);
}

void Sound::Stream(const char* aFile, const std::string &anIdentifier)
{
	myIdentifier = anIdentifier;
	mySFX = SoundManager::GetInstance()->CreateStream(aFile);
}
void Sound::Stream3D(const char* aFile, const std::string &anIdentifier)
{
	myIdentifier = anIdentifier;
	mySFX = SoundManager::GetInstance()->CreateStream3D(aFile);
}

void Sound::PlaySound(DX2D::Vector2f aPosition)
{
	bool aBool = false;
	if (myChannel != nullptr)
	{
		myChannel->isPlaying(&aBool);
	}
	if (aBool == false)
	{
		myPosition = aPosition;
		myChannel = SoundManager::GetInstance()->PlaySound(this->mySFX, myPosition, myIsLooping);
	}
	myChannel->setChannelGroup(SoundManager::GetInstance()->GetChannelGroup("SFX"));
}

void Sound::PlaySong()
{
	bool aBool = false;
	if (myChannel != nullptr)
	{
		myChannel->isPlaying(&aBool);
	}
	if (aBool == false)
	{
		myChannel = SoundManager::GetInstance()->PlaySound(this->mySFX, myPosition, true); // Now always loops due to the music fade system-solution.
		myChannel->setVolume(0.0f);
	}
	myChannel->setChannelGroup(SoundManager::GetInstance()->GetChannelGroup("Music"));
}

void Sound::Fade(eFade aFadeUpOrDown)
{
	if (eFadeInt(aFadeUpOrDown) == 0)
	{
		myIsFadingDown = false;
		myIsFadingUp = true;
	}
	else if (eFadeInt(aFadeUpOrDown) == 1)
	{
		myIsFadingDown = true;
		myIsFadingUp = false;
	}
}

void Sound::FadeDown(float aDeltaTime)
{
	float volume;
	myChannel->getVolume(&volume);

	if (volume > 0.0f)
	{
		volume -= 0.001f * aDeltaTime;
		myChannel->setVolume(volume);
		myChannel->getVolume(&volume);
		if (volume < 0)
		{
			myIsFadingDown = false;
			myChannel->setVolume(0.0f);
		}
	}
}
void Sound::FadeUp(float aDeltaTime)
{
	float volume;
	myChannel->getVolume(&volume);

	if (volume < 1.1f)
	{
		volume += 0.001f * aDeltaTime;
		myChannel->setVolume(volume);
		myChannel->getVolume(&volume);
		if (volume > 1)
		{
			myIsFadingUp = false;
			myChannel->setVolume(1.0f);
		}
	}
}

bool Sound::IsPlaying()
{
	bool tempBool = false;
	myChannel->isPlaying(&tempBool);
	return tempBool;
}

void Sound::Update(float aDeltaTime)
{
	if (myIsFadingDown == true)
	{
		FadeDown(aDeltaTime);
	}
	if (myIsFadingUp == true)
	{
		FadeUp(aDeltaTime);
	}
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