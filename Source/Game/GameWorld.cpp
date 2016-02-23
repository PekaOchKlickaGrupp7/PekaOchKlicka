#include "stdafx.h"
#include "GameWorld.h"
#include <iostream>

#include "..\CommonUtilities\Intersection.h"
#include "..\CommonUtilities\GrowingArray.h"

#include "StateStackProxy.h"
#include "Synchronizer.h"

#include "ResolutionManager.h"

#include "..\CommonUtilities\Macros.h"
#include "Game.h"
#include "EventManager.h"
#include "HitBox.h"
#include "Room.h"

#include "SoundFileHandler.h"
#include "EventVariablesManager.h"

#include "..\CommonUtilities\Vector3.h"
#include "EventParticleSystem.h"

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
	myTalkIsOn = false;
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

Options* CGameWorld::GetOptions()
{
	return &myOptionsMenu; 
};

void CGameWorld::Init()
{
	Vector3f Vec1({ 1, 0, 0 });
	Vector3f Vec2({ -1, 0, 0 });
	//std::cout << "Angle in rad: " << Vec1.Angle(Vec2) << std::endl;
	//std::cout << "Angle in degrees: " << (Vec1.Angle(Vec2) * (180.0f / 3.14159265359f)) << std::endl;

	std::string name = "";
	unsigned char timer = myTimerManager.CreateTimer();
	myTimerManager.UpdateTimers();

	myJson.Load("root.json", myRooms, this, name);

	myTimerManager.UpdateTimers();
	double delta = myTimerManager.GetTimer(timer).GetTimeElapsed().GetMiliseconds();
	std::cout << "Loading root.json and levels took " << delta << " milliseconds" << std::endl;

	myTimerManager.UpdateTimers();
	myJson.LoadMusic("JSON/Music.json");
	myTimerManager.UpdateTimers();
	delta = myTimerManager.GetTimer(timer).GetTimeElapsed().GetMiliseconds();
	std::cout << "Loading music took " << delta << " milliseconds" << std::endl;
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
	myTalkIsOn = false;
	myPlayerCanMove = true;

	myTextFPS = new DX2D::CText("Text/PassionOne-Regular.ttf_sdf");
	myTextFPS->myPosition = { 0.5f, 0.05f };
	myTextFPS->myText = "FPS: ";
	myTextFPS->mySize = 0.8f;

	myRenderPasses.Init(32);

	myPlayer.Init(DX2D::Vector2f(0.5f, 0.8f), this);

	myFadeIn = 1.0f;
	myDoFadeIn = false;

	myShouldRenderDebug = false;
	myShouldRenderFPS = false;
	myShouldRenderNavPoints = false;
#ifdef _DEBUG
	myShouldRenderFPS = true;
	myShouldRenderNavPoints = true;
#endif

	myCurrentWaypoint = 0;
	myHasPath = false;
	myHasNewTargetPosition = false;
	myTargetPosition = { 0.0f, 0.0f };
	myNewTargetPosition = myTargetPosition;
	
#ifdef _DEBUG
	myDotSprites.Init(12000);
	for (int i = 0; i < 12000; ++i)
	{
		DX2D::CSprite* sprite = new DX2D::CSprite("Sprites/Dot.dds");
		myDotSprites.Add(sprite);
	}
#endif
	
	myOptionsMenu.Initialize();
}

