#include "stdafx.h"

#include "Game.h"

#include <tga2d/error/error_manager.h>

#include <functional>
#include <time.h>
#include "..\CommonUtilities\ThreadHelper.h"



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
	delete myRenderThread;
	myRenderThread = nullptr;
	DL_Debug::Debug::Destroy();
	DX2D::CEngine::DestroyInstance();
}


void CGame::Init()
{
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

	myStateStack.PushMainGameState(new MainMenuState(myStateStackProxy, myInputManager, myTimerManager));


}
const bool CGame::Update()
{
	const float deltaTime = myTimerManager.GetMasterTimer().GetTimeElapsed().GetSeconds();
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
		myStateStack.Clear();
		mySynchronizer.Quit();
		myRenderThread->join();

		this->~CGame();
	}
	else
	{
		mySynchronizer.WaitForRender();
	}
}


void CGame::LogCallback(std::string aText)
{
}
