#include "stdafx.h"
#include "GameState.h"



GameState::GameState(StateStackProxy& aStateStackProxy
	, CU::DirectInput::InputWrapper& aInputWrapper, CU::TimeSys::TimerManager& aTimerManager
	, bool aLetThroughRender)
	: myStateStackProxy(aStateStackProxy)

	, myInputWrapper(aInputWrapper)
	, myTimerManager(aTimerManager)
	, myLetThroughRender(aLetThroughRender)
{
	myResolution = Vector2<float>(0, 0);
}

GameState::~GameState()
{
}

void GameState::InitState()
{

}

void GameState::EndState()
{

}

void GameState::Render(Synchronizer& aSynchronizer)
{

}

bool GameState::LetThroughRender() const
{
	return myLetThroughRender;
}
