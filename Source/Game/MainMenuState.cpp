#include "stdafx.h"
#include "MainMenuState.h"
#include "Synchronizer.h"
#include <tga2d/sprite/sprite.h>
#include "StateStackProxy.h"
#include "GameWorld.h"
#include "MenuImageItem.h"
#include "..\CommonUtilities\TimerManager.h"
#include "..\CommonUtilities\InputManager.h"

#include "rapidjson\document.h"
#include "..\CommonUtilities\DL_Debug.h"

#include "ResolutionManager.h"
#include "MouseManager.h"

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
	(aTimeDelta);
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
	LoadJson();
}

void MainMenuState::LoadJson()
{
	const char* data = ReadFile("Menu.json");
	rapidjson::Document root;
	root.Parse(data);

	if (root.HasParseError() == true)
	{
		DL_DEBUG("Failed to load Menu.json.");
		root.GetAllocator().~MemoryPoolAllocator();
		return;
	}

	myBackground = new DX2D::CSprite(root["background"].GetString());
	myTitle = new DX2D::CSprite(root["title"].GetString());

	rapidjson::Value& menuButtons = root["Buttons"];

	if (menuButtons.IsNull() == true)
	{
		DL_DEBUG("Buttons is not a member of Menu.json");
		root.GetAllocator().~MemoryPoolAllocator();
		return;
	}

	for (unsigned int i = 0; i < menuButtons.Size(); ++i)
	{
		rapidjson::Value& button = menuButtons[i];

		std::string selection = button["selection"].GetString();
		const char* path = button["path"].GetString();
		const char* highlightPath = button["highlightPath"].GetString();
		float positionX = static_cast<float>(button["positionX"].GetDouble());
		float positionY = static_cast<float>(button["positionY"].GetDouble());

		myButtons.Add(new MenuImageItem(GetActionType(selection), path,
			highlightPath, Vector2<float>(positionX, positionY), 1));
	}
	delete data;
}

void MainMenuState::CalcHighlights()
{
	for (int i = 0; i < myButtons.Size(); ++i)
	{
		
		if (myButtons[i]->Collide(
			Vector2<float>(MouseManager::GetInstance()->GetPosition().x, MouseManager::GetInstance()->GetPosition().y)) == true)
		{
			myButtons[i]->SetHighlight(true);
			mySelection = myButtons[i]->GetAction();
			break;
		}
		else
		{
			myButtons[i]->SetHighlight(false);
			mySelection = MenuItem::eAction::NONE;
		}
	}
}

void MainMenuState::Render(Synchronizer& aSynchronizer)
{
	RenderCommand command;
	
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

	MouseManager::GetInstance()->Render(aSynchronizer);
}

MenuItem::eAction MainMenuState::GetActionType(std::string& aSelection)
{
	if (aSelection == "play")
	{
		return MenuItem::eAction::PLAY;
	}

	if (aSelection == "exit")
	{
		return MenuItem::eAction::EXIT;
	}

	return MenuItem::eAction::NONE;
}

const char* MainMenuState::ReadFile(const char* aFile)
{
	std::ifstream input(aFile);

	std::string str((std::istreambuf_iterator<char>(input)),
		std::istreambuf_iterator<char>());

	char* data = new char[str.length() + 1];
	int a = 0;
	for (unsigned int i = 0; i < str.length(); ++i)
	{
		if (str[i] != 0)
		{
			data[a] = str[i];
			a++;
		}
	}

	data[a] = '\0';
	input.close();

	return data;
}
