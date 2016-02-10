#pragma once
#include "tga2d\sprite\sprite.h"
#include "SFXSlider.h"
#include "MusicSlider.h"

class Options
{
public:
	Options();

	void Initialize();
	void Update(float aDeltaTime);
	void Render(Synchronizer& aSynchronizer);

	void SetActive(bool aActive) { myIsActive = aActive; };
	bool GetActive(){ return myIsActive; };

	~Options();
private:
	DX2D::CSprite* myBackground;

	MusicSlider myMusicSlider;
	SFXSlider mySFXSlider;

	bool myIsActive;
};

