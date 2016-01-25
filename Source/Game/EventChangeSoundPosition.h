#pragma once
#include "Event.h"
class EventChangeSoundPosition :
	public Event
{
public:
	EventChangeSoundPosition();

	virtual void Init(Room* aRoom, CGameWorld* aGameWorld) override;
	virtual bool Update(const float aDeltaTime) override;
	virtual void Reset() override;

	std::string myIdentifier;
	DX2D::Vector2f myPosition;

	virtual ~EventChangeSoundPosition();
};

