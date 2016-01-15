#include "stdafx.h"

#include "engine.h"
#include "d3d/direct_3d.h"
#include "drawers/debug_drawer.h"
#include "error/error_manager.h"
#include "filewatcher/file_watcher.h"
#include "light/light_manager.h"
#include "render/renderer.h"
#include "text/text_service.h"
#include "texture/texture_manager.h"
#include "windows/windows_window.h"
//#include "FBX/FbxLoader_simple.h"
#include <windows.h>


#pragma comment( lib, "user32.lib" )


using namespace DX2D;


CEngine* DX2D::CEngine::myInstance = nullptr;


void DX2D::CEngine::CreateInstance( const SEngineCreateParameters& aCreateParameters )
{
	if (!myInstance)
	{
		myInstance = new CEngine(aCreateParameters);
	}
	else
	{
		ERROR_AUTO_PRINT("%s", "DX2D::CEngine::CreateInstance called twice, thats bad.");
	}
}


CEngine::CEngine( const SEngineCreateParameters& aCreateParameters)
: myWindow(nullptr)
, myDirect3D(nullptr)
, myRenderer(nullptr)
, myTextureManager(nullptr)
, myDebugDrawer(nullptr)
, myInputManager(nullptr)
, myHwnd(nullptr)
, myHInstance(nullptr)
, myLightManager(nullptr)
, myCreateParameters(aCreateParameters)
, myRunEngine(true)
/*, myFBXLoader(nullptr)*/
{
	myWindowSize.x = myCreateParameters.myWindowWidth;
	myWindowSize.y = myCreateParameters.myWindowHeight;

	myRenderSize.x = myCreateParameters.myRenderWidth;
	myRenderSize.y = myCreateParameters.myRenderHeight;

	myDebugDrawer = new CDebugDrawer(myCreateParameters.myActivateDebugSystems);

	myInitFunctionToCall = myCreateParameters.myInitFunctionToCall;
	myUpdateFunctionToCall = myCreateParameters.myUpdateFunctionToCall;
	myHwnd = myCreateParameters.myHwnd;
	myHInstance = myCreateParameters.myHInstance;
	myCreateParameters.myHInstance = myCreateParameters.myHInstance;
	myClearColor = myCreateParameters.myClearColor;

	myErrorManager = new CErrorManager();
	myErrorManager->AddLogListener(aCreateParameters.myLogFunction, aCreateParameters.myErrorFunction);
}


CEngine::~CEngine()
{
	SAFE_DELETE(myDirect3D);
	SAFE_DELETE(myDirect3D);
	SAFE_DELETE(myWindow);
	SAFE_DELETE(myRenderer);
	SAFE_DELETE(myTextureManager);
	SAFE_DELETE(myDebugDrawer);
	SAFE_DELETE(myTextService);
	SAFE_DELETE(myLightManager);
	SAFE_DELETE(myErrorManager);
	SAFE_DELETE(myFileWatcher);
	/*SAFE_DELETE(myFBXLoader);*/
}


void DX2D::CEngine::Destroy()
{
	if (myInstance != nullptr)
	{
		myInstance->Shutdown();
	}
}


bool CEngine::Start()
{
	INFO_PRINT("%s", "---TGA 2D Starting, dream big and dare to fail---");
	INFO_PRINT("%s", "Creating window");
	myFileWatcher = new CFileWatcher();
	myWindow = new CWindowsWindow();
	if (!myWindow->Init(myWindowSize, myHwnd, myCreateParameters.myApplicationName, myHInstance))
	{
		ERROR_AUTO_PRINT("%s", "Window failed to be created!");
		return false;
	}

	myDirect3D = new CDirectEngine();
	if (!myDirect3D->Init(*this, myWindowSize, myCreateParameters.myEnableVSync, myCreateParameters.myStartInFullScreen))
	{
		ERROR_AUTO_PRINT("%s", "D3D failed to be created!");
		return false;
	}

	myDirect3D->SetClearColor(myClearColor);

	myRenderer = new CRenderer(myCreateParameters.myMaxRenderedObjectsPerFrame);
	myTextureManager = new CTextureManager();
	myTextureManager->Init();

	/*myFBXLoader = new CFbxLoader();*/
	myTextService = new CTextService();
	myTextService->Init();


	myLightManager = new CLightManager();
	CalculateRatios();

	if (myDebugDrawer)
	{
		myDebugDrawer->Init();
	}

	if (myInitFunctionToCall)
	{
		myInitFunctionToCall();
	}

	StartStep();

	Destroy();
	return true;
}

void DX2D::CEngine::Shutdown()
{
	myRunEngine = false;
}

void DX2D::CEngine::StartStep()
{
	DoStep();
}


void DX2D::CEngine::DoStep()
{
	
	MSG msg = {0};

	while (myRunEngine)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if(msg.message == WM_QUIT)
			{
				INFO_PRINT("%s", "Exiting...");
				SAFE_DELETE(myInstance);
				break;
			}
		}
		else
		{
			//myTime->Tick([&]()
			{
				myFileWatcher->FlushChanges();
				myDirect3D->PostRenderFrame();
				myTextureManager->Update();

				if (myUpdateFunctionToCall)
				{
					myUpdateFunctionToCall();
				}

				myTextService->Update();
				if (myRenderer->DoRender())
				{
					if (myDebugDrawer)
					{
						myDebugDrawer->Update(0);
						myDebugDrawer->Render();
					}
					myDirect3D->RenderFrame();
				}
				myLightManager->PostFrameUpdate();
			}
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}
}


void DX2D::CEngine::SetWorldMatrixPosition( const Vector2f &aPosition )
{
	Vector2f correctedPos = aPosition;
	// Helpers to get the right camera position
	float ratio = GetWindowRatio();
	correctedPos.x = (((correctedPos.x * -1.0f) * 2.0f) ) + ratio;
	correctedPos.y *= 2.0f;
	correctedPos.y -= 1.0f;

	myDirect3D->SetWorldMatrixPosition(correctedPos, aPosition);
}


void DX2D::CEngine::SetWorldMatrixZoom( float aZoom )
{
	myDirect3D->SetWorldMatrixZoom(aZoom);
}


float DX2D::CEngine::GetWindowRatio() const
{
	return myWindowRatio;
}

float DX2D::CEngine::GetWindowRatioInversed() const
{
	return myWindowRatioInversed;
}


void DX2D::CEngine::SetResolution( const DX2D::Vector2<unsigned int> &aResolution)
{
	myWindowSize = aResolution;
	myWindow->SetResolution(aResolution);
	myDirect3D->SetResolution(aResolution);
	CalculateRatios();
}

void DX2D::CEngine::CalculateRatios()
{
	myWindowRatio = static_cast<float>(myWindowSize.x) / static_cast<float>(myWindowSize.y);
	myWindowRatioInversed = static_cast<float>(myWindowSize.y) / static_cast<float>(myWindowSize.x);
}

HWND* DX2D::CEngine::GetHWND() const
{
	return myHwnd;
}


HINSTANCE DX2D::CEngine::GetHInstance() const
{
	return myHInstance;
}

void DX2D::CEngine::SetFullScreen(bool aFullScreen)
{
	if (myDirect3D)
	{
		myDirect3D->SetFullScreen(aFullScreen);
	}
	
}
