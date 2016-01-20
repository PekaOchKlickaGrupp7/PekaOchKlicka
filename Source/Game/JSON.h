#pragma once
#include <string>
#include <map>
#include "rapidjson\document.h"

class Room;
class ObjectData;
class CGameWorld;

enum EventActions
{
	None,
	SetActive
};

class JSON
{
public:
	JSON();
	~JSON();

	bool JSON::Load(const std::string& aRootFile, std::map<std::string, Room*>& aRooms, CGameWorld* aGameWorld);
	
	bool LoadTestLevel(const std::string& aLevelPath, CommonUtilities::GrowingArray<ObjectData*, unsigned int>& aObjects);
	bool LoadLevel(const std::string& aLevelName, CommonUtilities::GrowingArray<ObjectData*, unsigned int>& aObjects);

	//CommonUtilities::GrowingArray<LevelData, unsigned int> GetLevels() const;

private:
	bool LoadLevel(const std::string& aLevelName, const char* aLevelPath, CommonUtilities::GrowingArray<ObjectData*, unsigned int>& aObjects, Room* aRoom, CGameWorld* aGameWorld);
	void LoadObject(rapidjson::Value& node, ObjectData* aParentObject, CommonUtilities::GrowingArray<ObjectData*, unsigned int>& aObjects, Room* aRoom, CGameWorld* aGameWorld, float x, float y);
	const char* ReadFile(const char* aFile);

	CommonUtilities::GrowingArray<ObjectData*, unsigned int> myCurrentLevel;

	//CommonUtilities::GrowingArray<LevelData, unsigned int> myLevels;
};

