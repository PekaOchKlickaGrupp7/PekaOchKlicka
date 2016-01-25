#include "stdafx.h"
#include "EventPlaySound.h"
#include "SoundFileHandler.h"

EventPlaySound::EventPlaySound()
{
}

void EventPlaySound::Init(Room* aRoom, CGameWorld* aGameWorld)
{
	myRoom = aRoom;
	myGameWorld = aGameWorld;

	SoundFileHandler::GetInstance()->Load(myTargetSound, myIdentifier);
}

bool EventPlaySound::Update(const float aDeltaTime)
{
	(aDeltaTime);



	if (myIs3D)
	{
		// not yet
		// Get3DSound
		// set looping
		// set position
		// (myObjectData->myX + myPosition.x) * 10.0f;
	}
	else
	{
		SoundFileHandler::GetInstance()->GetSound(myIdentifier)->SetLooping(myIsLooping);
		SoundFileHandler::GetInstance()->GetSound(myIdentifier)->Play();
	}
	return true;
}

void EventPlaySound::Reset()
{

}


EventPlaySound::~EventPlaySound()
{
}
