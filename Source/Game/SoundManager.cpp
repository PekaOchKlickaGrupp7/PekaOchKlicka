#include "stdafx.h"
#include "SoundManager.h"

SoundManager::SoundManager()
{
	if (FMOD::System_Create(&m_pSystem) != FMOD_OK)
	{
		// report error
		return;
	}

	int driverCount = 0;
	m_pSystem->getNumDrivers(&driverCount);

	if (driverCount == 0)
	{
		// report error
		return;
	}

	m_pSystem->init(36, FMOD_INIT_NORMAL, nullptr);
}

void SoundManager::createSound(SoundClass *pSound, const char* pFile)
{
	// FMOD_CREATESAMPLE compresses and streams. FMOD_DEFAULT is higher qual-ish
	m_pSystem->createSound(pFile, FMOD_CREATESAMPLE, 0, pSound);
}

void SoundManager::playSound(SoundClass pSound, bool bLoop)
{
	if (!bLoop)
	{
		pSound->setMode(FMOD_LOOP_OFF);
	}
	else
	{
		pSound->setMode(FMOD_LOOP_NORMAL);
		pSound->setLoopCount(-1);
	}

	FMOD::Channel *channel = nullptr;
	m_pSystem->playSound(pSound, nullptr, false, &channel);
}

void SoundManager::releaseSound(SoundClass pSound)
{
	pSound->release();
}


SoundManager::~SoundManager()
{
}
