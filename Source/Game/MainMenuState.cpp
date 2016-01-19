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

	myMenuCursor.Update(myInputManager);

	CalcHighlights();

	return eStateStatus::eKeepState;
}

void MainMenuState::InitState()
{
	myBackground = new DX2D::CSprite("Sprites/menu/background.dds");
	//myBackground->SetTextureRect(0, 0, 2048.f, 1024.f);
	myBackground->SetPivot(DX2D::Vector2<float>(myBackground->GetSize().x / 2, myBackground->GetSize().y / 2));


	myTitle = new DX2D::CSprite("Sprites/menu/title.dds");
	//myTitle->SetTextureRect(0, 0, 1024.f, 512.f);
	myTitle->SetPivot(DX2D::Vector2<float>(myTitle->GetSize().x / 2, myTitle->GetSize().y / 2));

	float scaleButtons = 0.75f;

	myButtons.Add(new MenuImageItem(MenuItem::eAction::PLAY, "Sprites/menu/play.dds", "Sprites/menu/playHighlight.dds", Vector2<float>(0, 5 * 720 / 10.f), scaleButtons * 720 / 1024.f));

	myButtons.Add(new MenuImageItem(MenuItem::eAction::EXIT, "Sprites/menu/exit.dds", "Sprites/menu/exitHighlight.dds", Vector2<float>(0, 8 * 720 / 10.f), scaleButtons * 720 / 1024.f));

}

void MainMenuState::CalcHighlights()
{
	bool highlightCursor = false;
	for (int i = 0; i < myButtons.Size(); ++i)
	{
		if (myButtons[i]->Collide(myMenuCursor.GetScreenPos()) == true)
		{
			highlightCursor = true;
			myButtons[i]->SetHighlight(true);
			mySelection = myButtons[i]->GetAction();
		}
		else
		{
			myButtons[i]->SetHighlight(false);
		}
	}
	myMenuCursor.SetHighlight(highlightCursor);
}

void MainMenuState::Render(Synchronizer& aSynchronizer)
{
	RenderCommand command;
	
	command.myConvertFromPixelToSpaceNormal = true;
	command.myType = eRenderType::eSprite;

	command.myPosition.y = 720;
	command.myPosition.x = 1280;
	command.mySprite = myBackground;
	aSynchronizer.AddRenderCommand(command);

	command.myPosition.x = 300;
	command.myPosition.y = 100;
	command.mySprite = myTitle;
	aSynchronizer.AddRenderCommand(command);

	for (int i = 0; i < myButtons.Size(); ++i)
	{
		myButtons[i]->Render(aSynchronizer);
	}

	myMenuCursor.Render(aSynchronizer);
}

