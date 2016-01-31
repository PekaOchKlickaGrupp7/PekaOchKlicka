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
	Event::Init(aRoom, aGameWorld);
}

bool EventToggleFullscreen::Update(const float)
{
	ResolutionManager::GetInstance()->ToggleFullscreen();
	return true;
}

void EventToggleFullscreen::Reset()
{
}
