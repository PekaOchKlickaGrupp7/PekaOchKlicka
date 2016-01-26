#include "stdafx.h"

#include "shaders/shader_distance_field_instanced.h"
#include "engine.h"
#include "d3d/direct_3d.h"
#include "sprite/textured_quad_batch.h"
#include "sprite/textured_quad.h"
#include "sprite/sprite_batch.h"
#include "sprite/sprite.h"
#include "texture/texture_manager.h"
#include "engine.h"
#include "math/color.h"

using namespace DX2D;


DX2D::CShaderDistanceFieldInstanced::CShaderDistanceFieldInstanced(CEngine* aEngine)
: CShader(aEngine)
{

}


bool CShaderDistanceFieldInstanced::Init()
{
	return CreateShaders("shaders/instanced_distance_shader.fx", "shaders/instanced_distance_shader.fx");
}

void CShaderDistanceFieldInstanced::Render(CTexturedQuadBatch* aObject, ID3D11Buffer* aInstanceBuffer, ID3D11Buffer* aVertexBuffer)
{
	if (!myIsReadyToRender)
	{
		return;
	}
	CShader::Render(aObject);
	CDirectEngine& engine = CEngine::GetInstance()->GetDirect3D();
	for (unsigned int i=0; i< aObject->GetSpriteBatch()->mySprites.size(); i++)
	{
		std::vector<CSprite*>& sprites = aObject->GetSpriteBatch()->mySprites[i];
		SetShaderParameters(sprites, aObject, aInstanceBuffer, aVertexBuffer);
		int asize = (int)sprites.size();
		engine.GetContext()->DrawInstanced(6, asize, 0, 0);
	}
}

void CShaderDistanceFieldInstanced::SetShaderParameters(std::vector<CSprite*>& someSprites, CTexturedQuadBatch* aBatch, ID3D11Buffer* aInstanceBuffer, ID3D11Buffer* aVertexBuffer)
{
	if(!aInstanceBuffer)
	{
		return;
	}

	myBufferedObjects.clear();
	myBufferedObjects.reserve(someSprites.size());
	const float ratioX = (float)myDirect3dEngine->myWindowSize.x / (float)myDirect3dEngine->myWindowSize.y;
	for (unsigned int i = 0; i < someSprites.size(); i++)
	{
		CSprite* sprite = someSprites[i];

		if (!sprite->GetShouldRender())
		{
			continue;
		}
		if (!sprite->myShouldRenderBatched)
		{
			continue;
		}

		ObjectBufferInstanceType type;

		type.myPosition = sprite->GetPosition();
		DX2D::Vector2f offsetPos(1.0f, -1.0f);
		Vector2f correctedPosition = DX2D::Vector2f(type.myPosition.x * 2.0f, -type.myPosition.y * 2.0f) - offsetPos;
		type.myPosition = correctedPosition;

		type.myPivot.Set(sprite->GetPivot().x, sprite->GetPivot().y, 0, 0);
		type.myPosition.x *= ratioX;
		type.myRotationAndSize.x = sprite->GetRotation();

		if (sprite->HasChangedSize())
		{
			CTexturedQuad* texQuad = sprite->GetTexturedQuad();
			//myQuad->mySize * myQuad->mySizeMultiplier
			type.myRotationAndSize.y = texQuad->mySize.x * texQuad->mySizeMultiplier.x;
			type.myRotationAndSize.z = texQuad->mySize.y * texQuad->mySizeMultiplier.y;
		}
		else
		{
			type.myRotationAndSize.y = aBatch->mySize.x;
			type.myRotationAndSize.z = aBatch->mySize.y;
		}

		// Crispiness of distance field
		type.myRotationAndSize.w = (1.0f - type.myRotationAndSize.y) * 0.1f;



		Vector2f uv = sprite->GetUVOffset();
		type.myUV = Vector4f(uv.x, uv.y, sprite->GetUVScale().x, sprite->GetUVScale().y);

		type.myColor = sprite->GetColor().AsVec4();
		myBufferedObjects.push_back(type);
	}

	D3D11_MAPPED_SUBRESOURCE mappedObjectResource;
	HRESULT result = myDirect3dEngine->GetContext()->Map(aInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedObjectResource);
	if(FAILED(result))
	{
		INFO_PRINT("Error in rendering!");
		return;
	}

	if (myBufferedObjects.size() == 0)
	{
		return;
	}

	memcpy(mappedObjectResource.pData, &myBufferedObjects[0], sizeof(ObjectBufferInstanceType) * myBufferedObjects.size());

	myDirect3dEngine->GetContext()->Unmap(aInstanceBuffer, 0);
	myDirect3dEngine->GetContext()->PSSetShaderResources(1, 1, &aBatch->myTexture->myResource);



	unsigned int strides[2];
	unsigned int offsets[2];
	ID3D11Buffer* bufferPointers[2];

	// Set the buffer strides.
	strides[0] = sizeof(SVertex); 
	strides[1] = sizeof(ObjectBufferInstanceType); 
	// Set the buffer offsets.
	offsets[0] = 0;
	offsets[1] = 0;

	bufferPointers[0] = aVertexBuffer;	
	bufferPointers[1] = aInstanceBuffer;

	myDirect3dEngine->GetContext()->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

}