#include "stdafx.h"
#include "EventManager.h"
#include "Event.h"
#include "ObjectData.h"
#include "Room.h"
#include "MouseManager.h"
#include <iostream>
#include "GameWorld.h"
#include "EventVariablesManager.h"
#include "EventRandom.h"

EventManager* EventManager::myInstance = nullptr;

EventManager::EventManager()
{
	myActiveEvents.Init(128);
	myIsSwitchingRoom = false;
	myClicked = false;
	myIsInsideAObject = false;

	myText = new DX2D::CText("Text/PassionOne-Regular.ttf_sdf");
	myText->myPosition = { 0.7f, 0.1f };
	myText->mySize = 0.4f;
}

EventManager::~EventManager()
{
	myActiveEvents.RemoveAll();
}

void EventManager::ChangeRoom(Room* aCurrentRoom)
{
	myCurrentRoom = aCurrentRoom;
	myObjects = aCurrentRoom->GetObjectList();
	myIsSwitchingRoom = true;
}

void EventManager::AddEvent(Event* aEvent)
{
	if (aEvent->myActive == false)
	{
		bool canActivate = true;
		if (aEvent->myObjectData != nullptr)
		{
			if (aEvent->myObjectData->myActive == false)
			{
				canActivate = false;
			}
		}
		if (canActivate == true)
		{
			if (GetRootIsClick(aEvent) == true)
			{
				++aEvent->myObjectData->myAmountActiveEvents;
			}
			aEvent->myActive = true;
			aEvent->Reset();
			myActiveEvents.Add(aEvent);
		}
	}
}

void EventManager::Reset()
{
	RemoveAllEvents();
	myVisitedRooms.clear();
}

bool EventManager::OnEvent(ObjectData* aData, const EventTypes& aType, float aMouseX, float aMouseY, float aRelativeX, float aRelativeY)
{
	if (aData->myActive == true)
	{
		if (aData->myChilds.GetIsInitialized() == true)
		{
			for (int i = aData->myChilds.Size() - 1; i >= 0; --i)
			{
				if (OnEvent(aData->myChilds[i], aType, aMouseX, aMouseY, aRelativeX + aData->myX, aRelativeY + aData->myY) == true)
				{
					return true;
				}
			}
		}

		if (aType == EventTypes::OnClick || aType == EventTypes::OnHover || aType == EventTypes::OnLeave)
		{
			if (aData->myHitBox.IsMouseColliding(MouseManager::GetInstance()->GetPosition().x, MouseManager::GetInstance()->GetPosition().y, aRelativeX, aRelativeY) == true)
			{
				myIsInsideAObject = true;
				if (aType == EventTypes::OnClick)
				{
					myClicked = true;
					if (aData->myName == "Chef")
					{
						std::cout << aData->myAmountActiveEvents << std::endl;
					}
					if (aData->myAmountActiveEvents == 0)
					{
						for (int j = aData->myEvents.Size() - 1; j >= 0; --j)
						{
							if (aData->myEvents[j]->myType == EventTypes::OnClick)
							{
								AddEvent(aData->myEvents[j]);
							}
						}
					}
					return true;
				}
				else if (aData->myIsHovering == false)
				{
					aData->myIsHovering = true;
					for (int j = aData->myEvents.Size() - 1; j >= 0; --j)
					{
						if (aData->myEvents[j]->myType == EventTypes::OnHover)
						{
							AddEvent(aData->myEvents[j]);
						}
					}
					return true;
				}
			}
			else
			{
				if (aType == EventTypes::OnHover && aData->myIsHovering == true)
				{
					aData->myIsHovering = false;
					for (int j = aData->myEvents.Size() - 1; j >= 0; --j)
					{
						if (aData->myEvents[j]->myType == EventTypes::OnLeave)
						{
							AddEvent(aData->myEvents[j]);
						}
					}
					return true;
				}
			}
		}
		else
		{
			for (int j = aData->myEvents.Size() - 1; j >= 0; --j)
			{
				if (aData->myEvents[j]->myType == aType)
				{
					AddEvent(aData->myEvents[j]);
				}
			}
		}
	}
	else if (aType == EventTypes::OnActivated || aType == EventTypes::OnDisabled)
	{
		for (int j = aData->myEvents.Size() - 1; j >= 0; --j)
		{
			if (aData->myEvents[j]->myType == aType)
			{
				AddEvent(aData->myEvents[j]);
			}
		}
	}
	return false;
}

bool EventManager::OnEvent(ObjectData* aData, EventTypes aType)
{
	DX2D::Vector2f& mousePosition = MouseManager::GetInstance()->GetPosition();

	return OnEvent(aData, aType, mousePosition.x, mousePosition.y, 0, 0);
}

