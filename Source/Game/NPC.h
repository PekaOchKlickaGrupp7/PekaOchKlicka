#pragma once
#include "Animation.h"

class NPC
{
public:
	NPC();
	~NPC();

	//Initialize the character
	void Init(const char* aSpriteFilePath, DX2D::Vector2f aPosition,
		DX2D::Vector2f aPivotPoint, float aMovementSpeed);

	//Render everything about the player
	void Render(Synchronizer& aSynchronizer);

	//Move the character
	void Move(DX2D::Vector2f aTargetPosition, float aMovementSpeed, float aDeltaT);

	//Update the character
	void Update(float aDeltaT);

	//Set the pivot point of the character
	void SetPivot(const DX2D::Vector2f& aPoint);

	//Set the characters position
	void SetPosition(const DX2D::Vector2f& aPoint);

	//Set the characters speed
	void SetSpeed(float aSpeed);

	//Check if the character is moving, returns (true | false)
	bool GetIsMoving();

	//Set if the character should move (true | false)
	void SetIsMoving(bool aValue);

private:
	//Animation myAnimation;

	DX2D::Vector2f myPosition;
	DX2D::Vector2f myTargetPosition;
	DX2D::Vector2f myRenderPosition;

	float myMovementSpeed;

	bool myIsMoving;
};

