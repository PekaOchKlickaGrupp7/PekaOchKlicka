#include "stdafx.h"
#include "EventChangeLevel.h"
#include "GameWorld.h"

EventChangeLevel::EventChangeLevel()
{
}


EventChangeLevel::~EventChangeLevel()
{
}

bool EventChangeLevel::Update(const float aDeltaTime)
{
	myGameWorld->ChangeLevel("Test");
	return true;
}
