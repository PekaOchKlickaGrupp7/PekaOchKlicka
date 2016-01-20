#include "stdafx.h"
#include "Player.h"
#include <iostream>

Player::Player()
{
	myPosition = DX2D::Vector2f(0.0, 0.0);
	myTargetPosition = DX2D::Vector2f(0.0, 0.0);
	myMovementSpeed = 1.0f;
	myInventory.Init("Sprites/menu/escMenu/background.dds", DX2D::Vector2f(0.0, 0.0));
	myIsMoving = false;
	myIsInventoryOpen = false;
}


Player::~Player()
{ 
	myAnimation.Destroy();
}

//Initialize the character
void Player::Init(const char* aSpriteFilePath, DX2D::Vector2f aPosition,
	DX2D::Vector2f aPivotPoint, float aMovementSpeed)
{
	myAnimation.Init(aSpriteFilePath, 0.33f,8,4);
	myPosition = aPosition;
	myRenderPosition = aPosition;
	myMovementSpeed = aMovementSpeed;
	myIsMoving = false;
	//mySprite->SetPivot(DX2D::Vector2f(0.5f, 0.5f));
}

//Update the character
void Player::Update(CU::DirectInput::InputManager& aInputManager, float aDeltaT)
{
	//Character movement
	DX2D::Vector2<int> windowSize = ResolutionManager::GetInstance()->GetRenderAreaDimension() +
		ResolutionManager::GetInstance()->GetRenderAreaPosition();
	if (aInputManager.LeftMouseButtonClicked())
	{
		myIsMoving = true;
		myTargetPosition.x = static_cast<float>(aInputManager.GetRelativeMousePos().x)
		/ static_cast<float>(windowSize.x);
		myTargetPosition.y = static_cast<float>(aInputManager.GetRelativeMousePos().y)
			/ static_cast<float>(windowSize.y);

		std::cout << "X: " << myTargetPosition.x << 
			"Y: " << myTargetPosition.y << std::endl;
	}
	if (myIsMoving == true)
	{
		Move(myTargetPosition, myMovementSpeed, aDeltaT);
	}

	//Opening/Closing the inventory
	if (aInputManager.KeyPressed(DIK_I))
	{
		if (myIsInventoryOpen == false)
		{
			myInventory.Open();
			myIsInventoryOpen = true;
		}
		else
		{
			myInventory.Close();
			myIsInventoryOpen = false;
		}
	}
	myAnimation.Update(aDeltaT);
}

//Render everything about the player
void Player::Render(Synchronizer& aSynchronizer)
{
	/*
	RenderCommand command;
	command.mySprite = mySprite;
	command.myPosition = myRenderPosition;
	command.myType = eRenderType::eSprite;

	aSynchronizer.AddRenderCommand(command);
	*/
	myAnimation.Render(aSynchronizer, myRenderPosition);
	myInventory.Render(aSynchronizer);
}

//Move the character
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
		myPosition=DX2D::Vector2f(
			myRenderPosition.x,
			myRenderPosition.y);

		//DRAW DEBUG ARROW
		DX2D::CEngine::GetInstance()->GetDebugDrawer().DrawArrow(
			DX2D::Vector2f(characterPos.x, characterPos.y),
			DX2D::Vector2f(aTargetPosition.x, aTargetPosition.y));
	}
	else
	{
		myIsMoving = false;
	}
}

//Set the pivot point of the character
void Player::SetPivot(const DX2D::Vector2f& aPoint)
{
	//mySprite->SetPivot(aPoint);
}

//Set the characters position
void Player::SetPosition(const DX2D::Vector2f& aPoint)
{
	//mySprite->SetPosition(aPoint);
	myPosition = aPoint;
}

//Set the characters speed
void Player::SetSpeed(float aSpeed)
{
	myMovementSpeed = aSpeed;
}

//Check if the character is moving, returns (true | false)
bool Player::GetIsMoving()
{
	return myIsMoving;
}

//Set if the character should move (true | false)
void Player::SetIsMoving(bool aValue)
{
	myIsMoving = aValue;
}

//Add an item to inventory
void Player::AddItemToInventory(Item* aItemToAdd)
{
	myInventory.Add(aItemToAdd);
}