#pragma once
#include <string>
#include <map>
#include "..\CommonUtilities\GrowingArray.h"
#include "ResolutionManager.h"

class ObjectData;
class Event;

class InputManager;
class Room;
enum EventTypes;

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

	void OnEvent(ObjectData* aData, const EventTypes& aType, float aMouseX, float aMouseY, float aRelativeX, float aRelativeY);
	void Init(CU::DirectInput::InputManager* aInputManager) { myInputManager = aInputManager; };
	void ChangeRoom(Room* aCurrentRoom);
	float Remap(float value, float from1, float to1, float from2, float to2);


	void Update(const float aDeltaTime);
	void Render(Synchronizer &aSynchronizer);
	void AddEvent(Event* aEvent);
	void RemoveAllEvents();
	//void LoadObjects(const CommonUtilities::GrowingArray<ObjectData*, unsigned int>& aObjects);


	//Event& CreateEvent(ObjectData& aObject, const std::string& aName, const std::string& aEvent);
	Event& GetEvent(const std::string& aName);

private:
	static EventManager* myInstance;

	CommonUtilities::GrowingArray<Event*, int> myActiveEvents;

	CU::DirectInput::InputManager* myInputManager;
	Room* myCurrentRoom;
	CommonUtilities::GrowingArray<ObjectData*, unsigned int> *myObjects;
	bool myIsSwitchingRoom;

	EventManager();
	~EventManager();
};