eStateStatus CGameWorld::Update(float aTimeDelta)
{
	myTextFPS->myText = "FPS " + std::to_string(myTimerManager.GetMasterTimer().GetFPS());
	myTextFPS->myPosition = { 0.5f - myTextFPS->GetWidth() / 2, 0.05f };

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
		ChangeLevel("kitchen");
		ResetGame();

		std::cout << "Resetted game" << std::endl;
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

	if (myResettedGame == true)
	{
		myResettedGame = false;
		return eStateStatus::eKeepState;
	}

	myPlayerIsPresent = false;
	myRenderPasses.RemoveAll();

	if (myCurrentRoom != nullptr)
	{
		for (unsigned int i = 0; i < myCurrentRoom->GetObjectList()->Size(); ++i)
		{
			UpdateObject((*myCurrentRoom->GetObjectList())[i], aTimeDelta);
		}
		if (myOptionsMenu.GetActive() == true)
		{
			PlayerMovement(false, false, false, aTimeDelta);
		}
	}
	if (myOptionsMenu.GetActive() == false)
	{
		bool myCachedTalkIsOn = myTalkIsOn;
		bool input = EventManager::GetInstance()->Update(aTimeDelta, myTalkIsOn);
		if (myResettedGame == true)
		{
			return eStateStatus::eKeepState;
		}
		if (myCurrentRoom != nullptr)
		{
			PlayerMovement(input, myCachedTalkIsOn, true, aTimeDelta);
		}
	}
	myOptionsMenu.Update(aTimeDelta);
	std::cout << myRenderPasses.Size() << std::endl;

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

bool CGameWorld::GetCinematicMode() const
{
	return !myPlayerCanMove;
}

void CGameWorld::SetTalkIsOn()
{
	myTalkIsOn = true;
}

void CGameWorld::SetTalkIsOff()
{
	myTalkIsOn = false;
}

void CGameWorld::ResetGame()
{
	SetCinematicMode(false);

	MouseManager::GetInstance()->SetHideGameMouse(false);

	myTalkIsOn = false;

	DX2D::Vector2f pos = GetPlayer()->GetPosition();
	SetPlayerTargetPosition(Point2f(pos.x, pos.y));

	myPlayer.GetInventory().Clear();

	for (std::map<std::string, Room*>::iterator iterator = myRooms.begin(); iterator != myRooms.end(); iterator++)
	{
		std::string name = iterator->second->GetName();
		CommonUtilities::GrowingArray<ObjectData*, unsigned int>& objects = *iterator->second->GetObjectList();
		for (unsigned int i = 0; i < objects.Size(); ++i)
		{
			std::string name = objects[i]->myName;
			objects[i]->ResetToOriginalData();
		}
	}

	DX2D::CColor color = { 1, 1, 1, 1 };
	myPlayer.SetColor(color);

	EventManager::GetInstance()->Reset();
	//ChangeLevel(aTargetLevel);

	myResettedGame = true;
}

void CGameWorld::Quit()
{
	myDoQuit = true;
}

void CGameWorld::Render(Synchronizer& aSynchronizer)
{
	if (myResettedGame == true)
	{
		return;
	}
	
	DX2D::CEngine::GetInstance()->GetLightManager().SetAmbience(myFadeIn);

	RenderCommand command;

	bool renderedPlayer = false;

	EventManager::GetInstance()->PostRender(aSynchronizer);
	
	if (myCurrentRoom != nullptr)
	{
		std::cout << myRenderPasses.Size() << std::endl;

		for (unsigned int i = 0; i < myCurrentRoom->GetObjectList()->Size(); ++i)
		{
			if ((*myCurrentRoom->GetObjectList())[i]->myName == "Player")
			{
				//RenderPlayer();
				if (renderedPlayer == false)
				{
					myPlayer.SetColor((*myCurrentRoom->GetObjectList())[i]->myColor);
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
							for (unsigned int i = 0; i < myCurrentRoom->GetObjectList()->Size(); ++i)
							{
								if ((*myCurrentRoom->GetObjectList())[i]->myName == "Player")
								{
									//RenderPlayer();
									if (renderedPlayer == false)
									{
										myPlayer.SetColor((*myCurrentRoom->GetObjectList())[i]->myColor);
										myPlayer.Render(aSynchronizer);
										renderedPlayer = true;
									}
								}
							}
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

		myPlayer.GetInventory().Render(aSynchronizer);

		if (myShouldRenderFPS == true)
		{
			RenderCommand fps;
			fps.myType = eRenderType::eText;
			fps.myPosition = myTextFPS->myPosition;
			fps.myText = myTextFPS;
			aSynchronizer.AddRenderCommand(fps);
		}

		if (myDotSprites.GetIsInitialized() == true && myShouldRenderNavPoints == true)
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
	
		EventManager::GetInstance()->Render(aSynchronizer);

		 // if options clicked in inventory
		//DX2D::CEngine::GetInstance()->GetLightManager().SetAmbience(1.0f);
		myOptionsMenu.Render(aSynchronizer);

		MouseManager::GetInstance()->Render(aSynchronizer);
	}
}

void CGameWorld::AddParticlePass(std::string& aObjectPass, EventParticleSystem* aParticleSystem)
{
	RenderPass pass;
	pass.myEvent = aParticleSystem;
	pass.myObjectPass = aObjectPass;
	myRenderPasses.Add(pass);
}

void CGameWorld::UpdateObject(ObjectData* aNode, float aDeltaTime)
{
	if (aNode->myActive == true)
	{
		if (aNode->myIsAnimation == true)
		{
			aNode->myAnimations[aNode->myCurrentAnimation]->Update(aDeltaTime);
		}
		else if (aNode->myName == "Player")
		{
			myPlayerIsPresent = true;
		}
		if (aNode->myChilds.GetIsInitialized() == true)
		{
			for (unsigned int i = 0; i < aNode->myChilds.Size(); ++i)
			{
				UpdateObject(aNode->myChilds[i], aDeltaTime);
			}
		}
	}
}

void CGameWorld::RenderObject(Synchronizer& aSynchronizer, ObjectData* aNode, float aRelativeX, float aRelativeY)
{
	RenderCommand command;
	command.myType = eRenderType::eSprite;
	if (aNode == nullptr)
	{
		return;
	}
	if (aNode->myActive == true)
	{
		aNode->myGlobalX = aRelativeX + aNode->myX;
		aNode->myGlobalY = aRelativeY + aNode->myY;

		command.myPosition = DX2D::Vector2f(aRelativeX + aNode->myX, aRelativeY + aNode->myY);

		if (aNode->myIsAnimation == true)
		{
			aNode->myAnimations[aNode->myCurrentAnimation]->Render(aSynchronizer, command.myPosition);
		}
		else
		{
			if (aNode->mySprite != nullptr)
			{
				aNode->mySprite->SetPivot(DX2D::Vector2f(aNode->myPivotX, aNode->myPivotY));
				aNode->mySprite->SetSize(DX2D::Vector2f(aNode->myScaleX, aNode->myScaleY));
				aNode->mySprite->SetRotation(aNode->myRotation);

				command.mySprite = aNode->mySprite;
				command.mySprite->SetColor(aNode->myColor);
				aSynchronizer.AddRenderCommand(command);
			}
		}

		for (int i = myRenderPasses.Size() - 1; i >= 0; --i)
		{
			RenderPass& pass = myRenderPasses[i];
			if (pass.myObjectPass == aNode->myName)
			{
				myRenderPasses[i].myEvent->DoRender(aSynchronizer);
				myRenderPasses.RemoveCyclicAtIndex(i);
			}
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

void CGameWorld::PlayerMovement(bool aCheckInput, bool aTalkIsOn, bool aPlayerCanMove, float aTimeDelta)
{
	//Move character if inside nav mesh
	if ((myPlayer.GetInventory().GetIsOpen() == false && aTalkIsOn == false && aCheckInput == true && myInputManager.LeftMouseButtonClicked() == true && myPlayerCanMove == true &&
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

	if (aPlayerCanMove == true && myHasPath == true && myWaypointNodes->Size() > 0)
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
	else if (myHasPath == true)
	{
		myHasPath = false;
	}

/*	std::cout << std::boolalpha << myPlayerCanMove << std::endl;
	std::cout << std::boolalpha << "MyHasPath " << myHasPath << std::endl << std::endl;*/
	if (myPlayerIsPresent == true)
	{
		myPlayer.Update(myInputManager, myTargetPosition, aTimeDelta, myPlayerCanMove && myTalkIsOn == false, myHasPath && myWaypointNodes->Size() > 0 && myFadeIn == 1.0f);
	}

	for (unsigned int i = 0; i < (*myCurrentRoom->GetObjectList()).Size(); ++i)
	{
		if ((*myCurrentRoom->GetObjectList())[i]->myName == "Player")
		{
			DX2D::Vector2f playerPos = myPlayer.GetPosition();
			playerPos.y -= myPlayer.GetAnimation()->GetSize().y;
			(*myCurrentRoom->GetObjectList())[i]->myX = playerPos.x;
			(*myCurrentRoom->GetObjectList())[i]->myY = playerPos.y;
			(*myCurrentRoom->GetObjectList())[i]->myGlobalX = playerPos.x;
			(*myCurrentRoom->GetObjectList())[i]->myGlobalY = playerPos.y;
			break;
		}
	}
}
