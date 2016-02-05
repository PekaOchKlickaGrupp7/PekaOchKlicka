#pragma once
#include "Inventory.h"
#include "Animation.h"
#include "rapidjson\document.h"

class CU::DirectInput::InputManager;

class CGameWorld;

class Player
{
public:
	Player();
	~Player();

	void Init(DX2D::Vector2f aPosition, CGameWorld* aGameWorldPtr);

	void Render(Synchronizer& aSynchronizer);
	void Move(DX2D::Vector2f aTargetPosition, float aMovementSpeed, float aDeltaT);

	//Update the character
	void Update(CU::DirectInput::InputManager& aInputManager, const DX2D::Vector2f& aTargetPos, float aDeltaT, bool aUpdateInput, bool aMovePlayer = true);

	//Set the pivot point of the character
	void SetPivot(const DX2D::Vector2f& aPoint);
	void SetPosition(const DX2D::Vector2f& aPoint);
	void SetPreviousPosition(const DX2D::Vector2f& aPoint);
	void SetSpeed(float aSpeed);
	bool GetIsMoving();
	void SetIsMoving(bool aValue);
	void AddItemToInventory(Item* aItemToAdd);
	Inventory& GetInventory();

	DX2D::Vector2f& GetPosition();
	DX2D::Vector2f& GetPreviousPosition();

private:
	void LoadAnimations(rapidjson::Value& aAnimations);
	const char* ReadFile(const char* aFile);
	void PlayApropriateAnimation(DX2D::Vector2f aTargetPosition);

	Inventory myInventory;

	CommonUtilities::GrowingArray<Animation*, int> myAnimations;
	int myCurentAnimation;
	DX2D::Vector2f myPosition;
	DX2D::Vector2f myPreviousPosition;
	DX2D::Vector2f myRenderPosition;

	float myMovementSpeed;
	float myDepthScaleFactor;

	bool myIsMoving;
	bool myIsInventoryOpen;

	CGameWorld* myGameWorldPtr;
};

