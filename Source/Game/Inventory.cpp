#include "stdafx.h"
#include "Inventory.h"
#include "Synchronizer.h"


Inventory::Inventory()
{
	myContents.Init(10);

	myIsOpen = false;

	myPosition = DX2D::Vector2f(0.0, 0.0);
	myStartPosition = DX2D::Vector2f(0.0, 0.0);
	myEndPosition = DX2D::Vector2f(0.0, 0.0);

	float myMovementPerFrame = 0.0f;

	myBackground = nullptr;
}

Inventory::~Inventory()
{
	SAFE_DELETE(myBackground);
	myContents.DeleteAll();
}

void Inventory::Init(const char* aFilePath)
{
	myBackground = new DX2D::CSprite(aFilePath);
	myEndPosition.y = 1.0f - myBackground->GetSize().y;
	myStartPosition.y = 1.0f;
	myPosition.y = myStartPosition.y;
	myMovementPerFrame = 0.1f;
}

//Adds an item to the inventory
void Inventory::Add(Item* aItemToAdd)
{
	myContents.Add(aItemToAdd);
}

//Removes an item from the inventory
void Inventory::Remove(Item* aItemToRemove)
{
	myContents.DeleteCyclic(aItemToRemove);
}

//Update the inventory
void Inventory::Update(float aDeltaTime)
{
	if (myIsOpen == true)
	{
		Open(aDeltaTime);
	}
	else if (myIsOpen == false)
	{
		Close(aDeltaTime);
	}
}

//Combine one item with another
void Inventory::Combine(Item& aItemToCombine, Item& aItemToCombineWith)
{
	if ((aItemToCombine.IsCombinable() == true && aItemToCombineWith.IsCombinable() == true))
	{
		//Compare the first items list of combinable item names with the name of the second item
		for (unsigned short i = 0; i < aItemToCombine.GetList().Size(); ++i)
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

	float myXOffset = 0.02f;
	float myYOffset = 0.02f;

	if (myIsOpen == true)
	{
		//Add inventory background to rendering
		aSynchronizer.AddRenderCommand(command);

		//For setting the position of the first item in the inventory
		DX2D::Vector2f tempItemPos = DX2D::Vector2f(myPosition.x + myXOffset, myPosition.y + myYOffset);

		//Render each item in the inventory
		for (unsigned short i = 0; i < myContents.Size(); ++i)
		{
			//Make sure it renders the inventory sprite
			myContents[i]->SetToInventorySprite();

			myContents[i]->SetPosition(tempItemPos);

			//Change the position to the end of the items image + the offset
			DX2D::Vector2f imageSize = myContents[i]->GetSprite()->GetSize();
			tempItemPos.x += imageSize.x + myXOffset;

			//If the image would be rendered outside the inventorys background image size in x-direction
			//Reset the x-position and change the y-position to one row down
			if (tempItemPos.x + imageSize.x + myXOffset >= myPosition.x + myBackground->GetSize().x)
			{
				tempItemPos.x = myPosition.x + myXOffset;
				tempItemPos.y += imageSize.y + myYOffset;
			}
			
			myContents[i]->Render(aSynchronizer);
		}
	}
}

//Opens the inventory
void Inventory::SetOpen()
{
	myIsOpen = true;
}

//Closes the inventory
void Inventory::SetClose()
{
	myIsOpen = false;
}

//Get the inventorys background sprite
const DX2D::CSprite* Inventory::GetSprite()
{
	return myBackground;
}

//Check if inventory is open
bool Inventory::IsOpen()
{
	return myIsOpen;
}

void Inventory::Open(float aDeltaTime)
{
	if (myPosition.y >= myEndPosition.y)
	{
		myPosition.y -= myMovementPerFrame * aDeltaTime;
	}
}

void Inventory::Close(float aDeltaTime)
{
	if (myPosition.y <= myStartPosition.y)
	{
		myPosition.y += myMovementPerFrame * aDeltaTime;
	}
}