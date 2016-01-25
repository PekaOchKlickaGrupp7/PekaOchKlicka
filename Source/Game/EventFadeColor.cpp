#include "stdafx.h"
#include "EventFadeColor.h"

EventFadeColor::EventFadeColor()
{
	myFadeTime = 1.0f;
	myTargetColor = { 1, 1, 1, 1 };
}

EventFadeColor::~EventFadeColor()
{
}

bool EventFadeColor::Update(const float aDeltaTime)
{
	return true;
}

void EventFadeColor::Reset()
{

}
