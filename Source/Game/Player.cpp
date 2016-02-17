#include "stdafx.h"
#include "Player.h"
#include "MouseManager.h"
#include "ResolutionManager.h"
#include <fstream>
#include <math.h>
#include "..\CommonUtilities\DL_Debug.h"
#include "SoundFileHandler.h"
#include "EventVariablesManager.h"

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

	myMovementSpeed = static_cast<float>(root["movmentSpeed"].GetDouble());


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

//Update the character
void Player::Update(CU::DirectInput::InputManager& aInputManager, const DX2D::Vector2f& aTargetPos, float aDeltaT, bool aUpdateInput, bool aMovePlayer)
{
	if (myGameWorldPtr->PlayerHasReachedTarget() == false)
	{
		if (SoundFileHandler::GetInstance()->GetSound(std::string("Walk"))->IsPlaying() == false)
		{
			SoundFileHandler::GetInstance()->GetSound(std::string("Walk"))->PlaySound();
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

	if (aMovePlayer == true)
	{
		Move(aTargetPos, myMovementSpeed, aDeltaT);
	}

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

	EventVariablesManager::GetInstance()->GetVariable(myDepthScaleFactor, "_DEPTH");

	myAnimations[myCurentAnimation]->SetSize(myPosition.y * myDepthScaleFactor);
	myAnimations[myCurentAnimation]->Update(aDeltaT);
}

void Player::Render(Synchronizer& aSynchronizer)
{
	myAnimations[myCurentAnimation]->SetColor(myColor);
	myAnimations[myCurentAnimation]->Render(aSynchronizer, myRenderPosition);
	myInventory.Render(aSynchronizer);
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