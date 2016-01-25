#pragma once
#include "Event.h"

class EventDelay : public Event
{
public:
	EventDelay();
	virtual ~EventDelay();

	virtual bool Update(const float aDeltaTime) override;
	virtual void Reset() override;

	float myDelay;
protected:
	float myTime;
};

