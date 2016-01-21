#pragma once
#include <string>
#include <map>
#include "rapidjson\document.h"

class Room;
class ObjectData;
class CGameWorld;
class Event;

enum EventActions
{
	None,
	SetActive,
	ChangeLevel
};

class JSON
{
public:
	JSON();
	~JSON();

	bool JSON::Load(const std::string& aRootFile, std::map<std::string, Room*>& aRooms, CGameWorld* aGameWorld);
	
	bool LoadTestLevel(const std::string& aLevelPath, CommonUtilities::GrowingArray<ObjectData*, unsigned int>& aObjects);
	bool LoadLevel(const std::string& aLevelName, CommonUtilities::GrowingArray<ObjectData*, unsigned int>& aObjects);
private:
	bool LoadLevel(const char* aLevelPath, CommonUtilities::GrowingArray<ObjectData*, unsigned int>* aObjects, Room* aRoom, CGameWorld* aGameWorld);
	void LoadObject(rapidjson::Value& node, ObjectData* aParentObject, CommonUtilities::GrowingArray<ObjectData*, unsigned int>* aObjects, Room* aRoom, CGameWorld* aGameWorld, float x, float y);

	void LoadEvent(ObjectData* aNode, rapidjson::Value& aParent, Room* aRoom, CGameWorld* aGameWorld);
	void LoadEvent(Event* aNode, rapidjson::Value& aParent, Room* aRoom, CGameWorld* aGameWorld);

	Event* CreateEventData(rapidjson::Value& aParent, Room* aRoom, CGameWorld* aGameWorld);

	const char* ReadFile(const char* aFile);

	CommonUtilities::GrowingArray<ObjectData*, unsigned int> myCurrentLevel;
};

