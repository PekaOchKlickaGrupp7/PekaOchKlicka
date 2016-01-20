#include "stdafx.h"
#include "MenuImageItem.h"
#include "Synchronizer.h"
#include "..\CommonUtilities\Intersection.h"


MenuImageItem::MenuImageItem(eAction anAction, const char* aSpritePath
	, const char* aSpriteHighlightPath, Vector2<float> aPos
	, float aScale, Vector2<float> aSize)
	: MenuItem(anAction, aPos, aSize)
	, myScale(aScale)
{
	mySprite = new DX2D::CSprite(aSpritePath);
	//mySprite->SetPivot(DX2D::Vector2<float>(mySprite->GetSize().x / 2, mySprite->GetSize().y / 2));

	mySpriteHighlight = new DX2D::CSprite(aSpriteHighlightPath);
	//mySpriteHighlight->SetPivot(DX2D::Vector2<float>(mySpriteHighlight->GetSize().x / 2, mySpriteHighlight->GetSize().y / 2));

	myPos = aPos;
}

MenuImageItem::~MenuImageItem()
{
	delete mySprite;
	delete mySpriteHighlight;
}

void MenuImageItem::Render(Synchronizer& aSynchronizer, unsigned long aARGB)
{
	RenderCommand command;
	command.myConvertFromPixelToSpaceNormal = true;

	command.myPosition.x = myPos.x;
	command.myPosition.y = myPos.y;
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

bool MenuImageItem::Collide(const Vector2<float>& aCursorPos) const
{
	/*return CommonUtilities::Intersection::PointVsRect(aCursorPos, Vector2<float>(myPos.x + 8, myPos.y + 20),
		Vector2<float>(myPos.x + 8, myPos.y + 20) + mySize);*/
	return CommonUtilities::Intersection::PointVsRect(aCursorPos,myPos,myPos + mySize);
}