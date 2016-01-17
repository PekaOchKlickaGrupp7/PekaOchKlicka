#include "stdafx.h"
#include "EventManager.h"
#include "Event.h"
#include "ObjectData.h"

EventManager* EventManager::myInstance = nullptr;

EventManager::EventManager()
{
	myObjects.Init(128);
}

EventManager::~EventManager()
{
}

void EventManager::LoadObjects(const CommonUtilities::GrowingArray<ObjectData*, unsigned int>& aObjects)
{
	myObjects.RemoveAll();
	for (unsigned int i = 0; i < aObjects.Size(); ++i)
	{
		myObjects.Add(aObjects[i]);
	}
}

//Event& EventManager::CreateEvent(ObjectData& aObject, const std::string& aName, const std::string& aEvent)
//{
//	Event event;
//	event.SetName(aName);
//	event.SetType(aEvent);
//	(aEvent);
//
//	myEvents.Add(event);
//
//	return myEvents[myEvents.Size() - 1];
//}

void EventManager::Update(const float aDeltaTime)
{
	for (unsigned int i = 0; i < myObjects.Size(); ++i)
	{
		for (unsigned int j = 0; j < myObjects[i]->myEvents.Size(); ++j)
		{
			Event& event = myObjects[i]->myEvents[j];
			event.Update(aDeltaTime);
		}
	}
}

