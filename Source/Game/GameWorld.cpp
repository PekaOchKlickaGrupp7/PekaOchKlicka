#include "stdafx.h"
#include "GameWorld.h"

#include "StateStackProxy.h"
#include "Synchronizer.h"

#include "ResolutionManager.h"

#include "..\CommonUtilities\GrowingArray.h"
#include <iostream>
#include "Game.h"
#include "EventManager.h"
#include "HitBox.h"
#include "Room.h"

CGameWorld::CGameWorld(StateStackProxy& aStateStackProxy, CU::DirectInput::InputManager& aInputManager, CU::TimeSys::TimerManager& aTimerManager) :
GameState(aStateStackProxy, aInputManager, aTimerManager)
{
	Init();
}

CGameWorld::~CGameWorld()
{
	delete text;
	delete myResolutionTestSprite;
	mySFXRain.Destroy();
}

void CGameWorld::ChangeLevel(const std::string& aString)
{
	myCurrentRoom = myRooms[aString];
	myCurrentRoom->OnLoad();

	if (myCurrentRoom == nullptr)
	{
		DL_PRINT("Current room is null!");
	}
	EventManager::GetInstance()->ChangeRoom(myCurrentRoom);
}

Player* CGameWorld::GetPlayer()
{
	return &myPlayer;
}

void CGameWorld::Init()
{
	myJson.Load("root.json", myRooms, this);

	std::cout << "Level: " << CGame::myTestLevel << std::endl;
	if (CGame::myTestLevel.size() > 0)
	{
		DL_PRINT(CGame::myTestLevel.c_str());
		ChangeLevel(CGame::myTestLevel);
	}
	
	mySFXRain.Create3D("SFX/rain.wav");
	mySFXRain.SetLooping(true);
	//mySFXRain.SetVolume(10.0f);
	mySFXRain.Play();

	text = new DX2D::CText("Text/calibril.ttf_sdf");
	text->myText = "Test";
	text->myPosition = DX2D::Vector2f(0.5f, 0.02f);
	text->myColor.Set(1, 1, 1, 1.0f);
	text->mySize = 0.4f;

	myResolutionTestSprite = new DX2D::CSprite("Sprites/ResolutionTest.dds");

	//Create the player character
	//BUG: Why does pivot.x = 0.25 refer to the center
	//of myAnimation.mySprite and not 0.5? ~Erik
	myPlayer.Init("Sprites/hallBoy.dds", DX2D::Vector2f(0.5f, 0.8f), DX2D::Vector2f(0.25f, 1.0f), 0.2f);

	//Test item
	myTestItem.Init("Sprites/inventoryItem.png", "Sprites/inventoryItem.png",
		"Shovel", "A Shovel", DX2D::Vector2f(0.2f, 0.7f), false, "Test Level");
	myTestItem2.Init("Sprites/inventoryItem.png", "Sprites/inventoryItem.png",
		"Shovel", "A Shovel", DX2D::Vector2f(0.2f, 0.7f), false, "Test Level");

	myPlayer.AddItemToInventory(&myTestItem);
	myPlayer.AddItemToInventory(&myTestItem2);
}


eStateStatus CGameWorld::Update(float aTimeDelta)
{
	EventManager::GetInstance()->Update(aTimeDelta);

	if (myInputManager.KeyPressed(DIK_ESCAPE) == true)
	{
		return eStateStatus::ePopMainState;
	}
	
	if (myInputManager.KeyPressed(DIK_SPACE) == true)
	{
		myJson.Load("root.json", myRooms, this);

		if (CGame::myTestLevel.size() > 0)
		{
			DL_PRINT(CGame::myTestLevel.c_str());
			ChangeLevel(CGame::myTestLevel);
		}
		}

	RECT windowSize;
	GetWindowRect(*DX2D::CEngine::GetInstance()->GetHWND(), &windowSize);

	DX2D::CEngine::GetInstance()->GetLightManager().SetAmbience(1.0f);

	//Move character if inside nav mesh
	if (myInputManager.LeftMouseButtonClicked())
	{
		myTargetPosition.x = static_cast<float>(MouseManager::GetInstance()->GetPosition().x);
		myTargetPosition.y = static_cast<float>(MouseManager::GetInstance()->GetPosition().y);

		if (myCurrentRoom != nullptr && myCurrentRoom->GetNavMeshes().Size() > 0)
		{
			if (myCurrentRoom->GetNavMeshes()[0].
				PointInsideCheck(Point2f(
				myTargetPosition.x,
				myTargetPosition.y)
				) == true)
			{
				myPlayer.SetIsMoving(true);
			}
			else
			{
				myPlayer.SetIsMoving(false);
			}
		}
	}

	//Makes sure player can not walk through obstacles
	if (myCurrentRoom->GetNavMeshes()[0].PointInsideCheck(Point2f(
		myPlayer.GetPosition().x,
		myPlayer.GetPosition().y)) == false)
	{
		myPlayer.SetPosition(myPlayer.GetPreviousPosition());
		myPlayer.SetIsMoving(false);
	}
	for (unsigned short i = 1; i < myCurrentRoom->GetNavMeshes().Size(); i++)
	{
		if (myCurrentRoom->GetNavMeshes()[i].
			PointInsideCheck(Point2f(
			myPlayer.GetPosition().x,
			myPlayer.GetPosition().y)
			) == true
			||
			myCurrentRoom->GetNavMeshes()[i].
			PointInsideCheck(Point2f(
			myTargetPosition.x,
			myTargetPosition.y)
			) == true)
		{
			myPlayer.SetPosition(myPlayer.GetPreviousPosition());
			myPlayer.SetIsMoving(false);
			break;
		}
	}

	myPlayer.Update(myInputManager, myTargetPosition, aTimeDelta);




	return eStateStatus::eKeepState;
}

void CGameWorld::Render(Synchronizer& aSynchronizer)
{
	RenderCommand command;

	if (myCurrentRoom != nullptr)
	{
		for (unsigned int i = 0; i < myCurrentRoom->GetObjectList()->Size(); ++i)
		{
			if ((*myCurrentRoom->GetObjectList())[i]->myName == "Player")
			{
				//RenderPlayer();
			}
			else
			{
				RenderLevel(aSynchronizer, (*myCurrentRoom->GetObjectList())[i]);
			}
		}
	}

	EventManager::GetInstance()->Render(aSynchronizer);

	command.myType = eRenderType::eText;
	command.myPosition = text->myPosition;
	command.myText = text;
	aSynchronizer.AddRenderCommand(command);

	myPlayer.Render(aSynchronizer);

	MouseManager::GetInstance()->Render(aSynchronizer);
}

void CGameWorld::RenderLevel(Synchronizer& aSynchronizer, ObjectData* aNode)
{
	RenderCommand command;
	command.myType = eRenderType::eSprite;
	if (aNode->myActive == true)
	{
		if (aNode->mySprite != nullptr)
		{
			command.myPosition = DX2D::Vector2f(aNode->myX, aNode->myY);
			command.mySprite = aNode->mySprite;
			command.mySprite->SetColor({ 1, 1, 1, 1 });
			aSynchronizer.AddRenderCommand(command);
		}
		for (unsigned int j = 0; j < aNode->myChilds.Size(); ++j)
		{
			RenderLevel(aSynchronizer, aNode->myChilds[j]);
		}
	}
}
