#include "stdafx.h"
#include "EventChangeLevel.h"
#include "GameWorld.h"

#include "..\CommonUtilities\DL_Debug.h"
#include "EventVariablesManager.h"

EventChangeLevel::EventChangeLevel()
{
	myUseFading = false;
	Reset();
}


EventChangeLevel::~EventChangeLevel()
{

}

bool EventChangeLevel::Update(const float aDeltaTime)
{
	if (myTargetLevelName != "")
	{
		bool doChange = true;
		if (myUseFading == true)
		{
			myGameWorld->SetFadeIn(true);
			doChange = myGameWorld->GetFadeIn() == 0.0f;
		}
		if (doChange == true)
		{
			myTime += aDeltaTime;
			if (myUseFading == false || myTime >= 0.5f)
			{
				myGameWorld->ChangeLevel(myTargetLevelName);
				myGameWorld->GetPlayer()->SetIsMoving(false);
				myGameWorld->GetPlayer()->SetPosition(myTargetPosition);
				myGameWorld->GetPlayer()->SetPreviousPosition(myTargetPosition);
				myGameWorld->SetFadeIn(false);


				if (myTargetLevelName == "mainmenu" || myTargetLevelName == "splashscreen" || myTargetLevelName == "optionsmenu")
				{
					EventVariablesManager::GetInstance()->SetVariable(true, "InMenu");
				}
				else
				{
					EventVariablesManager::GetInstance()->SetVariable(false, "InMenu");
				}


				return true;
			}
		}
	}
	return false;
}

void EventChangeLevel::Reset()
{
	myTime = 0.0f;
}
