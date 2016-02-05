#include "stdafx.h"
#include "MusicManager.h"
#include "SoundFileHandler.h"
#include <iostream>

MusicManager* MusicManager::myMusicManager = nullptr;

MusicManager::MusicManager()
{
	myCurrentThemeName = "";
}

void MusicManager::Load(std::string &aSongPath, std::string &anIdentifier)
{
	SoundFileHandler::GetInstance()->Load(aSongPath, anIdentifier, false, true);
}

void MusicManager::StartSystem()
{
	SoundFileHandler::GetInstance()->GetSong(std::string("BugsAndFish"))->PlaySong();
	SoundFileHandler::GetInstance()->GetSong(std::string("Fossils"))->PlaySong();
	SoundFileHandler::GetInstance()->GetSong(std::string("MainRoom"))->PlaySong();
	SoundFileHandler::GetInstance()->GetSong(std::string("Observatory"))->PlaySong();
	SoundFileHandler::GetInstance()->GetSong(std::string("Paintings"))->PlaySong();
}

bool MusicManager::ChangeTheme(const std::string &aNextTheme)
{
	if (myCurrentThemeName == aNextTheme)
	{
		return true;
	}

	if (myCurrentThemeName != "None")
	{
		SoundFileHandler::GetInstance()->GetSong(std::string(myCurrentThemeName))->Fade(eFade::eDown);
	}
	if (aNextTheme != "None")
	{
		SoundFileHandler::GetInstance()->GetSong(std::string(aNextTheme))->Fade(eFade::eUp);
	}

	myCurrentThemeName = aNextTheme;
	return true; // can return Fade if I change return types of the fade functions. Only necessary if this don't work.
}

void MusicManager::Update(float aDeltaTime)
{
	float vol = 0.0f;
	vol = SoundFileHandler::GetInstance()->GetSong(std::string("Fossils"))->GetVolume();
	std::cout << "Volume of song fossils: " << vol << std::endl;
	
	SoundFileHandler::GetInstance()->UpdateSongs(aDeltaTime);
}


void MusicManager::DestroySong(std::string &anID)
{
	SoundFileHandler::GetInstance()->DestroySong(anID);
}

void MusicManager::DestroyAll()
{
	SoundFileHandler::GetInstance()->DestroyAllMusic();
}

MusicManager::~MusicManager()
{
}
