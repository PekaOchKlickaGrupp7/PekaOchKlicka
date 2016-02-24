#include "stdafx.h"
#include "Player.h"
#include "MouseManager.h"
#include "ResolutionManager.h"
#include <fstream>
#include <math.h>
#include "..\CommonUtilities\DL_Debug.h"
#include "..\CommonUtilities\CU.h"
#include "SoundFileHandler.h"
#include "EventVariablesManager.h"
#include <iostream>
#include "..\CommonUtilities\Vector2.h"

using namespace rapidjson;

class Options;
class CGameWorld
{
public:
	bool PlayerHasReachedTarget();
	Options* GetOptions();
};

Player::Player()
{
	myPosition = DX2D::Vector2f(0.0, 0.0);
	myMovementSpeed = 1.0f;
	myIsInventoryOpen = false;
	myAnimations.Init(10);
	myCurentAnimation = 0;
	myPivotPoint = DX2D::Vector2f(0.5f, 0.85f);
}


Player::~Player()
{ 
	myAnimations.DeleteAll();
}

void Player::Init(DX2D::Vector2f aPosition, CGameWorld* aGameWorldPtr)
{
	myGameWorldPtr = aGameWorldPtr;

	const char* data = ReadFile("JSON/Player.json");
	rapidjson::Document root;
	root.Parse(data);

	if (root.HasParseError() == true)
	{
		DL_DEBUG("Failed to load Player.json.");
		root.GetAllocator().~MemoryPoolAllocator();
		return;
	}

	myMinSpeed = static_cast<float>(root["minSpeed"].GetDouble());
	myMaxSpeed = static_cast<float>(root["maxSpeed"].GetDouble());
	myAcceleration = static_cast<float>(root["acceleration"].GetDouble());
	myMovementSpeed = 0;

	rapidjson::Value& animations = root["animation"];

	if (animations.IsNull() == true)
{
		DL_DEBUG("Animation is not a member of Player.json");
		root.GetAllocator().~MemoryPoolAllocator();
		return;
	}

	LoadAnimations(animations);

	delete data;

	myPosition = aPosition;
	myPreviousPosition = aPosition;
	myRenderPosition = aPosition;

	myDepthScaleFactor = 1.5f;
	myMaxY = 1.0f;
	myMinY = 0.65f;
	myMaxScale = 2.0f;
	myMinScale = 0.7f;

	myInventory.Init("Sprites/Inventory/inventory.dds", myGameWorldPtr->GetOptions());

	SoundFileHandler::GetInstance()->SetupStream(std::string("Sound/SoundFX/Walk.ogg"), std::string("Walk"), false);
}

void Player::LoadAnimations(rapidjson::Value& aAnimations)
{
	for (unsigned int i = 0; i < aAnimations.Size(); ++i)
	{
	rapidjson::Value& animation = aAnimations[i];

	const char* path = animation["path"].GetString();

	int frames = animation["frames"].GetInt();
	int framesPerRow = animation["framesPerRow"].GetInt();
	float animationSpeed = static_cast<float>(animation["animationSpeed"].GetDouble());

	myAnimations.Add(new Animation(path, myPivotPoint, animationSpeed, frames, framesPerRow));

	}
}

void Player::SetColor(DX2D::CColor& aColor)
{
	myColor = aColor;
}

float Player::SpeedUp(float aDeltaTime)
{
	if (myMovementSpeed <= myMaxSpeed)
	{
		myMovementSpeed += myAcceleration * aDeltaTime;
	}
	if (myMovementSpeed >= myMaxSpeed)
	{
		myMovementSpeed = myMaxSpeed;
	}

	return myMovementSpeed;
}

float Player::SpeedDown(float, float aDifference)
{
	float diff2 = CU::Remap(aDifference, 0, 0.05f, 0, 1);

	if (myMovementSpeed >= myMinSpeed)
	{
		myMovementSpeed = diff2 * myMaxSpeed;
		//myMovementSpeed -= (myAcceleration * aDeltaTime);
	}
	if (myMovementSpeed <= myMinSpeed)
	{
		myMovementSpeed = myMinSpeed;
	}

	return myMovementSpeed;
}

