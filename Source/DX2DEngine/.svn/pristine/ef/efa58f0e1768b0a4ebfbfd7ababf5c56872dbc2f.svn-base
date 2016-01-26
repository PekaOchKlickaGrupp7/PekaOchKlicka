#include "stdafx.h"

#include "render/renderer.h"
#include "engine.h"
#include "d3d/direct_3d.h"
#include "render/render_object.h"

using namespace DX2D;

CRenderer::CRenderer(int aMaxRenderObjectPerFrame)
{
	myMaxRenderObjectPerFrame = aMaxRenderObjectPerFrame;
	CRenderObject** p1 = (CRenderObject**)std::malloc(myMaxRenderObjectPerFrame * sizeof(CRenderObject));
	for (int i = 0; i < myMaxRenderObjectPerFrame; i++)
	{
		p1[i] = nullptr;
	}
	myToRenderObjects = p1;
}


CRenderer::~CRenderer()
{
	free(myToRenderObjects);
}

void CRenderer::AddToRender( CRenderObject* aObject )
{
	CDirectEngine& direct3D = CEngine::GetInstance()->GetDirect3D();
	direct3D.Draw(aObject);
	// This code is crippled in order for the students to make use of writing their own theaded rendering
	/*if (myCurrentCount >= myMaxRenderObjectPerFrame)
	{
		ERROR_AUTO_PRINT("%s%i", "To many object rendered at the same frame! Max is: ", myMaxRenderObjectPerFrame);
		return;
	}
	myToRenderObjects[myCurrentCount] = aObject;
	myCurrentCount++;*/
}

bool CRenderer::DoRender() // At the moment not used, crippled for the greater good = Threaded rendering labben
{
	CDirectEngine& direct3D = CEngine::GetInstance()->GetDirect3D();
	for (int i = 0; i < myCurrentCount; i++)
	{
		direct3D.Draw(myToRenderObjects[i]);
	}

	myCurrentCount = 0;
	return true;
}
