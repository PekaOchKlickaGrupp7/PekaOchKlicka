#include "stdafx.h"
#include "Inventory.h"
#include "Synchronizer.h"
#include "..\CommonUtilities\Intersection.h"
#include "EventVariablesManager.h"
#include "Options.h"
#include "EventManager.h"
#include "EventTalk.h"
#include "MouseManager.h"
#include "SoundFileHandler.h"

Inventory::Inventory()
{
	myContents.Init(10);
	myIsOpen = false;
	myIsClicked = false;
	myPosition = DX2D::Vector2f(0.0, 1.0);

	myStartPosition = DX2D::Vector2f(0.0, 0.0);
	myEndPosition = DX2D::Vector2f(0.0, 0.0);

	myHoverText = nullptr;

	myMovementPerFrame = 0.0f;
	myXOffset = 0.02f;
	myYOffset = 0.02f;

	myBackground = nullptr;
	myMasterItemList = new ItemList;
	mySelectedItem = nullptr;
	myPreviouslySelectedItem = nullptr;

	eventTalkOnCombine = new EventTalk();
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
	myEndPosition.y = 1.0f - myBackground->GetSize().y;
	myStartPosition.y = 1.0f;
	myPosition.y = myStartPosition.y;
	myMovementPerFrame = 0.3f;
	mySelectedItem = nullptr;
	myPreviouslySelectedItem = nullptr;

	myOptionsPtr = aOptionsPtr;

	myHoverText = new DX2D::CText("Text/PassionOne-Regular.ttf_sdf");

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
	myPreviouslySelectedItem = mySelectedItem;
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
	static bool isInsideOptionsArea = false; // So that hover sound only plays once. /findus

	if (myIsOpen == true)
	{
		Open(aDeltaTime, aInputManager);
	}
	else if (myIsOpen == false)
	{
		Close(aDeltaTime);
	}

	
	if (aInputManager.KeyPressed(DIK_ESCAPE) == true)
	{
		MouseManager::GetInstance()->SetInteractiveMode(eInteractive::eRegular);
		myOptionsPtr->SetActive(!myOptionsPtr->GetActive());
	}


	//Options area in the inventory
	Vector2f topLeft = { 0.86458f, 1 - 0.0833f };
	Vector2f botRight = { 0.97656f, 1 - 0.032407f };

	if (MouseManager::GetInstance()->GetPosition().x >= topLeft.x &&
		MouseManager::GetInstance()->GetPosition().x <= botRight.x &&
		MouseManager::GetInstance()->GetPosition().y >= topLeft.y &&
		MouseManager::GetInstance()->GetPosition().y <= botRight.y)
	{
		if (isInsideOptionsArea == false)
		{
			isInsideOptionsArea = true;

			Sound* SoundPtr = SoundFileHandler::GetInstance()->GetSound("ButtonHover");

			SoundPtr->SetLooping(false);
			SoundPtr->PlaySound();

			MouseManager::GetInstance()->SetInteractiveMode(eInteractive::eActive);
		}
	}
	else
	{
		if (isInsideOptionsArea == true)
		{
			MouseManager::GetInstance()->SetInteractiveMode(eInteractive::eRegular);
		}
		isInsideOptionsArea = false;
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
				else if (mySelectedItem == myContents[i])
				{
					DeSelect();
				}
				else
				{
					mySelectedItem = myContents[i];
					myPreviouslySelectedItem = mySelectedItem;
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
		mySelectedItem = nullptr;
		myPreviouslySelectedItem = nullptr;
		myOptionsPtr->SetActive(!myOptionsPtr->GetActive());

		Sound* SoundPtr = SoundFileHandler::GetInstance()->GetSound("ButtonClick");

		SoundPtr->SetLooping(false);
		SoundPtr->PlaySound();
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
	std::string value = "";
	if (myPreviouslySelectedItem != nullptr)
	{
		value = myPreviouslySelectedItem->GetName();
	}
	EventVariablesManager::GetInstance()->SetVariable(value, "_PREV_SELECTED_ITEM");
	value = "";
	if (mySelectedItem != nullptr)
	{
		value = mySelectedItem->GetName();
	}
	EventVariablesManager::GetInstance()->SetVariable(value, "_SELECTED_ITEM");
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

						myMasterItemList->GetItemList()[j].PlayCombineSound();
						std::string &str = myMasterItemList->GetItemList()[j].GetCombinationText();

						eventTalkOnCombine->myCanBeInterupted = true;
						eventTalkOnCombine->myColor = { 0.78f, 0.85f, 0.68f, 1.0f };
						eventTalkOnCombine->myAction = EventActions::Talk;
						eventTalkOnCombine->myType = EventTypes::OnClick;
						eventTalkOnCombine->mySize = 0.5f;
						eventTalkOnCombine->myText = str;
						eventTalkOnCombine->myTarget = "Player";
						eventTalkOnCombine->myShowTime = 1.0f;
						eventTalkOnCombine->myLetterLength = 0.05f;

						eventTalkOnCombine->Init(EventManager::GetInstance()->GetCurrentRoom(), EventManager::GetInstance()->GetGameWorld());

						EventManager::GetInstance()->AddEvent(eventTalkOnCombine);

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
	DX2D::Vector2f mousePos = MouseManager::GetInstance()->GetPosition();

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

			if (CommonUtilities::Intersection::PointVsRect(
				Vector2<float>(mousePos.x, mousePos.y)
				, Vector2<float>(myContents[i]->GetPosition().x, myContents[i]->GetPosition().y)
				, Vector2<float>(myContents[i]->GetPosition().x + myContents[i]->GetSprite()->GetSize().x
				, myContents[i]->GetPosition().y + myContents[i]->GetSprite()->GetSize().y)))
			{
				myHoverText->myPosition = mousePos;
				myHoverText->mySize = 0.3f;
				myHoverText->myText = myContents[i]->GetDescription();

				command.myType = eRenderType::eText;
				command.myText = myHoverText;
				command.myPosition.x = myContents[i]->GetPosition().x;

				command.myPosition.y = myContents[i]->GetPosition().y - myHoverText->GetHeight();
				aSynchronizer.AddRenderCommand(command);
			}

			command.myType = eRenderType::eSprite;

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
		if (myPosition.y < myEndPosition.y)
		{
			myPosition.y = myEndPosition.y;
		}
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