#pragma once
#include <vector>
#include "SoundManager.h"
#include "GameState.h"
#include "Sound.h"
#include "ObjectData.h"
#include "JSON.h"
#include "Player.h"
#include "Item.h"

class Room;

class CGameWorld : public GameState
{
public:
	CGameWorld(StateStackProxy& aStateStackProxy, CU::DirectInput::InputManager& aInputManager,
		CU::TimeSys::TimerManager& aTimerManager);
	~CGameWorld();

	void Init();

	void ChangeLevel(const std::string& aString);

	eStateStatus Update(float aTimeDelta) override;
	void Render(Synchronizer& aSynchronizer)override;
	void RenderLevel(Synchronizer& aSynchronizer, ObjectData* aNode);
private:
	void ItemPickUp();
	DX2D::CText* text;
	DX2D::CSprite* myResolutionTestSprite;

	std::map<std::string, Room*> myRooms;
	Room* myCurrentRoom;

	JSON myJson;

	Sound mySFXRain;

	Player myPlayer;

	DX2D::Vector2f myTargetPosition;
};