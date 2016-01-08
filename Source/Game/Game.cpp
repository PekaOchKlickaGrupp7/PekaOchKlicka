#include "stdafx.h"

#include "Game.h"

#include <tga2d/error/error_manager.h>

#include <functional>
#include <time.h>



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
	unsigned short windowWidth = 1280;
	unsigned short windowHeight = 720;


	DX2D::SEngineCreateParameters createParameters;
	createParameters.myActivateDebugSystems = true;
	createParameters.myInitFunctionToCall = std::bind(&CGame::InitCallBack, this);
	createParameters.myUpdateFunctionToCall = std::bind(&CGame::UpdateCallBack, this);
	createParameters.myLogFunction = std::bind(&CGame::LogCallback, this, _1);
	createParameters.myWindowHeight = windowHeight;
	createParameters.myWindowWidth = windowWidth;
	createParameters.myRenderHeight = windowHeight;
	createParameters.myRenderWidth = windowWidth;
	createParameters.myClearColor.Set(0.0f, 0.0f, 0.0f, 1.0f);

	std::wstring appname = L"TGA 2D RELEASE";
#ifdef _DEBUG
	appname = L"TGA 2D DEBUG";
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
	DL_Debug::Debug::Create("DebugLog.txt");
	myInputManager.Initialize(DX2D::CEngine::GetInstance()->GetHInstance(), *DX2D::CEngine::GetInstance()->GetHWND());
	myRenderThread = new std::thread(&CGame::Render, this);
	myStateStack.PushMainGameState(new CGameWorld(myStateStackProxy, myInputManager, myTimerManager));


}
const bool CGame::Update()
{
	const float deltaTime = myTimerManager.GetMasterTimer().GetTimeElapsed().GetSeconds();
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

	DL_DEBUG("Hey");
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
