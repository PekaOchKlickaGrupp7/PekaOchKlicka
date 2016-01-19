#include "stdafx.h"
#include "Character.h"

Character::Character()
{
	myPosition = DX2D::Vector2f(0.0, 0.0);
	myTargetPosition = DX2D::Vector2f(0.0, 0.0);
	myMovementSpeed = 1.0f;
	myInventory.Init("Sprites/menu/escMenu/background.dds", DX2D::Vector2f(0.0, 0.0));
	mySprite = nullptr;
	myIsMoving = false;
}


Character::~Character()
{ 
	SAFE_DELETE(mySprite);
}

//Initialize the character
void Character::Init(const char* aSpriteFilePath, DX2D::Vector2f aPosition,
	DX2D::Vector2f aPivotPoint, float aMovementSpeed)
{
	mySprite = new DX2D::CSprite(aSpriteFilePath);
	myPosition = aPosition;
	myRenderPosition = aPosition;
	myIsMoving = false;
	mySprite->SetPivot(DX2D::Vector2f(0.5f, 0.5f));
}

//Update the character
void Character::Update(CU::DirectInput::InputManager& aInputManager, float aDeltaT)
{
	DX2D::Vector2ui windowSize = DX2D::CEngine::GetInstance()->GetWindowSize();
	if (aInputManager.LeftMouseButtonClicked())
	{
		myIsMoving = true;
		myTargetPosition.x = static_cast<float>(aInputManager.GetAbsoluteMousePos().x)
			/ static_cast<float>(windowSize.x);
		myTargetPosition.y = static_cast<float>(aInputManager.GetAbsoluteMousePos().y)
			/ static_cast<float>(windowSize.y);
	}
	if (myIsMoving == true)
	{
		Move(myTargetPosition, myMovementSpeed, aDeltaT);
	}
}

//Render everything about the player
void Character::Render(Synchronizer& aSynchronizer)
{
	RenderCommand command;
	command.mySprite = mySprite;
	command.myPosition = myRenderPosition;
	command.myType = eRenderType::eSprite;

	aSynchronizer.AddRenderCommand(command);
	
	myInventory.Render(aSynchronizer);
}

//Move the character
void Character::Move(DX2D::Vector2f aTargetPosition, float aMovementSpeed, float aDeltaT)
{
	DX2D::Vector2f characterPos(mySprite->GetPosition());
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
		mySprite->SetPosition(DX2D::Vector2f(
			myRenderPosition.x,
			myRenderPosition.y));

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
void Character::SetPivot(const DX2D::Vector2f& aPoint)
{
	mySprite->SetPivot(aPoint);
}

//Set the characters position
void Character::SetPosition(const DX2D::Vector2f& aPoint)
{
	mySprite->SetPosition(aPoint);
}

//Set the characters speed
void Character::SetSpeed(float aSpeed)
{
	myMovementSpeed = aSpeed;
}

//Check if the character is moving, returns (true | false)
bool Character::GetIsMoving()
{
	return myIsMoving;
}

//Set if the character should move (true | false)
void Character::SetIsMoving(bool aValue)
{
	myIsMoving = aValue;
}