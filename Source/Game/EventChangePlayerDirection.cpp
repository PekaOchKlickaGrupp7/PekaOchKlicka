#include "stdafx.h"
#include "EventChangePlayerDirection.h"
#include "GameWorld.h"

EventChangePlayerDirection::EventChangePlayerDirection()
{
	myPlayerDirection = -1;
}

EventChangePlayerDirection::~EventChangePlayerDirection()
{
}

bool EventChangePlayerDirection::Update(const float)
{
	if (myPlayerDirection >= 0 && myPlayerDirection < 4)
	{
		myGameWorld->GetPlayer()->SetAnimation(myPlayerDirection);
	}
	return true;
}

void EventChangePlayerDirection::Reset()
{

}
