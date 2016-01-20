#pragma once
#include <string>
#include "ObjectData.h"

class Room;
class CGameWorld;

class Event
{
public:
	virtual ~Event();

	virtual void Init(Room* aRoom, CGameWorld* aGameWorld);
	virtual bool Update(const float aDeltaTime) = 0;

	ObjectData* GetGameObject(const std::string& aName) const;

	bool myValue;
	std::string myTarget;
	EventTypes myType;
protected:
	Event();
	Room* myRoom;
	CGameWorld* myGameWorld;
};

