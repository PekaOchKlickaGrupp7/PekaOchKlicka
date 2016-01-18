#pragma once
#include "../CommonUtilities/GrowingArray.h"
#include "Item.h"

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

private:
	CommonUtilities::GrowingArray<Item> myContents;

	bool myIsOpen;
};

