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
	delete myTextFPS;
	for (std::map<std::string, Room*>::iterator iterator = myRooms.begin(); iterator != myRooms.end(); ++iterator)
	{
		delete iterator->second;
	}
}

void CGameWorld::DoChangeLevel(Room* aCurrentRoom)
{
	myCurrentRoom = aCurrentRoom;
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

void CGameWorld::Init()
{
	std::string name = "";
	myJson.Load("root.json", myRooms, this, name);
	myJson.LoadItems("JSON/items.json", myPlayer.GetInventory());

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

	myTextFPS = new DX2D::CText("Text/calibril.ttf_sdf");
	myTextFPS->myPosition = { 0.5f, 0.05f };
	myTextFPS->myText = "FPS: ";
	myTextFPS->mySize = 0.6f;
	

//Create the player character
//BUG: Why does pivot.x = 0.25 refer to the center
//of myAnimation.mySprite and not 0.5? ~Erik
myPlayer.Init(DX2D::Vector2f(0.5f, 0.8f));
}


eStateStatus CGameWorld::Update(float aTimeDelta)
{
	myTextFPS->myText = "FPS " + std::to_string(myTimerManager.GetMasterTimer().GetFPS());
	myTextFPS->myPosition = { 0.5f - myTextFPS->GetWidth() / 2, 0.05f };

	if (myInputManager.KeyPressed(DIK_ESCAPE) == true)
	{
		return eStateStatus::ePopMainState;
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

	DX2D::CEngine::GetInstance()->GetLightManager().SetAmbience(1.0f);
	EventManager::GetInstance()->Update(aTimeDelta);

	PlayerMovement(aTimeDelta);

	if (myDoQuit == true)
	{
		return eStateStatus::ePopMainState;
	}

	return eStateStatus::eKeepState;
}

void CGameWorld::SetPlayerTargetPosition(Point2f aPoint)
{
	myTargetPosition.x = aPoint.x;
	myTargetPosition.y = aPoint.y;
	myPlayer.SetIsMoving(true);
}

const Vector2f CGameWorld::GetPlayerTargetPosition() const
{
	return Vector2f(myTargetPosition.x, myTargetPosition.y);
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
	MouseManager::GetInstance()->Render(aSynchronizer);
	myTextFPS->Render();
}

void CGameWorld::RenderObject(Synchronizer& aSynchronizer, ObjectData* aNode, float aRelativeX, float aRelativeY)
{
	RenderCommand command;
	command.myType = eRenderType::eSprite;
	if (aNode->myActive == true)
	{
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

void CGameWorld::PlayerMovement(float aTimeDelta)
{
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

	//PATHFINDING
	//Create a vector from player pos to target pos
	CommonUtilities::Intersection::LineSegment2D playerToTarget;
	playerToTarget.myStartPos = Vector2f(myTargetPosition.x, myTargetPosition.y);
	playerToTarget.myEndPos = Vector2f(myPlayer.GetPosition().x, myPlayer.GetPosition().y);

	for (unsigned short i = 1; i < myCurrentRoom->GetNavMeshes().Size(); i++)
	{
		unsigned short j = myCurrentRoom->GetNavMeshes()[i].GetPoints().Size() - 1;
		CommonUtilities::GrowingArray<
			CommonUtilities::Intersection::LineSegment2D> obstacleSides;

		for (unsigned short i = 0; i < myCurrentRoom->GetNavMeshes()[i].GetPoints().Size(); i++)
		{
			CommonUtilities::Intersection::LineSegment2D side;
			side.myStartPos = myCurrentRoom->GetNavMeshes()[i].GetPoints()[i];
			side.myEndPos = myCurrentRoom->GetNavMeshes()[i].GetPoints()[j];
			obstacleSides.Add(side);
			j = i;
		}

		Vector2f intersectionPoint;

		//Create lines for each side of the obstacle and test LineVSLine with target line
		for (unsigned short i = 0; i < obstacleSides.Size(); i++)
		{
			if (CommonUtilities::Intersection::LineVsLine(
				playerToTarget, obstacleSides[i], intersectionPoint) == true)
			{
				for (unsigned short i = 0; i < myCurrentRoom->GetNavMeshes()[i].GetPoints().Size(); i++)
				{
					DX2D::Vector2f targetCandidate =
						DX2D::Vector2f(
						myCurrentRoom->GetNavMeshes()[i].GetPoints()[i].x,
						myCurrentRoom->GetNavMeshes()[i].GetPoints()[i].y)
						- myPlayer.GetPosition();
					Vector2f candidate = Vector2f(targetCandidate.x, targetCandidate.y);
					if (candidate.Length()
						< Vector2f(
						myTargetPosition.x - myPlayer.GetPosition().x,
						myTargetPosition.x - myPlayer.GetPosition().y).Length())
					{
						myTargetPosition = targetCandidate;
					}
				}
				myPlayer.SetIsMoving(true);
				break;
			}
		}
	}

	//Makes sure player can not walk through obstacles
	if (myCurrentRoom->GetNavMeshes().Size() > 0 && myCurrentRoom->GetNavMeshes()[0].PointInsideCheck(Point2f(
		myPlayer.GetPosition().x,
		myPlayer.GetPosition().y)) == false)
	{
		myPlayer.SetPosition(myPlayer.GetPreviousPosition());
		myPlayer.SetIsMoving(false);
	}

	//If character accidentally gets outside the nav mesh move him back inside
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
	for (unsigned int i = 0; i < (*myCurrentRoom->GetObjectList()).Size(); ++i)
	{
		if ((*myCurrentRoom->GetObjectList())[i]->myName == "Player")
		{
			(*myCurrentRoom->GetObjectList())[i]->myX = myPlayer.GetPosition().x;
			(*myCurrentRoom->GetObjectList())[i]->myY = myPlayer.GetPosition().y;
		}
	}
}
