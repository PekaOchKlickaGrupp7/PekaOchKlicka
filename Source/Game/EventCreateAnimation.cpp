#include "stdafx.h"
#include "EventCreateAnimation.h"
#include "Animation.h"

EventCreateAnimation::EventCreateAnimation()
{
	myFlip = false;
}

EventCreateAnimation::~EventCreateAnimation()
{

}

void EventCreateAnimation::Init(Room* aRoom, CGameWorld* aGameWorld)
{
	Event::Init(aRoom, aGameWorld);

	Animation* anim = new Animation(myFilePath.c_str(), DX2D::Vector2f(myObjectData->myPivotX, myObjectData->myPivotY), myFrameDuration, myNumberOfFrames, myFramesPerRow);
	DX2D::CColor color = { 1, 1, 1, 1 };
	anim->SetColor(color);
	anim->SetFlip(myFlip);
	anim->SetSize(myScale);
	myObjectData->myAnimations.Add(anim);
	myObjectData->myIsAnimation = true;
}

bool EventCreateAnimation::Update(const float)
{
	return true;
}

void EventCreateAnimation::Reset()
{

}
