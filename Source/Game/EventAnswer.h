#pragma once
#include "Event.h"
#include "../CommonUtilities/GrowingArray.h"

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

	CU::GrowingArray<std::string> myOptions;
	std::string myFontPath;
	std::string mySpritePath;

	DX2D::CColor myColor;
	DX2D::CColor myHighlightColor;

protected:

	DX2D::CSprite* mySprite;
	CU::GrowingArray<DX2D::CText*> myTextOptions;

	float myHeight;

};

