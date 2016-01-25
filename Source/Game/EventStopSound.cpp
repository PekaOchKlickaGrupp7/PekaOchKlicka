#include "stdafx.h"
#include "EventStopSound.h"
#include "SoundFileHandler.h"

EventStopSound::EventStopSound()
{
}

bool EventStopSound::Update(const float aDeltaTime)
{
	(aDeltaTime);
	SoundFileHandler::GetInstance()->GetSound(myTargetSound)->Stop();
	return true;
}

void EventStopSound::Reset()
{

}


EventStopSound::~EventStopSound()
{
}
