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
	float Lerp(float aFrom, float aTo, float aTime);

	float myTime;
	DX2D::CColor myPrevColor;
	bool myCached = false;
};

