#include "stdafx.h"
#include "GameWorld.h"

#include "StateStackProxy.h"
#include "Synchronizer.h"

#include <iostream>

CGameWorld::CGameWorld(StateStackProxy& aStateStackProxy, CU::DirectInput::InputWrapper& aInputWrapper, CU::TimeSys::TimerManager& aTimerManager) :
GameState(aStateStackProxy, aInputWrapper, aTimerManager)
{
	Init();
}


CGameWorld::~CGameWorld()
{
	delete text;
	delete myShape;


	mySoundManager.ReleaseSound(mySFXJaguar);
	mySoundManager.ReleaseSound(mySFXRain);
}

void CGameWorld::Init()
{
	mySoundManager.CreateSound(&mySFXJaguar, "SFX/jaguar.wav");
	mySoundManager.CreateSound(&mySFXRain, "SFX/rain.wav");
	

	mySoundManager.CreateChannel(std::string("RainChannel"));

	//mySoundManager.GetChannel(std::string("RainChannel")).GetFMODChannel()->setPan(-2001.0f);

	mySoundManager.PlaySound(mySFXRain, mySoundManager.GetChannel(std::string("RainChannel")).GetFMODChannel(), true);

	text = new DX2D::CText("Text/calibril.ttf_sdf");
	text->myText = "Test";
	text->myPosition = DX2D::Vector2f(0.5f, 0.02f);
	text->myColor.Set(1, 1, 1, 1.0f);
	text->mySize = 0.4f;

	myShape = new DX2D::CCustomShape();
	myShape->SetTexture("sprites/testBalll_64_norm.dds");

	myAudioListenerSprite = new DX2D::CSprite("Sprites/AudioListener.dds");
	myAudioListenerSprite->SetPosition({ 0.5f, 0.5f });
	myAudioSourceSprite = new DX2D::CSprite("Sprites/AudioSource.dds");
	myAudioSourceSprite->SetPosition({ 0.5f, 0.5f });
	myAudioSourcePosition = {0.5f, 0.5f};
}


eStateStatus CGameWorld::Update(float aTimeDelta)
{
	if (myInputWrapper.GetKeyWasPressed(DIK_ESCAPE) == true)
	{
		return eStateStatus::ePopMainState;
	}
	static float test = 0;
	test += aTimeDelta;

	float cosinusLow = cos(test) * 0.1f;
	myShape->Reset();
	myShape->AddPoint(DX2D::Vector2f(0.1f, 0.1f), DX2D::CColor(1, 1, 1, 1), DX2D::Vector2f(0, 0));
	myShape->AddPoint(DX2D::Vector2f(0.2f, 0.1f), DX2D::CColor(1, 1, 1, 1), DX2D::Vector2f(1, 0));
	myShape->AddPoint(DX2D::Vector2f(0.1f + cosinusLow, 0.2f), DX2D::CColor(1, 1, 1, 1), DX2D::Vector2f(0, 1));

	myShape->AddPoint(DX2D::Vector2f(0.2f, 0.1f), DX2D::CColor(1, 1, 1, 1), DX2D::Vector2f(1, 0));
	myShape->AddPoint(DX2D::Vector2f(0.1f + cosinusLow, 0.2f), DX2D::CColor(1, 1, 1, 1), DX2D::Vector2f(0, 1));
	myShape->AddPoint(DX2D::Vector2f(0.2f + cosinusLow, 0.2f), DX2D::CColor(1, 1, 1, 1), DX2D::Vector2f(1, 1));

	myShape->BuildShape();

	
	static int aSpeed = 1.0f;
	if (myAudioSourcePosition.x + myAudioSourceSprite->GetSize().x > 0 && myAudioSourcePosition.x - myAudioSourceSprite->GetSize().x < 1)
	{
		myAudioSourcePosition.x += myInputWrapper.GetMouseLocationX() * aSpeed * aTimeDelta;
	}
	else
	{
		myAudioSourcePosition.x = 0.5f;
	}
	if (myAudioSourcePosition.y + myAudioSourceSprite->GetSize().y > 0 && myAudioSourcePosition.y - myAudioSourceSprite->GetSize().y < 1)
	{
		myAudioSourcePosition.y += myInputWrapper.GetMouseLocationY() * aSpeed * aTimeDelta;
	}
	else
	{
		myAudioSourcePosition.y = 0.5f;
	}
	std::cout << myInputWrapper.GetMouseLocationX() << std::endl;
	myAudioSourceSprite->SetPosition(DX2D::Vector2f(myAudioSourcePosition.x, myAudioSourcePosition.y));

	mySoundManager.SetChannelAttributes(mySoundManager.GetChannel(std::string("RainChannel")).GetFMODChannel(),
		myAudioSourcePosition.x * 10, myAudioSourcePosition.y * 10);

	DX2D::CEngine::GetInstance()->GetLightManager().SetAmbience(1.0f);
	return eStateStatus::eKeepState;
}

void CGameWorld::Render(Synchronizer& aSynchronizer)
{
	RenderCommand command;

	command.myType = eRenderType::eSprite;
	command.myPosition = myAudioListenerSprite->GetPosition();
	command.mySprite = myAudioListenerSprite;
	aSynchronizer.AddRenderCommand(command);

	command.myType = eRenderType::eSprite;
	command.myPosition = myAudioSourceSprite->GetPosition();
	command.mySprite = myAudioSourceSprite;
	aSynchronizer.AddRenderCommand(command);

	command.myType = eRenderType::eCustomShape;
	command.myPosition = myShape->myPosition;
	command.myCustomShape = myShape;
	aSynchronizer.AddRenderCommand(command);

	command.myType = eRenderType::eText;
	command.myPosition = text->myPosition;
	command.myText = text;
	aSynchronizer.AddRenderCommand(command);
}
