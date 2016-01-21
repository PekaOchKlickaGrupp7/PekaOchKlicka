#include "stdafx.h"
#include "JSON.h"
#include "rapidjson\document.h"
#include "..\CommonUtilities\DL_Debug.h"
#include <iostream>
#include "tga2d\sprite\sprite.h"
#include <map>
#include "Room.h"
#include "GameWorld.h"
#include "Item.h"

//Events
#include "EventNone.h"
#include "EventSetActive.h"
#include "EventChangeLevel.h"

using namespace rapidjson;

JSON::JSON() { }
JSON::~JSON() { }

bool JSON::Load(const std::string& aRootFile, std::map<std::string, Room*>& aRooms, CGameWorld* aGameWorld)
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
	levels = levels["$values"];

	std::string levelName = "";

	for (unsigned int i = 0; i < levels.Size(); ++i)
	{
		Value& level = levels[i];
		if (level.IsNull() == true)
		{
			DL_DEBUG("level is null");
			root.GetAllocator().~MemoryPoolAllocator();
			return false;
		}
		
		Room* room = new Room();
		room->GetObjectList()->Init(128);

		std::string name = level["name"].GetString();
		aRooms[name] = room;
		
		if (i == 0)
		{
			levelName = name;
		}

		LoadLevel(level["path"].GetString(), room->GetObjectList(), room, aGameWorld);
	}
	
	root.GetAllocator().Clear();

	aGameWorld->ChangeLevel(levelName);

	delete data;

	return true;
}

bool JSON::LoadLevel(const char* aLevelPath, CommonUtilities::GrowingArray<ObjectData*, unsigned int>* aObjects, Room* aRoom, CGameWorld* aGameWorld)
{
	const char* data = ReadFile(aLevelPath);
	Document level;
	level.Parse(data);

	if (level.HasParseError() == true)
	{
		DL_DEBUG("Couldn't parse level file");
		level.GetAllocator().~MemoryPoolAllocator();
		return false;
	}

	for (unsigned int i = 0; i < level["objects"]["$values"].Size(); ++i)
	{
		LoadObject(level["objects"]["$values"][i], nullptr, aObjects, aRoom, aGameWorld, 0, 0);
	}

	if (level.HasMember("walkable_areas") == true)
	{
		for (unsigned int i = 0; i < level["walkable_areas"]["$values"].Size(); ++i)
		{
			Value& points = level["walkable_areas"]["$values"][i]["points"]["$values"];
			NavPolygon poly;
			for (unsigned int j = 0; j < points.Size(); j++)
			{
				poly.AddPoint(Point2f(static_cast<float>(points[j]["x"].GetDouble()) / 1920.0f, static_cast<float>(points[j]["y"].GetDouble()) / 1080.0f));
			}
			aRoom->AddNavPolygon(poly);
		}
	}

	level.GetAllocator().Clear();

	delete data;

	return true;
}

#pragma region Private Methods

