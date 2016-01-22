#include "stdafx.h"
#include "EventPlaySound.h"
#include "SoundFileHandler.h"

EventPlaySound::EventPlaySound()
{
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
	}
	else
	{
		SoundFileHandler::GetInstance()->GetSound(myTargetSound)->SetLooping(myIsLooping);
		SoundFileHandler::GetInstance()->GetSound(myTargetSound)->Play();
	}
	return true;
}


EventPlaySound::~EventPlaySound()
{
}
