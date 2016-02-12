#include "stdafx.h"
#include "ResumeButton.h"
#include "EventManager.h"
#include "GameWorld.h"

ResumeButton::ResumeButton()
{
	myShouldHover = true;
}

void ResumeButton::InitValue()
{
	myIsChecked = false;
}

void ResumeButton::ButtonSpecificUpdate()
{
	EventManager::GetInstance()->GetGameWorld()->GetOptions()->SetActive(false);
}

void ResumeButton::ProgressGet()
{

}

void ResumeButton::ProgressSet()
{

}

ResumeButton::~ResumeButton()
{
}
