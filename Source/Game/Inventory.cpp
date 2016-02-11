#include "stdafx.h"
#include "Inventory.h"
#include "Synchronizer.h"
#include "..\CommonUtilities\Intersection.h"
#include "EventVariablesManager.h"
#include "Options.h"

Inventory::Inventory()
{
	myContents.Init(10);
	myIsOpen = false;
	myIsClicked = false;
	myPosition = DX2D::Vector2f(0.0, 1.0);

	myStartPosition = DX2D::Vector2f(0.0, 0.0);
	myEndPosition = DX2D::Vector2f(0.0, 0.0);

	myMovementPerFrame = 0.0f;
	myXOffset = 0.02f;
	myYOffset = 0.02f;

	myBackground = nullptr;
	myMasterItemList = new ItemList;
	mySelectedItem = nullptr;
	myPreviouslySelectedItem = nullptr;
}

Inventory::~Inventory()
{
	SAFE_DELETE(myBackground);
	delete myMasterItemList;
	//myContents.DeleteAll();
}

void Inventory::Init(const char* aFilePath, Options* aOptionsPtr)
{
	myBackground = new DX2D::CSprite(aFilePath);
	myEndPosition.y = 1.0f - myBackground->GetSize().y + 0.005f;
	myStartPosition.y = 1.0f;
	myPosition.y = myStartPosition.y;
	myMovementPerFrame = 0.3f;
	mySelectedItem = nullptr;
	myPreviouslySelectedItem = nullptr;

	myOptionsPtr = aOptionsPtr;

	UpdateSelectedItem();
}

void Inventory::Add(Item* aItemToAdd)
{
	myContents.Add(aItemToAdd);
}

void Inventory::Remove(Item* aItemToRemove)
{
	myContents.RemoveCyclic(aItemToRemove);
}

void Inventory::DeSelect()
{
	mySelectedItem = nullptr;
	UpdateSelectedItem();
}

/* Tömmer inventory från ResetGame i GameWorld.cpp /Danne */
void Inventory::Clear()
{
	myContents.RemoveAll();
}

/* Jag la till den här för att den behövs för att ens kunna ta bort items genom editorn /Danne */
void Inventory::RemoveSelectedItem()
{
	Remove(myPreviouslySelectedItem);
	myPreviouslySelectedItem = nullptr;
	DeSelect();
}

//Update the inventory
void Inventory::Update(CU::DirectInput::InputManager& aInputManager, float aDeltaTime)
{
	if (myIsOpen == true)
	{
		Open(aDeltaTime, aInputManager);
	}
	else if (myIsOpen == false)
	{
		Close(aDeltaTime);
	}
}

//Check where in the inventory the user clicks and trigger appropiate actions
void Inventory::OnClick(DX2D::Vector2f& aPointerPosition)
{
	for (unsigned int i = 0; i < myContents.Size(); ++i)
	{
		if (CommonUtilities::Intersection::PointVsRect(
			Vector2<float>(aPointerPosition.x, aPointerPosition.y)
			, Vector2<float>(myContents[i]->GetPosition().x, myContents[i]->GetPosition().y)
			, Vector2<float>(myContents[i]->GetPosition().x + myContents[i]->GetSprite()->GetSize().x
			, myContents[i]->GetPosition().y + myContents[i]->GetSprite()->GetSize().y)))
		{
			if (mySelectedItem == nullptr)
			{
				mySelectedItem = myContents[i];
				myPreviouslySelectedItem = mySelectedItem;
				UpdateSelectedItem();
				return;
			}
			else
			{
				if (Combine(mySelectedItem, myContents[i]) == true)
				{
					mySelectedItem = nullptr;
					myPreviouslySelectedItem = nullptr;
					UpdateSelectedItem();
					return;
				}
			}
		}
	}
	
	//Options area in the inventory
	Vector2f topLeft = { 0.86458f, 1 - 0.0833f };
	Vector2f botRight = { 0.97656f, 1 - 0.032407f };

	if (CommonUtilities::Intersection::PointVsRect(
		Vector2<float>(aPointerPosition.x, aPointerPosition.y), topLeft, botRight) == true)
	{
		myOptionsPtr->SetActive(!myOptionsPtr->GetActive());
	}
}

bool Inventory::GetIsOpen()
{
	if (myOptionsPtr->GetActive() == true || myIsOpen == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Inventory::UpdateSelectedItem()
{
	std::string identifier = "_SELECTED_ITEM";
	std::string value = "";
	if (mySelectedItem != nullptr)
	{
		value = mySelectedItem->GetName();
	}
	EventVariablesManager::GetInstance()->SetVariable(value, identifier);
}

bool Inventory::IsClicked()
{
	return myIsClicked;
}
//Combine one item with another
bool Inventory::Combine(Item* aItemToCombine, Item* aItemToCombineWith)
{
	if ((aItemToCombine->IsCombinable() == true && aItemToCombineWith->IsCombinable() == true))
	{
		for (unsigned int i = 0; i < myContents.Size(); ++i)
		{
			std::string name = aItemToCombine->GetCombinableItem();
			if (aItemToCombineWith->GetName() == name)
			{
				for (unsigned short j = 0; j < myMasterItemList->GetItemList().Size(); ++j)
				{
					if (myMasterItemList->GetItemList()[j].GetName() == aItemToCombine->GetNameOfResultingItem())
					{
						myContents.RemoveCyclicAtIndex(myContents.Find(aItemToCombine));
						myContents.RemoveCyclicAtIndex(myContents.Find(aItemToCombineWith));

						myContents.Add(&myMasterItemList->GetItemList()[j]);
						mySelectedItem = nullptr;
						UpdateSelectedItem();
						return true;
					}
				}
			}
		}
		return false;
	}
	else
	{
		return false;
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

	if (mySelectedItem != nullptr)
	{
		command.mySprite = mySelectedItem->GetSprite();
		command.myPosition = MouseManager::GetInstance()->GetPosition();

		aSynchronizer.AddRenderCommand(command);
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

void Inventory::Open(float aDeltaTime, CU::DirectInput::InputManager& aInputManager)
{
	if (myPosition.y >= myEndPosition.y)
	{
		myPosition.y -= myMovementPerFrame * aDeltaTime;
	}

	if (MouseManager::GetInstance()->GetPosition().y >= myPosition.y &&
		aInputManager.LeftMouseButtonClicked() == true)
	{
		myIsClicked = true;
	}
}

void Inventory::Close(float aDeltaTime)
{
	myIsClicked = false;
	if (myPosition.y <= myStartPosition.y)
	{
		myPosition.y += myMovementPerFrame * aDeltaTime;
	}
	else
	{
		myPosition.y = myStartPosition.y;
	}
}