#include "stdafx.h"
#include "ResolutionManager.h"


#include <iostream>

ResolutionManager* ResolutionManager::myResolutionManager = nullptr;

ResolutionManager::ResolutionManager()
{
}

RECT ResolutionManager::RetrieveResolutionScreen()
{
	RECT resolution;
	GetWindowRect(GetDesktopWindow(), &resolution);
	return resolution;
}

RECT ResolutionManager::RetrieveResolutionWindow()
{
	RECT resolution;
	GetWindowRect(*DX2D::CEngine::GetInstance()->GetHWND(), &resolution);
	return resolution;
}

RECT ResolutionManager::RetrieveResolutionRender()
{
	RECT resolution;
	GetWindowRect(*DX2D::CEngine::GetInstance()->GetHWND(), &resolution);
	int border_thickness = GetSystemMetrics(SM_CXSIZEFRAME);
	resolution.right -= border_thickness * 2;
	resolution.bottom -= border_thickness * 2;
	resolution.left += border_thickness * 2;
	resolution.top += border_thickness;
	return resolution;
}

void ResolutionManager::Initialize(DX2D::Vector2<int> aVirtualScreenSize)
{
	myIsFullscreen = false;
	myRealScreenSize = { static_cast<int>(RetrieveResolutionScreen().right),
		static_cast<int>(RetrieveResolutionScreen().bottom) };

	myVirtualScreenSize = aVirtualScreenSize;

	myRenderAreaPosition = { 0, 0 };
	myRenderAreaDimensions = aVirtualScreenSize;
	myRenderRatio = 16 / 9;

	myRatio = 16 / 9;
}

void ResolutionManager::Update()
{
	RECT returnedResolution;
	GetClientRect(*DX2D::CEngine::GetInstance()->GetHWND(), &returnedResolution);
	CalculateRatio(returnedResolution, false);
}

void ResolutionManager::SetupWindow(int aX, int aY)
{
	DX2D::CEngine::GetInstance()->SetResolution(DX2D::Vector2<unsigned int>(aX, aY), true);

	RECT returnedResolution;
	GetClientRect(*DX2D::CEngine::GetInstance()->GetHWND(), &returnedResolution);
	CalculateRatio(returnedResolution, false);
}

void ResolutionManager::SetupWindow()
{
	RECT returnedResolution;
	GetClientRect(*DX2D::CEngine::GetInstance()->GetHWND(), &returnedResolution);
	CalculateRatio(returnedResolution, false);
}

void ResolutionManager::RenderLetterbox()
{
}

ResolutionManager::~ResolutionManager()
{

}

void ResolutionManager::ToggleFullscreen()
{
	myIsFullscreen = !myIsFullscreen;
	DX2D::CEngine::GetInstance()->SetFullScreen(myIsFullscreen);
	EventVariablesManager::GetInstance()->SetVariable(myIsFullscreen, "IsFullscreen");

	RECT returnedResolution;
	GetClientRect(*DX2D::CEngine::GetInstance()->GetHWND(), &returnedResolution);

	CalculateRatio(returnedResolution, false);
}

void ResolutionManager::CalculateRatio(RECT aResolution, bool aChangeWindow)
{
	RECT returnedResolution = aResolution;
	float screen_width = static_cast<float>(returnedResolution.right);
	float screen_height = static_cast<float>(returnedResolution.bottom);

	float virtual_width = 1920;
	float virtual_height = 1080;

	float targetAspectRatio = virtual_width / virtual_height;

	float width = screen_width;
	float height = (width / targetAspectRatio + 0.5f);

	if (height > screen_height)
	{
		height = screen_height;
		width = (height * targetAspectRatio + 0.5f);
	}

	float vp_x = (screen_width / 2) - (width / 2);
	float vp_y = (screen_height / 2) - (height / 2);
	DX2D::CEngine::GetInstance()->SetResolution({ static_cast<unsigned int>(width), static_cast<unsigned int>(height) }, aChangeWindow);
	myResViewport.SetViewport((vp_x), (vp_y), (width), (height), 0.0f, 1.0f);

	myRenderAreaDimensions.x = static_cast<int>(width);
	myRenderAreaDimensions.y = static_cast<int>(height);
	myRenderAreaPosition.x = static_cast<int>(vp_x);
	myRenderAreaPosition.y = static_cast<int>(vp_y);
}