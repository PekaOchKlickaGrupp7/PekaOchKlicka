#include "stdafx.h"
#include "Sound.h"
#include "SoundFileHandler.h"
#include "EventChangeSoundPosition.h"


EventChangeSoundPosition::EventChangeSoundPosition()
{
}

void EventChangeSoundPosition::Init(Room* aRoom, CGameWorld* aGameWorld)
{
	Event::Init(aRoom, aGameWorld);
}

bool EventChangeSoundPosition::Update(const float aDeltaTime)
{
	(aDeltaTime);

	Sound* SoundPtr = SoundFileHandler::GetInstance()->GetSound(myIdentifier);

	ObjectData* Target = GetGameObject(myTarget);
	if (Target != nullptr)
	{
		DX2D::Vector2f Position = { (Target->myX + myPosition.x) * 10.0f, 5.0f };
		SoundPtr->SetPosition(Position);
	}
	return true;
}


void EventChangeSoundPosition::Reset()
{

}

EventChangeSoundPosition::~EventChangeSoundPosition()
{
}
