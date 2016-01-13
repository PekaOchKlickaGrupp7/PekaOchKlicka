#pragma once
#include "..\DX2DEngine\tga2d\sprite\sprite.h"
class ResolutionManager
{
public:
	ResolutionManager();
	void Initialize(DX2D::Vector2f aRealScreenSize, DX2D::Vector2f aVirtualScreenSize = { 1920, 1080 });
	~ResolutionManager();
private:
	DX2D::Vector2f myRealScreenSize;
	DX2D::Vector2f myVirtualScreenSize;
};

