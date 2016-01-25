#include "stdafx.h"
#include "SoundFileHandler.h"

SoundFileHandler* SoundFileHandler::mySoundFileHandler = nullptr;

SoundFileHandler::SoundFileHandler()
{
}

void SoundFileHandler::Load(std::string &aSoundPath, std::string &anIdentifier)
{
	if (mySounds.find(anIdentifier) == mySounds.end())
	{
		Sound aSoundToAdd;
		aSoundToAdd.Create(aSoundPath.c_str(), anIdentifier);
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