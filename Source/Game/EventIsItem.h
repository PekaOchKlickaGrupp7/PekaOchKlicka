#pragma once
#include "Event.h"
class EventIsItem :
	public Event
{
public:
	EventIsItem();
	virtual ~EventIsItem();

	virtual bool Update(const float aDeltaTime) override;
	virtual void Reset() override;

	std::string myItemName;
};

