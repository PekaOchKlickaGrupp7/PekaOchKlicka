#include "stdafx.h"
#include "EventSetActive.h"

EventSetActive::EventSetActive()
{
}

EventSetActive::~EventSetActive()
{
}

bool EventSetActive::Update(const float aDeltaTime)
{
	(aDeltaTime);
	ObjectData* object = GetGameObject(myTarget);
	if (object != nullptr)
	{
		object->myActive = myValue;
	}
	return true;
}

void EventSetActive::Reset()
{
	
}
