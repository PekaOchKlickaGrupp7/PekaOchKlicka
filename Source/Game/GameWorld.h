#pragma once
#include <vector>
#include "SoundManager.h"

namespace DX2D
{
	class CSprite;
	class CSpriteBatch;
	class CText;
	class CCustomShape;
}

class CGameWorld
{
public:
	CGameWorld();
	~CGameWorld();

	void Init();
	void Update(float aTimeDelta);
private:

	DX2D::CSprite* myCarSprite;
	DX2D::CSprite* myCarSprite2;
	DX2D::CText* text;

	DX2D::CCustomShape* myShape;

	SoundManager sound;
	
};