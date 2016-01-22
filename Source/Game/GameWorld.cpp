#include "stdafx.h"
#include "GameWorld.h"
#include <iostream>

#include "..\CommonUtilities\Intersection.h"
#include "..\CommonUtilities\GrowingArray.h"

#include "StateStackProxy.h"
#include "Synchronizer.h"

#include "ResolutionManager.h"

#include "Game.h"
#include "EventManager.h"
#include "HitBox.h"
#include "Room.h"

#include "SoundFileHandler.h"


CGameWorld::CGameWorld(StateStackProxy& aStateStackProxy, CU::DirectInput::InputManager& aInputManager, CU::TimeSys::TimerManager& aTimerManager) :
GameState(aStateStackProxy, aInputManager, aTimerManager)
{
	Init();
}

CGameWorld::~CGameWorld()
{
	delete text;
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
	myJson.LoadItems("items.json", myPlayer.GetInventory());

	std::cout << "Level: " << CGame::myTestLevel << std::endl;
	if (CGame::myTestLevel.size() > 0)
	{
		DL_PRINT(CGame::myTestLevel.c_str());
		ChangeLevel(CGame::myTestLevel);
	}

	text = new DX2D::CText("Text/calibril.ttf_sdf");
	text->myText = "Test";
	text->myPosition = DX2D::Vector2f(0.5f, 0.02f);
	text->myColor.Set(1, 1, 1, 1.0f);
	text->mySize = 0.4f;

	Sound &SFXRain = *SoundFileHandler::GetInstance()->GetSound(eSoundInt(eSound::eRain));
	SFXRain.SetLooping(true);
	SFXRain.Play();


	//Create the player character
	//BUG: Why does pivot.x = 0.25 refer to the center
	//of myAnimation.mySprite and not 0.5? ~Erik
	myPlayer.Init("Sprites/hallBoy.dds", DX2D::Vector2f(0.5f, 0.8f), DX2D::Vector2f(0.25f, 1.0f), 0.2f);
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


		ItemPickUp();

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

	//TEST FOR COMMITING TEST

	//Makes sure player can not walk through obstacles
	if (myCurrentRoom->GetNavMeshes().Size() > 0 && myCurrentRoom->GetNavMeshes()[0].PointInsideCheck(Point2f(
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

void CGameWorld::ItemPickUp()
{
	if (myCurrentRoom != nullptr)
	{
		for (int i = 0; i < myCurrentRoom->GetItemListSize(); ++i)
		{
			if (CommonUtilities::Intersection::PointVsRect(
				Vector2<float>(myTargetPosition.x, myTargetPosition.y)
				, Vector2<float>(myCurrentRoom->GetItem(i)->GetPosition().x, myCurrentRoom->GetItem(i)->GetPosition().y)
				,Vector2<float>(myCurrentRoom->GetItem(i)->GetPosition().x + myCurrentRoom->GetItem(i)->GetSprite()->GetSize().x
				, myCurrentRoom->GetItem(i)->GetPosition().y + myCurrentRoom->GetItem(i)->GetSprite()->GetSize().y)))
			{
				myPlayer.AddItemToInventory(myCurrentRoom->GetItem(i));
				myCurrentRoom->RemoveItem(i);
				return;
			}
		}
	}
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
		for (int i = 0; i < myCurrentRoom->GetItemListSize(); ++i)
		{
			myCurrentRoom->GetItem(i)->Render(aSynchronizer);
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

		if (aNode->myChilds.GetIsInitialized() == true)
		{
			for (unsigned int j = 0; j < aNode->myChilds.Size(); ++j)
			{
				RenderLevel(aSynchronizer, aNode->myChilds[j]);
			}
		}
	}
}
