#pragma once
#include "Event.h"
#include "rapidjson\document.h"

class Room;
class CGameWorld;
class ObjectData;

enum EventActions
{
	None,
	SetActive,
	ChangeLevel,
	Talk,
	ChangeCursor,
	PlaySoundFile,
	ChangeImage,
	ChangeToOriginalImage,
	Delay,
	IfVariable,
	StopSound,
	ChangeSoundPosition,
	Quit,
	FadeColor,
	SetColor,
	FadePosition,
	SetPosition,
	SetVariable,
	IfGlobalVariable,
	SetGlobalVariable,
	ToggleFullscreen,
	WalkTo,
	HideMouse,
	SetCinematic,
	PickupItem,
	IsItem,
	ChangePlayerDirection,
	RemoveSelectedItem
};

class EventsFactory
{
public:
	EventsFactory();
	~EventsFactory();

	Event* CreateEventData(ObjectData* aData, rapidjson::Value& aParent, Room* aRoom, CGameWorld* aGameWorld) const;
};

