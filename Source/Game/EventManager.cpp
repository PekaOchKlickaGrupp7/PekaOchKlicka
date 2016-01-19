#include "stdafx.h"
#include "EventManager.h"
#include "Event.h"
#include "ObjectData.h"

EventManager* EventManager::myInstance = nullptr;

EventManager::EventManager()
{
	myActiveEvents.Init(128);
}

EventManager::~EventManager()
{
}

void EventManager::Update(const float aDeltaTime)
{
	for (int i = myActiveEvents.Size() - 1; i >= 0; --i)
	{
		Event* event = myActiveEvents[i];
		event->Update(aDeltaTime);
	}
}

