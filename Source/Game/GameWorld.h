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
	Player* CGameWorld::GetPlayer();

	eStateStatus Update(float aTimeDelta) override;
	void Render(Synchronizer& aSynchronizer)override;
private:
	void ItemPickUp();
	DX2D::CText* text;

	void RenderObject(Synchronizer& aData, ObjectData* aNode, float relativeX, float relativeY);

	std::map<std::string, Room*> myRooms;
	Room* myCurrentRoom;

	JSON myJson;

	Player myPlayer;

	DX2D::Vector2f myTargetPosition;
};