#pragma once
#include <string>
#include <map>
#include "rapidjson\document.h"

class Room;
class ObjectData;
class CGameWorld;
class Event;
class Inventory;

enum EventActions
{
	None,
	SetActive,
	ChangeLevel,
	Talk,
	ChangeCursor,
	PlaySoundFile,
	ChangeImage,
	ChangeToOriginalImage,
	Delay,
	IfVariable,
	StopSound,
	ChangeSoundPosition,
	Quit,
	FadeColor,
	SetColor,
	FadePosition,
	SetPosition,
	ToggleFullscreen
};

class JSON
{
public:
	JSON();
	~JSON();

	bool Load(const std::string& aRootFile, std::map<std::string, Room*>& aRooms, CGameWorld* aGameWorld, std::string& aName);
	
	bool LoadLevel(const std::string& aLevelName, CommonUtilities::GrowingArray<ObjectData*, unsigned int>& aObjects);

	bool LoadItems(const std::string& aRootFile, Inventory aInventory);

private:
	bool LoadLevel(const char* aLevelPath, CommonUtilities::GrowingArray<ObjectData*, unsigned int>* aObjects, Room* aRoom, CGameWorld* aGameWorld);
	void LoadObject(rapidjson::Value& node, ObjectData* aParentObject, CommonUtilities::GrowingArray<ObjectData*, unsigned int>* aObjects, Room* aRoom, CGameWorld* aGameWorld, float x, float y);

	void LoadEvent(ObjectData* aNode, Event* aEvent, rapidjson::Value& aParent, Room* aRoom, CGameWorld* aGameWorld);
	void LoadEvent(ObjectData* aNode, rapidjson::Value& aParent, Room* aRoom, CGameWorld* aGameWorld);
	//void LoadEvent(Event* aNode, rapidjson::Value& aParent, Room* aRoom, CGameWorld* aGameWorld);

	Event* CreateEventData(ObjectData* aData, rapidjson::Value& aParent, Room* aRoom, CGameWorld* aGameWorld);

	const char* ReadFile(const char* aFile);

	CommonUtilities::GrowingArray<ObjectData*, unsigned int> myCurrentLevel;
};

