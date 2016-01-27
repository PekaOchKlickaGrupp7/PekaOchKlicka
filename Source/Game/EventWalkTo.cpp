#include "stdafx.h"
#include "EventWalkTo.h"
#include "GameWorld.h"

EventWalkTo::EventWalkTo()
{
}


EventWalkTo::~EventWalkTo()
{
}

bool EventWalkTo::Update(const float aDeltaTime)
{
	ObjectData* object = GetGameObject(myTarget);
	if (object != nullptr)
	{
		myGameWorld->SetPlayerTargetPosition(Point2f(object->myGlobalX, object->myGlobalY));
	}
	return true;
}

void EventWalkTo::Reset()
{

}
