#include "stdafx.h"
#include "EventSetPosition.h"

EventSetPosition::EventSetPosition()
{
	Reset();
}

EventSetPosition::~EventSetPosition()
{
}

bool EventSetPosition::Update(const float)
{
	ObjectData* object = GetGameObject(myTarget);

	if (object != nullptr)
	{
		DX2D::Vector2f pos = { object->myX + myTargetOffset.x, object->myY + myTargetOffset.y };

		myObjectData->myX = pos.x;
		myObjectData->myY = pos.y;
		return true;
	}
	else
	{
		return true;
	}
	return false;
}

void EventSetPosition::Reset()
{

}
