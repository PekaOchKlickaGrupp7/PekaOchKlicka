#pragma once
#include <tga2d/Engine.h>
#include "GameWorld.h"

#include "..\CommonUtilities\TimerManager.h"
#include "..\CommonUtilities\InputWrapper.h"
#include "..\CommonUtilities\DL_Debug.h"

class CGame
{
public:
	CGame();
	~CGame();
	void Init();
private:
	void InitCallBack();
	void UpdateCallBack();
	void LogCallback(std::string aText);

	CGameWorld myGameWorld;

	CU::TimeSys::TimerManager myTimerManager;
	CU::DirectInput::InputWrapper myInputManager;
};
