#pragma once
#include "..\CommonUtilities\Vector2.h"
#include <tga2d/sprite/sprite.h>

namespace CU
{
	namespace DirectInput
	{
		class InputManager;
	}
}
class Synchronizer;


class MenuCursor
{
public:
	MenuCursor();
	~MenuCursor();

	void Update(CU::DirectInput::InputManager& anInputManager);
	void Render(Synchronizer& aSynchronizer);

	const Vector2<float>& GetScreenPos() const;
	bool Shoot() const;
	bool Melee() const;
	void SetHighlight(bool aHighlight);

private:
	Vector2<float> myScreenPos;
	DX2D::CSprite* mySprite;
	DX2D::CSprite* mySpriteHighlight;

	bool myHighlight;

	bool myShoot;
	bool myMelee;
};

inline bool MenuCursor::Shoot() const
{
	return myShoot;
}

inline bool MenuCursor::Melee() const
{
	return myMelee;
}

inline const Vector2<float>& MenuCursor::GetScreenPos() const
{
	return myScreenPos;
}

inline void MenuCursor::SetHighlight(bool aHighlight)
{
	myHighlight = aHighlight;
}