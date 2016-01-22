#pragma once
#include "Event.h"
class EventPlaySound :
	public Event
{
public:
	EventPlaySound();

	virtual bool Update(const float aDeltaTime) override;
	virtual void Reset() override;

	int myTargetSound;
	float myVolume;
	bool myIsLooping;
	bool myIs3D;
	DX2D::Vector2f myPosition;

	virtual ~EventPlaySound();
};

