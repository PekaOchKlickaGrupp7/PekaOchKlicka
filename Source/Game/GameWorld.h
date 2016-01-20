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
#include "ObjectData.h"
#include "JSON.h"
#include "Room.h"

class CGameWorld : public GameState
{
public:
	CGameWorld(StateStackProxy& aStateStackProxy, CU::DirectInput::InputWrapper& aInputWrapper,
		CU::TimeSys::TimerManager& aTimerManager);
	~CGameWorld();

	void Init();

	void ChangeLevel(const std::string& aString);

	eStateStatus Update(float aTimeDelta) override;
	float CGameWorld::Remap(float value, float from1, float to1, float from2, float to2);
	void Render(Synchronizer& aSynchronizer)override;
	void RenderLevel(Synchronizer& aSynchronizer, ObjectData* aNode);

	static CGameWorld* const myInstance;
private:


	DX2D::CText* text;
	DX2D::CSprite* myResolutionTestSprite;

	DX2D::CSprite* myAudioSourceSprite;
	DX2D::CSprite* myAudioListenerSprite;
	DX2D::Vector2f myAudioSourcePosition;

	std::map<std::string, Room*> myRooms;
	Room* myCurrentRoom;

	JSON myJson;

	Sound mySFXRain;
};