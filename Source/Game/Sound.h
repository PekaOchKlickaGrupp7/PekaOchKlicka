#pragma once
#include "fmod.hpp"
#include "..\DX2DEngine\tga2d\sprite\sprite.h"

enum class eFade
{
	eUp,
	eDown
};

// To avoid static_cast-ing all the time.
template <typename T>
int eFadeInt(T aElementType)
{
	return static_cast<int>(aElementType);
}

class Sound
{
public:
	Sound();
	void Create(const char* aFile, std::string &anIdentifier);
	void Create3D(const char* aFile, std::string &anIdentifier);
	void SetLooping(bool aBool) { myIsLooping = aBool; };
	void PlaySound(DX2D::Vector2f aPosition = { 0.0f, 0.0f });
	void PlaySong();
	void Pause();
	void Resume();
	void Stop();
	void Fade(eFade aFadeUpOrDown);
	void SetPosition(float aX, float aY, float aZ = 0);
	void SetPosition(DX2D::Vector2f aPosition);
	DX2D::Vector2f GetPosition() { return myPosition; };
	void SetPan(float aPan);
	void SetVolume(float aVolume);
	float GetVolume();

	void Update(float aDeltaTime);

	~Sound();
	void Destroy();
private:
	FMOD::Channel* myChannel;
	bool myIsLooping = false;
	FMOD::Sound* mySFX;
	DX2D::Vector2f myPosition;
	std::string myIdentifier;
	void FadeDown(float aDeltaTime);
	void FadeUp(float aDeltaTime);

	bool myIsFadingUp;
	bool myIsFadingDown;
};

