#include "stdafx.h"
#include "Inventory.h"


Inventory::Inventory()
{
	myInventory.Init(10);
}

Inventory::~Inventory()
{
}

void Inventory::Add(const Item& aItemToAdd)
{
	myInventory.Add(aItemToAdd);
}

void Inventory::Remove(const Item& aItemToRemove)
{
	myInventory.RemoveCyclic(aItemToRemove);
}

Item& Inventory::CombineInInventory(const Item& aItemToCombine)
{
	//CHANGE THIS
	return myInventory[0];
}

Item& Inventory::CombineInWorld(const Item& aItemToCombine)
{
	//CHANGE THIS
	return myInventory[0];
}