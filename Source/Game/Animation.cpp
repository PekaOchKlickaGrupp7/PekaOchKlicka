#include "stdafx.h"
#include "Animation.h"


Animation::Animation(const char* aSpriteFilePath, DX2D::Vector2f aPivot, float aFrameDuration, int aNumberOfFrames, int aFramesPerRow, bool aLooping)
{
	myFrameDuration = aFrameDuration;
	myCurentFrameDuration = 0;
	myFramesPerRow = aFramesPerRow;
	mySprite = new DX2D::CSprite(aSpriteFilePath);

	myNumberOfFrames = aNumberOfFrames;
	myFrame = 0;
	myPaused = false;
	myFlip = false;

	myCurentRow = 0;
	myRowFrameCounter = 0;

	myIsLooping = aLooping;

	mySprite->SetSize(DX2D::Vector2f(1.f / myFramesPerRow, 1.f / myFramesPerRow));

	mySprite->SetUVScale(DX2D::Vector2f(1.f / myFramesPerRow, 1.f / myFramesPerRow));

	mySprite->SetPivot(aPivot);

	UpdateTextureRect();
	//HEJ BERRA
}

void Animation::SetColor(DX2D::CColor& aColor)
{
	myColor = aColor;
}

void Animation::UpdateTextureRect()
{
	mySprite->SetUVOffset(DX2D::Vector2f((1.f / myFramesPerRow)*(myFrame% myFramesPerRow),
		(1.f / myFramesPerRow) * (myFrame / myFramesPerRow)));
}
void Animation::SetSize(float aScale)
{
	mySprite->SetSize(DX2D::Vector2f(aScale / myFramesPerRow, aScale / myFramesPerRow));
}
void Animation::SetFlip(bool aFlip)
{
	myFlip = aFlip;
}
DX2D::Vector2f Animation::GetSize()
{
	return mySprite->GetSize();
}
float Animation::GetScale()
{
	return mySprite->GetScale().x;
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
			myRowFrameCounter++;
			if (myNumberOfFrames == 16)
			{
				myRowFrameCounter = myRowFrameCounter;
			}

			if (myRowFrameCounter == myFramesPerRow)
			{
				myCurentRow++;
				myRowFrameCounter = 0;
			}
			if (myFrame >= myNumberOfFrames)
			{
				if (myIsLooping == false)
				{
					myFrame = myNumberOfFrames - 1;
					myRowFrameCounter = myFramesPerRow - 1;
					--myCurentRow;

					Pause();
				}
				else
				{
					myFrame = 0;
					myCurentRow = 0;
					myRowFrameCounter = 0;
				}
			}
			UpdateTextureRect();
		}
	}
}

bool Animation::GetIsLooping() const
{
	return myIsLooping;
}

bool Animation::GetIsPlaying() const
{
	return !myPaused;
}

void Animation::Render(Synchronizer& aSynchronizer, DX2D::Vector2f aPos)
{
	RenderCommand command;
	command.myType = eRenderType::eSprite;
	command.myPosition = aPos;
	mySprite->SetColor(myColor);
	
	if (myFlip == true)
	{
		DX2D::Vector2f size = mySprite->GetScale();
		mySprite->SetSize({ -abs(size.x), size.y });
	}

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
