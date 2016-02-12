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
#include "Options.h"

class Room;

class CGameWorld : public GameState
{
public:
	CGameWorld(StateStackProxy& aStateStackProxy, CU::DirectInput::InputManager& aInputManager,
		CU::TimeSys::TimerManager& aTimerManager);
	~CGameWorld();

	Options* GetOptions();
	void Init();
	void DoChangeLevel(Room* aCurrentRoom);
	void ChangeLevel(const std::string& aString);
	Player* GetPlayer();
	void SetPlayerTargetPosition(Point2f aPoint);
	const Vector2f GetPlayerTargetPosition() const;

	void SetCinematicMode(bool aOn = true);
	bool GetCinematicMode() const;

	void ResetGame();

	void SetFadeIn(bool aFade = true);
	float GetFadeIn() const;
	bool PlayerHasReachedTarget();
	void Quit();

	void SetTalkIsOn();
	void SetTalkIsOff();

	eStateStatus Update(float aTimeDelta) override;
	void Render(Synchronizer& aSynchronizer)override;
private:
	void PlayerMovement(bool aCheckInput, bool aTalkIsOn, bool aPlayerCanMove, float aTimeDelta);
	void RenderObject(Synchronizer& aData, ObjectData* aNode, float relativeX, float relativeY);

	CommonUtilities::GrowingArray<DX2D::CSprite*, int> myDotSprites;
	CommonUtilities::GrowingArray<Node*, int>* myWaypointNodes;
	std::map<std::string, Room*> myRooms;

	DX2D::Vector2f myTargetPosition;
	DX2D::Vector2f myNewTargetPosition;
	DX2D::CText* myTextFPS;
	DX2D::CSprite* myResTest;

	Player myPlayer;
	Pathfinding myPathfinding;
	JSON myJson;
	Room* myCurrentRoom;

	Options myOptionsMenu;

	int myCurrentWaypoint;
	float myFadeIn;

	bool myTalkIsOn;
	bool myDoQuit;
	bool myPlayerCanMove;
	bool myHasNewTargetPosition;
	bool myShouldRenderDebug;
	bool myShouldRenderFPS;
	bool myShouldRenderNavPoints;
	bool myDoFadeIn;
	bool myHasPath;
};
