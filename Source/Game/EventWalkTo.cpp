#include "stdafx.h"
#include "EventWalkTo.h"
#include "GameWorld.h"
#include <iostream>

EventWalkTo::EventWalkTo()
{
	mySet = false;
}

EventWalkTo::~EventWalkTo()
{
}

bool EventWalkTo::Update(const float)
{
	ObjectData* object = GetGameObject(myTarget);
	if (object != nullptr)
	{
		Point2f targetPos = Point2f(object->myGlobalX, object->myGlobalY);
		Point2f playerPos;
		DX2D::Vector2f pos = myGameWorld->GetPlayer()->GetPosition();
		playerPos.x = pos.x;
		playerPos.y = pos.y;
		if (mySet == false)
		{
			mySet = true;
			myGameWorld->SetPlayerTargetPosition(targetPos);
		}
		else if (myGameWorld->GetPlayerTargetPosition() != targetPos)
		{
			myAutoActivateRecursive = false;
			return true;
		}
		else if (myGameWorld->PlayerHasReachedTarget() == true)
		{
			myAutoActivateRecursive = true;
			return true;
		}
	}
	else
	{
		myAutoActivateRecursive = false;
		return true;
	}
	return false;
}

void EventWalkTo::Reset()
{
	mySet = false;
}
