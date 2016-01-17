#include "stdafx.h"

#include "Game.h"

#include <tga2d/error/error_manager.h>

#include <functional>
#include <time.h>
#include "..\CommonUtilities\ThreadHelper.h"

#include "GameWorld.h"
#include "SoundManager.h"
#include "EventManager.h"
#include <iostream>

using namespace std::placeholders;


#ifdef _DEBUG
#pragma comment(lib,"DX2DEngine_Debug.lib")

#endif // DEBUG
#ifdef NDEBUG
#pragma comment(lib,"DX2DEngine_Release.lib")
#endif // DEBUG

std::string CGame::myTestLevel = "";

CGame::CGame() :
myStateStackProxy(myStateStack),
myRenderer()
{
	myRenderThread = nullptr;
	myQuit = false;
}


CGame::~CGame()
{
	myStateStack.Clear();
	mySynchronizer.Quit();
	myRenderThread->join();
	delete myRenderThread;
	myRenderThread = nullptr;

	SoundManager::DestroyInstance();
	EventManager::DestroyInstance();
}


void CGame::Init(const char** argv, const int argc)
{

	std::cout << std::endl;
	for (int i = 0; i < argc; ++i)
	{
		std::cout << argv[i] << std::endl;
	}
	std::cout << std::endl;

	if (argc == 2)
	{
		//Test level
		std::string str = argv[1];
		while (str.find("%") != str.npos)
		{
			str = str.replace(str.find("%"), 1, " ");
		}


		myTestLevel = str;
		std::cout << "Level: " << myTestLevel << std::endl; 
	}

	myResolutionManager.Initialize({0,0});

	unsigned short windowWidth = 1280;
	unsigned short windowHeight = 720;

	DX2D::SEngineCreateParameters createParameters;
	createParameters.myActivateDebugSystems = false;
	createParameters.myInitFunctionToCall = std::bind(&CGame::InitCallBack, this);
	createParameters.myUpdateFunctionToCall = std::bind(&CGame::UpdateCallBack, this);
	createParameters.myLogFunction = std::bind(&CGame::LogCallback, this, _1);
	createParameters.myWindowHeight = windowHeight;
	createParameters.myWindowWidth = windowWidth;
	createParameters.myRenderHeight = windowHeight;
	createParameters.myRenderWidth = windowWidth;
	createParameters.myClearColor.Set(1.0f, 1.0f, 1.0f, 1.0f);

	std::wstring appname = L"Peka Och Klicka Grupp 7";
	createParameters.myStartInFullScreen = false;
#ifdef _DEBUG
	createParameters.myActivateDebugSystems = true;
	createParameters.myStartInFullScreen = false;
	appname = L"Peka Och Klicka Grupp 7 DEBUG";
#endif

	createParameters.myApplicationName = appname;
	createParameters.myEnableVSync = false;

	DX2D::CEngine::CreateInstance(createParameters);
	if (!DX2D::CEngine::GetInstance()->Start())
	{
		ERROR_AUTO_PRINT("Fatal error! Engine could not start!");
	}
}


void CGame::InitCallBack()
{
	DL_Debug::Debug::Create();

	myInputManager.Initialize(DX2D::CEngine::GetInstance()->GetHInstance(), 
		*DX2D::CEngine::GetInstance()->GetHWND());

	myRenderThread = new std::thread(&CGame::Render, this);
	ThreadHelper::SetThreadName(static_cast<DWORD>(-1), "Updater");

	SoundManager::GetInstance(); // Creates a sound manager instance.
	EventManager::CreateInstance();

	if (myTestLevel.size() > 0)
	{
		myStateStack.PushMainGameState(new CGameWorld(myStateStackProxy, myInputManager, myTimerManager));
	}
	else
	{
		myStateStack.PushMainGameState(new MainMenuState(myStateStackProxy, myInputManager, myTimerManager));
	}
}

const bool CGame::Update()
{
	const float deltaTime = static_cast<float>(myTimerManager.GetMasterTimer().GetTimeElapsed().GetSeconds());
	EventManager::GetInstance()->Update(deltaTime);
	if (myStateStack.UpdateCurrentState(deltaTime) == true)
	{
		myStateStack.RenderCurrentState(mySynchronizer);
		return false;
	}
	return true;
}

void CGame::Render()
{
	ThreadHelper::SetThreadName(static_cast<DWORD>(-1), "Renderer");
	while (mySynchronizer.HasQuit() == false)
	{
		mySynchronizer.WaitForLogic();

		mySynchronizer.SwapBuffer();
		myRenderer.Render(mySynchronizer);
		
		mySynchronizer.RenderIsDone();
	}
}

void CGame::UpdateCallBack()
{
	myTimerManager.UpdateTimers();
	myInputManager.Update();

	myQuit = Update();

	mySynchronizer.LogicIsDone();
	if (myQuit == true)
	{
		DX2D::CEngine::Destroy();
	}
	else
	{
		mySynchronizer.WaitForRender();
	}
}


void CGame::LogCallback(std::string aText)
{
}
