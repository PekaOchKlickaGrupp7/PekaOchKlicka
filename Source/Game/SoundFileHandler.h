#pragma once

#include "Sound.h"
#include "SoundManager.h"

enum class eSound
{
	eRain,
	eJaguar,
	eDoor1,
};

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


	void Load(CommonUtilities::GrowingArray<std::string> &someSoundPaths);
	Sound* GetSound(int anID){ return &mySounds[anID]; };

	void DestroySound(int anID){ mySounds[anID].Destroy(); };
	void DestroyAll() { for (int i = 0; i < mySounds.Size(); ++i) mySounds[i].Destroy(); };

	~SoundFileHandler();
private:
	SoundFileHandler();

	static SoundFileHandler* mySoundFileHandler;

	CommonUtilities::GrowingArray<Sound, int> mySounds;
};

