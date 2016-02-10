#pragma once
#include "Event.h"
class EventResetGame :
	public Event
{
public:
	EventResetGame();
	virtual ~EventResetGame();

	virtual bool Update(const float aDeltaTime) override;
	virtual void Reset() override;
};

