#pragma once
#include <string>
#include <map>
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
	void AddEvent(Event* aEvent);
	void RemoveAllEvents();
	//void LoadObjects(const CommonUtilities::GrowingArray<ObjectData*, unsigned int>& aObjects);


	//Event& CreateEvent(ObjectData& aObject, const std::string& aName, const std::string& aEvent);
	Event& GetEvent(const std::string& aName);

private:
	static EventManager* myInstance;

	CommonUtilities::GrowingArray<Event*, int> myActiveEvents;

	EventManager();
	~EventManager();
};
