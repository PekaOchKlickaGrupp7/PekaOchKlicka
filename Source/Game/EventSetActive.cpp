#include "stdafx.h"
#include "EventSetActive.h"

EventSetActive::EventSetActive()
{
	current = 0;
	target = 1;
}

EventSetActive::~EventSetActive()
{
}

bool EventSetActive::Update(float aDeltaTime)
{
	ObjectData* object = GetObject(myTarget);
	object->myActive = myValue;
	return true;
}
