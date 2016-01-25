#include "stdafx.h"
#include "EventQuit.h"
#include "GameWorld.h"

EventQuit::EventQuit()
{
}

EventQuit::~EventQuit()
{
}

bool EventQuit::Update(const float)
{
	myGameWorld->Quit();
	return true;
}

void EventQuit::Reset()
{
	
}
