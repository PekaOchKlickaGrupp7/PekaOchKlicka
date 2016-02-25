#pragma once
#include "Event.h"
#include "../CommonUtilities/GrowingArray.h"

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
	bool myCanBeInterupted;

	volatile static int myCountActive;
	static bool myIsActive;
protected:
	bool TypeNextLetter(float aDeltaTime);
	void CutUpString();

	DX2D::CText* myTextOutline;
	DX2D::CText* myTextRender;

	std::string mySoundPath;
	std::string myIdentifier;

	CommonUtilities::GrowingArray < DX2D::CText* > myTextRenders;

	unsigned int myCurrentLetter;
	int mySounds;
	//unsigned int myShakeStart;
	//unsigned int myShakeStop;
	unsigned int myCurrentIndex;

	float myShowedTime;
	float myCurrentTime;
	float myWidth;
	float myHeight;

	bool myFirstFrame;
	bool myCached;
	bool myMouseIsInvisible;
};

