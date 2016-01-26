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
	if (myCached == false)
	{
		ObjectData* object = GetGameObject(myTarget);
		if (object != nullptr)
		{
			myPrevColor = object->myColor;
		}
		myCached = true;
	}
	myTime += aDeltaTime;
	DX2D::CColor color = {	Lerp(myPrevColor.myR, myTargetColor.myR, myTime / myFadeTime),
							Lerp(myPrevColor.myG, myTargetColor.myG, myTime / myFadeTime),
							Lerp(myPrevColor.myB, myTargetColor.myB, myTime / myFadeTime),
							Lerp(myPrevColor.myA, myTargetColor.myA, myTime / myFadeTime)
						};

	ObjectData* object = GetGameObject(myTarget);
	if (object != nullptr)
	{
		object->myColor = color;
		if (myTime >= myFadeTime)
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

float EventFadeColor::Lerp(float aFrom, float aTo, float aTime)
{
	return aFrom + aTime * (aTo - aFrom);
}

void EventFadeColor::Reset()
{
	myTime = 0.0f;
	myCached = false;
}
