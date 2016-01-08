#include "stdafx.h"
#include "GameWorld.h"

#include <tga2d/sprite/sprite.h>
#include <tga2d/Engine.h>
#include <tga2d/light/light_manager.h>
#include <tga2d/sprite/sprite_batch.h>
#include <tga2d/drawers/debug_drawer.h>
#include <tga2d/text/text.h>
#include <tga2d/primitives/custom_shape.h>
#include <tga2d/math/color.h>

#include "StateStackProxy.h"
#include "Synchronizer.h"

using namespace DX2D;
CGameWorld::CGameWorld(StateStackProxy& aStateStackProxy, CU::DirectInput::InputWrapper& aInputWrapper, CU::TimeSys::TimerManager& aTimerManager) :
GameState(aStateStackProxy, aInputWrapper, aTimerManager)
{
	Init();
}


CGameWorld::~CGameWorld()
{
	delete myCarSprite;
	delete myCarSprite2;
	delete text;
	delete myShape;

}

void CGameWorld::Init()
{
	//sound.Initialize();

	SoundClass soundSample;
	sound.createSound(&soundSample, "SFX/jaguar.wav");

	sound.playSound(soundSample, true);
	sound.releaseSound(soundSample);

	myCarSprite = new DX2D::CSprite("sprites/car_1.dds");
	myCarSprite2 = new DX2D::CSprite("sprites/car_1.dds");


	myCarSprite->SetPosition(DX2D::Vector2f(0.0f, 0.0f));
	myCarSprite2->SetPosition(DX2D::Vector2f(myCarSprite->GetSize().x, myCarSprite->GetSize().y));


	DX2D::Vector2f size = myCarSprite->GetSize();

	text = new DX2D::CText("Text/calibril.ttf_sdf");
	text->myText = "Test";
	text->myPosition = DX2D::Vector2f(0.5f, 0.02f);
	text->myColor.Set(1, 1, 1, 1.0f);
	text->mySize = 0.4f;

	myShape = new DX2D::CCustomShape();
	myShape->SetTexture("sprites/testBalll_64_norm.dds");

}


eStateStatus CGameWorld::Update(float aTimeDelta)
{
	myInputWrapper.Update();
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

	CEngine::GetInstance()->GetLightManager().SetAmbience(1.0f);
	return eStateStatus::eKeepState;
}

void CGameWorld::Render(Synchronizer& aSynchronizer)
{
	RenderCommand command;
	command.myType = eRenderType::eSprite;
	command.myPosition = myCarSprite->GetPosition();
	command.mySprite = myCarSprite;
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
