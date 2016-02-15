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

	DX2D::CColor myColor;

	bool myIsTalking;
	float myShowTime;
	float myLetterLength;

	float mySize;

	static bool myIsActive;

protected:

	bool TypeNextLetter(float aDeltaTime);

	DX2D::CText* myTextOutline;
	DX2D::CText* myTextRender;

	unsigned int myCurrentLetter;

	float myShowedTime;
	float myCurrentTime;
	float myWidth;
	float myHeight;

	bool myFirstFrame;
};

