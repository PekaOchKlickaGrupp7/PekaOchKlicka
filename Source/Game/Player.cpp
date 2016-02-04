#include "stdafx.h"
#include "Player.h"
#include "MouseManager.h"
#include "ResolutionManager.h"
#include <fstream>
#include "..\CommonUtilities\DL_Debug.h"

using namespace rapidjson;

Player::Player()
{
	myPosition = DX2D::Vector2f(0.0, 0.0);
	myMovementSpeed = 1.0f;
	myIsMoving = false;
	myIsInventoryOpen = false;
	myAnimations.Init(10);
	myCurentAnimation = 0;
}


Player::~Player()
{ 
	myAnimations.DeleteAll();
}

void Player::Init(DX2D::Vector2f aPosition)
{
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
	myIsMoving = false;
	myInventory.Init("Sprites/inventory.png");
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
	
	myAnimations.Add(new Animation(path, DX2D::Vector2f(0.5f, 1.0f), animationSpeed, frames, framesPerRow));

	}
}

//Update the character
void Player::Update(CU::DirectInput::InputManager& aInputManager, const DX2D::Vector2f& aTargetPos, float aDeltaT, bool aUpdateInput, bool aMovePlayer)
{
	myPreviousPosition = myPosition;

	//Opening/Closing the inventory
	static float inventoryHoverArea = 1.0f - 0.01f;
	if (myInventory.IsOpen() == false && 
		MouseManager::GetInstance()->GetPosition().y >= inventoryHoverArea && aUpdateInput)
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
	

	myAnimations[myCurentAnimation]->SetSize(myPosition.y * myDepthScaleFactor);
	myAnimations[myCurentAnimation]->Update(aDeltaT);
}

void Player::Render(Synchronizer& aSynchronizer)
{
	myAnimations[myCurentAnimation]->Render(aSynchronizer, myRenderPosition);
	myInventory.Render(aSynchronizer);
}

void Player::Move(DX2D::Vector2f aTargetPosition, float aMovementSpeed, float aDeltaT)
{
	if (myInventory.IsClicked() == true)
	{
		myIsMoving = false;
	}
	//else if (myIsMoving == true)
	//{
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

			////DRAW DEBUG ARROW
			//DX2D::CEngine::GetInstance()->GetDebugDrawer().DrawArrow(
			//	DX2D::Vector2f(characterPos.x, characterPos.y),
			//	DX2D::Vector2f(aTargetPosition.x, aTargetPosition.y));
		}
		else
		{
			myIsMoving = false;
		}
	//}
}

void Player::SetPivot(const DX2D::Vector2f& aPoint)
{
	(aPoint);
	//mySprite->SetPivot(aPoint);
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

bool Player::GetIsMoving()
{
	return myIsMoving;
}

void Player::SetIsMoving(bool aValue)
{
	myIsMoving = aValue;
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