bool EventManager::Update(const float aDeltaTime, const bool aTalkIsOn)
{
	DX2D::Vector2f& mousePosition = MouseManager::GetInstance()->GetPosition();

	myClicked = false;
	myIsInsideAObject = false;

	if (myGameWorld->GetPlayer()->GetInventory().GetIsOpen() == false && aTalkIsOn == false)
	{
		if (myGameWorld->GetCinematicMode() == false && myInputManager->LeftMouseButtonClicked() == true)
		{
			for (int i = (*myObjects).Size() - 1; i >= 0; --i)
			{
				if (OnEvent((*myObjects)[i], EventTypes::OnClick, mousePosition.x, mousePosition.y, 0, 0) == true)
				{
					break;
				}
			}
		}

		for (int i = (*myObjects).Size() - 1; i >= 0; --i)
		{
			if (OnEvent((*myObjects)[i], EventTypes::OnHover, mousePosition.x, mousePosition.y, 0, 0) == true)
			{
				break;
			}
		}
	}

	UpdateActiveEvents(aDeltaTime);

	std::string value = "";
	EventVariablesManager::GetInstance()->GetVariable(value, "_SELECTED_ITEM");
	if (value != "" && myGameWorld->GetPlayer()->GetInventory().IsOpen() == false && myInputManager->LeftMouseButtonClicked() == true && myIsInsideAObject == false)
	{
		//myGameWorld->GetPlayer()->GetInventory().DeSelect();
		//value = "";
		//EventVariablesManager::GetInstance()->SetVariable(value, "_PREV_SELECTED_ITEM");
		myClicked = true;
	}

	myText->myText = std::string("Active Events: ") + std::to_string(myActiveEvents.Size());

	if (myIsSwitchingRoom == true)
	{
		myIsSwitchingRoom = false;

		RemoveAllEvents();

		if (myVisitedRooms.find(myCurrentRoom) == myVisitedRooms.end())
		{
			myVisitedRooms[myCurrentRoom] = true;
			for (unsigned int i = 0; i < (*myObjects).Size(); ++i)
			{
				OnEvent((*myObjects)[i], EventTypes::OnFirstLoad, mousePosition.x, mousePosition.y, 0, 0);
			}
		}

		for (unsigned int i = 0; i < (*myObjects).Size(); ++i)
		{
			OnEvent((*myObjects)[i], EventTypes::OnLoad, mousePosition.x, mousePosition.y, 0, 0);
		}

		UpdateActiveEvents(aDeltaTime);
		myGameWorld->DoChangeLevel(myCurrentRoom);
		return false;
	}
	return !myClicked;
}

void EventManager::Render(Synchronizer &aSynchronizer)
{
	for (int i = myActiveEvents.Size() - 1; i >= 0; --i)
	{
		Event* event = myActiveEvents[i];
		event->Render(aSynchronizer);
	}

	RenderCommand command;
	command.myPosition = myText->myPosition;
	command.myText = myText;
	command.myType = eRenderType::eText;
	aSynchronizer.AddRenderCommand(command);
}

void EventManager::UpdateActiveEvents(const float aDeltaTime)
{
	bool doRemoveAnswers = false;
	Event* removeAnswerEvent = nullptr;

	for (int i = myActiveEvents.Size() - 1; i >= 0; --i)
	{
		Event* event = myActiveEvents[i];
		if (event->myAction == EventActions::Answer && doRemoveAnswers == true)
		{
			continue;
		}
		if (event->Update(aDeltaTime) == true)
		{
			bool doRemove = true;
			event->myActive = false;
			if (myActiveEvents.Size() == 0)
			{
				break;
			}
			if (event->myChilds.GetIsInitialized() == true && event->myAutoActivateRecursive == true)
			{
				if (event->myAction == EventActions::Random)
				{
					EventRandom* random = static_cast<EventRandom*>(event);
					unsigned int index = static_cast<unsigned int>(random->myIndex);
					if (index >= 0 && index < event->myChilds.Size())
					{
						AddEvent(event->myChilds[index]);
					}
				}
				else if (event->myAction == EventActions::Answer)
				{
					if (doRemoveAnswers == true)
					{
						continue;
					}
					doRemoveAnswers = true;
					myClicked = true;
					removeAnswerEvent = event;
				}
				else
				{
					for (unsigned int j = 0; j < event->myChilds.Size(); ++j)
					{
						AddEvent(event->myChilds[j]);
					}
				}
			}
			if (GetRootIsClick(event) == true)
			{
				--event->myObjectData->myAmountActiveEvents;
			}
			if (doRemove == true)
			{
				myActiveEvents.RemoveCyclicAtIndex(i);
			}
		}
	}

	if (doRemoveAnswers == true)
	{
		RemoveAllAnswers();
	}

	if (removeAnswerEvent != nullptr)
	{
		for (unsigned int j = 0; j < removeAnswerEvent->myChilds.Size(); ++j)
		{
			AddEvent(removeAnswerEvent->myChilds[j]);
		}
	}
}

bool EventManager::GetRootIsClick(Event* aEvent)
{
	if (aEvent == nullptr)
	{
		return false;
	}
	if (aEvent->myType == EventTypes::OnClick)
	{
		return true;
	}

	return GetRootIsClick(aEvent->myParent);
}

void EventManager::RemoveAllAnswers()
{
	for (int i = myActiveEvents.Size() - 1; i >= 0; --i)
	{
		if (myActiveEvents[i]->myAction == EventActions::Answer)
		{
			myActiveEvents[i]->myActive = false;
			if (GetRootIsClick(myActiveEvents[i]) == true)
			{
				--myActiveEvents[i]->myObjectData->myAmountActiveEvents;
			}
			myActiveEvents.RemoveCyclicAtIndex(i);
		}
	}
}

void EventManager::RemoveAllEvents()
{
	for (int i = 0; i < myActiveEvents.Size(); ++i)
	{
		myActiveEvents[i]->myActive = false;
		myActiveEvents[i]->myObjectData->myAmountActiveEvents = 0;
	}
	myActiveEvents.RemoveAll();
}