void JSON::LoadObject(Value& node, ObjectData* aParentObject,
	CommonUtilities::GrowingArray<ObjectData*, unsigned int>* aObjects, Room* aRoom, CGameWorld* aGameWorld, float x, float y)
{
	Value& object = node;

	ObjectData* dataObject = new ObjectData();

	dataObject->myActive = object["active"].GetBool();
	dataObject->myName = object["name"].GetString();

	dataObject->myScaleX = static_cast<float>(object["sx"].GetDouble());
	dataObject->myScaleY = static_cast<float>(object["sy"].GetDouble());
	dataObject->myX = (static_cast<float>(object["x"].GetDouble()) + x) / 1920.0f;
	dataObject->myY = (static_cast<float>(object["y"].GetDouble()) + y) / 1080.0f;
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

	Value& events = object["events"]["list"]["$values"];
	for (unsigned int i = 0; i < events.Size(); ++i)
	{
		EventActions action = static_cast<EventActions>(events[i]["action"].GetInt());
		Event* event = nullptr;
		switch (action)
		{
		case EventActions::SetActive:
		{
			EventSetActive* setActive = new EventSetActive();
			setActive->Init(aRoom, aGameWorld);

			Value& extra = events[i]["extra"];
			if (extra.HasMember("value") == true)
			{
				Value& myValue = extra["value"];
				if (myValue.IsNull() == true)
				{
					DL_ASSERT("Event SetActive Value is null");
				}
				setActive->myValue = myValue.GetBool();
			}

			//dataObject->myEvents.Add(setActive);
			event = dynamic_cast<Event*>(setActive);
			break;
		}
		case EventActions::ChangeLevel:
		{
			EventChangeLevel* changeLevel = new EventChangeLevel();
			changeLevel->Init(aRoom, aGameWorld);
			//changeLevel->myTargetLevelName = events[i]["CHANGE_LEVEL_TargetScene"].GetString();

			//changeLevel->myType = static_cast<EventTypes>(events[i]["type"].GetInt());
			//changeLevel->myTarget = std::string(events[i]["target"].GetString());
			//changeLevel->myObjectData = dataObject;

			Value& extra = events[i]["extra"];
			if (extra.HasMember("TargetScene") == true)
			{
				Value& myValue = extra["TargetScene"];
				if (myValue.IsNull() == true)
				{
					DL_ASSERT("Event Change Level Value is null");
				}
				changeLevel->myTargetLevelName = myValue.GetString();
			}

			/*dataObject->myEvents.Add(changeLevel);*/
			event = dynamic_cast<Event*>(changeLevel);
			break;
		}
		default:
			event = new EventNone();
			event->Init(aRoom, aGameWorld);
			break;
		}
		event->myType = static_cast<EventTypes>(events[i]["type"].GetInt());
		event->myTarget = std::string(events[i]["target"].GetString());
		event->myObjectData = dataObject;
		dataObject->myEvents.Add(event);
	}

	ObjectData* parentData = nullptr;
	dataObject->myChilds.Init(12);

	if (false && object["item"]["isItem"].GetBool() == true)
	{
		//Item* item = new Item();

	}
	else
	{
		if (aParentObject == nullptr)
		{
			aObjects->Add(dataObject);
			parentData = (*aObjects)[aObjects->Size() - 1];
		}
		else
		{
			aParentObject->myChilds.Add(dataObject);
			parentData = aParentObject->myChilds[aParentObject->myChilds.Size() - 1];
		}
	}

	for (unsigned int j = 0; j < object["childs"]["$values"].Size(); ++j)
	{
		LoadObject(object["childs"]["$values"][j], parentData, aObjects, aRoom, aGameWorld, x + static_cast<float>(object["x"].GetDouble()), y + static_cast<float>(object["y"].GetDouble()));
	}
}
/*
void JSON::LoadEvent(Value& aParent)
{
	EventActions action = static_cast<EventActions>(events[i]["action"].GetInt());
	Event* event = nullptr;
	switch (action)
	{
	case EventActions::SetActive:
	{
		EventSetActive* setActive = new EventSetActive();
		setActive->Init(aRoom, aGameWorld);
		if (events[i].HasMember("Value") == true)
		{
			Value& myValue = events[i]["Value"];
			if (myValue.IsNull() == true)
			{
				//DL_ASSERT("Event SetActive Value is null");
			}
			setActive->myValue = myValue.GetBool();
		}

		event = dynamic_cast<Event*>(setActive);
		break;
	}
	case EventActions::ChangeLevel:
	{
		EventChangeLevel* changeLevel = new EventChangeLevel();
		changeLevel->Init(aRoom, aGameWorld);
		changeLevel->myTargetLevelName = events[i]["TargetSceneName"].GetString();

		event = dynamic_cast<Event*>(changeLevel);
		break;
	}
	default:

		break;
	}
	event->myType = static_cast<EventTypes>(events[i]["type"].GetInt());
	event->myTarget = std::string(events[i]["target"].GetString());
	dataObject->myEvents.Add(event);
}
*/
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
