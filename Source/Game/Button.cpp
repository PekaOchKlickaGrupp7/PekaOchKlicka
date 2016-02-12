#include "stdafx.h"
#include "Button.h"
#include "..\CommonUtilities\Macros.h"


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
			myIsChecked = !myIsChecked;
			ButtonSpecificUpdate();
			UpdateImage();
		}
	}

	
	ProgressSet();
}

bool Button::OnMouseHover()
{
	if (MouseManager::GetInstance()->GetPosition().x >= mySprite->GetPosition().x - mySprite->GetSize().x / 2 &&
		MouseManager::GetInstance()->GetPosition().x <= mySprite->GetPosition().x + mySprite->GetSize().x / 2)
	{
		if (MouseManager::GetInstance()->GetPosition().y >= mySprite->GetPosition().y - mySprite->GetSize().y / 2 &&
			MouseManager::GetInstance()->GetPosition().y <= mySprite->GetPosition().y + mySprite->GetSize().y / 2)
		{
			myIsHovering = true;
			return true;
		}
		else
		{
			myIsHovering = false;
			return false;
		}
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
