#pragma once
#include "..\CommonUtilities\GrowingArray.h"
#include "Event.h"

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

	float myX, myY, myScaleX, myScaleY, myRotation, myPivotX, myPivotY;
	bool myActive;

	CommonUtilities::GrowingArray<ObjectData*, unsigned int> myChilds;
	CommonUtilities::GrowingArray<Event, unsigned int> myEvents;
};

