#pragma once

#include "Sound.h"
#include <map>

// To avoid static_cast-ing all the time.
template <typename T>
int eSoundInt(T aElementType)
{
	return static_cast<int>(aElementType);
}

class SoundFileHandler
{
public:
	

	static void CreateInstance()
	{
		if (mySoundFileHandler == nullptr)
		{
			mySoundFileHandler = new SoundFileHandler();
		}
	}

	static SoundFileHandler* const GetInstance()
	{
		if (mySoundFileHandler == nullptr)
		{
			mySoundFileHandler = new SoundFileHandler();
		}
		return mySoundFileHandler;
	}

	static void DestroyInstance()
	{
		if (mySoundFileHandler != nullptr)
		{
			mySoundFileHandler->DestroyAll();
			delete mySoundFileHandler;
			mySoundFileHandler = nullptr;
		}
	}


	void Load(const std::string &aSoundPath, const std::string &anIdentifier, bool aIs3D, bool aIsSong = false);
	Sound* GetSound(const std::string &anID){ return &mySounds[anID]; };
	Sound* GetSong(const std::string &anID){ return &myMusic[anID]; };

	void DestroySound(const std::string &anID){ mySounds[anID].Destroy(); };
	void DestroyAll();

	void DestroySong(const std::string &anID){ myMusic[anID].Destroy(); };
	void DestroyAllMusic();

	void UpdateSongs(float aDeltaTime);

	~SoundFileHandler();
private:
	SoundFileHandler();

	static SoundFileHandler* mySoundFileHandler;

	std::map<std::string, Sound> mySounds;
	std::map<std::string, Sound> myMusic;
};

