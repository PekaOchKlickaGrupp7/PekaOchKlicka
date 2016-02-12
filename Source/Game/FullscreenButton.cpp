#include "stdafx.h"
#include "FullscreenButton.h"
#include "EventVariablesManager.h"
#include "ResolutionManager.h"

FullscreenButton::FullscreenButton()
{
}

void FullscreenButton::InitValue()
{
	EventVariablesManager::GetInstance()->GetVariable(myIsChecked, "IsFullscreen");
}

void FullscreenButton::ButtonSpecificUpdate()
{
	ResolutionManager::GetInstance()->ToggleFullscreen();
}

void FullscreenButton::ProgressGet()
{
	EventVariablesManager::GetInstance()->GetVariable(myIsChecked, "IsFullscreen");
}

void FullscreenButton::ProgressSet()
{
	EventVariablesManager::GetInstance()->SetVariable(myIsChecked, "IsFullscreen");
}


FullscreenButton::~FullscreenButton()
{
}
