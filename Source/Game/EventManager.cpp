#include "stdafx.h"
#include "EventManager.h"
#include "Event.h"
#include "ObjectData.h"
#include "Room.h"

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
	myCurrentRoom = aCurrentRoom; myObjects = &myCurrentRoom->GetObjectList();
}

void EventManager::AddEvent(Event* aEvent)
{
	myActiveEvents.Add(aEvent);
}

float EventManager::Remap(float value, float from1, float to1, float from2, float to2)
{
	return (value - from1) / (to1 - from1) * (to2 - from2) + from2;
}

void EventManager::Update(const float aDeltaTime)
{
	POINT mousePos = myInputManager->GetMousePos();
	DX2D::Vector2f mousePosition(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

	if (myInputManager->LeftMouseButtonClicked() == true)
	{
		for (unsigned int i = 0; i < myObjects->Size(); ++i)
		{
			if ((*myObjects)[i]->myHitBox.IsMouseColliding(
				Remap(mousePosition.x, 
				0, 1280, 
				0, 1920) / 1920.0f, 
				Remap(mousePosition.y, 0, 
				1024, 0, 1080) / 1080.0f) == true)
			{
				for (unsigned int j = 0; j < (*myObjects)[i]->myEvents.Size(); ++j)
				{
					if ((*myObjects)[i]->myEvents[j]->myType == EventTypes::OnClick)
					{
						EventManager::GetInstance()->AddEvent((*myObjects)[i]->myEvents[j]);
					}
				}
				//std::cout << "Collided with object" << std::endl;
			}
		}
	}



	for (int i = myActiveEvents.Size() - 1; i >= 0; --i)
	{
		Event* event = myActiveEvents[i];
		if (event->Update(aDeltaTime) == true)
		{
			myActiveEvents.RemoveCyclicAtIndex(i);
		}
	}
}

void EventManager::RemoveAllEvents()
{

}
