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
	void Init(const char* aFilePath, DX2D::Vector2f aPosition);

	//Adds an item to the inventory
	void Add(Item* aItemToAdd);

	//Removes an item from the inventory
	void Remove(Item* aItemToRemove);
	
	//Combine one item with another, generates a new item and removes the originals
	void Combine(Item& aItemToCombine, Item& aItemToCombineWith);

	//Render the inventory through the synchronizer
	void Render(Synchronizer& aSynchronizer);

	//Opens the inventory
	void Open();

	//Closes the inventory
	void Close();

private:
	CommonUtilities::GrowingArray<Item*> myContents;

	DX2D::CSprite* myBackground;
	DX2D::Vector2f myPosition;

	bool myIsOpen;
};

