#pragma once
#include "Event.h"
#include "rapidjson\document.h"
#include "EventActions.h"

class Room;
class CGameWorld;
class ObjectData;

class EventsFactory
{
public:
	EventsFactory();
	~EventsFactory();

	Event* CreateEventData(ObjectData* aData, rapidjson::Value& aParent, Room* aRoom, CGameWorld* aGameWorld) const;
};

