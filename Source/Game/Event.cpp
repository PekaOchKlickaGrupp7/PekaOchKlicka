#include "stdafx.h"
#include "Event.h"
#include "Room.h"
#include "..\CommonUtilities\DL_Debug.h"
#include "Synchronizer.h"

Event::Event()
{
	myActive = false;
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

void Event::Render(Synchronizer &aSynchronizer)
{

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
	CommonUtilities::GrowingArray<ObjectData*, unsigned int>* objects = myRoom->GetObjectList();
	for (unsigned int i = 0; i < objects->Size(); ++i)
	{
		if ((*objects)[i]->myName == aName)
		{
			return (*objects)[i];
		}

		for (unsigned int j = 0; j < (*objects)[i]->myChilds.Size(); ++j)
		{
			ObjectData* data = GetGameObject(aName, (*objects)[i]);
			if (data != nullptr)
			{
				return data;
			}
		}
	}
	DL_DEBUG(("Couldn't find object named: " + aName).c_str());
	return nullptr;
}

ObjectData* Event::GetGameObject(const std::string& aName, ObjectData* aParent) const
{
	for (unsigned int i = 0; i < aParent->myChilds.Size(); ++i)
	{
		if (aParent->myChilds[i]->myName == aName)
		{
			return aParent->myChilds[i];
		}

		ObjectData* data = GetGameObject(aName, aParent->myChilds[i]);
		if (data != nullptr)
		{
			return data;
		}
	}
	DL_DEBUG(("Couldn't find object named: " + aName).c_str());
	return nullptr;
}
