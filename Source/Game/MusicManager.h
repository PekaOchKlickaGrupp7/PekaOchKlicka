#pragma once
#include "Sound.h"
#include "..\CommonUtilities\Macros.h"
#include <map>

class MusicManager
{
public:
	static void CreateInstance()
	{
		if (myMusicManager == nullptr)
		{
			myMusicManager = new MusicManager();
		}
	}
	static MusicManager* const GetInstance()
	{
		if (myMusicManager == nullptr)
		{
			myMusicManager = new MusicManager();
		}
		return myMusicManager;
	}
	static void DestroyInstance()
	{
		if (myMusicManager != nullptr)
		{
			myMusicManager->DestroyAll();
			SAFE_DELETE(myMusicManager);
		}
	}

	void Load(std::string &aSongPath, std::string &anIdentifier);
	Sound* GetSong(std::string &anID);

	void DestroySong(std::string &anID);
	void DestroyAll();

	~MusicManager();
private:
	MusicManager();

	static MusicManager* myMusicManager;
};

