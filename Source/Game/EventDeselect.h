#pragma once
#include "Event.h"
class EventDeselect :
	public Event
{
public:
	EventDeselect();
	virtual ~EventDeselect();

	virtual bool Update(const float aDeltaTime) override;
	virtual void Reset() override;
};

