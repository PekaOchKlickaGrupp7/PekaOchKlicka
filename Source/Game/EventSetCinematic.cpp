#include "stdafx.h"
#include "EventSetCinematic.h"
#include "GameWorld.h"

EventSetCinematic::EventSetCinematic()
{
}

EventSetCinematic::~EventSetCinematic()
{
}

bool EventSetCinematic::Update(const float)
{
	myGameWorld->SetCinematicMode(mySetOn);
	return true;
}

void EventSetCinematic::Reset()
{

}
