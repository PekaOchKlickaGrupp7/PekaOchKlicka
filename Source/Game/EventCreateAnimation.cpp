#include "stdafx.h"
#include "EventCreateAnimation.h"
#include "Animation.h"

EventCreateAnimation::EventCreateAnimation()
{
	myFlip = false;
	myIsLooping = true;
}

EventCreateAnimation::~EventCreateAnimation()
{

}

void EventCreateAnimation::Init(Room* aRoom, CGameWorld* aGameWorld)
{
	Event::Init(aRoom, aGameWorld);

	myAnimation = new Animation(myFilePath.c_str(), DX2D::Vector2f(myObjectData->myPivotX, myObjectData->myPivotY), myFrameDuration, myNumberOfFrames, myFramesPerRow, myIsLooping);
	DX2D::CColor color = { 1, 1, 1, 1 };
	myAnimation->SetColor(color);
	myAnimation->SetFlip(myFlip);
	myAnimation->SetSize(myScale);
	myObjectData->myAnimations.Add(myAnimation);
	myObjectData->myIsAnimation = true;
}

bool EventCreateAnimation::Update(const float)
{
	return true;
}

void EventCreateAnimation::Reset()
{

}
