#include "stdafx.h"
#include "EventDeselect.h"
#include "GameWorld.h"

EventDeselect::EventDeselect()
{
}

EventDeselect::~EventDeselect()
{
}

bool EventDeselect::Update(const float)
{
	myGameWorld->GetPlayer()->GetInventory().DeSelect();
	return true;
}

void EventDeselect::Reset()
{

}
