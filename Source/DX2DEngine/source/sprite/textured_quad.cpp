#include "stdafx.h"

#include "sprite/textured_quad.h"
#include "d3d/direct_3d.h"
#include "DDSTextureLoader.h"
#include "engine.h"
#include "render/renderer.h"
#include "texture/texture_manager.h"
#include <d3d11.h>

using namespace DX2D;

CTexturedQuad::CTexturedQuad(void)
		:myTexture(nullptr)
{
	myEngine = CEngine::GetInstance();

	myTextureRect.myX = 0.0f;
	myTextureRect.myY = 0.0f;
	myTextureRect.myWidth = 1.0f;
	myTextureRect.myHeight = 1.0f;
	myVertexesDirty = false;
	for (int i = 0; i < MAP_MAX; i++)
	{
		myMaps[i] = NULL;
	}
}


CTexturedQuad::~CTexturedQuad(void)
{
}

void DX2D::CTexturedQuad::Init(const char* aTexturePath)
{
	if (aTexturePath)
	{
		myTexture = myEngine->GetTextureManager().GetTexture(aTexturePath);
		mySize = myTexture->mySize;
		myImageSize = myTexture->myImageSize;
	}
	else
	{
		myTexture = myEngine->GetTextureManager().GetWhiteSquareTexture();
	}
}

void DX2D::CTexturedQuad::Render()
{
	myEngine->GetRenderer().AddToRender(this);
}

void DX2D::CTexturedQuad::SetTextureRect( float aX, float aY, float aWidth, float aHeight )
{
	myTextureRect.myX = aX;
	myTextureRect.myY = aY;
	myTextureRect.myWidth = aWidth;
	myTextureRect.myHeight = aHeight;
	VertexChanged();
}

void CTexturedQuad::SetColor(CColor aColor)
{
	myColor = aColor;
}

const CColor DX2D::CTexturedQuad::GetColor() const
{
	return myColor;
}

void DX2D::CTexturedQuad::SetSize(const Vector2f& aSize)
{
	mySizeMultiplier = aSize;
	VertexChanged();
}

void DX2D::CTexturedQuad::SetUV(const Vector2f& aUV)
{
	myUV = aUV;
}

void DX2D::CTexturedQuad::SetMap(EShaderMap aMapType, const char* aPath)
{
	myMaps[aMapType] = myEngine->GetTextureManager().GetTexture(aPath);
}
