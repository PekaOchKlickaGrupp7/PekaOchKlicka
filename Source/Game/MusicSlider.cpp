#include "stdafx.h"
#include "MusicSlider.h"
#include "SoundManager.h"
#include <iostream>

MusicSlider::MusicSlider()
{
}

void MusicSlider::InitValue()
{
	SoundManager::GetInstance()->GetChannelGroup("Music")->getVolume(&myValue);
}

void MusicSlider::SliderSpecificUpdate()
{
	
}

void MusicSlider::ProgressGet()
{
	SoundManager::GetInstance()->GetChannelGroup("Music")->getVolume(&myValue);
	std::cout << "Music volume: " << myValue << std::endl;
}
void MusicSlider::ProgressSet()
{
	SoundManager::GetInstance()->GetChannelGroup("Music")->setVolume(myValue);
}

MusicSlider::~MusicSlider()
{
}
