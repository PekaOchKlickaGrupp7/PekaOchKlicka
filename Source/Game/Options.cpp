#include "stdafx.h"
#include "Options.h"
#include "..\CommonUtilities\Macros.h"


Options::Options()
{
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

	SliderArgsSFX.myPositionArguments.mySliderPosition = { 0.5f - mySFXSlider.GetSliderSize().x / 2, myBackground->GetPosition().y + 0.22f };
	SliderArgsMusic.myPositionArguments.mySliderPosition = { 0.5f - myMusicSlider.GetSliderSize().x / 2, myBackground->GetPosition().y + 0.32f };

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

	myFullscreenButton.InitSprites(ButtonArgsFullscreen.mySpriteArguments);

	ButtonArgsMenu.mySpriteArguments.myButtonCheckedPath = "Sprites/Options_InG/ReturnToMenu_Unchecked.dds";
	ButtonArgsMenu.mySpriteArguments.myButtonUnCheckedPath = "Sprites/Options_InG/ReturnToMenu_Unchecked.dds";
	ButtonArgsMenu.mySpriteArguments.myButtonHoveredPath = "Sprites/Options_InG/ReturnToMenu_Checked.dds";

	myMenuButton.InitSprites(ButtonArgsMenu.mySpriteArguments);

	ButtonArgsFullscreen.myPositionArguments.myButtonPivot = {0.5f, 0.5f};
	ButtonArgsFullscreen.myPositionArguments.myButtonPosition = {0.5f, myBackground->GetPosition().y + 0.462f };

	ButtonArgsMenu.myPositionArguments.myButtonPivot = { 0.5f, 0.5f };
	ButtonArgsMenu.myPositionArguments.myButtonPosition = { 0.5f, myBackground->GetPosition().y + 0.574f };

	myFullscreenButton.InitPositions(ButtonArgsFullscreen.myPositionArguments);
	myMenuButton.InitPositions(ButtonArgsMenu.myPositionArguments);
}

void Options::Update(float aDeltaTime)
{
	myMusicSlider.Update(aDeltaTime);
	mySFXSlider.Update(aDeltaTime);
	myFullscreenButton.Update(aDeltaTime);
	myMenuButton.Update(aDeltaTime);
}

void Options::Render(Synchronizer& aSynchronizer)
{
	if (myIsActive == true)
	{
		RenderCommand background;
		background.mySprite = myBackground;
		background.myType = eRenderType::eSprite;
		background.myPosition = myBackground->GetPosition();
		aSynchronizer.AddRenderCommand(background);

		myMusicSlider.Render(aSynchronizer);
		mySFXSlider.Render(aSynchronizer);
		myFullscreenButton.Render(aSynchronizer);
		myMenuButton.Render(aSynchronizer);
	}
}

Options::~Options()
{
	SAFE_DELETE(myBackground);
}
