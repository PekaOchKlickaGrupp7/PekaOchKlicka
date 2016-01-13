#pragma once
#include "fmod.hpp"
#include "fmod_errors.h"
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
	void SetPosition(int aX, int aY, int aZ = 0);
	void SetPosition(DX2D::Vector2f aPosition);
	DX2D::Vector2f GetPosition() { return myPosition; };
	void SetPan(float aPan);
	~Sound();
private:
	FMOD::Channel* myChannel;
	bool myIsLooping = false;
	FMOD::Sound* mySFX;
	DX2D::Vector2f myPosition;
};

