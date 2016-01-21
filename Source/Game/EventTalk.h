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

	float myShowTime;
	std::string myText;
	std::string myFontPath;

	DX2D::CColor myColor;

protected:
	DX2D::CText* myTextRender;

	int myWordCount;
	float myCurrentTime;
	
};

