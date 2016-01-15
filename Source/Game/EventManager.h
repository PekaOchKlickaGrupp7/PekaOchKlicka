#pragma once
#include <string>
#include "..\CommonUtilities\GrowingArray.h"

class ObjectData;
class Event;

class EventManager
{
public:
	static void CreateInstance()
	{
		if (myInstance == nullptr)
		{
			myInstance = new EventManager();
		}
	}
	static EventManager* const GetInstance()
	{
		return myInstance;
	}
	static void EventManager::DestroyInstance()
	{
		delete myInstance;
		myInstance = nullptr;
	}

	void Update(const float aDeltaTime);
	Event& CreateEvent(ObjectData& aObject, const std::string& aName, const std::string& aEvent);
	Event& GetEvent(const std::string& aName);

private:
	static EventManager* myInstance;

	CommonUtilities::GrowingArray<Event, unsigned int> myEvents;

	EventManager();
	~EventManager();
};
