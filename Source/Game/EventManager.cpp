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
	aEvent->Reset();
	myActiveEvents.Add(aEvent);
}

float EventManager::Remap(float value, float from1, float to1, float from2, float to2)
{
	return (value - from1) / (to1 - from1) * (to2 - from2) + from2;
}

void EventManager::OnEvent(ObjectData* aData, const EventTypes& aType, float aMouseX, float aMouseY)
{
	if (aData->myActive == true)
	{
		bool trigger = true;
		if (aType == EventTypes::OnHover && aData->myIsHovering == true)
		{
			trigger = false;
		}
		else if (aType == EventTypes::OnLeave && aData->myIsHovering == true)
		{
			trigger = false;
		}
		//if (aData->myHitBox.IsMouseColliding(
		//	Remap(aMouseX,
		//	0, 1920,
		//	0, 1920) / 1920.0f,
		//	Remap(aMouseY, 0,
		//	1080, 0, 1080) / 1080.0f) == true)

		if (aData->myHitBox.IsMouseColliding(
			MouseManager::GetInstance()->GetPosition().x,
			MouseManager::GetInstance()->GetPosition().y) == true)
		{
			if (trigger == true)
			{
				if (aType == EventTypes::OnHover)
				{
					aData->myIsHovering = true;
					std::cout << "Collided with object" << std::endl;
				}
				for (unsigned int j = 0; j < aData->myEvents.Size(); ++j)
				{
					if (aData->myEvents[j]->myType == aType)
					{
						AddEvent(aData->myEvents[j]);
					}
				}
				std::cout << "Is inside" << std::endl;
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
			std::cout << "Left object" << std::endl;
		}
	}
	for (unsigned int i = 0; i < aData->myChilds.Size(); ++i)
	{
		OnEvent(aData->myChilds[i], aType, aMouseX, aMouseY);
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
			event->Reset();
			myActiveEvents.RemoveCyclicAtIndex(i);
		}
	}
	if (myIsSwitchingRoom == true)
	{
		myIsSwitchingRoom = false;
		RemoveAllEvents();
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
