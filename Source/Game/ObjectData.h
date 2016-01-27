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
	OnLeave,
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

	DX2D::CSprite* mySprite;
	DX2D::CSprite* myOriginalSprite;

	std::string myName;
	float myX, myY, myGlobalX, myGlobalY, myScaleX, myScaleY, myRotation, myPivotX, myPivotY;
	bool myActive;
	DX2D::CColor myColor;

	bool myTriggerEnabled;
	HitBox myHitBox;
	float myRadius;
	TriggerType myTriggerType;

	bool myIsHovering;

	CommonUtilities::GrowingArray<ObjectData*, unsigned int> myChilds;
	CommonUtilities::GrowingArray<Event*, unsigned int> myEvents;
	CommonUtilities::GrowingArray<bool, unsigned int> myBoolVariables;
	CommonUtilities::GrowingArray<std::string, unsigned int> myStringVariables;
	CommonUtilities::GrowingArray<int, unsigned int> myFloatVariables;
	CommonUtilities::GrowingArray<int, unsigned int> myIntVariables;
};

