#include "stdafx.h"
#include "EventManager.h"
#include "Event.h"
#include "ObjectData.h"
#include "Room.h"
#include "MouseManager.h"

EventManager* EventManager::myInstance = nullptr;

EventManager::EventManager()
{
	myActiveEvents.Init(128);
}

EventManager::~EventManager()
{
}

void EventManager::ChangeRoom(Room* aCurrentRoom)
{
	myCurrentRoom = aCurrentRoom;
	myObjects = aCurrentRoom->GetObjectList();
	myLoadingLevel = false;
}

void EventManager::AddEvent(Event* aEvent)
{
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
			for (unsigned int j = 0; j < aData->myEvents.Size(); ++j)
			{
				if (aData->myEvents[j]->myType == aType)
				{
					AddEvent(aData->myEvents[j]);
				}
			}
			//std::cout << "Collided with object" << std::endl;
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

	for (int i = myActiveEvents.Size() - 1; i >= 0; --i)
	{
		Event* event = myActiveEvents[i];
		if (event->Update(aDeltaTime) == true)
		{
			event->Reset();
			myActiveEvents.RemoveCyclicAtIndex(i);
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
