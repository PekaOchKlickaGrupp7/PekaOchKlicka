#pragma once
#include "Event.h"

class EventTalk :
	public Event
{
public:
	EventTalk();
	virtual ~EventTalk();

	virtual void Init(Room* aRoom, CGameWorld* aGameWorld) override;
	virtual bool Update(const float aDeltaTime) override;
	virtual void Reset() override;
	virtual void Render(Synchronizer &aSynchronizer) override;

	std::string myText;
	std::string myFontPath;

	DX2D::CColor myColor;

	bool myIsTalking;
	float myShowTime;
	float myTextSize;

protected:

	bool NewSubString();


	DX2D::CText* myTextRender;

	int myWordCount;
	float myCurrentTime;
	
};

