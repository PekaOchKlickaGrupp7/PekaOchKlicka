#pragma once
#include "..\DX2DEngine\tga2d\sprite\sprite.h"
#include "..\DX2DEngine\tga2d\d3d\ResolutionViewPort.h"
#include "..\DX2DEngine\tga2d\engine.h"
#include <Windows.h>

class ResolutionManager
{
public:
	~ResolutionManager();

	static ResolutionManager* const GetInstance()
	{
		if (myResolutionManager == nullptr)
		{
			myResolutionManager = new ResolutionManager();
		}
		return myResolutionManager;
	}

	static void DestroyInstance()
	{
		myResolutionManager->Destroy();
		delete myResolutionManager;
	}

	DX2D::Vector2<int> &GetRenderAreaPosition() { return myRenderAreaPosition; };
	DX2D::Vector2<int> &GetRenderAreaDimension() { return myRenderAreaDimensions; };

	void Initialize(DX2D::Vector2<int> aVirtualScreenSize = { 1920, 1080 });
	DX2D::Vector2<int> GetMonitorResolution() { return myRealScreenSize; };
	void Update(int aWindowWidth, int aWindowHeight);
	void RenderLetterbox();
	float GetRatio(){ return myRatio; };
	
private:
	static ResolutionManager* myResolutionManager;
	ResolutionManager();
	void Destroy();
	void CalculateRatio();
	RECT RetrieveResolutionScreen();
	RECT RetrieveResolutionWindow();
	RECT RetrieveResolutionRender();

	ResolutionViewPort myResViewport;

	DX2D::Vector2<int> myRealScreenSize;
	DX2D::Vector2<int> myVirtualScreenSize;
	
	float myRatio;

	DX2D::Vector2<int> myRenderAreaPosition;
	DX2D::Vector2<int> myRenderAreaDimensions;

	float myRenderRatio;
};

