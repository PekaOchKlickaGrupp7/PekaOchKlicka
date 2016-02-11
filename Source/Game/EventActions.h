#pragma once

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
	RemoveSelectedItem,
	Deselect,
	ResetGame,
	ItemIsNot,
	Answer
};