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

	mySpriteHighlight = new DX2D::CSprite(aSpriteHighlightPath);

	//mySprite->SetTextureRect(0, 0, mySize.x, mySize.y);
	//mySpriteHighlight->SetTextureRect(0, 0, mySize.x, mySize.y);
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
	return CommonUtilities::Intersection::PointVsRect(aCursorPos, myPos, myPos + (myScale * mySize));
}