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
	SoundManager::DestroyInstance();
	myObjects.RemoveAll();
	delete myAudioListenerSprite;
	delete myAudioSourceSprite;
	delete myResolutionTestSprite;
}

void CGameWorld::Init()
{
	SoundManager::GetInstance(); // Creates a sound manager instance.

	myJson.Load("root.json");

	myObjects.Init(128);
	myJson.LoadLevel("Smiley_Face", myObjects);

	mySFXRain.Create3D("SFX/rain.wav");
	mySFXRain.SetLooping(true);
	mySFXRain.Play();

	text = new DX2D::CText("Text/calibril.ttf_sdf");
	text->myText = "Test";
	text->myPosition = DX2D::Vector2f(0.5f, 0.02f);
	text->myColor.Set(1, 1, 1, 1.0f);
	text->mySize = 0.4f;

	myAudioListenerSprite = new DX2D::CSprite("Sprites/AudioListener.dds");
	myAudioListenerSprite->SetPosition({ 0.5f, 0.5f });
	myAudioSourceSprite = new DX2D::CSprite("Sprites/AudioSource.dds");
	myAudioSourceSprite->SetPosition({ 0.5f, 0.5f });
	myAudioSourcePosition = {0.5f, 0.5f};

	myResolutionTestSprite = new DX2D::CSprite("Sprites/ResolutionTest.dds");
}


eStateStatus CGameWorld::Update(float aTimeDelta)
{
	SoundManager::GetInstance()->Update(static_cast<float>(myTimerManager.GetMasterTimer().GetTimeElapsed().GetMiliseconds()));
	if (myInputWrapper.GetKeyWasPressed(DIK_ESCAPE) == true)
	{
		return eStateStatus::ePopMainState;
	}
	
	static float aSpeed = 0.01f;

	if (myInputWrapper.GetKeyWasPressed(DIK_SPACE) == true)
	{
		std::cout << "Smiley face" << std::endl;
		myJson.LoadLevel("Smiley_Face", myObjects);
	}

	myAudioSourcePosition.x += static_cast<float>(myInputWrapper.GetMouseLocationX()) * aSpeed * aTimeDelta;
	myAudioSourcePosition.y += myInputWrapper.GetMouseLocationY() * aSpeed * aTimeDelta;

	myAudioSourceSprite->SetPosition(DX2D::Vector2f(myAudioSourcePosition.x, myAudioSourcePosition.y));

	mySFXRain.SetPosition(myAudioSourcePosition.x * 10, myAudioSourcePosition.y * 10);
	//std::cout << "Sound Pos X: " << mySFXRain.GetPosition().x << ", Y: " << mySFXRain.GetPosition().y << std::endl;

	if (myInputWrapper.GetKeyWasPressed(DIK_SPACE) == true)
	{
		myAudioSourcePosition.x = 0.5f;
		myAudioSourcePosition.y = 0.5f;
		myAudioListenerSprite->SetPosition(myAudioSourcePosition);
	}

	DX2D::CEngine::GetInstance()->GetLightManager().SetAmbience(1.0f);
	return eStateStatus::eKeepState;
}

void CGameWorld::Render(Synchronizer& aSynchronizer)
{
	RenderCommand command;

	/*command.myType = eRenderType::eSprite;
	command.myPosition = myResolutionTestSprite->GetPosition();
	command.mySprite = myResolutionTestSprite;
	aSynchronizer.AddRenderCommand(command);*/
	for (unsigned int i = 0; i < myObjects.Size(); ++i)
	{
		RenderLevel(aSynchronizer, myObjects[i]);
	}

	command.myType = eRenderType::eSprite;
	command.myPosition = myAudioListenerSprite->GetPosition();
	command.mySprite = myAudioListenerSprite;
	aSynchronizer.AddRenderCommand(command);

	command.myType = eRenderType::eSprite;
	command.myPosition = myAudioSourceSprite->GetPosition();
	command.mySprite = myAudioSourceSprite;
	aSynchronizer.AddRenderCommand(command);

	command.myType = eRenderType::eText;
	command.myPosition = text->myPosition;
	command.myText = text;
	aSynchronizer.AddRenderCommand(command);

}

void CGameWorld::RenderLevel(Synchronizer& aSynchronizer, ObjectData& aNode)
{
	RenderCommand command;
	command.myType = eRenderType::eSprite;
	if (aNode.myActive == true)
	{
		if (aNode.mySprite != nullptr)
		{
			command.myPosition = DX2D::Vector2f(aNode.myX, aNode.myY);
			command.mySprite = aNode.mySprite;
			command.mySprite->SetColor({ 1, 1, 1, 1 });
			aSynchronizer.AddRenderCommand(command);
		}
		for (unsigned int j = 0; j < aNode.myChilds.Size(); ++j)
		{
			RenderLevel(aSynchronizer, aNode.myChilds[j]);
		}
	}
}
