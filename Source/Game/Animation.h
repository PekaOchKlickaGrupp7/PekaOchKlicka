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
	Animation();
	~Animation();
	void Init(const char* aSpriteFilePath, float aFrameDuration, int aNumberOfFrames);
	void Update(float aDelta);
	void Render(Synchronizer& aSynchronizer, DX2D::Vector2f aPos);
	void Pause();
	void Resume();
	void Reset();
private:
	void UpdateTextureRect();
	float myFrameDuration,
		myCurentFrameDuration,
		myOriginalSpriteWidht;
	DX2D::CSprite *mySprite;
	int myNumberOfFrames,
		myFrame;
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
