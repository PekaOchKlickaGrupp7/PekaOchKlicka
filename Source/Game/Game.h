#pragma once
#include <tga2d/Engine.h>
#include "GameWorld.h"


class CGame
{
public:
	CGame();
	~CGame();
	void Init();
private:
	void InitCallBack();
	void UpdateCallBack();
	void LogCallback(std::string aText);

	CGameWorld myGameWorld;


};
