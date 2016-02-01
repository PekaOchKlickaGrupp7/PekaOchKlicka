#pragma once
#include "Event.h"
class EventHideMouse :
	public Event
{
public:
	EventHideMouse();

	virtual bool Update(const float aDeltaTime) override;
	virtual void Reset() override;

	bool myHideGameMouse;

	virtual ~EventHideMouse();
};

