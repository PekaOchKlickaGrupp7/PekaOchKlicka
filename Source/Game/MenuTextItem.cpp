#include "stdafx.h"
#include "MenuTextItem.h"
#include "Synchronizer.h"
#include "..\CommonUtilities\Intersection.h"

MenuTextItem::MenuTextItem(eAction anAction
	, DX2D::CText* aText
	, Vector2<float> aPos
	, Vector2<float> aSize
	, int aId)
	: MenuItem(anAction, aPos, aSize)
	, myText(aText)
	, myId(aId)
{

}

MenuTextItem::~MenuTextItem()
{
}

void MenuTextItem::Render(Synchronizer& aSynchronizer, unsigned long aARGB)
{
	aARGB;
	RenderCommand command;
	command.myPosition.x = myPos.x;
	command.myPosition.y = myPos.y;
	command.myType = eRenderType::eText;
	command.myText = myText;
	aSynchronizer.AddRenderCommand(command);
}

bool MenuTextItem::Collide(const Vector2<float>& aCursorPos) const
{
	return CommonUtilities::Intersection::PointVsRect(aCursorPos, myPos, myPos + mySize);
}