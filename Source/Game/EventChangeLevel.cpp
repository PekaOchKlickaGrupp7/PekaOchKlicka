#include "stdafx.h"
#include "EventChangeLevel.h"
#include "GameWorld.h"

#include "..\CommonUtilities\DL_Debug.h"

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

				// Set InMenu to FALSE if in game, and TRUE if in menu. (so that the cursor wont scale by Y-pos in a menu)
				// if(bla bla level is menu)
					// EventVariableManager::GetInstance()->SetVariable(true, "InMenu");


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