//Update the character
void Player::Update(CU::DirectInput::InputManager& aInputManager, const DX2D::Vector2f& aTargetPos, const Vector2f& aFinalTargetPos, float aDeltaT, bool aUpdateInput, bool aMovePlayer)
{
	if (myGameWorldPtr->PlayerHasReachedTarget() == false)
	{
		if (SoundFileHandler::GetInstance()->GetSound(std::string("Walk"))->IsPlaying() == false)
		{
			SoundFileHandler::GetInstance()->GetSound(std::string("Walk"))->PlaySound();
			SoundFileHandler::GetInstance()->GetSound(std::string("Walk"))->SetVolume(0.25f);
		}
	}
	else
	{
		if (SoundFileHandler::GetInstance()->GetSound(std::string("Walk"))->IsPlaying() == true)
		{
			SoundFileHandler::GetInstance()->GetSound(std::string("Walk"))->Stop();
		}
	}

	myPreviousPosition = myPosition;

	//Opening/Closing the inventory
	static float inventoryHoverArea = 1.0f - 0.01f;
	if (myInventory.IsOpen() == false && 
		MouseManager::GetInstance()->GetPosition().y >= inventoryHoverArea && aUpdateInput == true)
	{
		myInventory.SetOpen();
	}

	if ((myInventory.IsOpen() == true && 
		MouseManager::GetInstance()->GetPosition().y <
		myInventory.GetFullyOpenPosition().y) || (aUpdateInput == false && myInventory.IsOpen() == true))
	{
		myInventory.SetClose();
	}

	myInventory.Update(aInputManager, aDeltaT);

	PlayApropriateAnimation(aTargetPos, aMovePlayer);

	EventVariablesManager::GetInstance()->GetVariable(myMinY, "_DEPTH_MIN");
	EventVariablesManager::GetInstance()->GetVariable(myMaxY, "_DEPTH_MAX");

	EventVariablesManager::GetInstance()->GetVariable(myMinScale, "_SCALE_MIN");
	EventVariablesManager::GetInstance()->GetVariable(myMaxScale, "_SCALE_MAX");

	float scale = CU::Remap(myPosition.y, myMinY, myMaxY, 0, 1);
	float scaleY = CU::Remap(scale, 0, 1, myMinScale, myMaxScale);
	Vector2<float> v1, v2;
	v1.x = aFinalTargetPos.x; v1.y = aFinalTargetPos.y;
	if (v1.x == -1)
	{
		v1.x = 0;
	}
	if (v1.y == -1)
	{
		v1.y = 0;
	}
	v2.x = myPosition.x; v2.y = myPosition.y;
	float Distance = (v2 - v1).Length();
	if (myMovementSpeed <= 0.0045)
	{
		myMovementSpeed = myMinSpeed;
	}
	if (aMovePlayer == true)
	{
		if (Distance > 0.05f)
		{
			SpeedUp(aDeltaT);
		}
		else
		{
			SpeedDown(aDeltaT, Distance);
		}

		if (Distance < 0.002f)
		{
			myPosition = DX2D::Vector2f(aFinalTargetPos.x, aFinalTargetPos.y);
		}

		

		Move(aTargetPos, myMovementSpeed * (scaleY / 2), aDeltaT);
	}
	std::cout << "speed: " << myMovementSpeed * (scaleY / 2) << std::endl;
	//std::cout << "distance: " << Distance << std::endl;

	if (MouseManager::GetInstance()->ButtonClicked(eMouseButtons::eLeft))
	{
		if (myInventory.IsOpen() == true)
		{
			myInventory.OnClick(MouseManager::GetInstance()->GetPosition());
		}
		else
		{
			myInventory.DeSelect();
		}
	}

	bool hasClass = false;
	EventVariablesManager::GetInstance()->GetVariable(hasClass, "PlayerHasClass");
	if (hasClass == true)
	{
		scaleY *= 1.5f;
	}

	myAnimations[myCurentAnimation]->SetSize(scaleY);
	myAnimations[myCurentAnimation]->Update(aDeltaT);
}

void Player::Render(Synchronizer& aSynchronizer)
{
	myAnimations[myCurentAnimation]->SetColor(myColor);
	myAnimations[myCurentAnimation]->Render(aSynchronizer, myRenderPosition);
	//myInventory.Render(aSynchronizer);
}

void Player::Move(DX2D::Vector2f aTargetPosition, float aMovementSpeed, float aDeltaT)
{
	DX2D::Vector2f characterPos(myPosition);
	//Calculate distance between target and object
	DX2D::Vector2f delta = DX2D::Vector2f(
		characterPos.x - aTargetPosition.x,
		characterPos.y - aTargetPosition.y);
	//Pythagoras to get the vector distance
	float distance = sqrt(powf(delta.x, 2) + (powf(delta.y, 2)));
	if (distance > 0.01f)
	{
		//Divide the X & Y distances with the vector distance to get a normalized direction vector
		delta.Normalize();

		//Move the object
		myRenderPosition.x = characterPos.x - delta.x * aMovementSpeed * aDeltaT;
		myRenderPosition.y = characterPos.y - delta.y * aMovementSpeed * aDeltaT;
			myPosition = DX2D::Vector2f(
			myRenderPosition.x,
			myRenderPosition.y);
		PlayApropriateAnimation(delta,true);
	}
}

void Player::SetPosition(const DX2D::Vector2f& aPoint)
{
	//mySprite->SetPosition(aPoint);
	myPosition = aPoint;
	myRenderPosition = aPoint;
}

void Player::SetPreviousPosition(const DX2D::Vector2f& aPoint)
{
	myPreviousPosition = aPoint;
}

void Player::SetSpeed(float aSpeed)
{
	myMovementSpeed = aSpeed;
}

void Player::AddItemToInventory(Item* aItemToAdd)
{
	myInventory.Add(aItemToAdd);
}

Inventory& Player::GetInventory()
{
	return myInventory;
}

DX2D::Vector2f& Player::GetPosition()
{
	return myPosition;
}

DX2D::Vector2f& Player::GetPreviousPosition()
{
	return myPreviousPosition;
}

void Player::SetAnimation(const int aIndex)
{
	myCurentAnimation = aIndex;
}

void Player::PlayApropriateAnimation(DX2D::Vector2f aTargetPosition,bool aMove)
{
	int resultAnimation = myCurentAnimation;
	bool hasClass = false;
	EventVariablesManager::GetInstance()->GetVariable(hasClass, "PlayerHasClass");

	if (aMove == true)
	{
		if (abs(aTargetPosition.y) > abs(aTargetPosition.x))
		{
			//Upp
			if (aTargetPosition.y > 0)
			{
				resultAnimation = 2;
			}
			//Down
			else
			{
				resultAnimation = 0;
			}
		}
		else
		{
			//Right
			if (aTargetPosition.x < 0)
			{
				resultAnimation = 3;
			}
			//Left
			else
			{
				resultAnimation = 1;
			}
		}
	}
	else
	{
		if (resultAnimation < 4 && hasClass == false
			|| resultAnimation < 12 && hasClass == true)
		{
			resultAnimation += 4;
		}
	}

	if (hasClass == true)
	{
		if (resultAnimation < 8)
		{
			resultAnimation += 8;	
		}
	}
	myCurentAnimation = resultAnimation;
}

const char* Player::ReadFile(const char* aFile)
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