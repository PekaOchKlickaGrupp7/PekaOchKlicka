#pragma once
#include "Button.h"

class EventChangeLevel;

class MainMenuButton :
	public Button
{
public:
	MainMenuButton();

	void InitValue() override;
	void ButtonSpecificUpdate() override;
	void ProgressGet() override;
	void ProgressSet() override;

	~MainMenuButton();
private:
	EventChangeLevel* myEvent;
};

