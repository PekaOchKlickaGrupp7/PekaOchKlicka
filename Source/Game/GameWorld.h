#pragma once
#include <vector>
#include "SoundManager.h"
#include "GameState.h"
#include "Sound.h"
#include "ObjectData.h"
#include "JSON.h"
#include "Character.h"

class CGameWorld : public GameState
{
public:
	CGameWorld(StateStackProxy& aStateStackProxy, CU::DirectInput::InputManager& aInputManager,
		CU::TimeSys::TimerManager& aTimerManager);
	~CGameWorld();

	void Init();
	eStateStatus Update(float aTimeDelta) override;
	void Render(Synchronizer& aSynchronizer)override;
	void RenderLevel(Synchronizer& aSynchronizer, ObjectData* aNode);
private:
	DX2D::CText* text;
	DX2D::CSprite* myResolutionTestSprite;

	DX2D::CSprite* myAudioSourceSprite;
	DX2D::CSprite* myAudioListenerSprite;
	DX2D::Vector2f myAudioSourcePosition;

	CommonUtilities::GrowingArray<ObjectData*, unsigned int> myObjects;

	JSON myJson;

	Sound mySFXRain;

	Character myPlayer;
};