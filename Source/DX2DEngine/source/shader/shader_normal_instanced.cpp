#include "stdafx.h"

#include "shaders/shader_normal_instanced.h"
#include "engine.h"
#include "d3d/direct_3d.h"
#include "sprite/textured_quad_batch.h"
#include "sprite/textured_quad.h"
#include "sprite/sprite_batch.h"
#include "sprite/sprite.h"
#include "texture/texture_manager.h"
#include "engine.h"
#include "light/light_manager.h"


using namespace DX2D;


CShaderNormalInstanced::CShaderNormalInstanced(CEngine* aEngine)
	:CShader(aEngine)
{

}

CShaderNormalInstanced::~CShaderNormalInstanced()
{
	for (int i = 0; i < myBufferedObjects.TotalCount(); i++)
	{
		delete myBufferedObjects.myData[i];
	}
}

bool CShaderNormalInstanced::Init()
{
	myBufferedObjects.Init(SPRITE_BATCH_COUNT+1);
	for (int i = 0; i < myBufferedObjects.TotalCount(); i++)
	{
		ObjectBufferInstanceType *text = new ObjectBufferInstanceType();
		myBufferedObjects.myData[i] = text;
	}

	return CreateShaders("shaders/instanced_sprite_shader.fx", "shaders/instanced_sprite_shader.fx");
}

void CShaderNormalInstanced::Render(CTexturedQuadBatch* aObject, ID3D11Buffer* aInstanceBuffer, ID3D11Buffer* aVertexBuffer)
{
	if (!myIsReadyToRender)
	{
		return;
	}
	CShader::Render(aObject);

	for (unsigned int i=0; i< aObject->GetSpriteBatch()->mySprites.size(); i++)
	{
		std::vector<CSprite*>& sprites = aObject->GetSpriteBatch()->mySprites[i];
		int asize = SetShaderParameters(sprites, aObject, aInstanceBuffer, aVertexBuffer);
		myDirect3dEngine->GetContext()->DrawInstanced(6, asize, 0, 0);
	}
}

int CShaderNormalInstanced::SetShaderParameters(std::vector<CSprite*>& someSprites, CTexturedQuadBatch* aBatch, ID3D11Buffer* aInstanceBuffer, ID3D11Buffer* aVertexBuffer)
{
	if(!aInstanceBuffer)
	{
		return 0;
	}

	bool haveLights = CEngine::GetInstance()->GetLightManager().GetNumberOfLights() > 0;
	float lightType = haveLights ? 1.0f : 0.0f;
	if (aBatch->myMaps[NORMAL_MAP] && haveLights)
	{
		ID3D11ShaderResourceView* textures[2];
		textures[0] = aBatch->myTexture->myResource;
		textures[1] = aBatch->myMaps[NORMAL_MAP]->myResource;
		myDirect3dEngine->GetContext()->PSSetShaderResources(1, 2, textures);
		lightType = 2.0f;
	}
	else
	{
		myDirect3dEngine->GetContext()->PSSetShaderResources(1, 1, &aBatch->myTexture->myResource);
	}


	myBufferedObjects.ResetCount();
	//const float ratioX = (float)myDirect3dEngine->myWindowSize.x / (float)myDirect3dEngine->myWindowSize.y;
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


		ObjectBufferInstanceType* next = myBufferedObjects.GetNext();
		if (!next)
		{
			break;
		}

		ObjectBufferInstanceType& type = *next;

		type.myPosition = sprite->GetPosition();
		DX2D::Vector2f offsetPos(DX2D::CEngine::GetInstance()->GetWindowRatio(), -1.0f);
		//Vector2f correctedPosition = DX2D::Vector2f( (type.myPosition.x * 2.0f) - 1.0f, (type.myPosition.y * 2.0f) + 1);
		Vector2f correctedPosition = DX2D::Vector2f((type.myPosition.x * 2.0f) - 1.0f, -(type.myPosition.y * 2.0f) + 1.0f);
	
		type.myPosition = correctedPosition;

		type.myPivot.Set(sprite->GetPivot().x, sprite->GetPivot().y, 0, 0);
		type.myRotationAndSize.x = sprite->GetRotation();

		if (sprite->HasChangedSize())
		{
			type.myRotationAndSize.y = (aBatch->mySize.x * 2) * sprite->GetTexturedQuad()->mySizeMultiplier.x;
			type.myRotationAndSize.z = (aBatch->mySize.y * 2) * sprite->GetTexturedQuad()->mySizeMultiplier.y;
		}
		else
		{
			type.myRotationAndSize.y = aBatch->mySize.x * 2;
			type.myRotationAndSize.z = aBatch->mySize.y * 2;
		}

		type.myRotationAndSize.w = lightType;



		Vector2f uv = sprite->GetUVOffset();
		type.myUV = Vector4f(uv.x, uv.y, sprite->GetUVScale().x, sprite->GetUVScale().y);

		type.myColor = sprite->GetColor().AsVec4();
	}


	if (myBufferedObjects.NextCount() == -1)
	{
		return 0;
	}

	D3D11_MAPPED_SUBRESOURCE mappedObjectResource;
	HRESULT result = myDirect3dEngine->GetContext()->Map(aInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedObjectResource);
	if(FAILED(result))
	{
		INFO_PRINT("Error in rendering!");
		return 0;
	}


	for (int i = 0; i < myBufferedObjects.NextCount()+1; i++)
	{
		memcpy((ObjectBufferInstanceType*)mappedObjectResource.pData + i, myBufferedObjects.myData[i], sizeof(ObjectBufferInstanceType));
	}

	


	
	myDirect3dEngine->GetContext()->Unmap(aInstanceBuffer, 0);


	unsigned int strides[2];
	unsigned int offsets[2];
	ID3D11Buffer* bufferPointers[2];


	strides[0] = sizeof(SVertex); 
	strides[1] = sizeof(ObjectBufferInstanceType); 

	offsets[0] = 0;
	offsets[1] = 0;

	bufferPointers[0] = aVertexBuffer;	
	bufferPointers[1] = aInstanceBuffer;

	myDirect3dEngine->GetContext()->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);
	return myBufferedObjects.NextCount()+1;
}