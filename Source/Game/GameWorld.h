#pragma once
#include <vector>
#include "SoundManager.h"
#include "GameState.h"
#include <tga2d/sprite/sprite.h>
#include <tga2d/Engine.h>
#include <tga2d/light/light_manager.h>
#include <tga2d/sprite/sprite_batch.h>
#include <tga2d/drawers/debug_drawer.h>
#include <tga2d/text/text.h>
#include <tga2d/primitives/custom_shape.h>
#include <tga2d/math/color.h>
#include "Sound.h"

class CGameWorld : public GameState
{
public:
	CGameWorld(StateStackProxy& aStateStackProxy, CU::DirectInput::InputWrapper& aInputWrapper, CU::TimeSys::TimerManager& aTimerManager);
	~CGameWorld();

	void Init();
	eStateStatus Update(float aTimeDelta) override;
	void Render(Synchronizer& aSynchronizer)override;
private:
	DX2D::CText* text;
	DX2D::CSprite* myResolutionTestSprite;

	DX2D::CSprite* myAudioSourceSprite;
	DX2D::CSprite* myAudioListenerSprite;
	DX2D::Vector2f myAudioSourcePosition;

	Sound mySFXRain;
};