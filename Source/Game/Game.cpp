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

#include "ResolutionManager.h"

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
	myIsFullscreen = false;
	ResolutionManager::GetInstance()->Initialize();

	std::cout << std::endl;
	for (int i = 0; i < argc; ++i)
	{
		std::cout << argv[i] << std::endl;
	}
	std::cout << std::endl;
	unsigned short windowWidth = 1920;
	unsigned short windowHeight = 1080;

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

	DX2D::SEngineCreateParameters createParameters;
	createParameters.myActivateDebugSystems = false;
	createParameters.myInitFunctionToCall = std::bind(&CGame::InitCallBack, this);
	createParameters.myUpdateFunctionToCall = std::bind(&CGame::UpdateCallBack, this);
	createParameters.myLogFunction = std::bind(&CGame::LogCallback, this, _1);
	createParameters.myWindowHeight = windowHeight;
	createParameters.myWindowWidth = windowWidth;
	createParameters.myRenderHeight = windowHeight;
	createParameters.myRenderWidth = windowWidth;
	createParameters.myClearColor.Set(0.0f, 0.0f, 0.0f, 1.0f);

	std::wstring appname = L"Peka Och Klicka Grupp 7";
	createParameters.myStartInFullScreen = myIsFullscreen;
#ifdef _DEBUG
	//createParameters.myWindowHeight = 720;
	//createParameters.myWindowWidth = 1280;
	//createParameters.myRenderHeight = 720;
	//createParameters.myRenderWidth = 1280;
	createParameters.myActivateDebugSystems = true;
	createParameters.myStartInFullScreen = myIsFullscreen;
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
		*DX2D::CEngine::GetInstance()->GetHWND(),
		DX2D::CEngine::GetInstance()->GetWindowSize().x, DX2D::CEngine::GetInstance()->GetWindowSize().y);

	
	myRenderThread = new std::thread(&CGame::Render, this);
	ThreadHelper::SetThreadName(static_cast<DWORD>(-1), "Updater");

	SoundManager::GetInstance(); // Creates a sound manager instance.
	EventManager::CreateInstance();

	ResolutionManager::GetInstance()->Update(DX2D::CEngine::GetInstance()->GetWindowSize().x, DX2D::CEngine::GetInstance()->GetWindowSize().y);
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
	std::cout << "Render x: " << DX2D::CEngine::GetInstance()->GetRenderSize().x << " Render y: " << DX2D::CEngine::GetInstance()->GetRenderSize().y << std::endl;

	ResolutionManager::GetInstance()->Update(DX2D::CEngine::GetInstance()->GetWindowSize().x, DX2D::CEngine::GetInstance()->GetWindowSize().y);


	if (myInputManager.KeyPressed(DIK_F1) == true)
	{
		myIsFullscreen = !myIsFullscreen;
		DX2D::CEngine::GetInstance()->SetFullScreen(myIsFullscreen);
	}

	const float deltaTime = myTimerManager.GetMasterTimer().GetTimeElapsed().GetSecondsFloat();
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
