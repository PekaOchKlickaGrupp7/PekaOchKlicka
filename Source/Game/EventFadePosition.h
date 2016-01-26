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

	DX2D::Vector2f myTargetOffset;
	float myDuration;
protected:
	float Lerp(float aFrom, float aTo, float aTime);

	DX2D::Vector2f myPrevPosition;
	float myTime;
	bool myCached;
};

