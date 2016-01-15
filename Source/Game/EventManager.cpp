#include "stdafx.h"
#include "EventManager.h"
#include "Event.h"

EventManager* EventManager::myInstance = nullptr;

EventManager::EventManager()
{
	myEvents.Init(128);
}

EventManager::~EventManager()
{
}

Event& EventManager::CreateEvent(ObjectData& aObject, const std::string& aName, const std::string& aEvent)
{
	Event event;
	event.SetName(aName);

	(aEvent);

	myEvents.Add(event);

	return myEvents[myEvents.Size() - 1];
}

void EventManager::Update(const float aDeltaTime)
{
	for (unsigned int i = 0; i < myEvents.Size(); ++i)
	{
		Event& event = myEvents[i];
		event.Update(aDeltaTime);
	}
}

