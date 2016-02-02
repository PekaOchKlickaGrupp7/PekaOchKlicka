#include "stdafx.h"
#include "Inventory.h"
#include "Synchronizer.h"
#include "..\CommonUtilities\Intersection.h"
#include "EventVariablesManager.h"

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
}

Inventory::~Inventory()
{
	SAFE_DELETE(myBackground);
	delete myMasterItemList;
	//myContents.DeleteAll();
}

void Inventory::Init(const char* aFilePath)
{
	myBackground = new DX2D::CSprite(aFilePath);
	myEndPosition.y = 1.0f - myBackground->GetSize().y;
	myStartPosition.y = 1.0f;
	myPosition.y = myStartPosition.y;
	myMovementPerFrame = 0.3f;
	mySelectedItem = nullptr;

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
				UpdateSelectedItem();
				return;
			}
			else
			{
				if (Combine(mySelectedItem, myContents[i]) == true)
				{
					mySelectedItem = nullptr;
					UpdateSelectedItem();
					return;
				}
			}
		}
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