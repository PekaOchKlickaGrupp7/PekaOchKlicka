#include "stdafx.h"
#include "SoundFileHandler.h"

SoundFileHandler* SoundFileHandler::mySoundFileHandler = nullptr;

SoundFileHandler::SoundFileHandler()
{
}

void SoundFileHandler::Load(CommonUtilities::GrowingArray<std::string> &someSoundPaths)
{
	mySounds.Init(someSoundPaths.Size());

	for (unsigned short i = 0; i < someSoundPaths.Size(); i++)
	{
		Sound aSoundToAdd;
		aSoundToAdd.Create(someSoundPaths[i].c_str());
		mySounds.Add(aSoundToAdd);
	}
}


SoundFileHandler::~SoundFileHandler()
{
}
