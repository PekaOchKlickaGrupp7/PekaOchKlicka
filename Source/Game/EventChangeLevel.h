#pragma once
#include "Event.h"
class EventChangeLevel :
	public Event
{
public:
	EventChangeLevel();
	virtual ~EventChangeLevel();

	virtual bool Update(const float aDeltaTime) override;
	virtual void Reset() override {};

	std::string myTargetLevelName;
protected:
};

