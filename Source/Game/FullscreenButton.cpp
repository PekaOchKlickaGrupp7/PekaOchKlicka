#include "stdafx.h"
#include "FullscreenButton.h"
#include "EventVariablesManager.h"
#include "ResolutionManager.h"

#include "SoundFileHandler.h"

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

bool FullscreenButton::OnMouseHover()
{
	if (MouseManager::GetInstance()->GetPosition().x >= mySprite->GetPosition().x - mySprite->GetSize().x / 2 &&
		MouseManager::GetInstance()->GetPosition().x <= mySprite->GetPosition().x + mySprite->GetSize().x / 2)
	{
		if (MouseManager::GetInstance()->GetPosition().y >= mySprite->GetPosition().y - mySprite->GetSize().y / 2 &&
			MouseManager::GetInstance()->GetPosition().y <= mySprite->GetPosition().y + mySprite->GetSize().y / 2)
		{
			if (myIsHovering == false)
			{
				Sound* SoundPtr = SoundFileHandler::GetInstance()->GetSound("ButtonHover");
				

				SoundPtr->SetLooping(false);
				SoundPtr->PlaySound();
				SoundPtr->SetVolume(0.6f);

				MouseManager::GetInstance()->SetInteractiveMode(eInteractive::eActive);
			}


			myIsHovering = true;
			return true;
		}
	}

	if (myIsHovering == true)
	{
		MouseManager::GetInstance()->SetInteractiveMode(eInteractive::eRegular);
	}

	myIsHovering = false;
	return false;
}


FullscreenButton::~FullscreenButton()
{
}
