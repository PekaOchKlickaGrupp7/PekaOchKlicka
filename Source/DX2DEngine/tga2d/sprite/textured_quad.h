#pragma once
#include "math/vector2.h"
#include "render/render_object.h"
#include "render/render_common.h"

namespace DX2D
{
	struct STextureRext
	{
		float myX;
		float myY;
		float myWidth;
		float myHeight;
	};
	struct ID3D11Resource;
	class CEngine;
	class CTextureManager;
	class CTexture;
	class CTexturedQuad : public CRenderObjectSprite
	{
	public:
		CTexturedQuad(void);
		~CTexturedQuad(void);
		void Init(const char* aTexturePath);
		void Render();
		void SetTextureRect(float aX, float aY, float aWidth, float aHeight);
		CEngine* myEngine;	
		CTexture* myTexture;

		STextureRext& GetTextureRect(){return myTextureRect;}
		bool IsVertexesDirty() const {return myVertexesDirty;}
		void SetCleanVertexes(){myVertexesDirty = false;}
		void SetColor(const CColor aColor);
		const CColor GetColor() const;
		void SetSize(const Vector2f& aSize);
		virtual void SetUV(const Vector2f& aUV);
		void SetMap(EShaderMap aMapType, const char* aPath);
		
		CTexture* myMaps[MAP_MAX];

	private:
		void VertexChanged(){ myVertexesDirty = true; }
		STextureRext myTextureRect;
		bool myVertexesDirty;
	};
}
