#pragma once
#include <tga2d/Engine.h>
#include "MainMenuState.h"

#include "..\CommonUtilities\TimerManager.h"
#include "..\CommonUtilities\InputWrapper.h"
#include "..\CommonUtilities\DL_Debug.h"

#include "Synchronizer.h"
#include <thread>
#include "Renderer.h"

#include "StateStack.h"
#include "StateStackProxy.h"

#include "ResolutionManager.h"

class CGame
{
public:
	CGame();
	~CGame();
	void Init(const char** argv, const int argc);

	static std::string testLevel;

private:
	void InitCallBack();
	void UpdateCallBack();
	void LogCallback(std::string aText);

	const bool Update();
	void Render();

	StateStack myStateStack;
	StateStackProxy myStateStackProxy;

	CU::TimeSys::TimerManager myTimerManager;
	CU::DirectInput::InputWrapper myInputManager;

	Synchronizer mySynchronizer;
	Renderer myRenderer;

	std::thread* myRenderThread;
	volatile bool myQuit;

	ResolutionManager myResolutionManager;
};
