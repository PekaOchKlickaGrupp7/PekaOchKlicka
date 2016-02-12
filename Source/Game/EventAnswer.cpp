#include "stdafx.h"
#include "EventAnswer.h"

//Work in progress

EventAnswer::EventAnswer() : myBackgroundSprite(nullptr)
{
	myTextIndex = 0;
}

EventAnswer::~EventAnswer()
{
	SAFE_DELETE(myBackgroundSprite);
}

void EventAnswer::Init(Room* aRoom, CGameWorld* aGameWorld)
{
	Event::Init(aRoom, aGameWorld);
	myBackgroundSprite = new DX2D::CSprite("Text/Courier.");
}

bool EventAnswer::Update(const float)
{
	return true;
}

void EventAnswer::Render(Synchronizer& aSynchronzier)
{
	float y = myTextIndex * myBackgroundSprite->GetSize().y;

	RenderCommand command;
	command.mySprite = myBackgroundSprite;
	command.myPosition = { 0, y };
	aSynchronzier.AddRenderCommand(command);
}

void EventAnswer::Reset()
{

}
