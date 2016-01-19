#include "stdafx.h"
#include "Inventory.h"
#include "Synchronizer.h"


Inventory::Inventory()
{
	myContents.Init(10);

	myIsOpen = false;

	myPosition = DX2D::Vector2f(0.0, 0.0);

	myBackground = nullptr;
}

Inventory::~Inventory()
{
	SAFE_DELETE(myBackground);
}

void Inventory::Init(const char* aFilePath, DX2D::Vector2f aPosition)
{
	myBackground = new DX2D::CSprite(aFilePath);
	myPosition = aPosition;
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

//Render the inventory through the synchronizer
void Inventory::Render(Synchronizer& aSynchronizer)
{
	RenderCommand command;
	command.mySprite = myBackground;
	command.myPosition = myPosition;
	command.myType = eRenderType::eSprite;

	if (myIsOpen == true)
	{
		aSynchronizer.AddRenderCommand(command);
	}
}

//Opens the inventory
void Inventory::Open()
{
	myIsOpen = true;
}

//Closes the inventory
void Inventory::Close()
{
	myIsOpen = false;
}