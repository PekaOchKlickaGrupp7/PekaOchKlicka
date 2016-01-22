#include "stdafx.h"
#include "tga2d\d3d\ResolutionViewPort.h"
#include "tga2d\engine.h"

namespace DX2D
{
	class CDirectEngine
	{
	public:
		D3D11_VIEWPORT &GetViewport();
		void SetViewPort(float aTopLeftX, float aTopLeftY, float aWidth, float aHeight, float aMinDepth, float aMaxDepth);
	};
}

ResolutionViewPort::ResolutionViewPort()
{
}

//D3D11_VIEWPORT &ResolutionViewPort::GetViewport()
//{
//	return DX2D::CEngine::GetInstance()->GetDirect3D().GetViewport();
//}

void ResolutionViewPort::SetViewport(float aTopLeftX, float aTopLeftY, float aWidth, float aHeight, float aMinDepth, float aMaxDepth)
{
	DX2D::CEngine::GetInstance()->GetDirect3D().SetViewPort(aTopLeftX, aTopLeftY, aWidth, aHeight, aMinDepth, aMaxDepth);
}


ResolutionViewPort::~ResolutionViewPort()
{
}
