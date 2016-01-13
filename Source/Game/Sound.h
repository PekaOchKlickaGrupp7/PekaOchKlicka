#pragma once
#include "fmod.hpp"
#include "..\DX2DEngine\tga2d\sprite\sprite.h"
class Sound
{
public:
	Sound();
	void Create(const char* aFile);
	void Create3D(const char* aFile);
	void SetLooping(bool aBool) { myIsLooping = aBool; };
	void Play(DX2D::Vector2f aPosition = { 0.0f, 0.0f });
	void Pause();
	void Resume();
	void Stop();
	void SetPosition(float aX, float aY, float aZ = 0);
	void SetPosition(DX2D::Vector2f aPosition);
	DX2D::Vector2f GetPosition() { return myPosition; };
	void SetPan(float aPan);
	void SetVolume(float aVolume);
	float GetVolume();
	~Sound();
private:
	FMOD::Channel* myChannel;
	bool myIsLooping = false;
	FMOD::Sound* mySFX;
	DX2D::Vector2f myPosition;
};

