#pragma once
#include <string>
#include "ObjectData.h"

class Room;
class CGameWorld;
class Synchronizer;

class Event
{
public:
	virtual ~Event();
	Event();
	virtual void Init(Room* aRoom, CGameWorld* aGameWorld);
	virtual bool Update(const float aDeltaTime) = 0;
	virtual void Reset() = 0;
	virtual void Render(Synchronizer &aSynchronizer);

	ObjectData* GetGameObject(const std::string& aName) const;
	ObjectData* GetGameObject(const std::string& aName, ObjectData* aParent) const;

	std::string myTarget;
	EventTypes myType;

	ObjectData* myObjectData;

	CommonUtilities::GrowingArray<Event*, unsigned int> myChilds;
	bool myActive;
protected:
	Room* myRoom;
	CGameWorld* myGameWorld;
};

