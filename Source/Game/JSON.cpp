#include "stdafx.h"
#include "JSON.h"
#include "rapidjson\document.h"
#include "..\CommonUtilities\DL_Debug.h"
#include <iostream>
#include "tga2d\sprite\sprite.h"

using namespace rapidjson;

JSON::JSON() { }
JSON::~JSON() { }

bool JSON::Load(const std::string& aRootFile)
{
	const char* data = ReadFile(aRootFile.c_str());

	Document root;
	root.Parse(data);
	if (root.HasParseError() == true)
	{
		DL_DEBUG("Failed to load root.json.");
		root.GetAllocator().~MemoryPoolAllocator();
		return false;
	}

	Value& levels = root["levels"];
	if (levels.IsNull() == true)
	{
		DL_DEBUG("Levels is not a member of root file");
		root.GetAllocator().~MemoryPoolAllocator();
		return false;
	}
	for (unsigned int i = 0; i < levels.Capacity(); ++i)
	{
		Value& level = levels[i];
		if (level.IsNull() == true)
		{
			DL_DEBUG("level is null");
			root.GetAllocator().~MemoryPoolAllocator();
			return false;
		}
		
		LevelData data;
		data.myLevelName = level["name"].GetString();
		data.myLevelPath = level["path"].GetString();
		myLevels[level["name"].GetString()].Init(128);

		LoadLevel(level["name"].GetString(), data.myLevelPath.c_str(), myLevels[level["name"].GetString()]);

	//	std::cout << myLevels[myLevels.Size() - 1].myLevelName << std::endl;
	}
	
	root.GetAllocator().Clear();

	delete data;

	return true;
}

bool JSON::LoadTestLevel(const std::string& aLevelPath, CommonUtilities::GrowingArray<ObjectData*, unsigned int>& aObjects)
{
	for (unsigned int i = 0; i < aObjects.Size(); ++i)
	{
		aObjects[i] = nullptr;
	}
	aObjects.RemoveAll();

	LoadLevel(std::string(""), aLevelPath.c_str(), aObjects);

	return true;
}

bool JSON::LoadLevel(const std::string& aLevelName, CommonUtilities::GrowingArray<ObjectData*, unsigned int>& aObjects)
{
	for (unsigned int i = 0; i < aObjects.Size(); ++i)
	{
		aObjects[i] = nullptr;
	}
	aObjects.RemoveAll();

	for (unsigned int i = 0; i < myLevels[aLevelName].Size(); ++i)
	{
		aObjects.Add(myLevels[aLevelName][i]);
	}

	return true;
}

//CommonUtilities::GrowingArray<LevelData, unsigned int> JSON::GetLevels() const
//{
//	return myLevels;
//}

bool JSON::LoadLevel(const std::string& aLevelName, const char* aLevelPath, CommonUtilities::GrowingArray<ObjectData*, unsigned int>& aObjects)
{
	/*bool found = testLevel;
	unsigned int index = 0;
	for (unsigned int i = 0; i < myLevels.Size(); ++i)
	{
		if (myLevels[i].myLevelName == aLevelName)
		{
			index = i;
			found = true;
		}
	}
	if (found == false)
	{
		return false;
	}*/
	for (unsigned int i = 0; i < aObjects.Size(); ++i)
	{
		delete aObjects[i];
	}
	aObjects.RemoveAll();

	const char* data = "";
	/*if (testLevel == true)
	{
		data = ReadFile(aLevelName.c_str());
	}
	else
	{*/
		data = ReadFile(aLevelPath);
	//}

	Document level;
	level.Parse(data);

	if (level.HasParseError() == true)
	{
		DL_DEBUG("Couldn't parse level file");
		level.GetAllocator().~MemoryPoolAllocator();
		return false;
	}

	for (unsigned int i = 0; i < level["objects"].Size(); ++i)
	{
		LoadObject(level["objects"][i], nullptr, aObjects, 0, 0);
	}

	level.GetAllocator().Clear();

	delete data;

	return true;
}

#pragma region Private Methods

void JSON::LoadObject(Value& node, ObjectData* aParentObject, 
	CommonUtilities::GrowingArray<ObjectData*, unsigned int>& aObjects, float x, float y)
{
	Value& object = node;

	ObjectData* dataObject = new ObjectData();

	dataObject->myActive = object["active"].GetBool();

	dataObject->myScaleX = static_cast<float>(object["sx"].GetDouble());
	dataObject->myScaleY = static_cast<float>(object["sy"].GetDouble());
	dataObject->myX = (static_cast<float>(object["x"].GetDouble()) + x) / 1280.0f;
	dataObject->myY = (static_cast<float>(object["y"].GetDouble()) + y) / 720.0f;
	dataObject->myRotation = static_cast<float>(object["rotation"].GetDouble());
	dataObject->myPivotX = static_cast<float>(object["pivotX"].GetDouble());
	dataObject->myPivotY = static_cast<float>(object["pivotY"].GetDouble());

	if (std::string(object["image"].GetString()).size() > 0)
	{
		std::cout << object["image"].GetString() << std::endl;
		dataObject->mySprite = new DX2D::CSprite(object["image"].GetString());
		dataObject->mySprite->SetPivot(DX2D::Vector2f(dataObject->myPivotX, dataObject->myPivotY));
		dataObject->mySprite->SetSize(DX2D::Vector2f(dataObject->myScaleX, dataObject->myScaleY));
		dataObject->mySprite->SetRotation(dataObject->myRotation);
		dataObject->mySprite->SetPosition(DX2D::Vector2f(dataObject->myX, dataObject->myY));
	}
	else
	{
		dataObject->mySprite = nullptr;
	}

	dataObject->myEvents.Init(128);

	HitBox box;
	box.Init(dataObject);
	box.Parse(object["events"]["hitbox"].GetString());

	dataObject->myHitBox = box;
	dataObject->myTriggerType = static_cast<TriggerType>(object["events"]["type"].GetInt());
	dataObject->myTriggerEnabled = object["events"]["enabled"].GetBool();
	dataObject->myRadius = static_cast<float>(object["events"]["radius"].GetDouble());

	Value& events = object["events"]["list"];
	for (unsigned int i = 0; i < events.Size(); ++i)
	{
		Event event;
		event.myType = static_cast<EventTypes>(events[i]["type"].GetInt());
		event.myAction = static_cast<EventActions>(events[i]["action"].GetInt());
		event.myTarget = events[i]["target"].GetString();
		dataObject->myEvents.Add(event);
	}

	ObjectData* parentData = nullptr;
	dataObject->myChilds.Init(12);
	if (aParentObject == nullptr)
	{
		aObjects.Add(dataObject);
		parentData = aObjects[aObjects.Size() - 1];
	}
	else
	{
		aParentObject->myChilds.Add(dataObject);
		parentData = aParentObject->myChilds[aParentObject->myChilds.Size() - 1];
	}

	for (unsigned int j = 0; j < object["childs"].Size(); ++j)
	{
		LoadObject(object["childs"][j], parentData, aObjects, x + static_cast<float>(object["x"].GetDouble()), y + static_cast<float>(object["y"].GetDouble()));
	}
}

const char* JSON::ReadFile(const char* aFile)
{
	std::ifstream input(aFile);

	std::string str((std::istreambuf_iterator<char>(input)),
		std::istreambuf_iterator<char>());

	char* data = new char[str.length() + 1];
	int a = 0;
	for (unsigned int i = 0; i < str.length(); ++i)
	{
		if (str[i] != 0)
		{
			data[a] = str[i];
			a++;
		}
	}

	data[a] = '\0';
	input.close();
	
	return data;
}

#pragma endregion
