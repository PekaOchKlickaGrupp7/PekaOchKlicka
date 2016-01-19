#include "stdafx.h"

#include "Game.h"

#include <tga2d/error/error_manager.h>

#include <functional>
#include <time.h>



#include "..\CommonUtilities\ThreadHelper.h"

#include <iostream>

#include "ResolutionManager.h"

using namespace std::placeholders;


#ifdef _DEBUG
#pragma comment(lib,"DX2DEngine_Debug.lib")

#endif // DEBUG
#ifdef NDEBUG
#pragma comment(lib,"DX2DEngine_Release.lib")
#endif // DEBUG


CGame::CGame() :
myStateStackProxy(myStateStack),
myRenderer()
{
	myRenderThread = nullptr;
	myQuit = false;
}


CGame::~CGame()
{
	myRenderThread->join();
	mySynchronizer.Quit();


	myStateStack.Clear();

	delete myRenderThread;
	myRenderThread = nullptr;
	DL_Debug::Debug::Destroy();
	DX2D::CEngine::DestroyInstance();
}


void CGame::Init()
{
	myIsFullscreen = false;
	ResolutionManager::GetInstance()->Initialize();

	unsigned short windowWidth = 1920;
	unsigned short windowHeight = 1080;


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
	myInputManager.Initialize(DX2D::CEngine::GetInstance()->GetHInstance(), *DX2D::CEngine::GetInstance()->GetHWND());
	myInputManager.SetMouseLocation(ResolutionManager::GetInstance()->GetMonitorResolution().x / 2, ResolutionManager::GetInstance()->GetMonitorResolution().y / 2);
	myRenderThread = new std::thread(&CGame::Render, this);
	ThreadHelper::SetThreadName(static_cast<DWORD>(-1), "Updater");
	myStateStack.PushMainGameState(new CGameWorld(myStateStackProxy, myInputManager, myTimerManager));

	ResolutionManager::GetInstance()->Update(DX2D::CEngine::GetInstance()->GetWindowSize().x, DX2D::CEngine::GetInstance()->GetWindowSize().y);
}
const bool CGame::Update()
{
	std::cout << "Render x: " << DX2D::CEngine::GetInstance()->GetRenderSize().x << " Render y: " << DX2D::CEngine::GetInstance()->GetRenderSize().y << std::endl;

	ResolutionManager::GetInstance()->Update(DX2D::CEngine::GetInstance()->GetWindowSize().x, DX2D::CEngine::GetInstance()->GetWindowSize().y);


	if (myInputManager.GetKeyWasPressed(DIK_F1) == true)
	{
		myIsFullscreen = !myIsFullscreen;
		DX2D::CEngine::GetInstance()->SetFullScreen(myIsFullscreen);
	}

	const float deltaTime = myTimerManager.GetMasterTimer().GetTimeElapsed().GetSecondsFloat();
	if (myStateStack.UpdateCurrentState(deltaTime) == true)
	{
		myStateStack.RenderCurrentState(mySynchronizer);
	}
	else
	{


		myQuit = true;
	}


	if (myQuit == true)
	{
		return true;
	}
	return false;
}

void CGame::Render()
{
	ThreadHelper::SetThreadName(static_cast<DWORD>(-1), "Renderer");
	while (mySynchronizer.HasQuit() == false)
	{
		mySynchronizer.WaitForLogic();
		myRenderer.Render(mySynchronizer);

		mySynchronizer.SwapBuffer();
		mySynchronizer.RenderIsDone();
	}
}

void CGame::UpdateCallBack()
{
	myTimerManager.UpdateTimers();
	myInputManager.Update();

	myQuit = Update();

	mySynchronizer.LogicIsDone();
	mySynchronizer.WaitForRender();
	if (myQuit == true)
	{
		this->~CGame();
	}
}


void CGame::LogCallback(std::string aText)
{
}
