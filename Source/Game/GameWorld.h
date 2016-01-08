#pragma once
#include <vector>
#include "SoundManager.h"
#include "GameState.h"

namespace DX2D
{
	class CSprite;
	class CSpriteBatch;
	class CText;
	class CCustomShape;
}

class CGameWorld : public GameState
{
public:
	CGameWorld(StateStackProxy& aStateStackProxy, CU::DirectInput::InputWrapper& aInputWrapper, CU::TimeSys::TimerManager& aTimerManager);
	~CGameWorld();

	void Init();
	eStateStatus Update(float aTimeDelta) override;
	void Render(Synchronizer& aSynchronizer)override;
private:

	DX2D::CSprite* myCarSprite;
	DX2D::CSprite* myCarSprite2;
	DX2D::CText* text;

	DX2D::CCustomShape* myShape;

	SoundManager sound;

};