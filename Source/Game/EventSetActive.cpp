#include "stdafx.h"
#include "EventSetActive.h"

EventSetActive::EventSetActive()
{
}

EventSetActive::~EventSetActive()
{
}

bool EventSetActive::Update(float aDeltaTime)
{
	(aDeltaTime);
	ObjectData* object = GetGameObject(myTarget);
	object->myActive = myValue;
	return true;
}
