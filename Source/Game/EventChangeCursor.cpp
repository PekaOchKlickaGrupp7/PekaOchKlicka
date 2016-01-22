#include "stdafx.h"
#include "EventChangeCursor.h"
#include "MouseManager.h"


EventChangeCursor::EventChangeCursor()
{
}

bool EventChangeCursor::Update(const float aDeltaTime)
{
	MouseManager::GetInstance()->SetInteractiveMode(static_cast<eInteractive>(myTargetCursor));
	return true;
}

void EventChangeCursor::Reset()
{

}

EventChangeCursor::~EventChangeCursor()
{
}
