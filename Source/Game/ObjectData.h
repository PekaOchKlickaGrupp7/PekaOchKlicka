#pragma once
#include "..\CommonUtilities\GrowingArray.h"
#include "HitBox.h"

class Event;

enum TriggerType
{
	Box,
	Circle
};

enum EventTypes
{
	OnClick,
	OnHover,
	OnLoad,
	Custom
};

namespace DX2D
{
	class CSprite;
}

class ObjectData
{
public:
	ObjectData();
	~ObjectData();

	void ObjectData::Update(float aDeltaTime);


	DX2D::CSprite* mySprite;

	std::string myName;
	float myX, myY, myScaleX, myScaleY, myRotation, myPivotX, myPivotY;
	bool myActive;

	bool myTriggerEnabled = true;
	HitBox myHitBox;
	float myRadius;
	TriggerType myTriggerType;

	CommonUtilities::GrowingArray<ObjectData*, unsigned int> myChilds;
	CommonUtilities::GrowingArray<Event*, unsigned int> myEvents;
};

