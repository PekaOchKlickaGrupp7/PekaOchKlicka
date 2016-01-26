#include "stdafx.h"
#include "EventSetColor.h"

EventSetColor::EventSetColor()
{
	myTargetColor = { 1, 1, 1, 1 };
}

EventSetColor::~EventSetColor()
{
}

bool EventSetColor::Update(const float)
{
	ObjectData* object = GetGameObject(myTarget);
	if (object != nullptr)
	{
		object->myColor = myTargetColor;
	}
	return true;
}

void EventSetColor::Reset()
{

}
