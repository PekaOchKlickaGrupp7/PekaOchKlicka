#include "stdafx.h"
#include "MusicManager.h"
#include "SoundFileHandler.h"

MusicManager* MusicManager::myMusicManager = nullptr;

MusicManager::MusicManager()
{
}

void MusicManager::Load(std::string &aSongPath, std::string &anIdentifier)
{
	SoundFileHandler::GetInstance()->Load(aSongPath, anIdentifier, false);
}

Sound* MusicManager::GetSong(std::string &anID)
{
	return SoundFileHandler::GetInstance()->GetSong(anID);
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
