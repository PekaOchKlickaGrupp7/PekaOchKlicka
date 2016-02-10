#include "stdafx.h"
#include "SFXSlider.h"
#include "SoundManager.h"


SFXSlider::SFXSlider()
{
}

void SFXSlider::InitValue()
{
	SoundManager::GetInstance()->GetChannelGroup("SFX")->getVolume(&myValue);
}

void SFXSlider::SliderSpecificUpdate()
{
	//SoundManager::GetInstance()->GetChannelGroup("SFX")->setVolume(myValue);
}

void SFXSlider::ProgressGet()
{
	SoundManager::GetInstance()->GetChannelGroup("SFX")->getVolume(&myValue);
}
void SFXSlider::ProgressSet()
{
	SoundManager::GetInstance()->GetChannelGroup("SFX")->setVolume(myValue);
}

SFXSlider::~SFXSlider()
{
}
