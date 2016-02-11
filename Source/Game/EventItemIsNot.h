#pragma once
#include "Event.h"
class EventItemIsNot :
	public Event
{
public:
	EventItemIsNot();
	virtual ~EventItemIsNot();

	virtual bool Update(const float aDeltaTime) override;
	virtual void Reset() override;

	std::string myItemName;
};

