#pragma once
#include "MenuItem.h"
#include "..\CommonUtilities\Vector2.h"
#include <tga2d/sprite/sprite.h>

class MenuImageItem : public MenuItem
{
public:
	MenuImageItem(eAction anAction, const char* aSpritePath
		, const char* aSpriteHighlightPath, Vector2<float> aPos
		, float aScale, Vector2<float> aSize = Vector2<float>(128.f, 32.f));
	~MenuImageItem();
	void Render(Synchronizer& aSynchronizer, unsigned long aARGB = 0xFFFFFFFF) override;
	bool Collide(const Vector2<float>& aCursorPos) const override;

private:
	DX2D::CSprite* mySprite;
	DX2D::CSprite* mySpriteHighlight;
	float myScale;
};

