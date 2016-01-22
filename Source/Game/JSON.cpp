#include "stdafx.h"
#include "JSON.h"
#include "..\CommonUtilities\DL_Debug.h"
#include <iostream>
#include "tga2d\sprite\sprite.h"
#include <map>
#include "Room.h"
#include "GameWorld.h"
#include "Item.h"
#include "Inventory.h"

//Events
#include "EventNone.h"
#include "EventSetActive.h"
#include "EventChangeLevel.h"
#include "EventTalk.h"
#include "EventChangeCursor.h"
#include "EventPlaySound.h"

enum class eSound
{
	eRain,
	eJaguar,
};

using namespace rapidjson;

Event* JSON::CreateEventData(ObjectData* aData, Value& aParent, Room* aRoom, CGameWorld* aGameWorld)
{
	EventActions action = static_cast<EventActions>(aParent["action"].GetInt());
	Event* event = nullptr;
	Value& extra = aParent["extra"];
	switch (action)
	{
	case EventActions::SetActive:
	{
		EventSetActive* setActive = new EventSetActive();
		setActive->Init(aRoom, aGameWorld);

		if (extra.HasMember("value") == true)
		{
			Value& myValue = extra["value"];
			if (myValue.IsNull() == true)
			{
				DL_ASSERT("Event SetActive Value is null");
			}
			setActive->myValue = myValue.GetBool();
		}

		event = setActive;
		break;
	}
	case EventActions::ChangeLevel:
	{
		EventChangeLevel* changeLevel = new EventChangeLevel();
		changeLevel->Init(aRoom, aGameWorld);

		if (extra.HasMember("TargetScene") == true)
		{
			Value& myValue = extra["TargetScene"];
			if (myValue.IsNull() == true)
			{
				DL_ASSERT("Event Change Level Value is null");
			}
			changeLevel->myTargetLevelName = myValue.GetString();
			changeLevel->myTargetPosition = DX2D::Vector2f(static_cast<float>(extra["x"].GetDouble()) / 1920.0f, static_cast<float>(extra["y"].GetDouble()) / 1080.0f);
		}

		event = changeLevel;
		break;
	}
	case EventActions::Talk:
	{
		EventTalk* talk = new EventTalk();

		if (extra.HasMember("text") == true)
		{
			talk->myText = extra["text"].GetString();
		}
		if (extra.HasMember("length") == true)
		{
			talk->myShowTime = static_cast<float>(extra["length"].GetDouble());
		}
		if (extra.HasMember("wordLength") == true)
		{
			talk->myWordLength = static_cast<float>(extra["wordLength"].GetDouble());
		}
		if (extra.HasMember("color") == true)
		{
			Value &colorVal = extra["color"];
			talk->myColor = DX2D::CColor(static_cast<float>(colorVal["r"].GetDouble()),
				static_cast<float>(colorVal["g"].GetDouble()),
				static_cast<float>(colorVal["b"].GetDouble()),
				static_cast<float>(colorVal["a"].GetDouble()));
		}
		if (extra.HasMember("size") == true)
		{
			talk->mySize = static_cast<float>(extra["size"].GetDouble());
		}
		if (extra.HasMember("fontPath") == true)
		{
			talk->myFontPath = extra["fontPath"].GetString();
		}

		talk->Init(aRoom, aGameWorld);

		event = talk;
		break;
	}
	case EventActions::ChangeCursor:
	{
		EventChangeCursor* changeCursor = new EventChangeCursor();
		if (extra.HasMember("cursor") == true)
		{
			changeCursor->myTargetCursor = extra["cursor"].GetInt();
		}

		changeCursor->Init(aRoom, aGameWorld);

		event = changeCursor;
		break;
	}
	case EventActions::PlaySound:
	{
		EventPlaySound* sound = new EventPlaySound();
		if (extra.HasMember("id") == true)
		{
			sound->myTargetSound = extra["id"].GetInt();
		}
		if (extra.HasMember("volume") == true)
		{
			sound->myVolume = static_cast<float>(extra["volume"].GetDouble());
		}
		if (extra.HasMember("looping") == true)
		{
			sound->myIsLooping = extra["looping"].GetBool();
		}
		if (extra.HasMember("is3D") == true)
		{
			sound->myIs3D = extra["is3D"].GetBool();
		}
		if (extra.HasMember("positionX") == true && extra.HasMember("positionY") == true)
		{
			sound->myPosition = DX2D::Vector2f(static_cast<float>(extra["positionX"].GetDouble()) / 1920.0f, static_cast<float>(extra["positionY"].GetDouble()) / 1080.0f);
		}

		sound->Init(aRoom, aGameWorld);

		event = sound;
		break;
	}
	default:
		event = new EventNone();
		event->Init(aRoom, aGameWorld);
		break;
	}
	event->myType = static_cast<EventTypes>(aParent["type"].GetInt());
	event->myTarget = std::string(aParent["target"].GetString());
	event->myObjectData = aData;
	

	return event;
}

JSON::JSON() { }
JSON::~JSON() { }

bool JSON::Load(const std::string& aRootFile, std::map<std::string, Room*>& aRooms, CGameWorld* aGameWorld, std::string& aName)
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
			aName = name;
		}

		LoadLevel(level["path"].GetString(), room->GetObjectList(), room, aGameWorld);
	}
	
	root.GetAllocator().Clear();

	//aGameWorld->ChangeLevel(levelName);

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

