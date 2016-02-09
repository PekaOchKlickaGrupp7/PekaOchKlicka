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
#include "EventVariablesManager.h"


CGameWorld::CGameWorld(StateStackProxy& aStateStackProxy, CU::DirectInput::InputManager& aInputManager, CU::TimeSys::TimerManager& aTimerManager) :
GameState(aStateStackProxy, aInputManager, aTimerManager)
{
	Init();
}

CGameWorld::~CGameWorld()
{
	delete myTextFPS;
	for (std::map<std::string, Room*>::iterator iterator = myRooms.begin(); iterator != myRooms.end(); ++iterator)
	{
		delete iterator->second;
	}
	myRooms.clear();

	myDotSprites.DeleteAll();
}

void CGameWorld::DoChangeLevel(Room* aCurrentRoom)
{
	myCurrentRoom = aCurrentRoom;
	myHasPath = false;
	myHasNewTargetPosition = false;
	myInputManager.Update();
}

void CGameWorld::ChangeLevel(const std::string& aString)
{
	myCurrentRoom = nullptr;
	EventManager::GetInstance()->ChangeRoom(myRooms[aString]);
}

Player* CGameWorld::GetPlayer()
{
	return &myPlayer;
}

void CGameWorld::SetFadeIn(bool aFade)
{
	myDoFadeIn = aFade;
}

void CGameWorld::Init()
{
	std::string name = "";
	myJson.Load("root.json", myRooms, this, name);
	myJson.LoadMusic("JSON/Music.json");
	//myJson.LoadItems("JSON/items.json", myPlayer.GetInventory());

	std::cout << "Level: " << CGame::myTestLevel << std::endl;
	if (CGame::myTestLevel.size() > 0)
	{
		DL_PRINT(CGame::myTestLevel.c_str());
		ChangeLevel(CGame::myTestLevel);
	}
	else
	{
		ChangeLevel(name);
	}

	myDoQuit = false;
	myPlayerCanMove = true;

	myTextFPS = new DX2D::CText("Text/courier.ttf_sdf");
	myTextFPS->myPosition = { 0.5f, 0.05f };
	myTextFPS->myText = "FPS: ";
	myTextFPS->mySize = 0.8f;

	myPlayer.Init(DX2D::Vector2f(0.5f, 0.8f), this);

	myFadeIn = 1.0f;
	myDoFadeIn = false;

	myResTest = new DX2D::CSprite("Sprites/ResolutionTest.dds");
	myShouldRenderDebug = false;
	myShouldRenderFPS = true;
	myShouldRenderNavPoints = true;

	myCurrentWaypoint = 0;
	myHasPath = false;
	myHasNewTargetPosition = false;
	myTargetPosition = { 0.0f, 0.0f };
	myNewTargetPosition = myTargetPosition;

	myDotSprites.Init(12000);
	for (int i = 0; i < 12000; ++i)
	{
		DX2D::CSprite* sprite = new DX2D::CSprite("Sprites/Dot.dds");
		myDotSprites.Add(sprite);
	}
	
	myOptionsMenu.Initialize();
}

eStateStatus CGameWorld::Update(float aTimeDelta)
{
	myTextFPS->myText = "FPS " + std::to_string(myTimerManager.GetMasterTimer().GetFPS());
	myTextFPS->myPosition = { 0.5f - myTextFPS->GetWidth() / 2, 0.05f };

	if (myInputManager.KeyPressed(DIK_ESCAPE) == true)
	{
		return eStateStatus::ePopMainState;
	}

	if (myInputManager.KeyPressed(DIK_F1) == true)
	{
		ResolutionManager::GetInstance()->ToggleFullscreen();
	}

	if (myInputManager.KeyPressed(DIK_F2) == true)
	{
		myShouldRenderDebug = !myShouldRenderDebug;
	}

	if (myInputManager.KeyPressed(DIK_F3) == true)
	{
		myShouldRenderFPS = !myShouldRenderFPS;
	}

	if (myInputManager.KeyPressed(DIK_F4) == true)
	{
		myShouldRenderNavPoints = !myShouldRenderNavPoints;
	}

	if (myInputManager.KeyPressed(DIK_SPACE) == true)
	{
		std::string name = "";
		myJson.Load("root.json", myRooms, this, name);

		if (CGame::myTestLevel.size() > 0)
		{
			DL_PRINT(CGame::myTestLevel.c_str());
			ChangeLevel(CGame::myTestLevel);
		}
		else
		{
			ChangeLevel(name);
		}
	}

	float fadeSpeed = 2.0f;
	if (myDoFadeIn == true)
	{
		myFadeIn -= aTimeDelta * fadeSpeed;
		if (myFadeIn <= 0.0f)
		{
			myFadeIn = 0.0f;
		}
	}
	else
	{
		myFadeIn += aTimeDelta * fadeSpeed;
		if (myFadeIn >= 1.0f)
		{
			myFadeIn = 1.0f;
		}
	}

	DX2D::CEngine::GetInstance()->GetLightManager().SetAmbience(myFadeIn);

	myOptionsMenu.Update(aTimeDelta);

	bool input = EventManager::GetInstance()->Update(aTimeDelta);
	if (myCurrentRoom != nullptr)
	{
		PlayerMovement(input, aTimeDelta);
	}

	if (myDoQuit == true)
	{
		return eStateStatus::ePopMainState;
	}

	return eStateStatus::eKeepState;
}

