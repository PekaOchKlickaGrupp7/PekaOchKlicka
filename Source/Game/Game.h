#pragma once
#include <tga2d/Engine.h>

#include "..\CommonUtilities\TimerManager.h"
#include "..\CommonUtilities\InputManager.h"
#include "..\CommonUtilities\DL_Debug.h"

#include "Synchronizer.h"
#include <thread>
#include "Renderer.h"
#include "JSON.h"

#include "StateStack.h"
#include "StateStackProxy.h"

class CGame
{
public:
	CGame();
	~CGame();
	void Init(const char** argv, const int argc);

	static std::string myTestLevel;

private:
	void InitCallBack();
	void UpdateCallBack();
	void LogCallback(std::string aText);

	const bool Update();
	void Render();

	//JSON myJson;
	std::map<std::string, Room*> myRooms;

	StateStack myStateStack;
	StateStackProxy myStateStackProxy;

	CU::TimeSys::TimerManager myTimerManager;
	CU::DirectInput::InputManager myInputManager;

	Synchronizer mySynchronizer;
	Renderer myRenderer;

	std::thread* myRenderThread;
	volatile bool myQuit;
};
