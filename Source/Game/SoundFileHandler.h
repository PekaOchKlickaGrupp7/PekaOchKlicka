#pragma once

#include "Sound.h"
#include "SoundManager.h"
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


	void Load(std::string &aSoundPath, std::string &anIdentifier);
	Sound* GetSound(std::string &anID){ return &mySounds[anID]; };

	void DestroySound(std::string &anID){ mySounds[anID].Destroy(); };
	void DestroyAll();

	~SoundFileHandler();
private:
	SoundFileHandler();

	static SoundFileHandler* mySoundFileHandler;

	std::map<std::string, Sound> mySounds;
};

