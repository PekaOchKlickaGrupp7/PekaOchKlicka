#pragma once
#include <string>
#include <map>
#include "rapidjson\document.h"
#include "EventsFactory.h"

class Room;
class ObjectData;
class CGameWorld;
class Event;
class Inventory;

class JSON
{
public:
	JSON();
	~JSON();

	bool Load(const std::string& aRootFile, std::map<std::string, Room*>& aRooms, CGameWorld* aGameWorld, std::string& aName);
	
	bool LoadLevel(const std::string& aLevelName, CommonUtilities::GrowingArray<ObjectData*, unsigned int>& aObjects);

	bool LoadMusic(const std::string& aMusicFile);

private:
	bool LoadLevel(const char* aLevelPath, CommonUtilities::GrowingArray<ObjectData*, unsigned int>* aObjects, Room* aRoom, CGameWorld* aGameWorld);
	void LoadObject(rapidjson::Value& aNode, ObjectData* aParentObject, CommonUtilities::GrowingArray<ObjectData*, unsigned int>* aObjects, Room* aRoom, CGameWorld* aGameWorld, float aX, float aY);

	void LoadEvent(ObjectData* aNode, Event* aEvent, rapidjson::Value& aParent, Room* aRoom, CGameWorld* aGameWorld);
	void LoadEvent(ObjectData* aNode, rapidjson::Value& aParent, Room* aRoom, CGameWorld* aGameWorld);

	EventsFactory myEventsFactory;

	const char* ReadFile(const char* aFile);
};

