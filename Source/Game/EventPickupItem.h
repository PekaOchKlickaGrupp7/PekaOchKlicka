#pragma once
#include "Event.h"

class Item;

class EventPickupItem :
	public Event
{
public:
	EventPickupItem();
	virtual ~EventPickupItem();

	virtual void Init(Room* aRoom, CGameWorld* aGameWorld) override;
	virtual bool Update(const float aDeltaTime) override;
	virtual void Reset() override;

	std::string myItem;
protected:
	Item* myItemVar;
};

