#pragma once
#include "..\CommonUtilities\GrowingArray.h"
#include "RenderCommand.h"

class Synchronizer;

class Renderer
{
public:
	Renderer();
	~Renderer();
	void Render(Synchronizer& aSynchronizer);
private:
	void ConvertPosFromNormalToPixelSpace(RenderCommand& aRenderCommand);
};

