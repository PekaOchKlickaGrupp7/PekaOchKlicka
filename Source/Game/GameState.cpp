#include "stdafx.h"
#include "GameState.h"



GameState::GameState(StateStackProxy& aStateStackProxy
	, CU::DirectInput::InputManager& aInputManager, CU::TimeSys::TimerManager& aTimerManager
	, bool aLetThroughRender)
	: myStateStackProxy(aStateStackProxy)

	, myInputManager(aInputManager)
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

bool GameState::LetThroughRender() const
{
	return myLetThroughRender;
}
