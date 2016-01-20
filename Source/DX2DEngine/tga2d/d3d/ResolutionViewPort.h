#pragma once

class ResolutionViewPort
{
public:
	ResolutionViewPort();
	~ResolutionViewPort();
	//D3D11_VIEWPORT &GetViewport();
	void SetViewport(float aTopLeftX, float aTopLeftY, float aWidth, float aHeight, float aMinDepth, float aMaxDepth, float aSetRatio);
private:

};

