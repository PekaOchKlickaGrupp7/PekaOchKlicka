#include "stdafx.h"
#include "EventChangeImage.h"

EventChangeImage::EventChangeImage()
{
}

EventChangeImage::~EventChangeImage()
{
	ObjectData* data = GetGameObject(myTarget);
	if (data != nullptr && data->mySprite == myNewSprite)
	{
		data->mySprite = nullptr;
	}
	SAFE_DELETE(myNewSprite);
}

void EventChangeImage::Init(Room* aRoom, CGameWorld* aGameWorld)
{
	Event::Init(aRoom, aGameWorld);

	myNewSprite = new DX2D::CSprite(myImagePath.c_str());
}

bool EventChangeImage::Update(const float)
{
	if (myImagePath != "")
	{
		ObjectData* data = GetGameObject(myTarget);

		if (data != nullptr)
		{
			data->mySprite = myNewSprite;
		}
	}
	return true;
}

void EventChangeImage::Reset()
{
}
