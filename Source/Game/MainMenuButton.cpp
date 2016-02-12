#include "stdafx.h"
#include "MainMenuButton.h"
#include "EventManager.h"
#include "EventChangeLevel.h"
#include "GameWorld.h"

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
	EventManager::GetInstance()->GetGameWorld()->GetOptions()->SetActive(false);
	EventManager::GetInstance()->GetGameWorld()->ResetGame();
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
