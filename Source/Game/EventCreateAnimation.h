#pragma once
#include "Event.h"
class EventCreateAnimation :
	public Event
{
public:
	EventCreateAnimation();
	virtual ~EventCreateAnimation();

	virtual void Init(Room* aRoom, CGameWorld* aGameWorld) override;
	virtual bool Update(const float aDeltaTime) override;
	virtual void Reset() override;

	std::string myFilePath;
	int myNumberOfFrames;
	int myFramesPerRow;
	float myFrameDuration;
	float myScale;
	bool myFlip;
protected:
	Animation* myAnimation;
};

