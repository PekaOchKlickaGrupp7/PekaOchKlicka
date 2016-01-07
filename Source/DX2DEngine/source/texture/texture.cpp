#include "stdafx.h"
#include "texture/Texture.h"

DX2D::CTexture::CTexture()
{
	myPath = "undefined";
	myResource = nullptr;
	myIsFailedTexture = false;
}

DX2D::CTexture::~CTexture()
{
	if (myIsFailedTexture)
	{
		return;
	}
	SAFE_RELEASE(myResource);
	myPath = "undefined";
}


DX2D::CRendertarget::CRendertarget() : CTexture()
{
	myTarget = nullptr;
}

DX2D::CRendertarget::~CRendertarget()
{
	SAFE_RELEASE(myTarget)
}