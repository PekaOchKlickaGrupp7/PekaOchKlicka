#pragma once
#include "Event.h"
class EventFadeColor :
	public Event
{
public:
	EventFadeColor();
	virtual ~EventFadeColor();

	virtual bool Update(const float aDeltaTime) override;
	virtual void Reset() override;

	DX2D::CColor myTargetColor;
	float myFadeTime;
protected:
	float myTime;
	DX2D::CColor myPrevColor;
};

