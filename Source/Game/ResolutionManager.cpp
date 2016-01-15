#include "stdafx.h"
#include "ResolutionManager.h"


ResolutionManager::ResolutionManager()
{
}

void ResolutionManager::Initialize(DX2D::Vector2f aRealScreenSize, DX2D::Vector2f aVirtualScreenSize)
{
	myRealScreenSize = aRealScreenSize;
	myVirtualScreenSize = aVirtualScreenSize;
}


ResolutionManager::~ResolutionManager()
{
}
