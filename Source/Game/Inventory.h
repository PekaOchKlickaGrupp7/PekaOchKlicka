#pragma once
#include "../CommonUtilities/GrowingArray.h"
#include "../CommonUtilities/Macros.h"
#include "Item.h"
#include <tga2d/sprite/sprite.h>
#include "ItemList.h"

class Synchronizer;
class InputManager;
class Options;

class Inventory
{
public:
	Inventory();
	~Inventory();

	//Initialize the inventory
	void Init(const char* aFilePath, Options* aOptionsPtr);

	//Adds an item to the inventory
	void Add(Item* aItemToAdd);
	void Remove(Item* aItemToRemove);
	bool Combine(Item* aItemToCombine, Item* aItemToCombineWith);
	void Clear();

	void RemoveSelectedItem();

	//Update the inventory
	void Update(CU::DirectInput::InputManager& aInputManager, float aDeltaTime);

	//Check where in the inventory the user clicks and trigger appropiate actions
	void OnClick(DX2D::Vector2f& aPointerPosition);

	//On hover over an inventory item
	void OnHover(DX2D::Vector2f& aPointerPosition);

	bool IsClicked();

	//Render the inventory through the synchronizer
	void Render(Synchronizer& aSynchronizer);

	//Opens the inventory
	void SetOpen();

	//Closes the inventory
	void SetClose();

	void DeSelect();

	//Get the inventorys background sprite
	const DX2D::CSprite* GetSprite();

	//Check if inventory is open
	bool IsOpen();

	inline DX2D::Vector2f& GetFullyOpenPosition();

	inline ItemList* GetMasterItemList();

	bool GetIsOpen();

private:

	void Open(float aDeltaTime, CU::DirectInput::InputManager& aInputManager);
	void Close(float aDeltaTime);

	void UpdateSelectedItem();

	ItemList *myMasterItemList;

	CommonUtilities::GrowingArray<Item*, unsigned int> myContents;
	DX2D::CSprite* myBackground;
	DX2D::Vector2f myPosition;
	DX2D::CText* myHoverText;

	Item* mySelectedItem;
	Item* myPreviouslySelectedItem;

	DX2D::Vector2f myStartPosition;
	DX2D::Vector2f myEndPosition;

	Options* myOptionsPtr;

	float myMovementPerFrame;
	float myXOffset;
	float myYOffset;

	bool myIsOpen;
	bool myIsClicked;
};


inline ItemList* Inventory::GetMasterItemList()
{
	return myMasterItemList;
}
DX2D::Vector2f& Inventory::GetFullyOpenPosition()
{
	return myEndPosition;
}
