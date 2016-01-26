#pragma once
#include "Event.h"
class EventFadePosition :
	public Event
{
public:
	EventFadePosition();
	virtual ~EventFadePosition();

	virtual bool Update(const float aDeltaTime) override;
	virtual void Reset() override;

protected:
	float myTime;
	bool myCached;
};

