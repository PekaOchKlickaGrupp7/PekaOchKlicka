#include "stdafx.h"
#include "ResolutionManager.h"

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

	// May do things several times. Check what CalculateRatio does. May remove code from here/call from it.
	myRealScreenSize = { static_cast<int>(RetrieveResolutionScreen().right),
		static_cast<int>(RetrieveResolutionScreen().bottom) };

	myVirtualScreenSize = aVirtualScreenSize;

	myRenderAreaPosition = { 0, 0 };
	myRenderAreaDimensions = aVirtualScreenSize;
	myRenderRatio = 16 / 9;

	myRatio = 16 / 9; // A dummy value.
	//CalculateRatio(); // Calculates ratio.
}

void ResolutionManager::Update(int aWindowWidth, int aWindowHeight)
{
	RECT returnedResolution = RetrieveResolutionRender();
	/*	DX2D::CEngine::GetInstance()->SetResolution({ static_cast<unsigned int>(1920), static_cast<unsigned int>(1080) });*/
	//CalculateRatio(1920, 1080);
	DX2D::CEngine::GetInstance()->SetResolution({ static_cast<unsigned int>(RetrieveResolutionWindow().right - RetrieveResolutionWindow().left), static_cast<unsigned int>(RetrieveResolutionWindow().bottom - RetrieveResolutionWindow().top) });
	CalculateRatio(returnedResolution.right - returnedResolution.left, returnedResolution.bottom - returnedResolution.top);
}

void ResolutionManager::RenderLetterbox() // not needed due to the fact that the viewport solves this.
{

	// render custom shapes here to cover letterbox
}

ResolutionManager::~ResolutionManager()
{
}

void ResolutionManager::CalculateRatio(int aWindowWidth, int aWindowHeight)
{
	myRealScreenSize = { aWindowWidth, aWindowHeight };


	float hRatio = static_cast<float>(myRealScreenSize.x) / static_cast<float>(myVirtualScreenSize.x);
	float vRatio = static_cast<float>(myRealScreenSize.y) / static_cast<float>(myVirtualScreenSize.y);
	myRatio = 1;
	float difference = 0.0f;

	if (hRatio < vRatio)
	{
		myRatio = hRatio;
		myRenderRatio = static_cast<float>(myVirtualScreenSize.x) / static_cast<float>(myRealScreenSize.x);
		difference = vRatio - hRatio;
	}
	else
	{
		myRatio = vRatio;
		myRenderRatio = static_cast<float>(myVirtualScreenSize.y) / static_cast<float>(myRealScreenSize.y);
		difference = hRatio - vRatio;
	}


	int viewPortX;
	int viewPortY;
	int viewPortWidth;
	int viewPortHeight;

	if (myVirtualScreenSize.y * hRatio > myRealScreenSize.y)
	{
		viewPortX = ((myRealScreenSize.x - (myVirtualScreenSize.x * myRatio)) / 2);
		viewPortY = 0;
		viewPortWidth = myVirtualScreenSize.x * myRatio;
		viewPortHeight = myRealScreenSize.y;
	}
	else
	{
		viewPortX = 0;



		int test = (myVirtualScreenSize.y - myRealScreenSize.y) * 0.5f;
		test = (((myRealScreenSize.y) - (myVirtualScreenSize.y * myRatio)) / 2);
		viewPortY = test;
		viewPortWidth = myRealScreenSize.x;
		viewPortHeight = myVirtualScreenSize.y * myRatio;
	}


	float screenScale = myRealScreenSize.x / 1920.0f;
	float origo = ((static_cast<float>(myRealScreenSize.y) - (1080.0f * screenScale)) / 2.0f);
	if (origo <= 0)
	{
		viewPortY = 0;
		screenScale = myRealScreenSize.y / 1080.0f;
		origo = -((static_cast<float>(myRealScreenSize.x) - (1920.0f * screenScale)) / 2.0f);
		viewPortX = -origo;
	}
	else
	{
		viewPortX = 0;
		viewPortY = origo;
	}

	// translate and scale (set viewport and scale)
	if (hRatio < vRatio)
	{
		myResViewport.SetViewport(viewPortX, viewPortY, myRealScreenSize.x, myRealScreenSize.y - viewPortY, 0, 1, myRatio);
	}
	else
	{
		myResViewport.SetViewport(viewPortX, viewPortY, myRealScreenSize.x, myRealScreenSize.y, 0, 1, myRatio);
	}



	myRenderAreaPosition.x = viewPortX;
	myRenderAreaDimensions.x = viewPortWidth;
	myRenderAreaPosition.y = viewPortY;
	myRenderAreaDimensions.y = viewPortHeight;
}
