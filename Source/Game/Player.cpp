#include "stdafx.h"
#include "Player.h"
#include "MouseManager.h"
#include "ResolutionManager.h"

Player::Player()
{
	myPosition = DX2D::Vector2f(0.0, 0.0);
	myMovementSpeed = 1.0f;
	myIsMoving = false;
	myIsInventoryOpen = false;
}


Player::~Player()
{ 
	myAnimation.Destroy();
}

void Player::Init(const char* aSpriteFilePath, DX2D::Vector2f aPosition,
	DX2D::Vector2f aPivotPoint, float aMovementSpeed)
{
	myAnimation.Init(aSpriteFilePath, aPivotPoint, 0.33f, 4, 4);
	myPosition = aPosition;
	myPreviousPosition = aPosition;
	myRenderPosition = aPosition;
	myMovementSpeed = aMovementSpeed;
	myDepthScaleFactor = 1.5f;
	myIsMoving = false;
	myInventory.Init("Sprites/inventory.png");
}

//Update the character
void Player::Update(CU::DirectInput::InputManager& aInputManager, const DX2D::Vector2f& aTargetPos, float aDeltaT)
{
	myPreviousPosition = myPosition;

	//Opening/Closing the inventory
	static float inventoryHoverArea = 1.0f - 0.01f;
	if (myInventory.IsOpen() == false && 
		MouseManager::GetInstance()->GetPosition().y >= inventoryHoverArea)
	{
		myInventory.SetOpen();
	}

	if (myInventory.IsOpen() == true && 
		MouseManager::GetInstance()->GetPosition().y <
		myInventory.GetFullyOpenPosition().y)
	{
		myInventory.SetClose();
	}

	myInventory.Update(aInputManager, aDeltaT);

	Move(aTargetPos, myMovementSpeed, aDeltaT);


	if (myInventory.IsOpen() == true && MouseManager::GetInstance()->ButtonClicked(eMouseButtons::eLeft))
	{
		myInventory.OnClick(MouseManager::GetInstance()->GetPosition());
	}

	myAnimation.SetSize(myPosition.y * myDepthScaleFactor);
	myAnimation.Update(aDeltaT);
}

void Player::Render(Synchronizer& aSynchronizer)
{
	myAnimation.Render(aSynchronizer, myRenderPosition);
	myInventory.Render(aSynchronizer);
}

void Player::Move(DX2D::Vector2f aTargetPosition, float aMovementSpeed, float aDeltaT)
{
	if (myInventory.IsClicked() == true)
	{
		myIsMoving = false;
	}
	else if (myIsMoving == true)
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

			////DRAW DEBUG ARROW
			//DX2D::CEngine::GetInstance()->GetDebugDrawer().DrawArrow(
			//	DX2D::Vector2f(characterPos.x, characterPos.y),
			//	DX2D::Vector2f(aTargetPosition.x, aTargetPosition.y));
		}
		else
		{
			myIsMoving = false;
		}
	}
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

Inventory Player::GetInventory()
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