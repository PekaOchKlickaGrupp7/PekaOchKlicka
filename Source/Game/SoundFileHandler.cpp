#include "stdafx.h"
#include "SoundFileHandler.h"

SoundFileHandler* SoundFileHandler::mySoundFileHandler = nullptr;

SoundFileHandler::SoundFileHandler()
{
}

void SoundFileHandler::Load(std::string &aSoundPath, std::string &anIdentifier, bool aIs3D)
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
	else
	{
		// did find
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