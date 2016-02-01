#pragma once
#include "Event.h"
class EventSetCinematic :
	public Event
{
public:
	EventSetCinematic();
	virtual ~EventSetCinematic();

	virtual bool Update(const float aDeltaTime) override;
	virtual void Reset() override;

	bool mySetOn;
};

