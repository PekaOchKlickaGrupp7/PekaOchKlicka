#pragma once
#include "Event.h"
class EventChangeImage :
	public Event
{
public:
	EventChangeImage();
	virtual ~EventChangeImage();

	virtual void Init(Room* aRoom, CGameWorld* aGameWorld) override;
	virtual bool Update(const float aDeltaTime) override;
	virtual void Reset() override;

	std::string myImagePath;
protected:
	DX2D::CSprite* myNewSprite;
};

