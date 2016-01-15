#pragma once
#include "GameState.h"
#include "..\CommonUtilities\Vector.h"
#include "MenuCursor.h"
#include "..\CommonUtilities\VectorOnStack.h"
#include "MenuItem.h"

class Synchronizer;


namespace DX2D
{
	class CSprite;
}

class MainMenuState : public GameState
{
public:
	MainMenuState(StateStackProxy& aStateStackProxy
		, CU::DirectInput::InputWrapper& aInputWrapper,
		CU::TimeSys::TimerManager& aTimerManager);
	~MainMenuState();
	eStateStatus Update(float aTimeDelta) override;
	void InitState() override;
	void Render(Synchronizer& aSynchronizer) override;

private:
	void CalcHighlights();

	MenuItem::eAction mySelection;

	DX2D::CSprite* myBackground;
	DX2D::CSprite* myTitle;

	CommonUtilities::VectorOnStack<MenuItem*, 4, int> myButtons;

	MenuCursor myMenuCursor;
};

