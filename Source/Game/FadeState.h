#pragma once
#include "GameState.h"
class FadeState :
	public GameState
{
public:
	FadeState(StateStackProxy& aStateStackProxy
		, CU::DirectInput::InputManager& aInputManager,
		CU::TimeSys::TimerManager& aTimerManager);
	~FadeState();
	eStateStatus Update(float aTimeDelta) override;
	void Render(Synchronizer& aSynchronizer) override;
private:

	DX2D::CSprite* mySprite;
	float myFadeTime;
	float myTime;
};

