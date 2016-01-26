#pragma once
#include "Event.h"
class EventSetColor :
	public Event
{
public:
	EventSetColor();
	virtual ~EventSetColor();

	virtual bool Update(const float aDeltaTime) override;
	virtual void Reset() override;

	DX2D::CColor myTargetColor;
};

