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
	SoundFileHandler::GetInstance()->GetSong("Ballroom")			->PlaySong();
	SoundFileHandler::GetInstance()->GetSong("EntranceToBallRoom")	->PlaySong();
	SoundFileHandler::GetInstance()->GetSong("Hub")					->PlaySong();
	SoundFileHandler::GetInstance()->GetSong("Kitchen")				->PlaySong();
	SoundFileHandler::GetInstance()->GetSong("Foodcellar")			->PlaySong();
	SoundFileHandler::GetInstance()->GetSong("Sittingroom")			->PlaySong();
	SoundFileHandler::GetInstance()->GetSong("Bathroom")			->PlaySong();
	SoundFileHandler::GetInstance()->GetSong("Bedroom")				->PlaySong();
	SoundFileHandler::GetInstance()->GetSong("ServantsQuarters")	->PlaySong();
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
