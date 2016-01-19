#include "stdafx.h"
#include "MenuCursor.h"
#include "..\CommonUtilities\InputManager.h"
#include "Synchronizer.h"


MenuCursor::MenuCursor()
{
	mySprite = new DX2D::CSprite("Sprites/menu/cursor.dds");

	mySprite->SetPivot(DX2D::Vector2<float>(mySprite->GetSize().x / 2.0f,
		mySprite->GetSize().y / 2.0f));

	mySpriteHighlight = new DX2D::CSprite("Sprites/menu/cursorHighlight.dds");

	mySpriteHighlight->SetPivot(DX2D::Vector2<float>(mySpriteHighlight->GetSize().x / 2.0f,
		mySpriteHighlight->GetSize().y / 2.0f));

	myShoot = false;
	myMelee = false;
	myHighlight = false;
}


MenuCursor::~MenuCursor()
{
	delete mySprite;
	delete mySpriteHighlight;
}


void MenuCursor::Update(CU::DirectInput::InputManager& anInputManager)
{
	DX2D::Vector2ui windowSize = DX2D::CEngine::GetInstance()->GetWindowSize();
	myScreenPos.x = anInputManager.GetAbsoluteMousePos().x - (mySprite->GetSize().x * windowSize.x) / 2;
	myScreenPos.y = anInputManager.GetAbsoluteMousePos().y - (mySprite->GetSize().y * windowSize.y) / 2;
	if (myScreenPos.x > windowSize.x || myScreenPos.y > windowSize.y
		|| myScreenPos.x < 0 || myScreenPos.y < 0)
	{
		return;
	}


	myShoot = anInputManager.LeftMouseButtonClicked();
	myMelee = anInputManager.RightMouseButtonClicked();
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