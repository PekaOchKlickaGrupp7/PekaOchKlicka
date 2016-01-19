#pragma once
#include <string>
#include <map>
#include "ObjectData.h"
#include "LevelData.h"
#include "rapidjson\document.h"

class JSON
{
public:
	JSON();
	~JSON();

	bool Load(const std::string& aRootFile);
	bool LoadTestLevel(const std::string& aLevelPath, CommonUtilities::GrowingArray<ObjectData*, unsigned int>& aObjects);
	bool LoadLevel(const std::string& aLevelName, CommonUtilities::GrowingArray<ObjectData*, unsigned int>& aObjects);

	//CommonUtilities::GrowingArray<LevelData, unsigned int> GetLevels() const;

private:
	bool LoadLevel(const std::string& aLevelName, const char* aLevelPath, CommonUtilities::GrowingArray<ObjectData*, unsigned int>& aObjects);
	void LoadObject(rapidjson::Value& node, ObjectData* aParentObject, CommonUtilities::GrowingArray<ObjectData*, unsigned int>& aObjects, float x, float y);
	const char* ReadFile(const char* aFile);

	std::map<std::string, CommonUtilities::GrowingArray<ObjectData*, unsigned int>> myLevels;

	CommonUtilities::GrowingArray<ObjectData*, unsigned int> myCurrentLevel;

	//CommonUtilities::GrowingArray<LevelData, unsigned int> myLevels;
};

