#pragma once
#include "Event.h"
class EventNone :
	public Event
{
public:
	EventNone();
	virtual ~EventNone();

	virtual bool Update(const float aDeltaTime) override;
	virtual void Reset() override;
};

