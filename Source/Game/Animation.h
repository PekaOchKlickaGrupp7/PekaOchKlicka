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
	Animation(const char* aSpriteFilePath, DX2D::Vector2f aPivot, float aFrameDuration, int aNumberOfFrames, int aFramesPerRow, bool aLooping = true);
	~Animation();
	void Update(float aDelta);
	void Render(Synchronizer& aSynchronizer, DX2D::Vector2f aPos);
	void Pause();
	void Resume();
	void Reset();
	void Destroy();
	void SetColor(DX2D::CColor& aColor);
	void SetSize(float aScale);
	void SetFlip(bool aFlip);
	bool GetIsLooping() const;
	bool GetIsPlaying() const;
	float GetScale();
	DX2D::Vector2f GetSize();
private:
	void UpdateTextureRect();
	float myFrameDuration,
		myCurentFrameDuration;

	DX2D::CColor myColor;

	bool myFlip;
	DX2D::CSprite *mySprite;
	int myNumberOfFrames,
		myFrame,
		myFramesPerRow,
		myCurentRow,
		myRowFrameCounter;
	bool myPaused;
	bool myIsLooping;
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
	myRowFrameCounter = 0;
	myCurentRow = 0;
	UpdateTextureRect();
}
