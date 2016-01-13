#pragma once
#include "fmod.hpp"
#include "..\CommonUtilities\GrowingArray.h"
#include <string>
#include "..\DX2DEngine\tga2d\sprite\sprite.h"

typedef FMOD::Sound* SoundClass;

class SoundManager
{
public:
	~SoundManager();

	static SoundManager* const GetInstance()
	{
		if (mySoundManager == nullptr)
		{
			mySoundManager = new SoundManager();
		}
		return mySoundManager;
	}

	static void DestroyInstance()
	{
		mySoundManager->Destroy();
		delete mySoundManager;
	}

	void Update();
	void Destroy();
	FMOD::Channel* PlaySound(SoundClass aSound, DX2D::Vector2f aPosition = { 0.0f, 0.0f }, bool isLooping = false);
	void ReleaseSound(SoundClass aSound);
	SoundClass CreateSound(const char* aFile);
	SoundClass CreateSound3D(const char* aFile);
	void SetPan(FMOD::Channel *aChannel, float aPanFloat);
	void CreateChannel(std::string &aChannel);
	FMOD::Channel* GetChannel();
	void SetChannelAttributes(FMOD::Channel *aChannel, float aX, float aY, float aZ = 0);
	void SetChannelAttributes(FMOD::Channel *aChannel, DX2D::Vector2f aPosition);
private:
	FMOD::System *mySystem;
	static SoundManager* mySoundManager;
	SoundManager();
};

