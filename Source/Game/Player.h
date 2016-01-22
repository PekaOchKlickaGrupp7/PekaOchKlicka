#pragma once
#include "Inventory.h"
#include "Animation.h"
class CU::DirectInput::InputManager;

class Player
{
public:
	Player();
	~Player();


	void Init(const char* aSpriteFilePath, DX2D::Vector2f aPosition, 
		DX2D::Vector2f aPivotPoint, float aMovementSpeed);

	void Render(Synchronizer& aSynchronizer);
	void Move(DX2D::Vector2f aTargetPosition, float aMovementSpeed, float aDeltaT);
	void Update(CU::DirectInput::InputManager& aInputManager, float aDeltaT);
	void SetPivot(const DX2D::Vector2f& aPoint);
	void SetPosition(const DX2D::Vector2f& aPoint);
	void SetSpeed(float aSpeed);
	bool GetIsMoving();
	void SetIsMoving(bool aValue);
	void AddItemToInventory(Item* aItemToAdd);
	Inventory GetInventory();

private:

	Inventory myInventory;

	Animation myAnimation;

	DX2D::Vector2f myPosition;
	DX2D::Vector2f myTargetPosition;
	DX2D::Vector2f myRenderPosition;

	float myMovementSpeed;

	bool myIsMoving;
	bool myIsInventoryOpen;
};

