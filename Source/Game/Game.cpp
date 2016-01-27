#include "stdafx.h"

#include "Game.h"

#include <tga2d/error/error_manager.h>

#include <functional>
#include <time.h>
#include "..\CommonUtilities\ThreadHelper.h"

#include "GameWorld.h"
#include "FadeState.h"

#include "SoundManager.h"
#include "SoundFileHandler.h"

#include "EventManager.h"
#include "ResolutionManager.h"
#include "MouseManager.h"

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

	SoundFileHandler::DestroyInstance();
	SoundManager::DestroyInstance();
	EventManager::DestroyInstance();

	DX2D::CEngine::GetInstance()->DestroyInstance();
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
	createParameters.myStartInFullScreen = false;
	createParameters.myClearColor.Set(0.0f, 0.0f, 0.0f, 1.0f);

	std::wstring appname = L"Peka Och Klicka Grupp 7";
	//createParameters.myStartInFullScreen = myIsFullscreen;
#ifdef _DEBUG
	createParameters.myActivateDebugSystems = true;
	//createParameters.myStartInFullScreen = myIsFullscreen;
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

	#pragma region Mouse Manager
	// Create a mouse instance
	MouseManager::CreateInstance();
	// Create all file paths to the different cursors
	CommonUtilities::GrowingArray<std::string> spriteFilePaths;
	spriteFilePaths.Init(6); // There are six different cursors
	for (size_t i = 0; i < 6; i++)
	{
		std::string tempString = "Sprites/Cursor/" + std::to_string(i) + ".dds";
		spriteFilePaths.Add(tempString);
	}
	// Send the file paths to the mouse manager
	MouseManager::GetInstance()->Initialize(spriteFilePaths, &myInputManager);
	#pragma endregion

	#pragma region Sound Manager
	
	SoundManager::CreateInstance();
	SoundFileHandler::CreateInstance();

	#pragma endregion

	std::string name = "";
	//ResolutionManager::GetInstance()->Update(0, 0);
	//myJson.Load("root.json", myRooms, this, name);

	#pragma region Event Manager

	EventManager::CreateInstance();
	EventManager::GetInstance()->Init(&myInputManager);

	#pragma endregion

	myStateStack.PushMainGameState(new FadeState(myStateStackProxy, myInputManager, myTimerManager));
	/*if (myTestLevel.size() > 0)
	{
	}
	else
	{
		myStateStack.PushMainGameState(new MainMenuState(myStateStackProxy, myInputManager, myTimerManager));
	}*/
}

const bool CGame::Update()
{
	ResolutionManager::GetInstance()->Update(DX2D::CEngine::GetInstance()->GetWindowSize().x, DX2D::CEngine::GetInstance()->GetWindowSize().y);
	SoundManager::GetInstance()->Update(static_cast<float>(myTimerManager.GetMasterTimer().GetTimeElapsed().GetMiliseconds()));
	MouseManager::GetInstance()->Update(static_cast<float>(myTimerManager.GetMasterTimer().GetTimeElapsed().GetMiliseconds()));


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
		DX2D::CEngine::GetInstance()->Shutdown();
	}
	else
	{
		mySynchronizer.WaitForRender();
	}
}


void CGame::LogCallback(std::string aText)
{
}
