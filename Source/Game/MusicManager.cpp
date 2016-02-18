#include "stdafx.h"
#include "MusicManager.h"
#include "SoundFileHandler.h"
#include <iostream>

MusicManager* MusicManager::myMusicManager = nullptr;

MusicManager::MusicManager()
{
	myCurrentThemeName = "";
}

void MusicManager::Load(const std::string &aSongPath, const std::string &anIdentifier)
{
	SoundFileHandler::GetInstance()->SetupStream(aSongPath, anIdentifier, false, true);
}

void MusicManager::StartSystem()
{
	SoundFileHandler::GetInstance()->GetSong("BugsAndFish")->PlaySong();
	SoundFileHandler::GetInstance()->GetSong("Fossils")->PlaySong();
	SoundFileHandler::GetInstance()->GetSong("MainRoom")->PlaySong();
	SoundFileHandler::GetInstance()->GetSong("Observatory")->PlaySong();
	SoundFileHandler::GetInstance()->GetSong("Paintings")->PlaySong();
	SoundFileHandler::GetInstance()->GetSong("MainMenu")->PlaySong();
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
	SoundFileHandler::GetInstance()->UpdateSongs(aDeltaTime);
}


void MusicManager::DestroySong(const std::string &anID)
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
