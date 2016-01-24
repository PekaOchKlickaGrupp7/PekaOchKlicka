#include "stdafx.h"
#include "EventChangeToOriginalImage.h"

EventChangeToOriginalImage::EventChangeToOriginalImage()
{
}

EventChangeToOriginalImage::~EventChangeToOriginalImage()
{
}

bool EventChangeToOriginalImage::Update(const float aDeltaTime)
{
	myObjectData->mySprite = myObjectData->myOriginalSprite;
	return true;
}

void EventChangeToOriginalImage::Reset()
{

}
