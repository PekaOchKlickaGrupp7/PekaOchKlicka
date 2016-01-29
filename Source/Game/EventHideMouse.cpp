#include "stdafx.h"
#include "EventHideMouse.h"
#include "MouseManager.h"

EventHideMouse::EventHideMouse()
{
}

bool EventHideMouse::Update(const float aDeltaTime)
{
	MouseManager::GetInstance()->SetHideGameMouse(myHideGameMouse);
	return true;
}

void EventHideMouse::Reset()
{

}

EventHideMouse::~EventHideMouse()
{
}
