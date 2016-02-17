#include "stdafx.h"
#include "EventUpdateAnimation.h"

EventUpdateAnimation::EventUpdateAnimation()
{
}

EventUpdateAnimation::~EventUpdateAnimation()
{
}

bool EventUpdateAnimation::Update(const float)
{
	ObjectData* target = GetGameObject(myTarget);
	if (target != nullptr)
	{
		target->myCurrentAnimation = myAnimationIndex;
	}
	return true;
}

void EventUpdateAnimation::Reset()
{

}
