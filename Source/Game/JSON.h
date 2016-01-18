#pragma once
#include <string>
#include "ObjectData.h"
#include "LevelData.h"
#include "rapidjson\document.h"

class JSON
{
public:
	JSON();
	~JSON();

	bool Load(const std::string& aRootFile);
	CommonUtilities::GrowingArray<LevelData, unsigned int> GetLevels() const;
	bool LoadLevel(const std::string& aLevelName, CommonUtilities::GrowingArray<ObjectData*, unsigned int>& aObjects, bool testLevel = false);

	void LoadObject(rapidjson::Value& node, ObjectData* aParentObject, 
		CommonUtilities::GrowingArray<ObjectData*, unsigned int>& aObjects, float x, float y);
private:
	const char* ReadFile(const char* aFile);
	
	CommonUtilities::GrowingArray<LevelData, unsigned int> myLevels;
};

