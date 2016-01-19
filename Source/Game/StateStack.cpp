#include "stdafx.h"
#include "StateStack.h"
#include "GameState.h"
#include "Synchronizer.h"

StateStack::StateStack()
{
	myMainIndex = -1;
	mySubIndex = -1;
	myGameStates.Init(16);
}

StateStack::~StateStack()
{
}

void StateStack::PopSubGameState()
{
	//DL_ASSERT_EXP(myGameStates.Size() > 0, "Can't pop an empty stack.");

	myGameStates[myMainIndex][mySubIndex]->EndState();

	myGameStates[myMainIndex].DeleteCyclicAtIndex(mySubIndex);
	--mySubIndex;
}

void StateStack::PopMainGameState()
{
	while (myGameStates[myMainIndex].Size() > 0)
	{
		PopSubGameState();
	}

	myGameStates.RemoveCyclicAtIndex(myMainIndex);
	--myMainIndex;

	if (myMainIndex >= 0)
	{
		mySubIndex = myGameStates[myMainIndex].Size() - 1;
	}
	else
	{
		mySubIndex = -1;
	}
}

void StateStack::PushSubGameState(GameState* aSubGameState)
{
	//DL_ASSERT_EXP(myGameStates.Size() > 0, "Can't push sub game state, no main game state present.");

	myGameStates[myMainIndex].Add(aSubGameState);
	aSubGameState->InitState();

	mySubIndex = myGameStates[myMainIndex].Size() - 1;
}

void StateStack::PushMainGameState(GameState* aMainGameState)
{
	myGameStates.Add(CommonUtilities::GrowingArray<GameState*, int>(16));
	myMainIndex = myGameStates.Size() - 1;
	PushSubGameState(aMainGameState);
}

bool StateStack::UpdateCurrentState(float aTimeDelta)
{
	if (myGameStates.Size() > 0 == true)
	{
		switch (myGameStates[myMainIndex][mySubIndex]->Update(aTimeDelta))
		{
		case eStateStatus::ePopSubState:
			PopSubGameState();
			break;
		case eStateStatus::ePopMainState:
			PopMainGameState();
			if (myGameStates.Size() > 0 == false)
				return false;
			break;
		case eStateStatus::eKeepState:
			break;
		}
		return true;
	}
	return false;
}

void StateStack::RenderCurrentState(Synchronizer& aSynchronizer)
{
	//DL_ASSERT_EXP(myGameStates.Size() > 0, "Can't render, no gamestate present.");
	RenderStateAtIndex(mySubIndex, aSynchronizer);
}

void StateStack::RenderStateAtIndex(int aIndex, Synchronizer& aSynchronizer)
{
	if (aIndex < 0)
	{
		return;
	}

	if (myGameStates[myMainIndex][aIndex]->LetThroughRender() == true)
	{
		RenderStateAtIndex(aIndex - 1, aSynchronizer);
	}

	myGameStates[myMainIndex][aIndex]->Render(aSynchronizer);
}

void StateStack::Clear()
{
	while (myGameStates.Size() > 0)
	{
		PopMainGameState();
	}
}