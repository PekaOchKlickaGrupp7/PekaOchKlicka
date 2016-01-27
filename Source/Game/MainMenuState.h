#pragma once
#include "GameState.h"
#include "..\CommonUtilities\Vector.h"
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
		, CU::DirectInput::InputManager& aInputManager,
		CU::TimeSys::TimerManager& aTimerManager);
	~MainMenuState();
	eStateStatus Update(float aTimeDelta) override;
	void InitState() override;
	void Render(Synchronizer& aSynchronizer) override;

private:
	void CalcHighlights();
	void LoadJson();
	const char* ReadFile(const char* aFile);
	MenuItem::eAction GetActionType(std::string& aSelection);
	MenuItem::eAction mySelection;

	DX2D::CSprite* myBackground;
	DX2D::CSprite* myTitle;

	CommonUtilities::VectorOnStack<MenuItem*, 4, int> myButtons;
};

