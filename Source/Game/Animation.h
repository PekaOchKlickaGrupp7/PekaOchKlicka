#pragma once
#include "../DX2DEngine/tga2d/math/vector2.h"
namespace DX2D
{
	class CSprite;
}
class Synchronizer;

class Animation
{
public:
	Animation(const char* aSpriteFilePath, DX2D::Vector2f aPivot, float aFrameDuration, int aNumberOfFrames, int aFramesPerRow);
	~Animation();
	void Update(float aDelta);
	void Render(Synchronizer& aSynchronizer, DX2D::Vector2f aPos);
	void Pause();
	void Resume();
	void Reset();
	void Destroy();
	void SetSize(float aScale);
	float GetScale();
	DX2D::Vector2f GetSize();
private:
	void UpdateTextureRect();
	float myFrameDuration,
		myCurentFrameDuration;
	DX2D::CSprite *mySprite;
	int myNumberOfFrames,
		myFrame,
		myFramesPerRow,
		myCurentRow,
		myRowFrameCounter;
	bool myPaused;

};

inline void Animation::Pause()
{
	myPaused = true;
}
inline void Animation::Resume()
{
	myPaused = false;
}
inline void Animation::Reset()
{
	myPaused = false;
	myFrame = 0;
	myCurentFrameDuration = 0;
}
