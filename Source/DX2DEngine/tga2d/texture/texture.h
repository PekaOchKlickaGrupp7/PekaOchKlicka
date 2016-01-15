/*
This class will store a texture bound to DX11
*/


#pragma once
#include <string>
#include "math/Vector2.h"

struct ID3D11ShaderResourceView;
struct ID3D11RenderTargetView;
namespace DX2D
{
	
	class CTexture
	{
	public:
		CTexture();
		~CTexture();

	public:
		std::string myPath;
		ID3D11ShaderResourceView* myResource;
		Vector2f mySize;
		Vector2<unsigned int> myImageSize;
		bool myIsFailedTexture;
		bool myIsReleased;
	};

	
	class CRendertarget : public CTexture
	{
	public:
		CRendertarget();
		~CRendertarget();

	public:
		ID3D11RenderTargetView* myTarget;
	};
}
