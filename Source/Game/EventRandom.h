#pragma once
#include "Event.h"
class EventRandom :
	public Event
{
public:
	EventRandom();
	virtual ~EventRandom();

	virtual bool Update(const float aDeltaTime) override;
	virtual void Reset() override;

	int myRandomFactor;
	int myIndex;
};

