#include "stdafx.h"
#include "EventResetGame.h"
#include "GameWorld.h"

EventResetGame::EventResetGame()
{
}

EventResetGame::~EventResetGame()
{
}

bool EventResetGame::Update(const float)
{
	myGameWorld->ResetGame();

	return true;
}

void EventResetGame::Reset()
{

}