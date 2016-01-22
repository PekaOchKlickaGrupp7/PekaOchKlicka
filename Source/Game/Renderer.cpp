#include "stdafx.h"
#include "Renderer.h"
#include "../DX2DEngine/tga2d/sprite/sprite.h"
#include "Synchronizer.h"



Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::Render(Synchronizer& aSynchronizer)
{
	for (int i = 0; i < aSynchronizer.GetSize(); ++i)
	{
		switch (aSynchronizer.GetRenderCommand(i).myType)
		{
		case eRenderType::eSprite:
			aSynchronizer.GetRenderCommand(i).mySprite->SetPosition(aSynchronizer.GetRenderCommand(i).myPosition);
			aSynchronizer.GetRenderCommand(i).mySprite->Render();
			break;
		case eRenderType::eCustomShape:
			aSynchronizer.GetRenderCommand(i).myCustomShape->myPosition = aSynchronizer.GetRenderCommand(i).myPosition;
			aSynchronizer.GetRenderCommand(i).myCustomShape->Render();
			break;
		case eRenderType::eText:
			aSynchronizer.GetRenderCommand(i).myText->myPosition = aSynchronizer.GetRenderCommand(i).myPosition;
			aSynchronizer.GetRenderCommand(i).myText->Render();
			break;
		default:
			break;
		}
	}
}