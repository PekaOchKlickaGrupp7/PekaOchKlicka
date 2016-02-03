/*
Use this class to create and show a sprite
*/

#pragma once
#include "tga2d/render/render_common.h"
#include "tga2d/math/Vector2.h"
#include "tga2d/math/Color.h"
namespace DX2D
{
	class CTexturedQuad;
	class CTexturedQuadBatch;
	struct STextureRext;
	class CSprite
	{
		friend class CTexturedQuadBatchDrawer;
		friend class CShaderNormalInstanced;
		friend class CShaderDistanceFieldInstanced;
		friend class CSpriteBatch;
		friend class CVideoInstance;
	public:
		CSprite(const char* aTexturePath = nullptr);
		CSprite(const CSprite& aCSprite);
		~CSprite();

		CSprite& operator=(const CSprite& aCSprite);
		
		void Render();
		/* Set a normalized position on the sprite*/
		void SetPosition(const Vector2f& aPosition);
		const Vector2f& GetPosition() const;

		void SetPivot(const Vector2f& aPivot);
		const Vector2f& GetPivot() const;


		/* Set a rotation in radians*/
		void SetRotation(const float aRotationInRadians);
		float GetRotation() const;

		void SetColor(const CColor& aColor);
		CColor GetColor() const;

		void SetSize(const Vector2f& aSize);
		virtual Vector2f GetSize() const;
		virtual Vector2f GetScale() const;
		virtual Vector2<unsigned int> GetImageSize() const;

		void SetUVScale(Vector2f aScale);
		Vector2f GetUVScale() const;

		bool HasValidTexture() const;
		/*
		aX = The texture region top-left corner X-coordinate.
		aY = The texture region top-left corner Y-coordinate.
		aEndX =  The texture region top-right corner X-coordinate.
		aEndY = The texture region bottom-right corner X-coordinate.
		*/
		void SetTextureRect(float aStartX, float aStartY, float aEndX, float aEndY);
		STextureRext* GetTextureRect();

		bool HasSameTextureAs(CSprite* aSprite) const;

		virtual void Update(float /*aTimeDelta*/){}

		const Vector2f& GetUVOffset() const;
		void SetUVOffset(const Vector2f& aUV);

		bool HasChangedSize() const {return myHasChangedSize;}

		/* If this sprite is rendered by a spritebatch, this is useful to not render the sprite but keep it in the batch*/
		void SetShouldRender(bool aShouldRender){ myShouldRender = aShouldRender; }
		bool GetShouldRender(){ return myShouldRender; }

		void SetMap(const EShaderMap aMapType, const char* aPath);

		/* Will delete the texturedata, be aware that if anything else uses this texture it will be invalid*/
		void ReleaseAttachedTexture();
		
	private:
		
		void InternalInit(const char* aTexturePath);
		CTexturedQuad *myQuad;
		bool myHasChangedSize;
		bool myShouldRender;
		bool myIsPartOfbatch;
	protected:
		CTexturedQuad* GetTexturedQuad() const { return myQuad; }
		bool myShouldRenderBatched;
	};
}