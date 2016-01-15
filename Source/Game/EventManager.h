#pragma once
#include <string>
#include "..\CommonUtilities\GrowingArray.h"

class ObjectData;
class Event;

class EventManager
{
public:
	static void CreateInstance();
	static EventManager* const GetInstance();
	static void EventManager::DestroyInstance();

	void Update(const float aDeltaTime);
	Event& CreateEvent(ObjectData& aObject, const std::string& aName, const std::string& aEvent);
	Event& GetEvent(const std::string& aName);

private:
	static EventManager* myInstance;

	CommonUtilities::GrowingArray<Event, unsigned int> myEvents;

	EventManager();
	~EventManager();
};

void EventManager::CreateInstance()
{
	myInstance = new EventManager();
}

EventManager* const EventManager::GetInstance()
{
	return myInstance;
}

void EventManager::DestroyInstance()
{
	delete myInstance;
}

