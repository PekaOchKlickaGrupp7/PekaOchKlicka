#include "stdafx.h"
#include "Slider.h"
#include "..\CommonUtilities\Macros.h"

#include "SoundFileHandler.h"


Slider::Slider()
{
	myIsHoveringKnob = false;
	myIsDraggingKnob = false;
}

void Slider::InitValue()
{

}

void Slider::InitSprites(SliderSpriteArguments &aSpriteArguments)
{
	mySlider = new DX2D::CSprite(aSpriteArguments.mySliderPath);
	myKnobRegular = new DX2D::CSprite(aSpriteArguments.myKnobPath);
	myKnobHovered = new DX2D::CSprite(aSpriteArguments.myKnobHoveredPath);
	myKnobPressed = new DX2D::CSprite(aSpriteArguments.myKnobPressedPath);

	myKnob = myKnobRegular;
}

void Slider::InitPositions(SliderPositionArguments &aPositionsArguments)
{
	myKnobRegular->SetPivot(aPositionsArguments.myKnobPivot);
	myKnobHovered->SetPivot(aPositionsArguments.myKnobPivot);
	myKnobPressed->SetPivot(aPositionsArguments.myKnobPivot);

	mySlider->SetPosition(aPositionsArguments.mySliderPosition);
	myKnobRegular->SetPosition(aPositionsArguments.myKnobPosition);
	myKnobHovered->SetPosition(aPositionsArguments.myKnobPosition);
	myKnobPressed->SetPosition(aPositionsArguments.myKnobPosition);
}

float Slider::ConvertValueToPosition()
{
	float finalPosition;

	float beginPos = mySlider->GetPosition().x;
	float endPos = mySlider->GetPosition().x + mySlider->GetSize().x - 0.058f;

	finalPosition = beginPos + (myValue * (endPos - beginPos));

	return finalPosition;
}

float Slider::ConvertPositionToValue(float aPos)
{
	float finalValue;

	float beginPos = mySlider->GetPosition().x;
	float endPos = mySlider->GetPosition().x + mySlider->GetSize().x - 0.058f;

	
	finalValue = (aPos - beginPos) / (endPos - beginPos);

	return finalValue;
}

void Slider::SetAllKnobs(float aPos)
{
	if (aPos < mySlider->GetPosition().x)
	{
		aPos = mySlider->GetPosition().x;
	}
	myKnob->SetPosition({aPos, myKnob->GetPosition().y });
	myKnobRegular->SetPosition({ aPos, myKnob->GetPosition().y });
	myKnobHovered->SetPosition({ aPos, myKnob->GetPosition().y });
	myKnobPressed->SetPosition({ aPos, myKnob->GetPosition().y });
}

void Slider::Update(float)
{
	SetAllKnobs(ConvertValueToPosition());
	ProgressGet();

	if (myIsDraggingKnob == true)
	{
		myKnob = myKnobPressed;

		if (MouseManager::GetInstance()->GetPosition().x > mySlider->GetPosition().x + mySlider->GetSize().x - 0.058f)
		{
			myValue = 1.0f;
			SetAllKnobs(ConvertValueToPosition());
		}
		else if (MouseManager::GetInstance()->GetPosition().x < mySlider->GetPosition().x)
		{
			myValue = 0.0f;
			SetAllKnobs(ConvertValueToPosition());
		}
		else
		{
			myKnob->SetPosition({ MouseManager::GetInstance()->GetPosition().x, myKnob->GetPosition().y });
			myValue = ConvertPositionToValue(MouseManager::GetInstance()->GetPosition().x);
			SetAllKnobs(ConvertValueToPosition());
		}
	}
	else if (myIsHoveringKnob == true && myIsDraggingKnob == false)
	{
		myKnob = myKnobHovered;
	}
	else
	{
		myKnob = myKnobRegular;
	}

	if (OnMouseClick() == true)
	{
		myIsDraggingKnob = true;
		// do drag stuff
	}
	else
	{
		//  stop dragging
		myIsDraggingKnob = false;
	}

	if (OnMouseHover() == true)
	{
		myIsHoveringKnob = true;
	}
	else
	{
		myIsHoveringKnob = false;
	}
	SliderSpecificUpdate();
	ProgressSet();
}

void Slider::ProgressGet()
{

}
void Slider::ProgressSet()
{

}

bool Slider::OnMouseHover()
{
	if (MouseManager::GetInstance()->GetPosition().x >= myKnob->GetPosition().x - myKnob->GetSize().x / 2 &&
		MouseManager::GetInstance()->GetPosition().x <= myKnob->GetPosition().x + myKnob->GetSize().x / 2)
	{
		if (MouseManager::GetInstance()->GetPosition().y + 0.010f >= myKnob->GetPosition().y - myKnob->GetSize().y / 2 &&
			MouseManager::GetInstance()->GetPosition().y - 0.010f <= myKnob->GetPosition().y + myKnob->GetSize().y / 2)
		{
			if (myIsHoveringKnob == false)
			{
				Sound* SoundPtr = SoundFileHandler::GetInstance()->GetSound("ButtonHover");

				SoundPtr->SetLooping(false);
				SoundPtr->PlaySound();
			}

			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

bool Slider::OnMouseClick()
{
	if (OnMouseHover() == true)
	{
		if (MouseManager::GetInstance()->ButtonPressed(eMouseButtons::eLeft) == true)
		{
			if (myIsDraggingKnob == false)
			{
				Sound* SoundPtr = SoundFileHandler::GetInstance()->GetSound("ButtonClick");

				SoundPtr->SetLooping(false);
				SoundPtr->PlaySound();
			}

			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

void Slider::SliderSpecificUpdate()
{

}

void Slider::Render(Synchronizer& aSynchronizer)
{
	RenderCommand renderCommandSlider;
	renderCommandSlider.mySprite = mySlider;
	renderCommandSlider.myType = eRenderType::eSprite;
	renderCommandSlider.myPosition = mySlider->GetPosition();
	aSynchronizer.AddRenderCommand(renderCommandSlider);

	RenderCommand renderCommandKnob;
	renderCommandKnob.mySprite = myKnob;
	renderCommandKnob.myType = eRenderType::eSprite;
	renderCommandKnob.myPosition = myKnob->GetPosition();
	aSynchronizer.AddRenderCommand(renderCommandKnob);
}


Slider::~Slider()
{
	SAFE_DELETE(mySlider);
	SAFE_DELETE(myKnobRegular);
	SAFE_DELETE(myKnobPressed);
	SAFE_DELETE(myKnobHovered);
}
