#pragma once
#include <vector>
#include "SoundManager.h"
#include "GameState.h"
#include "Sound.h"
#include "ObjectData.h"
#include "JSON.h"
#include "Player.h"
#include "Item.h"
#include <tga2d\text\text.h>
#include "Pathfinding.h"

class Room;

class CGameWorld : public GameState
{
public:
	CGameWorld(StateStackProxy& aStateStackProxy, CU::DirectInput::InputManager& aInputManager,
		CU::TimeSys::TimerManager& aTimerManager);
	~CGameWorld();

	void Init();

	void DoChangeLevel(Room* aCurrentRoom);
	void ChangeLevel(const std::string& aString);
	Player* CGameWorld::GetPlayer();
	void SetPlayerTargetPosition(Point2f aPoint);
	const Vector2f GetPlayerTargetPosition() const;
	void SetCinematicMode(bool aOn = true);
	void SetFadeIn(bool aFade = true);
	float GetFadeIn() const;

	void Quit();

	eStateStatus Update(float aTimeDelta) override;
	void Render(Synchronizer& aSynchronizer)override;
	void PlayerMovement(bool aCheckInput, float aTimeDelta);

private:
	void ItemPickUp();
	DX2D::CText* myTextFPS;

	bool myDoQuit;
	bool myPlayerCanMove;
	float myFadeIn;
	bool myDoFadeIn;

	Pathfinding myPathfinding;

	void RenderObject(Synchronizer& aData, ObjectData* aNode, float relativeX, float relativeY);

	std::map<std::string, Room*> myRooms;
	Room* myCurrentRoom;

	CommonUtilities::GrowingArray<DX2D::CSprite*, int> myDotSprites;

	JSON myJson;

	Player myPlayer;

	DX2D::CSprite* myResTest;
	bool myShouldRenderDebug;
	bool myShouldRenderFPS;

	DX2D::Vector2f myTargetPosition;
};
