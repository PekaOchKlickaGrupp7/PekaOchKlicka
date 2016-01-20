#pragma once
#include "Event.h"
class EventSetActive :
	public Event
{
public:
	EventSetActive();
	virtual ~EventSetActive();

	virtual bool Update(const float aDeltaTime) override;
	virtual void Reset() override;


	bool myValue;
};

