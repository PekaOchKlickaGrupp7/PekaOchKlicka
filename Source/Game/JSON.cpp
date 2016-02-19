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
#include "Triangle.h"
#include "..\CommonUtilities\Macros.h"
#include "..\CommonUtilities\TimerManager.h"
#include "MusicManager.h"

using namespace rapidjson;

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

	if (root.HasMember("items") == true)
	{
		Value& inventoryItems = root["items"]["$values"];
		for (unsigned int i = 0; i < inventoryItems.Size(); ++i)
		{
			Value& item = inventoryItems[i];

			std::string name = item["Name"].GetString();
			const char* path = item["Image"].GetString();
			std::string description = item["Description"].GetString();
			std::string combinableWith = item["CombinableWith"].GetString();
			std::string resultingItem = item["ResultingItem"].GetString();
			bool isCombinable = item["IsCombinable"].GetBool();
			std::string aPath = item["SoundPath"].GetString();
			aGameWorld->GetPlayer()->GetInventory().GetMasterItemList()->Add(new Item(name, path, description, combinableWith, resultingItem, isCombinable, aPath));
		}
	}


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

		float gridSize = 24;
		float windowWidth = 1920.0f;
		float windowHeight = 1080.0f;
		float numberOfMaxPoints = 1920.0f / gridSize + 1080.0f / gridSize;
		numberOfMaxPoints *= numberOfMaxPoints;

		CommonUtilities::GrowingArray<Node, int> nodes;
		nodes.Init(static_cast<int>(numberOfMaxPoints));

		CommonUtilities::GrowingArray<NavPolygon, unsigned short>& navMeshes = aRoom->GetNavMeshes();

		CU::TimeSys::TimerManager manager;
		unsigned char timer = manager.CreateTimer();
		manager.UpdateTimers();

		for (float y = 0; y < windowHeight; y += gridSize)
		{
			for (float x = 0; x < windowWidth; x += gridSize)
			{
				bool inside = false;
				for (unsigned short j = 0; j < navMeshes.Size(); ++j)
				{
					bool isInside = navMeshes[j].PointInsideCheck(Point2f(x / windowWidth, y / windowHeight));
					if (j == 0)
					{
						inside = isInside;
					}
					else if (isInside == true)
					{
						inside = false;
						break;
					}
				}
				nodes.Add(Node(static_cast<int>(x / gridSize), static_cast<int>(y / gridSize), !inside));
			}
		}

		manager.UpdateTimers();
		double diff = manager.GetTimer(timer).GetTimeElapsed().GetMiliseconds();

		std::cout << diff << std::endl;
		aRoom->SetGridSize(gridSize);
		aRoom->SetNavPoints(nodes);
	}

	level.GetAllocator().Clear();

	delete data;

	return true;
}

bool JSON::LoadMusic(const std::string& aMusicFile)
{
	const char* data = ReadFile(aMusicFile.c_str());

	Document music;
	music.Parse(data);

	if (music.HasParseError() == true)
	{
		DL_DEBUG("Failed to load Music.json");
		music.GetAllocator().~MemoryPoolAllocator();
		return false;
	}

	Value& musicfiles = music["music"];
	if (musicfiles.IsNull() == true)
	{
		DL_DEBUG("music is not a member of music.json");
		music.GetAllocator().~MemoryPoolAllocator();
		return false;
	}

	for (unsigned int i = 0; i < musicfiles.Size(); ++i)
	{
		Value& song = musicfiles[i];

		std::string songPath = song["file"].GetString();
		std::string songName = song["name"].GetString();

		MusicManager::GetInstance()->Load(songPath, songName);
	}

	MusicManager::GetInstance()->StartSystem();

	return true;
}

#pragma region Private Methods

