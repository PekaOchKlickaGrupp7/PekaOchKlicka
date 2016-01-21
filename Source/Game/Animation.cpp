#include "stdafx.h"
#include "Animation.h"


Animation::Animation()
{
	myFrameDuration = 0;
	myCurentFrameDuration = 0;
	mySprite = nullptr;
	myNumberOfFrames = 0;
	myFrame = 0;
	myFramesPerRow = 0;
	myPaused = false;
}

void Animation::Init(const char* aSpriteFilePath, float aFrameDuration, int aNumberOfFrames, int aFramesPerRow)
{
	myFrameDuration = aFrameDuration;
	myCurentFrameDuration = 0;
	myFramesPerRow = aFramesPerRow;
	mySprite = new DX2D::CSprite(aSpriteFilePath);

	myNumberOfFrames = aNumberOfFrames;
	myFrame = 0;
	myPaused = false;

	mySprite->SetSize(DX2D::Vector2f(1.f / myFramesPerRow, 1));

	mySprite->SetUVScale(DX2D::Vector2f(1.f / myFramesPerRow, 1));

	mySprite->SetPivot(DX2D::Vector2f(0.5f,0.5f));

	UpdateTextureRect();

}
void Animation::UpdateTextureRect()
{
	mySprite->SetUVOffset(DX2D::Vector2f((1.f / myFramesPerRow)*(myFrame% myFramesPerRow),
		mySprite->GetSize().y*(myFrame / myFramesPerRow)));
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

void Animation::Destroy()
{
	delete mySprite;
	mySprite = nullptr;
}
Animation::~Animation()
{
	Destroy();
}
