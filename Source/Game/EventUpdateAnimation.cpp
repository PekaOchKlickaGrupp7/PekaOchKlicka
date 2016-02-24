#include "stdafx.h"
#include "EventUpdateAnimation.h"
#include "Animation.h"

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
		if (myCached == false)
		{
			myCached = true;
			if (target->myAnimations[target->myCurrentAnimation]->GetIsLooping() == false)
			{
				target->myAnimations[target->myCurrentAnimation]->Reset();
			}
		}
		if (target->myAnimations[target->myCurrentAnimation]->GetIsLooping() == true)
		{
			return true;
		}
		else
		{
			if (target->myAnimations[target->myCurrentAnimation]->GetIsPlaying() == true)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	}
	else
	{
		return true;
	}
}

void EventUpdateAnimation::Reset()
{
	myCached = false;
}
