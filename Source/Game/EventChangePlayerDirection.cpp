#include "stdafx.h"
#include "EventChangePlayerDirection.h"
#include "GameWorld.h"

EventChangePlayerDirection::EventChangePlayerDirection()
{
}

EventChangePlayerDirection::~EventChangePlayerDirection()
{
}

bool EventChangePlayerDirection::Update(const float aDeltaTime)
{
	myGameWorld->GetPlayer()->SetAnimation(myPlayerDirection);
	return true;
}

void EventChangePlayerDirection::Reset()
{

}
