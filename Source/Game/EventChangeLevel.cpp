#include "stdafx.h"
#include "EventChangeLevel.h"
#include "GameWorld.h"

#include "..\CommonUtilities\DL_Debug.h"

EventChangeLevel::EventChangeLevel()
{
}


EventChangeLevel::~EventChangeLevel()
{
	DL_PRINT("Destructor");
}

bool EventChangeLevel::Update(const float aDeltaTime)
{
	(aDeltaTime);
	if (myTargetLevelName != "")
	{
		myGameWorld->ChangeLevel(myTargetLevelName);
		myGameWorld->GetPlayer()->SetPosition(myTargetPosition);
	}
	return true;
}

void EventChangeLevel::Reset()
{
}