void JSON::LoadObject(Value& node, ObjectData* aParentObject,
	CommonUtilities::GrowingArray<ObjectData*, unsigned int>* aObjects, Room* aRoom, CGameWorld* aGameWorld, float aX, float aY)
{
	Value& object = node;

	ObjectData* dataObject = new ObjectData();

	dataObject->myActive = object["active"].GetBool();
	dataObject->myName = object["name"].GetString();

	dataObject->myScaleX = static_cast<float>(object["sx"].GetDouble());
	dataObject->myScaleY = static_cast<float>(object["sy"].GetDouble());
	dataObject->myX = (static_cast<float>(object["x"].GetDouble())) / 1920.0f;
	dataObject->myY = (static_cast<float>(object["y"].GetDouble())) / 1080.0f;
	dataObject->myGlobalX = (static_cast<float>(object["x"].GetDouble()) + aX) / 1920.0f;
	dataObject->myGlobalY = (static_cast<float>(object["y"].GetDouble()) + aY) / 1080.0f;
	dataObject->myRotation = static_cast<float>(object["rotation"].GetDouble());
	dataObject->myPivotX = static_cast<float>(object["pivotX"].GetDouble());
	dataObject->myPivotY = static_cast<float>(object["pivotY"].GetDouble());

	if (std::string(object["image"].GetString()).size() > 0)
	{
		dataObject->mySprite = new DX2D::CSprite(object["image"].GetString());
		dataObject->myOriginalSprite = dataObject->mySprite;
		dataObject->mySprite->SetPivot(DX2D::Vector2f(dataObject->myPivotX, dataObject->myPivotY));
		dataObject->mySprite->SetSize(DX2D::Vector2f(dataObject->myScaleX, dataObject->myScaleY));
		dataObject->mySprite->SetRotation(dataObject->myRotation);
		dataObject->mySprite->SetPosition(DX2D::Vector2f(dataObject->myX, dataObject->myY));
	}
	else
	{
		dataObject->mySprite = nullptr;
		dataObject->myOriginalSprite = nullptr;
	}

	dataObject->myEvents.Init(128);

	HitBox box;
	box.Init(dataObject);
	box.Parse(object["events"]["hitbox"].GetString());

	dataObject->myHitBox = box;
	dataObject->myTriggerType = static_cast<TriggerType>(object["events"]["type"].GetInt());
	dataObject->myTriggerEnabled = object["events"]["enabled"].GetBool();
	dataObject->myRadius = static_cast<float>(object["events"]["radius"].GetDouble());

	if (object["events"].HasMember("triangles") == true)
	{
		Value& triangles = object["events"]["triangles"]["$values"];
		if (triangles.Size() == 2)
		{
			Triangle tr1;
			tr1.myObject = dataObject;
			tr1.myV1 = Vector2f(static_cast<float>(triangles[0]["x1"].GetDouble()) / 1920.0f, static_cast<float>(triangles[0]["y1"].GetDouble()) / 1080.0f);
			tr1.myV2 = Vector2f(static_cast<float>(triangles[0]["x2"].GetDouble()) / 1920.0f, static_cast<float>(triangles[0]["y2"].GetDouble()) / 1080.0f);
			tr1.myV3 = Vector2f(static_cast<float>(triangles[1]["x2"].GetDouble()) / 1920.0f, static_cast<float>(triangles[1]["y2"].GetDouble()) / 1080.0f);

			Triangle tr2;
			tr2.myObject = dataObject;
			tr2.myV1 = Vector2f(static_cast<float>(triangles[1]["x1"].GetDouble()) / 1920.0f, static_cast<float>(triangles[1]["y1"].GetDouble()) / 1080.0f);
			tr2.myV2 = Vector2f(static_cast<float>(triangles[1]["x2"].GetDouble()) / 1920.0f, static_cast<float>(triangles[1]["y2"].GetDouble()) / 1080.0f);
			tr2.myV3 = Vector2f(static_cast<float>(triangles[0]["x2"].GetDouble()) / 1920.0f, static_cast<float>(triangles[0]["y2"].GetDouble()) / 1080.0f);

			dataObject->AddTriangle(tr1);
			dataObject->AddTriangle(tr2);
		}
	}

	Value& events = object["events"]["list"]["$values"];
	for (unsigned int i = 0; i < events.Size(); ++i)
	{
		LoadEvent(dataObject, events[i], aRoom, aGameWorld);
	}

	ObjectData* parentData = nullptr;
	dataObject->myChilds.Init(12);

	dataObject->CreateOriginalData();
	if (aParentObject == nullptr)
	{
		aObjects->Add(dataObject);
		CommonUtilities::GrowingArray<ObjectData*, unsigned int>& objects = *aObjects;
		parentData = objects[objects.Size() - 1];
	}
	else
	{
		aParentObject->myChilds.Add(dataObject);
		parentData = aParentObject->myChilds[aParentObject->myChilds.Size() - 1];
	}

	for (unsigned int j = 0; j < object["childs"]["$values"].Size(); ++j)
	{
		LoadObject(object["childs"]["$values"][j], parentData, aObjects, aRoom, aGameWorld, aX + static_cast<float>(object["x"].GetDouble()), aY + static_cast<float>(object["y"].GetDouble()));
	}
}

void JSON::LoadEvent(ObjectData* aNode, Value& aParent, Room* aRoom, CGameWorld* aGameWorld)
{
	Event* event = myEventsFactory.CreateEventData(aNode, aParent, aRoom, aGameWorld);
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
	Event* event = myEventsFactory.CreateEventData(aNode, aParent, aRoom, aGameWorld);
	if (aEvent != nullptr)
	{
		if (aEvent->myChilds.GetIsInitialized() == false)
		{
			aEvent->myChilds.Init(12);
		}
		aEvent->myChilds.Add(event);
		event->myParent = aEvent;
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
