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
	if (myCached == false)
	{
		ObjectData* object = GetGameObject(myTarget);
		if (object != nullptr)
		{
			myPrevPosition = { myObjectData->myX, myObjectData->myY };
		}
		myCached = true;
	}
	myTime += aDeltaTime;

	ObjectData* object = GetGameObject(myTarget);
	if (object != nullptr)
	{
		DX2D::Vector2f pos = {	Lerp(myPrevPosition.x, object->myX + myTargetOffset.x, myTime / myDuration),
								Lerp(myPrevPosition.y, object->myY + myTargetOffset.y, myTime / myDuration)
							};
		myObjectData->myX = pos.x;
		myObjectData->myY = pos.y;
		if (myTime >= myDuration)
		{
			return true;
		}
	}
	else
	{
		return true;
	}
	return false;
}

float EventFadePosition::Lerp(float aFrom, float aTo, float aTime)
{
	return aFrom + aTime * (aTo - aFrom);
}

void EventFadePosition::Reset()
{
	myTime = 0.0f;
	myCached = false;
}
