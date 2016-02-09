#include "stdafx.h"
#include "Options.h"


Options::Options()
{
}

void Options::Initialize()
{
	myIsActive = false;

	myBackground = new DX2D::CSprite("Sprites/Options_InG/Background.dds");
	myVolumeSFXSlider = new DX2D::CSprite("Sprites/Options_InG/Slider.dds");
	myVolumeMusicSlider = new DX2D::CSprite("Sprites/Options_InG/Slider.dds");
	myVolumeSFXKnob = new DX2D::CSprite("Sprites/Options_InG/Knob.dds");
	myVolumeMusicKnob = new DX2D::CSprite("Sprites/Options_InG/Knob.dds");

	myBackground->SetPosition({ 0.5f - myBackground->GetSize().x / 2, 0.5f - myBackground->GetSize().y / 2 + 0.05f });

	myVolumeSFXSlider->SetPosition({ 0.5f - myVolumeSFXSlider->GetSize().x / 2, myBackground->GetPosition().y + 0.22f  });
	myVolumeMusicSlider->SetPosition({ 0.5f - myVolumeMusicSlider->GetSize().x / 2, myBackground->GetPosition().y + 0.32f });

	myVolumeSFXKnob->SetPivot({ 0.5f, 0.5f });
	myVolumeMusicKnob->SetPivot({ 0.5f, 0.5f });

	myVolumeSFXKnob->SetPosition({ myVolumeSFXSlider->GetPosition().x + myVolumeSFXSlider->GetSize().x, myVolumeSFXSlider->GetPosition().y + myVolumeSFXSlider->GetSize().y / 2 });
	myVolumeMusicKnob->SetPosition({ myVolumeMusicSlider->GetPosition().x + myVolumeMusicSlider->GetSize().x, myVolumeMusicSlider->GetPosition().y + myVolumeMusicSlider->GetSize().y / 2 });
}

void Options::Update(float aDeltaTime)
{

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

		RenderCommand volumeSFXSlider;
		volumeSFXSlider.mySprite = myVolumeSFXSlider;
		volumeSFXSlider.myType = eRenderType::eSprite;
		volumeSFXSlider.myPosition = myVolumeSFXSlider->GetPosition();
		aSynchronizer.AddRenderCommand(volumeSFXSlider);

		RenderCommand volumeMusicSlider;
		volumeMusicSlider.mySprite = myVolumeMusicSlider;
		volumeMusicSlider.myType = eRenderType::eSprite;
		volumeMusicSlider.myPosition = myVolumeMusicSlider->GetPosition();
		aSynchronizer.AddRenderCommand(volumeMusicSlider);

		RenderCommand volumeMusicKnob;
		volumeMusicKnob.mySprite = myVolumeMusicKnob;
		volumeMusicKnob.myType = eRenderType::eSprite;
		volumeMusicKnob.myPosition = myVolumeMusicKnob->GetPosition();
		aSynchronizer.AddRenderCommand(volumeMusicKnob);

		RenderCommand volumeSFXKnob;
		volumeSFXKnob.mySprite = myVolumeSFXKnob;
		volumeSFXKnob.myType = eRenderType::eSprite;
		volumeSFXKnob.myPosition = myVolumeSFXKnob->GetPosition();
		aSynchronizer.AddRenderCommand(volumeSFXKnob);
	}
}

Options::~Options()
{
}
