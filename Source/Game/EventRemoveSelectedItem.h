#pragma once
#include "Event.h"
class EventRemoveSelectedItem :
	public Event
{
public:
	EventRemoveSelectedItem();
	virtual ~EventRemoveSelectedItem();

	virtual bool Update(const float aDeltaTime) override;
	virtual void Reset() override;
};

