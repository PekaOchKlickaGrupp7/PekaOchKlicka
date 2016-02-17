#pragma once
#include "Event.h"
class EventUpdateAnimation :
	public Event
{
public:
	EventUpdateAnimation();
	virtual ~EventUpdateAnimation();

	virtual bool Update(const float aDeltaTime) override;
	virtual void Reset() override;

	int myAnimationIndex;
};

