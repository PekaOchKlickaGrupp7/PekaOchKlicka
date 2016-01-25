#include "stdafx.h"
#include "EventChangeToOriginalImage.h"

EventChangeToOriginalImage::EventChangeToOriginalImage()
{
}

EventChangeToOriginalImage::~EventChangeToOriginalImage()
{
}

bool EventChangeToOriginalImage::Update(const float)
{
	myObjectData->mySprite = myObjectData->myOriginalSprite;
	return true;
}

void EventChangeToOriginalImage::Reset()
{

}
