#include "stdafx.h"
#include "EventChangeLevel.h"
#include "GameWorld.h"

#include "..\CommonUtilities\DL_Debug.h"
#include "EventVariablesManager.h"
#include "MusicManager.h"
#include "EventManager.h"

EventChangeLevel::EventChangeLevel()
{
	myUseFading = false;
	myPlayerDirection = -1;
	myTargetPosition = { 0, 0 };
	myResetGame = false;
	Reset();
}

EventChangeLevel::~EventChangeLevel()
{

}

bool EventChangeLevel::Update(const float aDeltaTime)
{
	if (myTargetLevelName != "")
	{
		if (myCached == false)
		{
			MusicManager::GetInstance()->ChangeTheme(myNextTheme);
			myCached = true;
		}

		bool doChange = true;
		if (myUseFading == true)
		{
			myGameWorld->SetFadeIn(true);
			doChange = myGameWorld->GetFadeIn() == 0.0f;
		}
		if (doChange == true)
		{
			myTime += aDeltaTime;
			if (myUseFading == false || myTime >= 0.2f)
			{
				if (myResetGame == true)
				{
					EventManager::GetInstance()->GetGameWorld()->ResetGame();
				}
				myGameWorld->ChangeLevel(myTargetLevelName);
				myGameWorld->GetPlayer()->SetPosition(myTargetPosition);
				myGameWorld->GetPlayer()->SetPreviousPosition(myTargetPosition);
				if (myPlayerDirection > -1)
				{
					myGameWorld->GetPlayer()->SetAnimation(myPlayerDirection);
				}
				myGameWorld->SetFadeIn(false);
				myGameWorld->SetTalkIsOff();
				MouseManager::GetInstance()->SetHideGameMouse(false);

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
	myCached = false;
}
