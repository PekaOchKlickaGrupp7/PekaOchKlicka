#include "stdafx.h"
#include "EventAnswer.h"
#include "MouseManager.h"
#include "GameWorld.h"

#include "SoundFileHandler.h"

//Work in progress

EventAnswer::EventAnswer() : myBackgroundSprite(nullptr)
{
	myTextIndex = 0;
}

EventAnswer::~EventAnswer()
{
	SAFE_DELETE(myBackgroundSprite);
	SAFE_DELETE(myTextRender);
}

void EventAnswer::Init(Room* aRoom, CGameWorld* aGameWorld)
{
	Event::Init(aRoom, aGameWorld);
	myTextRender = new DX2D::CText("Text/PassionOne-Regular.ttf_sdf");
	myTextRender->myText = myText;
	myTextRender->mySize = 0.3f;

	myHighlightColor = { 1.0f, 0.45f, 0.0f, 1.0f };

	myBackgroundSprite = new DX2D::CSprite("Sprites/TalkBackground.dds");
	myBackgroundSprite->SetColor({ 1.0f, 1.0f, 1.0f, 0.5f });

	myHeight = myBackgroundSprite->GetSize().y;
	myY = myTextIndex * myBackgroundSprite->GetSize().y;

	myTextRender->myPosition = { 0.02f, (myY + myBackgroundSprite->GetSize().y / 2.0f) - myTextRender->GetHeight() / 2.0f};
}

bool EventAnswer::Update(const float)
{
	DX2D::Vector2f mousePos = MouseManager::GetInstance()->GetPosition();

	myGameWorld->SetTalkIsOn();
	if (mousePos.y > myY && mousePos.y < myY + myHeight)
	{
		if (myIsInside == false)
		{
			//make sure to only play once
			Sound* SoundPtr = SoundFileHandler::GetInstance()->GetSound("ButtonHover");

			SoundPtr->SetLooping(false);
			SoundPtr->PlaySound();
			SoundPtr->SetVolume(0.4f);
		}

		myIsInside = true;

		myTextRender->myColor = myHighlightColor;
		if (MouseManager::GetInstance()->ButtonClicked(eMouseButtons::eLeft) == true)
		{
			myGameWorld->SetTalkIsOff();
			return true;
		}
	}
	else
	{
		myIsInside = false;
		myTextRender->myColor = myColor;
	}
	
	return false;
}

void EventAnswer::Render(Synchronizer& aSynchronzier)
{

	RenderCommand command;
	command.myType = eRenderType::eSprite;
	command.mySprite = myBackgroundSprite;
	command.myPosition = { 0, myY };
	aSynchronzier.AddRenderCommand(command);

	command.myType = eRenderType::eText;
	command.myText = myTextRender;
	command.myPosition = myTextRender->myPosition;
	aSynchronzier.AddRenderCommand(command);
}

void EventAnswer::Reset()
{

}
