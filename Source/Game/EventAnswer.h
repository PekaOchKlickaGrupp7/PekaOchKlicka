#pragma once
#include "Event.h"

//Work in progress
class EventAnswer :
	public Event
{
public:
	EventAnswer();
	virtual ~EventAnswer();

	virtual void Init(Room* aRoom, CGameWorld* aGameWorld) override;
	virtual bool Update(const float aDeltaTime) override;
	virtual void Reset() override;
	virtual void Render(Synchronizer &aSynchronizer) override;

	std::string myText;

	DX2D::CColor myColor;
	DX2D::CColor myHighlightColor;
	int myTextIndex;
protected:
	
	DX2D::CText* myTextRender;
	DX2D::CSprite* myBackgroundSprite;

	float myHeight;
};

