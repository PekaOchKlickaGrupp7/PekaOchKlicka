#include "stdafx.h"
#include "EventAnswer.h"

//Work in progress


EventAnswer::EventAnswer() : mySprite(nullptr)
{

}


EventAnswer::~EventAnswer()
{
	delete mySprite;
	mySprite = nullptr;

	myTextOptions.DeleteAll();
}


void EventAnswer::Init(Room* aRoom, CGameWorld* aGameWorld)
{
	Event::Init(aRoom, aGameWorld);
	mySprite = new DX2D::CSprite(mySpritePath.c_str());

	myOptions.Init(4);

}