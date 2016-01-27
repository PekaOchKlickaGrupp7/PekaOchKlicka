#include "stdafx.h"
#include "FadeState.h"
#include "Synchronizer.h"
#include "StateStackProxy.h"
#include "MainMenuState.h"
#include "ResolutionManager.h"

FadeState::FadeState(StateStackProxy& aStateStackProxy,
	CU::DirectInput::InputManager& aInputManager,
	CU::TimeSys::TimerManager& aTimerManager) :
	GameState(aStateStackProxy, aInputManager, aTimerManager)
{
	mySprite = new DX2D::CSprite("Sprites/tga_logo.dds");
	myFadeTime = 7.0f;
	myTime = 0;
	DX2D::CColor colour = mySprite->GetColor();

	colour.myA = 0;

	mySprite->SetColor(colour);

}

eStateStatus FadeState::Update(float aTimeDelta)
{
	myTime += aTimeDelta;
	DX2D::CColor colour = mySprite->GetColor();

	colour.myA += aTimeDelta*0.1f;

	mySprite->SetColor(colour);
	if (myTime >= myFadeTime)
	{
		myStateStackProxy.PushSubGameState(new MainMenuState(myStateStackProxy, myInputManager
			, myTimerManager));
	}
	return eStateStatus::eKeepState;
}

void FadeState::Render(Synchronizer& aSynchronizer)
{
	RenderCommand command;

	command.myType = eRenderType::eSprite;
	command.myPosition.y = 0.5f;
	command.myPosition.x = 0.5f;

	command.mySprite = mySprite;
	aSynchronizer.AddRenderCommand(command);
}

FadeState::~FadeState()
{
	delete mySprite;
}
