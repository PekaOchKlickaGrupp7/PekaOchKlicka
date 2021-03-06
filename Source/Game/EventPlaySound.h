#pragma once
#include "Event.h"
class EventPlaySound :
	public Event
{
public:
	EventPlaySound();

	virtual void Init(Room* aRoom, CGameWorld* aGameWorld) override;
	virtual bool Update(const float aDeltaTime) override;
	virtual void Reset() override;

	std::string myTargetSound;
	float myVolume;
	bool myIsLooping;
	bool myIs3D;
	DX2D::Vector2f myPosition;
	std::string myIdentifier;

	virtual ~EventPlaySound();
};

