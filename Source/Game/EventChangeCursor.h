#pragma once
#include "Event.h"
class EventChangeCursor : public Event
{
public:
	EventChangeCursor();

	virtual bool Update(const float aDeltaTime) override;
	virtual void Reset() override;

	virtual ~EventChangeCursor();

	int myTargetCursor;
};

