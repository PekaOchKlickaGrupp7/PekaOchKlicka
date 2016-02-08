#pragma once
#include "Event.h"
class EventChangePlayerDirection :
	public Event
{
public:
	EventChangePlayerDirection();
	virtual ~EventChangePlayerDirection();

	virtual bool Update(const float aDeltaTime) override;
	virtual void Reset() override;

	int myPlayerDirection;
};

