#include "stdafx.h"
#include "SoundFileHandler.h"
#include <iostream>

SoundFileHandler* SoundFileHandler::mySoundFileHandler = nullptr;

SoundFileHandler::SoundFileHandler()
{

}

void SoundFileHandler::Load(const std::string &aSoundPath, const std::string &anIdentifier, bool aIs3D, bool aIsSong)
{
	if (aIsSong == false)
	{
		if (mySounds.find(anIdentifier) == mySounds.end())
		{
			Sound aSoundToAdd;
			if (aIs3D == false)
			{
				aSoundToAdd.Create(aSoundPath.c_str(), anIdentifier);
			}
			else
			{
				aSoundToAdd.Create3D(aSoundPath.c_str(), anIdentifier);
			}
			mySounds[anIdentifier] = aSoundToAdd;
		}
	}
	else
	{
		if (myMusic.find(anIdentifier) == myMusic.end())
		{
			Sound aSoundToAdd;
			aSoundToAdd.Create(aSoundPath.c_str(), anIdentifier);
			myMusic[anIdentifier] = aSoundToAdd;
		}
	}
}

void SoundFileHandler::UpdateSongs(float aDeltaTime)
{
	for (std::map<std::string, Sound>::iterator i = myMusic.begin(); i != myMusic.end(); ++i)
	{
		i->second.Update(aDeltaTime);
	}
}


SoundFileHandler::~SoundFileHandler()
{
}

void SoundFileHandler::DestroyAll() 
{ 
	for (std::map<std::string, Sound>::iterator i = mySounds.begin(); i != mySounds.end(); ++i)
	{
		i->second.Destroy();
	}
	mySounds.clear();
};

void SoundFileHandler::DestroyAllMusic()
{
	for (std::map<std::string, Sound>::iterator i = myMusic.begin(); i != myMusic.end(); ++i)
	{
		i->second.Destroy();
	}
	myMusic.clear();
};