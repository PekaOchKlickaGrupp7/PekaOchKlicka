#pragma once
#include "MenuItem.h"
#include <tga2d/text/text.h>

class MenuTextItem : public MenuItem
{
public:
	MenuTextItem(eAction anAction
		, DX2D::CText* aText
		, Vector2<float> aPos
		, Vector2<float> aSize = Vector2<float>(512.f, 128.f)
		, int aId = 0);
	~MenuTextItem();
	void Render(Synchronizer& aSynchronizer, unsigned long aARGB = 0xFFFFFFFF) override;
	bool Collide(const Vector2<float>& aCursorPos) const override;
	int GetId() const override;
private:
	DX2D::CText* myText;
	int myId;
};

inline int MenuTextItem::GetId() const
{
	return myId;
}