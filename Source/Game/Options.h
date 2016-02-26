#pragma once
#include "tga2d\sprite\sprite.h"
#include "SFXSlider.h"
#include "MusicSlider.h"
#include "FullscreenButton.h"
#include "MainMenuButton.h"
#include "ResumeButton.h"

class Options
{
public:
	Options();

	void Initialize();
	void Update(float aDeltaTime);
	void Render(Synchronizer& aSynchronizer);

	void SetActive(bool aActive)
	{
		if (myMouseWasInvisible == true && aActive == false)
		{
			myMouseWasInvisible = false;
			MouseManager::GetInstance()->SetInteractiveMode(eInteractive::eRegular);
			MouseManager::GetInstance()->SetHideGameMouse(true);
		}
		else if (aActive == true)
		{
			if (MouseManager::GetInstance()->GetHideGameMouse() == true)
			{
				myMouseWasInvisible = true;
				MouseManager::GetInstance()->SetInteractiveMode(eInteractive::eRegular);
				MouseManager::GetInstance()->SetHideGameMouse(false);
			}
		}
		myIsActive = aActive;
	};
	bool GetActive(){ return myIsActive; };

	~Options();
private:
	DX2D::CSprite* myBackground;

	MusicSlider myMusicSlider;
	SFXSlider mySFXSlider;

	FullscreenButton myFullscreenButton;
	MainMenuButton myMenuButton;
	ResumeButton myResumeButton;

	bool myMouseWasInvisible;
	bool myIsActive;
};

