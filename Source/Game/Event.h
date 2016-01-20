#pragma once
#include <string>
#include "ObjectData.h"

class Room;
class CGameWorld;

class Event
{
public:
	virtual ~Event();
	Event();
	virtual void Init(Room* aRoom, CGameWorld* aGameWorld);
	virtual bool Update(const float aDeltaTime);

	ObjectData* GetGameObject(const std::string& aName) const;

	std::string myTarget;
	EventTypes myType;
protected:
	Room* myRoom;
	CGameWorld* myGameWorld;
};

