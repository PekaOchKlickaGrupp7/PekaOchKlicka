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
	//CommonUtilities::GrowingArray<RenderCommand, int> textBuffer;
	//textBuffer.Init(10);

	for (unsigned short i = 0; i < aSynchronizer.GetSize(); ++i)
	{
		if (aSynchronizer.GetRenderCommand(i).myChangeAmbience == true)
		{
			DX2D::CEngine::GetInstance()->GetLightManager().SetAmbience(aSynchronizer.GetRenderCommand(i).myAmbience);
		}

		RenderCommand& command = aSynchronizer.GetRenderCommand(i);

		switch (aSynchronizer.GetRenderCommand(i).myType)
		{
		case eRenderType::eSprite:
			if (command.myHasSize == true)
			{
				command.mySprite->SetSize(command.mySize);
			}
			if (command.myHasColor == true)
			{
				command.mySprite->SetColor(command.myColor);
			}
			command.mySprite->SetPosition(command.myPosition);
			command.mySprite->Render();
			break;
		case eRenderType::eCustomShape:
			aSynchronizer.GetRenderCommand(i).myCustomShape->myPosition = aSynchronizer.GetRenderCommand(i).myPosition;
			aSynchronizer.GetRenderCommand(i).myCustomShape->Render();
			break;
		case eRenderType::eText:
			//textBuffer.Add(aSynchronizer.GetRenderCommand(i));

			//OBS
			aSynchronizer.GetRenderCommand(i).myText->myPosition = aSynchronizer.GetRenderCommand(i).myPosition;
			aSynchronizer.GetRenderCommand(i).myText->Render();

			break;
		default:
			break;
		}
	}

	//for (int i = 0; i < textBuffer.Size(); i++)
	//{
	//	textBuffer[i].myText->myPosition = textBuffer[i].myPosition;
	//	textBuffer[i].myText->Render();
	//}
}