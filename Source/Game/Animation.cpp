#include "stdafx.h"
#include "Animation.h"


Animation::Animation()
{
	myFrameDuration = 0;
	myCurentFrameDuration = 0;
	mySprite = nullptr;
	myNumberOfFrames = 0;
	myFrame = 0;
	myOriginalSpriteWidht = 0;
	myPaused = false;
}

void Animation::Init(const char* aSpriteFilePath, float aFrameDuration, int aNumberOfFrames)
{
	myFrameDuration = aFrameDuration;
	myCurentFrameDuration = 0;

	mySprite = new DX2D::CSprite(aSpriteFilePath);

	myNumberOfFrames = aNumberOfFrames;
	myFrame = 3;
	myPaused = false;
	myOriginalSpriteWidht = mySprite->GetSize().x;
	mySprite->SetSize(DX2D::Vector2f(1.f / myNumberOfFrames, 1));

	UpdateTextureRect();

}

void Animation::Update(float aDelta)
{
	if (myPaused == false)
	{
		myCurentFrameDuration += aDelta;
		if (myCurentFrameDuration >= myFrameDuration)
		{
			myFrame++;
			myCurentFrameDuration = 0;
			if (myFrame >= myNumberOfFrames)
			{
				myFrame = 0;
			}
			UpdateTextureRect();
		}
	}
}

void Animation::Render(Synchronizer& aSynchronizer, DX2D::Vector2f aPos)
{
	
	RenderCommand command;
	command.myType = eRenderType::eSprite;
	command.myPosition = aPos;
	command.mySprite = mySprite;

	aSynchronizer.AddRenderCommand(command);
}

void Animation::UpdateTextureRect()
{
	
	float x = (myOriginalSpriteWidht * myFrame);

	mySprite->SetTextureRect(x, 0,
		mySprite->GetSize().x / myNumberOfFrames, 1);
}

Animation::~Animation()
{
	delete mySprite;
}
