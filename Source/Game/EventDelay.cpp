#include "stdafx.h"
#include "EventDelay.h"


EventDelay::EventDelay()
{
	myTime = 0.0f;
}

EventDelay::~EventDelay()
{
}

bool EventDelay::Update(const float aDeltaTime)
{
	myTime += aDeltaTime;
	if (myTime >= myDelay)
	{
		return true;
	}
	return false;
}

void EventDelay::Reset()
{
	myTime = 0.0f;
}
