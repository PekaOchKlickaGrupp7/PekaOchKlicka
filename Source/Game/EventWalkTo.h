#pragma once
#include "Event.h"
class EventWalkTo :
	public Event
{
public:
	EventWalkTo();
	virtual ~EventWalkTo();

	virtual bool Update(const float aDeltaTime) override;
	virtual void Reset() override;
};

