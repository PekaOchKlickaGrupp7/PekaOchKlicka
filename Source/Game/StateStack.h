#pragma once
#include "..\CommonUtilities\GrowingArray.h"

class GameState;
class Synchronizer;

class StateStack
{
public:
	StateStack();
	~StateStack();

	void PushSubGameState(GameState* aSubGameState);
	void PushMainGameState(GameState* aMainGameState);
	bool UpdateCurrentState(float aTimeDelta);
	void RenderCurrentState(Synchronizer& aSynchronizer);
	void Clear();

private:
	CommonUtilities::GrowingArray<CommonUtilities::GrowingArray<GameState*, int>, int> myGameStates;
	void PopSubGameState();
	void PopMainGameState();
	void RenderStateAtIndex(int aIndex, Synchronizer& aSynchronizer);
	int myMainIndex;
	int mySubIndex;
};