float CGameWorld::GetFadeIn() const
{
	return myFadeIn;
}

void CGameWorld::SetPlayerTargetPosition(Point2f aPoint)
{
	myNewTargetPosition.x = aPoint.x;
	myNewTargetPosition.y = aPoint.y;
	myHasNewTargetPosition = true;
}

const Vector2f CGameWorld::GetPlayerTargetPosition() const
{
	return Vector2f(myNewTargetPosition.x, myNewTargetPosition.y);
}

bool CGameWorld::PlayerHasReachedTarget()
{
	return !myHasPath;
}

void CGameWorld::SetCinematicMode(bool aOn)
{
	myPlayerCanMove = !aOn;
}

void CGameWorld::Quit()
{
	myDoQuit = true;
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

	bool renderedPlayer = false;

	if (myCurrentRoom != nullptr)
	{
		for (unsigned int i = 0; i < myCurrentRoom->GetObjectList()->Size(); ++i)
		{
			if ((*myCurrentRoom->GetObjectList())[i]->myName == "Player")
			{
				//RenderPlayer();
				if (renderedPlayer == false)
				{
					myPlayer.Render(aSynchronizer);
				}
			}
			else if ((*myCurrentRoom->GetObjectList())[i]->myName == "Layers")
			{
				std::vector<ObjectData*> objects;
				for (unsigned int j = 0; j < (*myCurrentRoom->GetObjectList())[i]->myChilds.Size(); ++j)
				{
					objects.push_back((*myCurrentRoom->GetObjectList())[i]->myChilds[j]);
				}
				std::sort(objects.begin(), objects.end(), [](const ObjectData* aFirstVoxel, const ObjectData* aSecondVoxel)
				{
					return aFirstVoxel->myY < aSecondVoxel->myY;
				});

				for (unsigned int j = 0; j < objects.size(); j++)
				{
					if (objects[j]->myY > myPlayer.GetPosition().y)
					{
						if (renderedPlayer == false)
						{
							myPlayer.Render(aSynchronizer);
							renderedPlayer = true;
						}
					}
					RenderObject(aSynchronizer, objects[j], 0, 0);
				}
			}
			else
			{
				RenderObject(aSynchronizer, (*myCurrentRoom->GetObjectList())[i], 0, 0);
			}
		}
		for (int i = 0; i < myCurrentRoom->GetItemListSize(); ++i)
		{
			myCurrentRoom->GetItem(i)->Render(aSynchronizer);
		}
	}

	EventManager::GetInstance()->Render(aSynchronizer);

	if (myShouldRenderDebug == true)
	{
		RenderCommand resTest;
		resTest.myType = eRenderType::eSprite;
		resTest.mySprite = myResTest;
		aSynchronizer.AddRenderCommand(resTest);
	}
	if (myShouldRenderFPS == true)
	{
		RenderCommand fps;
		fps.myType = eRenderType::eText;
		fps.myPosition = myTextFPS->myPosition;
		fps.myText = myTextFPS;
		aSynchronizer.AddRenderCommand(fps);
	}

	if (myShouldRenderNavPoints == true)
	{
		CommonUtilities::GrowingArray<Node, int>& points = myCurrentRoom->GetNavPoints();
		int gridSize = static_cast<int>(myCurrentRoom->GetGridSize());
		float x = 0;
		float y = 0;

		for (int i = 0; i < points.Size(); ++i)
		{
			if (points[i].GetIsBlocked() == false)
			{
				command.myType = eRenderType::eSprite;
				myDotSprites[i]->SetPivot({ 0, 0 });
				
				if (points[i].GetPath() == true)
				{
					myDotSprites[i]->SetColor(DX2D::CColor(0, 0, 1, 1));
				}
				else if (points[i].myDrawBlue == true)
				{
					myDotSprites[i]->SetColor({ 0, 1, 0, 1 });
				}
				else if (points[i].myDrawRed == true)
				{
					myDotSprites[i]->SetColor({ 1, 0, 0, 1 });
				}
				else
				{
					myDotSprites[i]->SetColor(DX2D::CColor(1, 1, 1, 1));
				}
				command.myPosition = DX2D::Vector2f(x / 1920.0f, y / 1080.0f);
				command.mySprite = myDotSprites[i];
				aSynchronizer.AddRenderCommand(command);
			}
			x += gridSize;
			if (x >= 1920.0f)
			{
				x = 0.0f;
				y += gridSize;
			}
			if (i == points.Size() - 1)
			{
				//std::cout << x << std::endl;
			}
		}
	}
	
	 // if options clicked in inventory
	myOptionsMenu.Render(aSynchronizer);

	MouseManager::GetInstance()->Render(aSynchronizer);
}

