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

	void SetPosition(const DX2D::Vector2f& aPoint);
	void SetPreviousPosition(const DX2D::Vector2f& aPoint);
	void SetSpeed(float aSpeed);
	void AddItemToInventory(Item* aItemToAdd);
	Inventory& GetInventory();

	Animation* GetAnimation() { return myAnimations[myCurentAnimation];};
	DX2D::Vector2f& GetPosition();
	DX2D::Vector2f& GetPreviousPosition();
	void SetAnimation(const int aIndex);

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
	DX2D::Vector2f myPivotPoint;

	float myMovementSpeed;
	float myDepthScaleFactor;

	bool myIsInventoryOpen;

	CGameWorld* myGameWorldPtr;
};

