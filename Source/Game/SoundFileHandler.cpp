#include "stdafx.h"
#include "SoundFileHandler.h"

SoundFileHandler* SoundFileHandler::mySoundFileHandler = nullptr;

SoundFileHandler::SoundFileHandler()
{
	mySounds.Init(10);
}

void SoundFileHandler::Load(std::string aSoundPath)
{
	Sound aSoundToAdd;
	aSoundToAdd.Create(aSoundPath.c_str());
	mySounds.Add(aSoundToAdd);
}


SoundFileHandler::~SoundFileHandler()
{
}