void CGameWorld::RenderObject(Synchronizer& aSynchronizer, ObjectData* aNode, float aRelativeX, float aRelativeY)
{
	RenderCommand command;
	command.myType = eRenderType::eSprite;
	if (aNode->myActive == true)
	{
		aNode->myGlobalX = aRelativeX + aNode->myX;
		aNode->myGlobalY = aRelativeY + aNode->myY;
		if (aNode->mySprite != nullptr)
		{
			aNode->mySprite->SetPivot(DX2D::Vector2f(aNode->myPivotX, aNode->myPivotY));
			aNode->mySprite->SetSize(DX2D::Vector2f(aNode->myScaleX, aNode->myScaleY));
			aNode->mySprite->SetRotation(aNode->myRotation);

			command.myPosition = DX2D::Vector2f(aRelativeX + aNode->myX, aRelativeY + aNode->myY);
			command.mySprite = aNode->mySprite;
			command.mySprite->SetColor(aNode->myColor);
			aSynchronizer.AddRenderCommand(command);
		}

		if (aNode->myChilds.GetIsInitialized() == true)
		{
			for (unsigned int j = 0; j < aNode->myChilds.Size(); ++j)
			{
				RenderObject(aSynchronizer, aNode->myChilds[j], aRelativeX + aNode->myX, aRelativeY + aNode->myY); /*aRelativeX + aNode->myX, aRelativeY + aNode->myY);*/
			}
		}
	}
}

void CGameWorld::PlayerMovement(bool aCheckInput, float aTimeDelta)
{
	//Move character if inside nav mesh
	if ((aCheckInput == true && myInputManager.LeftMouseButtonClicked() == true && myPlayerCanMove == true &&
		myPlayer.GetInventory().IsOpen() == false) || myHasNewTargetPosition == true)
	{
		std::string identifier = "_SELECTED_ITEM";
		std::string value = "";
		if (EventVariablesManager::GetInstance()->GetVariable(value, identifier) && value == "" ||
			myHasNewTargetPosition == true)
		{
			DX2D::Vector2f tempTargetPos;
			if (myHasNewTargetPosition == true)
			{
				tempTargetPos = myNewTargetPosition;
			}
			else
			{
				tempTargetPos.x = static_cast<float>(MouseManager::GetInstance()->GetPosition().x);
				tempTargetPos.y = static_cast<float>(MouseManager::GetInstance()->GetPosition().y);
				myNewTargetPosition = tempTargetPos;
			}
			
			myHasNewTargetPosition = false;

			if (tempTargetPos.x > 0 && tempTargetPos.x < 1 && tempTargetPos.y > 0 && tempTargetPos.y < 1)
			{
				//Pathfinding stuff
				if (myPathfinding.FindPath(myCurrentRoom, myPlayer.GetPosition(), tempTargetPos))
				{
					myWaypointNodes = &myPathfinding.GetPath();
					myCurrentWaypoint = 0;
					myTargetPosition.x = -1;
					myTargetPosition.y = -1;
					myHasPath = true;
				}
			}
		}
	}

	if (myHasPath == true && myWaypointNodes->Size() > 0)
	{
		Vector2f playerPosition;
		playerPosition.x = myPlayer.GetPosition().x;
		playerPosition.y = myPlayer.GetPosition().y;

		Vector2f nodePosition;
		nodePosition.x = myTargetPosition.x;
		nodePosition.y = myTargetPosition.y;

		float distanceToNextNode = (nodePosition - playerPosition).Length();

		if (distanceToNextNode < 0.01f)
		{
			(*myWaypointNodes)[myCurrentWaypoint]->SetPath(false);
			++myCurrentWaypoint;
		}

		if (myCurrentWaypoint > myWaypointNodes->Size() - 1)
		{
			//Framme
			myHasPath = false;
		}
		else
		{
			//Gå till nästa node
			myTargetPosition.x = (static_cast<float>((*myWaypointNodes)[myCurrentWaypoint]->GetX()) * myCurrentRoom->GetGridSize()) / 1920.0f;
			myTargetPosition.y = (static_cast<float>((*myWaypointNodes)[myCurrentWaypoint]->GetY()) * myCurrentRoom->GetGridSize()) / 1080.0f;
		}
	}

	myPlayer.Update(myInputManager, myTargetPosition, aTimeDelta, myPlayerCanMove, myHasPath && myWaypointNodes->Size() > 0);

	for (unsigned int i = 0; i < (*myCurrentRoom->GetObjectList()).Size(); ++i)
	{
		if ((*myCurrentRoom->GetObjectList())[i]->myName == "Player")
		{
			DX2D::Vector2f playerPos = myPlayer.GetPosition();
			(*myCurrentRoom->GetObjectList())[i]->myX = playerPos.x;
			(*myCurrentRoom->GetObjectList())[i]->myY = playerPos.y;
			(*myCurrentRoom->GetObjectList())[i]->myGlobalX = playerPos.x;
			(*myCurrentRoom->GetObjectList())[i]->myGlobalY = playerPos.y;
			break;
		}
	}
}
