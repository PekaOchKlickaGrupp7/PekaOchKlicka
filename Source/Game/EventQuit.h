#pragma once
#include "Event.h"
class EventQuit :
	public Event
{
public:
	EventQuit();
	virtual ~EventQuit();

	virtual bool Update(const float aDeltaTime) override;
	virtual void Reset() override;
};