bool JSON::LoadItems(const std::string& aRootFile, Inventory aInventory)
{
	const char* data = ReadFile(aRootFile.c_str());

	Document items;
	items.Parse(data);

	if (items.HasParseError() == true)
	{
		DL_DEBUG("Failed to load items.json.");
		items.GetAllocator().~MemoryPoolAllocator();
		return false;
	}

	Value& inventoryItems = items["inventoryItems"];
	if (inventoryItems.IsNull() == true)
	{
		DL_DEBUG("inventoryItems is not a member of items.json");
		items.GetAllocator().~MemoryPoolAllocator();
		return false;
	}

	for (unsigned int i = 0; i < inventoryItems.Size(); ++i)
	{
		Value& item = inventoryItems[i];

		std::string name = item["name"].GetString();
		const char* path = item["path"].GetString();
		std::string description = item["description"].GetString();
		std::string combinableWith = item["combinableWith"].GetString();
		std::string resultingItem = item["resultingItem"].GetString();
		bool isCombinable = item["isCombinable"].GetBool();
		aInventory.GetMasterItemList()->Add(new Item(name, path, description, combinableWith, resultingItem, isCombinable));
	}
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
		LoadEvent(dataObject, events[i], aRoom, aGameWorld);
		/*EventActions action = static_cast<EventActions>(events[i]["action"].GetInt());
		Event* event = nullptr;
		Value& extra = events[i]["extra"];
		switch (action)
		{
		case EventActions::SetActive:
		{
			EventSetActive* setActive = new EventSetActive();
			setActive->Init(aRoom, aGameWorld);

			if (extra.HasMember("value") == true)
			{
				Value& myValue = extra["value"];
				if (myValue.IsNull() == true)
				{
					DL_ASSERT("Event SetActive Value is null");
				}
				setActive->myValue = myValue.GetBool();
			}

			event = setActive;
			break;
		}
		case EventActions::ChangeLevel:
		{
			EventChangeLevel* changeLevel = new EventChangeLevel();
			changeLevel->Init(aRoom, aGameWorld);

			if (extra.HasMember("TargetScene") == true)
			{
				Value& myValue = extra["TargetScene"];
				if (myValue.IsNull() == true)
				{
					DL_ASSERT("Event Change Level Value is null");
				}
				changeLevel->myTargetLevelName = myValue.GetString();
				changeLevel->myTargetPosition = DX2D::Vector2f(static_cast<float>(extra["x"].GetDouble()) / 1920.0f, static_cast<float>(extra["y"].GetDouble()) / 1080.0f);
			}

			event = changeLevel;
			break;
		}
		case EventActions::Talk:
		{
			EventTalk* talk = new EventTalk();

			if (extra.HasMember("text") == true)
			{
				talk->myText = extra["text"].GetString();
			}
			if (extra.HasMember("length") == true)
			{
				talk->myShowTime = static_cast<float>(extra["length"].GetDouble());
			}
			if (extra.HasMember("wordLength") == true)
			{
				talk->myWordLength = static_cast<float>(extra["wordLength"].GetDouble());
			}
			if (extra.HasMember("color") == true)
			{
				Value &colorVal = extra["color"];
				talk->myColor = DX2D::CColor(static_cast<float>(colorVal["r"].GetDouble()),
					static_cast<float>(colorVal["g"].GetDouble()), 
					static_cast<float>(colorVal["b"].GetDouble()), 
					static_cast<float>(colorVal["a"].GetDouble()));
			}
			if (extra.HasMember("size") == true)
			{
				talk->mySize = static_cast<float>(extra["size"].GetDouble());
			}
			if (extra.HasMember("fontPath") == true)
			{
				talk->myFontPath = extra["fontPath"].GetString();
			}

			talk->Init(aRoom, aGameWorld);

			event = talk;
			break;
		}
		case EventActions::ChangeCursor:
		{
			EventChangeCursor* changeCursor = new EventChangeCursor();
			changeCursor->myTargetCursor = events[i]["extra"]["cursor"].GetInt();
			changeCursor->Init(aRoom, aGameWorld);

			event = changeCursor;
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
		dataObject->myEvents.Add(event);*/
	}

	ObjectData* parentData = nullptr;
	//dataObject->myChilds.Init(12);

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

	for (unsigned int j = 0; j < object["childs"]["$values"].Size(); ++j)
	{
		LoadObject(object["childs"]["$values"][j], parentData, aObjects, aRoom, aGameWorld, x + static_cast<float>(object["x"].GetDouble()), y + static_cast<float>(object["y"].GetDouble()));
	}
}

void JSON::LoadEvent(ObjectData* aNode, Value& aParent, Room* aRoom, CGameWorld* aGameWorld)
{
	Event* event = CreateEventData(aNode, aParent, aRoom, aGameWorld);
	if (aNode != nullptr)
	{
		aNode->myEvents.Add(event);
	}

	for (unsigned int i = 0; i < aParent["childs"]["$values"].Size(); ++i)
	{
		LoadEvent(aNode, event, aParent["childs"]["$values"][i], aRoom, aGameWorld);
	}
}

void JSON::LoadEvent(ObjectData* aNode, Event* aEvent, Value& aParent, Room* aRoom, CGameWorld* aGameWorld)
{
	Event* event = CreateEventData(aNode, aParent, aRoom, aGameWorld);
	if (aEvent != nullptr)
	{
		if (aEvent->myChilds.GetIsInitialized() == false)
		{
			aEvent->myChilds.Init(12);
		}
		aEvent->myChilds.Add(event);
	}

	for (unsigned int i = 0; i < aParent["childs"]["$values"].Size(); ++i)
	{
		LoadEvent(aNode, event, aParent["childs"]["$values"][i], aRoom, aGameWorld);
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
