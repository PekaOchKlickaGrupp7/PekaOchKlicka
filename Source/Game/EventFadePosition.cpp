#include "stdafx.h"
#include "EventFadePosition.h"

EventFadePosition::EventFadePosition()
{
	Reset();
}

EventFadePosition::~EventFadePosition()
{
}

bool EventFadePosition::Update(const float aDeltaTime)
{

	return true;
}

void EventFadePosition::Reset()
{
	myTime = 0.0f;
	myCached = false;
}
