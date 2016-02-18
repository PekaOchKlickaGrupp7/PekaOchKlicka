#pragma once
#include <string>
#include <map>
#include "..\CommonUtilities\GrowingArray.h"
#include "ResolutionManager.h"

#pragma region Forward Declerations

class ObjectData;
class Event;

class CGameWorld;
class InputManager;
class Room;
enum EventTypes;

#pragma endregion

class EventManager
{
public:
	#pragma region Singleton
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
	#pragma endregion

	void Reset();
	bool OnEvent(ObjectData* aData, EventTypes aType);
	bool OnEvent(ObjectData* aData, const EventTypes& aType, float aMouseX, float aMouseY, float aRelativeX, float aRelativeY);
	void Init(CU::DirectInput::InputManager* aInputManager, CGameWorld* aGameWorld) { myInputManager = aInputManager; myGameWorld = aGameWorld; };
	void ChangeRoom(Room* aCurrentRoom);

	bool Update(const float aDeltaTime, const bool aTalkIsOn);
	void Render(Synchronizer &aSynchronizer);
	void AddEvent(Event* aEvent);
	void RemoveAllEvents();

	Room* GetCurrentRoom() { return myCurrentRoom; }
	CGameWorld* GetGameWorld() { return myGameWorld; }

private:
	static EventManager* myInstance;

	DX2D::CText* myText;

	void UpdateActiveEvents(const float aDeltaTime);
	bool GetRootIsClick(Event* aEvent);
	void RemoveAllAnswers();

	CommonUtilities::GrowingArray<Event*, int> myActiveEvents;

	CGameWorld* myGameWorld;
	CU::DirectInput::InputManager* myInputManager;
	Room* myCurrentRoom;
	CommonUtilities::GrowingArray<ObjectData*, unsigned int> *myObjects;

	std::map<Room*, bool> myVisitedRooms;

	bool myIsSwitchingRoom;
	bool myIsInsideAObject;
	bool myClicked;

	EventManager();
	~EventManager();
};
