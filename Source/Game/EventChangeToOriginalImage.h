#pragma once
#include "Event.h"
class EventChangeToOriginalImage :
	public Event
{
public:
	EventChangeToOriginalImage();
	virtual ~EventChangeToOriginalImage();

	virtual bool Update(const float aDeltaTime) override;
	virtual void Reset() override;
};

