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
		myGameWorld->GetPlayer()->SetIsMoving(false);
		myGameWorld->GetPlayer()->SetPosition(myTargetPosition);
		myGameWorld->GetPlayer()->SetPreviousPosition(myTargetPosition);
		return true;
	}
	return false;
}

void EventChangeLevel::Reset()
{
}
