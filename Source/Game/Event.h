#pragma once
#include "ObjectData.h"
#include "EventActions.h"

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
	EventActions myAction;

	ObjectData* myObjectData;

	CommonUtilities::GrowingArray<Event*, unsigned int> myChilds;
	bool myActive;
	bool myAutoActivateRecursive;
protected:
	Room* myRoom;
	CGameWorld* myGameWorld;
};

