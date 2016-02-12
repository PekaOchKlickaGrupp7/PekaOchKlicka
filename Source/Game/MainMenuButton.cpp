#include "stdafx.h"
#include "MainMenuButton.h"
#include "EventManager.h"
#include "EventChangeLevel.h"


MainMenuButton::MainMenuButton()
{
	myEvent = new EventChangeLevel();
	myShouldHover = true;
}

void MainMenuButton::InitValue()
{
	myIsChecked = false;
	
}

void MainMenuButton::ButtonSpecificUpdate()
{
	myEvent->myNextTheme = "None";
	myEvent->myTargetLevelName = "mainmenu";
	myEvent->Init(EventManager::GetInstance()->GetCurrentRoom(), EventManager::GetInstance()->GetGameWorld());
	EventManager::GetInstance()->AddEvent(myEvent);
}

void MainMenuButton::ProgressGet()
{
	
}

void MainMenuButton::ProgressSet()
{
	
}

MainMenuButton::~MainMenuButton()
{
}
