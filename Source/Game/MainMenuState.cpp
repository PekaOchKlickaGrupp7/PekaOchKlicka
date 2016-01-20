#include "stdafx.h"
#include "MainMenuState.h"
#include "Synchronizer.h"
#include <tga2d/sprite/sprite.h>
#include "StateStackProxy.h"
#include "GameWorld.h"
#include "MenuImageItem.h"
#include "..\CommonUtilities\TimerManager.h"
#include "..\CommonUtilities\InputManager.h"

MainMenuState::MainMenuState(StateStackProxy& aStateStackProxy, 
	CU::DirectInput::InputManager& aInputManager,
	CU::TimeSys::TimerManager& aTimerManager) :
	GameState(aStateStackProxy, aInputManager, aTimerManager)
{
	mySelection = MenuItem::eAction::NONE;
}

MainMenuState::~MainMenuState()
{
	delete myBackground;
	delete myTitle;
	myButtons.DeleteAll();
}

eStateStatus MainMenuState::Update(float aTimeDelta)
{
	if (myInputManager.KeyPressed(DIK_ESCAPE))
	{
		return eStateStatus::ePopMainState;
	}

	if (myInputManager.LeftMouseButtonClicked() == true)
	{
		switch (mySelection)
		{
		case MenuItem::eAction::NONE:
			break;
		case MenuItem::eAction::PLAY:
			myStateStackProxy.PushMainGameState(new CGameWorld(myStateStackProxy, myInputManager
				, myTimerManager));
			break;
		case MenuItem::eAction::EXIT:
			return eStateStatus::ePopMainState;
			break;
		default:
			break;
		}
	}

	CalcHighlights();

	return eStateStatus::eKeepState;
}

void MainMenuState::InitState()
{
	myBackground = new DX2D::CSprite("Sprites/menu/background.dds");

	myTitle = new DX2D::CSprite("Sprites/menu/title.dds");

	float scaleButtons = 1.f;

	myButtons.Add(new MenuImageItem(MenuItem::eAction::PLAY, "Sprites/menu/play.dds",
		"Sprites/menu/playHighlight.dds", Vector2<float>(0, 5 * 720 / 10.f), scaleButtons));

	myButtons.Add(new MenuImageItem(MenuItem::eAction::EXIT, "Sprites/menu/exit.dds",
		"Sprites/menu/exitHighlight.dds", Vector2<float>(0, 8 * 720 / 10.f), scaleButtons));

}

void MainMenuState::CalcHighlights()
{
	for (int i = 0; i < myButtons.Size(); ++i)
	{
		
		if (myButtons[i]->Collide(Vector2<float>(myInputManager.GetAbsoluteMousePos().x,
			myInputManager.GetAbsoluteMousePos().y)) == true)
		{
			myButtons[i]->SetHighlight(true);
			mySelection = myButtons[i]->GetAction();
		}
		else
		{
			myButtons[i]->SetHighlight(false);
		}
	}
}

void MainMenuState::Render(Synchronizer& aSynchronizer)
{
	RenderCommand command;
	
	command.myConvertFromPixelToSpaceNormal = true;
	command.myType = eRenderType::eSprite;

	command.myPosition.y = 0;
	command.myPosition.x = 0;
	command.mySprite = myBackground;
	aSynchronizer.AddRenderCommand(command);

	command.mySprite = myTitle;
	aSynchronizer.AddRenderCommand(command);

	for (int i = 0; i < myButtons.Size(); ++i)
	{
		myButtons[i]->Render(aSynchronizer);
	}
}

