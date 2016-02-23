#include "stdafx.h"
#include "Options.h"
#include "..\CommonUtilities\Macros.h"
#include "SoundFileHandler.h"


Options::Options()
{
	SoundFileHandler::GetInstance()->SetupStream("Sound/SoundFX/GUI/Hover.ogg", "ButtonHover", false);
	SoundFileHandler::GetInstance()->SetupStream("Sound/SoundFX/GUI/Click.ogg", "ButtonClick", false);
}

void Options::Initialize()
{
	myIsActive = false;

	myBackground = new DX2D::CSprite("Sprites/Options_InG/Background.dds");

	myBackground->SetPosition({ 0.5f - myBackground->GetSize().x / 2, 0.5f - myBackground->GetSize().y / 2 });

	SliderSetOfArguments SliderArgsSFX;
	SliderSetOfArguments SliderArgsMusic;

	mySFXSlider.InitSprites(SliderArgsSFX.mySpriteArguments);
	myMusicSlider.InitSprites(SliderArgsMusic.mySpriteArguments);

	SliderArgsSFX.myPositionArguments.mySliderPosition = { 0.5f - mySFXSlider.GetSliderSize().x / 2 + (0.058f / 2), myBackground->GetPosition().y + 0.22f + 0.145f };
	SliderArgsMusic.myPositionArguments.mySliderPosition = { 0.5f - myMusicSlider.GetSliderSize().x / 2 + (0.058f / 2), myBackground->GetPosition().y + 0.32f + 0.15f };

	SliderArgsSFX.myPositionArguments.myKnobPivot = {0.5f, 0.5f};
	SliderArgsMusic.myPositionArguments.myKnobPivot = { 0.5f, 0.5f };

	mySFXSlider.InitPositions(SliderArgsSFX.myPositionArguments);
	myMusicSlider.InitPositions(SliderArgsMusic.myPositionArguments);

	SliderArgsSFX.myPositionArguments.myKnobPosition = { mySFXSlider.GetSliderPosition().x + mySFXSlider.GetSliderSize().x, mySFXSlider.GetSliderPosition().y + mySFXSlider.GetSliderSize().y / 2 };
	SliderArgsMusic.myPositionArguments.myKnobPosition = { myMusicSlider.GetSliderPosition().x + myMusicSlider.GetSliderSize().x, myMusicSlider.GetSliderPosition().y + myMusicSlider.GetSliderSize().y / 2 };

	mySFXSlider.InitPositions(SliderArgsSFX.myPositionArguments);
	myMusicSlider.InitPositions(SliderArgsMusic.myPositionArguments);

	ButtonSetOfArguments ButtonArgsFullscreen;
	ButtonSetOfArguments ButtonArgsMenu;
	ButtonSetOfArguments ButtonArgsResume;
	

	myFullscreenButton.InitSprites(ButtonArgsFullscreen.mySpriteArguments);

	ButtonArgsMenu.mySpriteArguments.myButtonCheckedPath = "Sprites/Options_InG/ReturnToMenu_Unchecked.dds";
	ButtonArgsMenu.mySpriteArguments.myButtonUnCheckedPath = "Sprites/Options_InG/ReturnToMenu_Unchecked.dds";
	ButtonArgsMenu.mySpriteArguments.myButtonHoveredPath = "Sprites/Options_InG/ReturnToMenu_Checked.dds";
	ButtonArgsResume.mySpriteArguments.myButtonCheckedPath = "Sprites/Options_InG/Resume_Unchecked.dds";
	ButtonArgsResume.mySpriteArguments.myButtonUnCheckedPath = "Sprites/Options_InG/Resume_Unchecked.dds";
	ButtonArgsResume.mySpriteArguments.myButtonHoveredPath = "Sprites/Options_InG/Resume_Checked.dds";

	myMenuButton.InitSprites(ButtonArgsMenu.mySpriteArguments);
	myResumeButton.InitSprites(ButtonArgsResume.mySpriteArguments);

	ButtonArgsFullscreen.myPositionArguments.myButtonPivot = {0.5f, 0.5f};
	ButtonArgsFullscreen.myPositionArguments.myButtonPosition = {0.5f, myBackground->GetPosition().y + 0.445f + 0.14f};

	ButtonArgsMenu.myPositionArguments.myButtonPivot = { 0.0f, 0.0f };
	ButtonArgsMenu.myPositionArguments.myButtonPosition = { 
		myBackground->GetPosition().x + 0.015f, 
		myBackground->GetPosition().y + 0.574f + 0.15f };

	ButtonArgsResume.myPositionArguments.myButtonPivot = { 0.0f, 0.0f };
	ButtonArgsResume.myPositionArguments.myButtonPosition = { 
		myBackground->GetPosition().x + myBackground->GetSize().x - myResumeButton.GetSize().x + (0.056f / 2) - 0.015f, 
		myBackground->GetPosition().y + 0.574f + 0.15f };

	myFullscreenButton.InitPositions(ButtonArgsFullscreen.myPositionArguments);
	myMenuButton.InitPositions(ButtonArgsMenu.myPositionArguments);
	myResumeButton.InitPositions(ButtonArgsResume.myPositionArguments);
}

void Options::Update(float aDeltaTime)
{
	if (myIsActive == true)
	{
		myMusicSlider.Update(aDeltaTime);
		mySFXSlider.Update(aDeltaTime);
		myFullscreenButton.Update(aDeltaTime);
		myMenuButton.Update(aDeltaTime);
		myResumeButton.Update(aDeltaTime);
	}
}

void Options::Render(Synchronizer& aSynchronizer)
{
	if (myIsActive == true)
	{
		RenderCommand background;
		background.mySprite = myBackground;
		background.myType = eRenderType::eSprite;
		background.myPosition = myBackground->GetPosition();
		background.myAmbience = 1.0f;
		background.myChangeAmbience = true;
		aSynchronizer.AddRenderCommand(background);

		myMusicSlider.Render(aSynchronizer);
		mySFXSlider.Render(aSynchronizer);
		myFullscreenButton.Render(aSynchronizer);
		myMenuButton.Render(aSynchronizer);
		myResumeButton.Render(aSynchronizer);
	}
}

Options::~Options()
{
	SAFE_DELETE(myBackground);
}
