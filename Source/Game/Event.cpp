#include "stdafx.h"
#include "Event.h"
#include "Room.h"
#include "..\CommonUtilities\DL_Debug.h"

Event::Event()
{
}

Event::~Event()
{
	myRoom = nullptr;
}

void Event::Init(Room* aRoom, CGameWorld* aGameWorld)
{
	myRoom = aRoom;
	myGameWorld = aGameWorld;
	myChilds.Init(1);
}

ObjectData* Event::GetGameObject(const std::string& aName) const
{
	if (aName == "Self")
	{
		return myObjectData;
	}
	else if (aName == "None")
	{
		return nullptr;
	}
	CommonUtilities::GrowingArray<ObjectData*, unsigned int>& objects = myRoom->GetObjectList();
	for (unsigned int i = 0; i < objects.Size(); ++i)
	{
		if (objects[i]->myName == aName)
		{
			return objects[i];
		}
	}
	DL_DEBUG(("Couldn't find object named: " + aName).c_str());
	return nullptr;
}
