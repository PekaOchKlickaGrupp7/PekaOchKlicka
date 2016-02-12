#include "stdafx.h"
#include "EventAnswer.h"

//Work in progress

EventAnswer::EventAnswer() : mySprite(nullptr)
{
	myTextIndex = 1;
}

EventAnswer::~EventAnswer()
{
	delete mySprite;
	mySprite = nullptr;
}

void EventAnswer::Init(Room* aRoom, CGameWorld* aGameWorld)
{
	Event::Init(aRoom, aGameWorld);
	mySprite = new DX2D::CSprite(mySpritePath.c_str());
}

bool EventAnswer::Update(const float)
{
	return true;
}

void EventAnswer::Render(Synchronizer& aSynchronzier)
{
	/*
	float y = myTextIndex * myBackground.GetSize().y;

	RenderCommand command;
	command.mySprite = myBackground;
	command.myPosition = 

	myBackground->
	myBackground->Render();
	*/
}

void EventAnswer::Reset()
{

}
