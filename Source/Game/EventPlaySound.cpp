#include "stdafx.h"
#include "EventPlaySound.h"
#include "SoundFileHandler.h"

EventPlaySound::EventPlaySound()
{
}

void EventPlaySound::Init(Room* aRoom, CGameWorld* aGameWorld)
{
	Event::Init(aRoom, aGameWorld);

	//SoundFileHandler::GetInstance()->Load(myTargetSound, myIdentifier, myIs3D);
}

bool EventPlaySound::Update(const float aDeltaTime)
{
	(aDeltaTime);

	SoundFileHandler::GetInstance()->SetupStream(myTargetSound, myIdentifier, myIs3D);
	Sound* SoundPtr = SoundFileHandler::GetInstance()->GetSound(myIdentifier);

	
	SoundPtr->SetLooping(myIsLooping);

	if (myIs3D)
	{
		ObjectData* Target = GetGameObject(myTarget);
		if (Target != nullptr)
		{
			DX2D::Vector2f Position = { (Target->myX + myPosition.x) * 10.0f, 5.0f };
			SoundPtr->SetPosition(Position);
		}
	}

	SoundPtr->PlaySound();
	SoundPtr->SetVolume(myVolume);
	return true;
}

void EventPlaySound::Reset()
{

}


EventPlaySound::~EventPlaySound()
{
}
