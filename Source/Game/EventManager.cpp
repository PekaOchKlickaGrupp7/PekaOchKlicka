#include "stdafx.h"
#include "EventManager.h"
#include "Event.h"
#include "ObjectData.h"
#include "Room.h"
#include "MouseManager.h"
#include <iostream>

EventManager* EventManager::myInstance = nullptr;

EventManager::EventManager()
{
	myActiveEvents.Init(128);
	myIsSwitchingRoom = false;
}

EventManager::~EventManager()
{
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
		aEvent->myActive = true;
		aEvent->Reset();
		myActiveEvents.Add(aEvent);
	}
}

void EventManager::OnEvent(ObjectData* aData, const EventTypes& aType, float aMouseX, float aMouseY)
{
	if (aData->myActive == true)
	{
		bool trigger = true;
		if (aType == EventTypes::OnHover && aData->myIsHovering == true)
		{
			trigger = true;
		}
		else if (aType == EventTypes::OnLeave && aData->myIsHovering == true)
		{
			trigger = false;
		}

		if (aType != EventTypes::OnLoad && aData->myHitBox.IsMouseColliding(
			MouseManager::GetInstance()->GetPosition().x,
			MouseManager::GetInstance()->GetPosition().y) == true)
		{
			if (trigger == true)
			{
				if (aType == EventTypes::OnHover)
				{
					aData->myIsHovering = true;
				}
				for (unsigned int j = 0; j < aData->myEvents.Size(); ++j)
				{
					if (aData->myEvents[j]->myType == aType)
					{
						AddEvent(aData->myEvents[j]);
					}
				}
			}
		}
		else if (aType == EventTypes::OnLeave && aData->myIsHovering == true)
		{
			aData->myIsHovering = false;
			for (unsigned int j = 0; j < aData->myEvents.Size(); ++j)
			{
				if (aData->myEvents[j]->myType == aType)
				{
					AddEvent(aData->myEvents[j]);
				}
			}
		}
		else if (aType == EventTypes::OnLoad)
		{
			for (unsigned int j = 0; j < aData->myEvents.Size(); ++j)
			{
				if (aData->myEvents[j]->myType == aType)
				{
					AddEvent(aData->myEvents[j]);
				}
			}
		}

		if (aData->myChilds.GetIsInitialized() == true)
		{
			for (unsigned int i = 0; i < aData->myChilds.Size(); ++i)
			{
				OnEvent(aData->myChilds[i], aType, aMouseX, aMouseY);
			}
		}
	}
}

void EventManager::Update(const float aDeltaTime)
{
	DX2D::Vector2f& mousePosition = MouseManager::GetInstance()->GetPosition();

	if (myInputManager->LeftMouseButtonClicked() == true)
	{
		for (unsigned int i = 0; i < (*myObjects).Size(); ++i)
		{
			OnEvent((*myObjects)[i], EventTypes::OnClick, mousePosition.x, mousePosition.y);
		}
	}

	for (unsigned int i = 0; i < (*myObjects).Size(); ++i)
	{
		OnEvent((*myObjects)[i], EventTypes::OnHover, mousePosition.x, mousePosition.y);
		OnEvent((*myObjects)[i], EventTypes::OnLeave, mousePosition.x, mousePosition.y);
	}

	for (int i = myActiveEvents.Size() - 1; i >= 0; --i)
	{
		Event* event = myActiveEvents[i];
		if (event->Update(aDeltaTime) == true)
		{
			event->myActive = false;
			if (event->myChilds.GetIsInitialized() == true)
			{
				for (unsigned int j = 0; j < event->myChilds.Size(); ++j)
				{
					AddEvent(event->myChilds[j]);
				}
			}
			myActiveEvents.RemoveCyclicAtIndex(i);
		}
	}
	if (myIsSwitchingRoom == true)
	{
		myIsSwitchingRoom = false;
		RemoveAllEvents();

		for (unsigned int i = 0; i < (*myObjects).Size(); ++i)
		{
			OnEvent((*myObjects)[i], EventTypes::OnLoad, mousePosition.x, mousePosition.y);
		}
	}
}

void EventManager::Render(Synchronizer &aSynchronizer)
{
	for (int i = myActiveEvents.Size() - 1; i >= 0; --i)
	{
		Event* event = myActiveEvents[i];
		event->Render(aSynchronizer);
	}
}

void EventManager::RemoveAllEvents()
{
	myActiveEvents.RemoveAll();
}
