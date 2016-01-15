#include "stdafx.h"
#include "MenuCursor.h"
#include "..\CommonUtilities\InputWrapper.h"
#include "Synchronizer.h"


MenuCursor::MenuCursor()
{
	mySprite = new DX2D::CSprite("Sprites/menu/cursor.dds");

	mySprite->SetPivot(DX2D::Vector2<float>(mySprite->GetSize().x / 2,
		mySprite->GetSize().y / 2));

	mySpriteHighlight = new DX2D::CSprite("Sprites/menu/cursorHighlight.dds");

	mySpriteHighlight->SetPivot(DX2D::Vector2<float>(mySpriteHighlight->GetSize().x / 2,
		mySpriteHighlight->GetSize().y / 2));

	myShoot = false;
	myMelee = false;
	myHighlight = false;
}


MenuCursor::~MenuCursor()
{
	delete mySprite;
	delete mySpriteHighlight;
}


void MenuCursor::Update(CU::DirectInput::InputWrapper& anInputWrapper)
{
	myScreenPos.x = anInputWrapper.GetMouseLocationXInPixels() - (mySprite->GetSize().x * 1280) / 2;
	myScreenPos.y = anInputWrapper.GetMouseLocationYInPixels() - (mySprite->GetSize().y * 720) / 2;
	if (myScreenPos.x > 1280 || myScreenPos.y > 720
		|| myScreenPos.x < 0 || myScreenPos.y < 0)
	{
		return;
	}


	myShoot = anInputWrapper.GetMouseWasDown(0);
	myMelee = anInputWrapper.GetMouseWasDown(1);
}


void MenuCursor::Render(Synchronizer& aSynchronizer)
{
	RenderCommand command;

	command.myConvertFromPixelToSpaceNormal = true;

	command.myPosition.x = myScreenPos.x;
	command.myPosition.y = myScreenPos.y;
	command.myType = eRenderType::eSprite;

	if (myHighlight == false)
	{
		command.mySprite = mySprite;
	}
	else
	{
		command.mySprite = mySpriteHighlight;
	}
	aSynchronizer.AddRenderCommand(command);
}