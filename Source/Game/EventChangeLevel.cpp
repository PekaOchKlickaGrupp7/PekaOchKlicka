#include "stdafx.h"
#include "EventChangeLevel.h"
#include "GameWorld.h"

#include "..\CommonUtilities\DL_Debug.h"

EventChangeLevel::EventChangeLevel()
{
}


EventChangeLevel::~EventChangeLevel()
{
}

bool EventChangeLevel::Update(const float aDeltaTime)
{
	(aDeltaTime);
	if (myTargetLevelName == "")
	{
		DL_PRINT("Room to change to was empty string");
	}
	myGameWorld->ChangeLevel(myTargetLevelName);
	return true;
}
