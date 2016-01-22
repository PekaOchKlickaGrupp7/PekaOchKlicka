#include "stdafx.h"
#include "Inventory.h"
#include "Synchronizer.h"


Inventory::Inventory()
{
	myContents.Init(10);
	myIsOpen = false;
	myIsClicked = false;
	myPosition = DX2D::Vector2f(0.0, 1.0);

	float myMovementPerFrame = 0.0f;

	myBackground = nullptr;
	myMasterItemList = new ItemList;
}

Inventory::~Inventory()
{
	SAFE_DELETE(myBackground);
	myContents.DeleteAll();
}

void Inventory::Init(const char* aFilePath)
{
	myBackground = new DX2D::CSprite(aFilePath);
}

void Inventory::Add(Item* aItemToAdd)
{
	myContents.Add(aItemToAdd);
}

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

//Check where in the inventory the user clicks and trigger appropiate actions
bool Inventory::OnClick(DX2D::Vector2f& aPointerPosition)
{
	return myIsClicked;
}

//Combine one item with another
void Inventory::Combine(Item* aItemToCombine, Item* aItemToCombineWith)
{
	if ((aItemToCombine->IsCombinable() == true && aItemToCombineWith->IsCombinable() == true))
	{
		for (unsigned int i = 0; i < myContents.Size(); ++i)
		{
			std::string name = aItemToCombine->GetName();
			if (aItemToCombineWith->GetName() == name)
			{
				myContents.RemoveCyclicAtIndex(myContents.Find(aItemToCombine));
				myContents.RemoveCyclicAtIndex(myContents.Find(aItemToCombineWith));

				for (unsigned short j = 0; j < myMasterItemList->GetItemList().Size(); ++j)
				{
					if (myMasterItemList->GetItemList()[j]->GetName() == aItemToCombine->GetNameOfResultingItem())
					{
						myContents.Add(new Item(myMasterItemList->GetItemList()[j]));
						break;
					}
				}
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

	if (myPosition.y <= myStartPosition.y)
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

	if (MouseManager::GetInstance()->GetPosition().y >= myPosition.y)
	{
		myIsClicked = true;
	}
}

void Inventory::Close(float aDeltaTime)
{
	if (myPosition.y <= myStartPosition.y)
	{
		myPosition.y += myMovementPerFrame * aDeltaTime;
	}
}