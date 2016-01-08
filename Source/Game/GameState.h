#pragma once
#include "GameStatus.h"
#include "..\CommonUtilities\Vector2.h"
#include "..\CommonUtilities\InputWrapper.h"
#include "..\CommonUtilities\TimerManager.h"

class StateStackProxy;
class AudioSystem;
class Synchronizer;


class GameState
{
public:
	GameState(StateStackProxy& aStateStackProxy,
		CU::DirectInput::InputWrapper& aInputWrapper, CU::TimeSys::TimerManager& aTimerManager,
		const bool aLetThroughRender = false);
	virtual ~GameState();
	virtual eStateStatus Update(float aTimeDelta) = 0;
	virtual void InitState();
	virtual void EndState();
	virtual void Render(Synchronizer& aSynchronizer);
	bool LetThroughRender() const;

protected:
	GameState& operator=(const GameState&);
	StateStackProxy& myStateStackProxy;
	CU::DirectInput::InputWrapper& myInputWrapper;
	CU::TimeSys::TimerManager& myTimerManager;

	Vector2<float> myResolution;
	const bool myLetThroughRender;
};

