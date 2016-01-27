#include "stdafx.h"
#include "EventToggleFullscreen.h"
#include "ResolutionManager.h"

EventToggleFullscreen::EventToggleFullscreen()
{
}

EventToggleFullscreen::~EventToggleFullscreen()
{
}

void EventToggleFullscreen::Init(Room* aRoom, CGameWorld* aGameWorld)
{
	myRoom = aRoom;
	myGameWorld = aGameWorld;
}

bool EventToggleFullscreen::Update(const float)
{
	ResolutionManager::GetInstance()->ToggleFullscreen();
	return true;
}

void EventToggleFullscreen::Reset()
{
}
