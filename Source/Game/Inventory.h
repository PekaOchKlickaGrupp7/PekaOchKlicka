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

	//Adds an item to the inventory
	void Add(const Item& aItemToAdd);

	//Removes an item from the inventory
	void Remove(const Item& aItemToRemove);
	
	//Combine one item with another, generates a new item and removes the originals
	void Combine(Item& aItemToCombine, Item& aItemToCombineWith);

	//Render the inventory through the synchronizer
	void Render(Synchronizer& aSynchronizer);

private:
	CommonUtilities::GrowingArray<Item> myContents;

	bool myIsOpen;
	DX2D::CSprite* myBackground;
	DX2D::Vector2f myPosition;
};

