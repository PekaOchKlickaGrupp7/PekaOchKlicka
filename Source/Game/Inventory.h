#pragma once
#include "../CommonUtilities/GrowingArray.h"
#include "../CommonUtilities/Macros.h"
#include "Item.h"
#include <tga2d/sprite/sprite.h>

class Synchronizer;

class Inventory
{
public:
	Inventory();
	~Inventory();

	//Initialize the inventory
	void Init(const char* aFilePath);

	//Adds an item to the inventory
	void Add(Item* aItemToAdd);

	//Removes an item from the inventory
	void Remove(Item* aItemToRemove);

	//Update the inventory
	void Update(float aDeltaTime);
	
	//Combine one item with another, generates a new item and removes the originals
	void Combine(Item& aItemToCombine, Item& aItemToCombineWith);

	//Render the inventory through the synchronizer
	void Render(Synchronizer& aSynchronizer);

	//Opens the inventory
	void SetOpen();

	//Closes the inventory
	void SetClose();

	//Get the inventorys background sprite
	const DX2D::CSprite* GetSprite();

	//Check if inventory is open
	bool IsOpen();

	inline DX2D::Vector2f& GetFullyOpenPosition();

private:

	void Open(float aDeltaTime);
	void Close(float aDeltaTime);

	CommonUtilities::GrowingArray<Item*> myContents;

	DX2D::CSprite* myBackground;
	DX2D::Vector2f myPosition;

	DX2D::Vector2f myStartPosition;
	DX2D::Vector2f myEndPosition;

	float myMovementPerFrame;

	bool myIsOpen;
};

DX2D::Vector2f& Inventory::GetFullyOpenPosition()
{
	return myEndPosition;
}

