#pragma once
#include "Event.h"
class EventSetPosition :
	public Event
{
public:
	EventSetPosition();
	virtual ~EventSetPosition();

	virtual bool Update(const float aDeltaTime) override;
	virtual void Reset() override;

	DX2D::Vector2f myTargetOffset;
};

