#pragma once
#include "Event.h"
class EventStopTalk :
	public Event
{
public:
	EventStopTalk();
	virtual ~EventStopTalk();

	virtual bool Update(const float aDeltaTime) override;
	virtual void Reset() override;
};

