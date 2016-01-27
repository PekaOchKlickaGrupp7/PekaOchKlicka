#pragma once
#include "Event.h"
class EventToggleFullscreen :
	public Event
{
public:
	EventToggleFullscreen();
	virtual ~EventToggleFullscreen();

	virtual void Init(Room* aRoom, CGameWorld* aGameWorld) override;
	virtual bool Update(const float aDeltaTime) override;
	virtual void Reset() override;
protected:

};

