#include "stdafx.h"
#include "Inventory.h"


Inventory::Inventory()
{
	myContents.Init(10);

	myIsOpen = false;
}

Inventory::~Inventory()
{
}

//Adds an item to the inventory
void Inventory::Add(const Item& aItemToAdd)
{
	myContents.Add(aItemToAdd);
}

//Removes an item from the inventory
void Inventory::Remove(const Item& aItemToRemove)
{
	myContents.RemoveCyclic(aItemToRemove);
}

//Combine one item with another
void Inventory::Combine(Item& aItemToCombine, Item& aItemToCombineWith)
{
	if ((aItemToCombine.IsCombinable() == true && aItemToCombineWith.IsCombinable() == true))
	{
		//Compare the first items list of combinable item names with the name of the second item
		for (int i = 0; i < aItemToCombine.GetList().Size(); ++i)
		{
			std::string name = aItemToCombine.GetList()[i];
			if (aItemToCombineWith.GetName() == name)
			{

			}
		}
	}
	else
	{
		//Trigger message to indicate that they cant be combined
	}
}