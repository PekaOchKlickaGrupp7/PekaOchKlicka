#include "stdafx.h"
#include "GameWorld.h"

#include "StateStackProxy.h"
#include "Synchronizer.h"

#include "ResolutionManager.h"

#include "..\CommonUtilities\GrowingArray.h"
#include <iostream>
#include "Game.h"
#include "EventManager.h"
#include "Event.h"
#include "HitBox.h"

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
}

void CGameWorld::Init()
{
	myJson.Load("root.json", myRooms, this);

	/*std::cout << "Level: " << CGame::myTestLevel << std::endl;
	if (CGame::myTestLevel.size() > 0)
	{
		
		myJson.LoadTestLevel(CGame::myTestLevel, myObjects);
	}
	else
	{
	myJson.LoadLevel("Smiley_Face", myObjects);
	}*/

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
	myPlayer.Init("Sprites/Player.dds", DX2D::Vector2f(0.5, 0.5), DX2D::Vector2f(0.5f, 0.5f), 0.01f);

	//Test item
	myTestItem.Init("Sprites/TestItems/GraveShovel.png", "Sprites/TestItems/GraveShovel_inventory.png",
		"Shovel", "A Shovel", DX2D::Vector2f(0.2f, 0.7f), false, "Test Level");
	myTestItem2.Init("Sprites/TestItems/GraveShovel.png", "Sprites/TestItems/GraveShovel_inventory.png",
		"Shovel", "A Shovel", DX2D::Vector2f(0.2f, 0.7f), false, "Test Level");

	myPlayer.AddItemToInventory(myTestItem);
	myPlayer.AddItemToInventory(myTestItem2);
}


eStateStatus CGameWorld::Update(float aTimeDelta)
{
	SoundManager::GetInstance()->Update(static_cast<float>(myTimerManager.GetMasterTimer().GetTimeElapsed().GetMiliseconds()));


	if (myInputManager.KeyPressed(DIK_ESCAPE) == true)
	{
		return eStateStatus::ePopMainState;
	}
	
	static float aSpeed = 0.01f;

	if (myInputManager.KeyPressed(DIK_SPACE) == true)
	{
		/*if (CGame::myTestLevel.size() > 0)
		{
			myJson.LoadTestLevel(CGame::myTestLevel, myObjects);
		}
		else
		{
			myJson.LoadLevel("Test", myObjects);
		}*/
	}



	RECT windowSize;
	GetWindowRect(*DX2D::CEngine::GetInstance()->GetHWND(), &windowSize);

	//std::cout << windowSize.right - windowSize.left << std::endl;

	POINT mousePos = myInputManager.GetMousePos();
	DX2D::Vector2f mousePosition(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

	if (myInputManager.LeftMouseButtonClicked() == true)
	{
		std::cout << Remap(mousePosition.x, 0, 1280, 0, 1280) << ":" << Remap(mousePosition.y, 0, 720, 0, 720) << std::endl;
		CommonUtilities::GrowingArray<ObjectData*, unsigned int>& objects = myCurrentRoom->GetObjectList();
		for (unsigned int i = 0; i < objects.Size(); ++i)
		{
			if (objects[i]->myHitBox.IsMouseColliding(Remap(mousePosition.x, 0, 1280, 0, 1280) / 1280.0f, Remap(mousePosition.y, 0, 720, 0, 720) / 720.0f) == true)
			{
				for (unsigned int j = 0; j < objects[i]->myEvents.Size(); ++j)
				{
					if (objects[i]->myEvents[j]->myType == EventTypes::OnClick)
					{
						EventManager::GetInstance()->AddEvent(objects[i]->myEvents[j]);
					}
				}
				std::cout << "Collided with object" << std::endl;
			}
		}
	}

	DX2D::CEngine::GetInstance()->GetLightManager().SetAmbience(1.0f);

	myPlayer.Update(myInputManager, aTimeDelta);

	return eStateStatus::eKeepState;
}

float CGameWorld::Remap(float value, float from1, float to1, float from2, float to2)
{
	return (value - from1) / (to1 - from1) * (to2 - from2) + from2;
}

void CGameWorld::Render(Synchronizer& aSynchronizer)
{
	RenderCommand command;

	if (myCurrentRoom != nullptr)
	{
		for (unsigned int i = 0; i < myCurrentRoom->GetObjectList().Size(); ++i)
	{
			RenderLevel(aSynchronizer, myCurrentRoom->GetObjectList()[i]);
		}
	}

	command.myType = eRenderType::eSprite;
	command.myPosition = myResolutionTestSprite->GetPosition();
	command.mySprite = myResolutionTestSprite;
	aSynchronizer.AddRenderCommand(command);

	command.myType = eRenderType::eText;
	command.myPosition = text->myPosition;
	command.myText = text;
	aSynchronizer.AddRenderCommand(command);

	myPlayer.Render(aSynchronizer);
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
