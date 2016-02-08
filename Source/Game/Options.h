#pragma once
#include "tga2d\sprite\sprite.h"
class Options
{
public:
	Options();

	void Initialize();
	void Update(float aDeltaTime);
	void Render(float aDeltaTime);

	~Options();
private:
	DX2D::CSprite* myBackground;

	DX2D::CSprite* myVolumeSFXSlider;
	DX2D::CSprite* myVolumeSFXKnob;

	DX2D::CSprite* myVolumeMusicSlider;
	DX2D::CSprite* myVolumeMusicKnob;

	float myVolumeWidth
};

