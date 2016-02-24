#include "stdafx.h"
#include "Button.h"
#include "..\CommonUtilities\Macros.h"

#include "SoundFileHandler.h"

#include <iostream>

Button::Button()
{
	myIsHovering = false;
	myShouldHover = false;
}

void Button::InitValue()
{

}

void Button::InitSprites(ButtonSpriteArguments &aSpriteArguments)
{
	myButtonChecked = new DX2D::CSprite(aSpriteArguments.myButtonCheckedPath);
	myButtonUnChecked = new DX2D::CSprite(aSpriteArguments.myButtonUnCheckedPath);
	myButtonHovered = new DX2D::CSprite(aSpriteArguments.myButtonHoveredPath);

	mySprite = myButtonUnChecked;
}

void Button::InitPositions(ButtonPositionArguments &aPositionsArguments)
{
	myButtonChecked->SetPivot(aPositionsArguments.myButtonPivot);
	myButtonUnChecked->SetPivot(aPositionsArguments.myButtonPivot);
	myButtonHovered->SetPivot(aPositionsArguments.myButtonPivot);

	myButtonChecked->SetPosition(aPositionsArguments.myButtonPosition);
	myButtonUnChecked->SetPosition(aPositionsArguments.myButtonPosition);
	myButtonHovered->SetPosition(aPositionsArguments.myButtonPosition);
}

void Button::UpdateImage()
{
	if (myShouldHover == true)
	{
		if (myIsHovering == false)
		{
			if (myIsChecked == true)
			{
				mySprite = myButtonChecked;
			}
			else
			{
				mySprite = myButtonUnChecked;
			}
		}
		else
		{
			mySprite = myButtonHovered;
		}
	}
	else
	{
		if (myIsChecked == true)
		{
			mySprite = myButtonChecked;
		}
		else
		{
			mySprite = myButtonUnChecked;
		}
	}
}

void Button::Update(float)
{
	ProgressGet();
	UpdateImage();

	if (OnMouseHover() == true)
	{
		if (MouseManager::GetInstance()->ButtonClicked(eMouseButtons::eLeft) == true)
		{
			Sound* SoundPtr = SoundFileHandler::GetInstance()->GetSound("ButtonClick");

			SoundPtr->SetLooping(false);
			SoundPtr->PlaySound();
			SoundPtr->SetVolume(0.4f);

			myIsChecked = !myIsChecked;
			ButtonSpecificUpdate();
			MouseManager::GetInstance()->SetInteractiveMode(eInteractive::eRegular);
			UpdateImage();
		}
	}

	
	ProgressSet();
}

bool Button::OnMouseHover()
{
	if (MouseManager::GetInstance()->GetPosition().x >= mySprite->GetPosition().x &&
		MouseManager::GetInstance()->GetPosition().x <= mySprite->GetPosition().x + mySprite->GetSize().x - (0.056f / 2))
	{
		if (MouseManager::GetInstance()->GetPosition().y >= mySprite->GetPosition().y &&
			MouseManager::GetInstance()->GetPosition().y <= mySprite->GetPosition().y + mySprite->GetSize().y - (0.0111f / 2))
		{
			if (myIsHovering == false)
			{
				Sound* SoundPtr = SoundFileHandler::GetInstance()->GetSound("ButtonHover");
				

				SoundPtr->SetLooping(false);
				SoundPtr->PlaySound();
				SoundPtr->SetVolume(0.4f);

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

void Button::ButtonSpecificUpdate()
{

}

void Button::Render(Synchronizer &aSynchronizer)
{
	RenderCommand renderCommandButton;
	renderCommandButton.mySprite = mySprite;
	renderCommandButton.myPosition = mySprite->GetPosition();
	renderCommandButton.myType = eRenderType::eSprite;
	aSynchronizer.AddRenderCommand(renderCommandButton);
}


Button::~Button()
{
	SAFE_DELETE(myButtonChecked);
	SAFE_DELETE(myButtonUnChecked);
}
