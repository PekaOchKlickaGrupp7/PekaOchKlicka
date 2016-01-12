#pragma once
#include "../CommonUtilities/GrowingArray.h"
#include "Item.h"

class Inventory
{
public:
	Inventory();
	~Inventory();
	void Add(const Item& aItemToAdd);
	void Remove(const Item& aItemToRemove);
	
	//Combine in inventory
	Item& CombineInInventory(const Item& aItemToCombine);

	//Combine in World
	Item& CombineInWorld(const Item& aItemToCombine);

private:
	CommonUtilities::GrowingArray<Item> myInventory;
};

