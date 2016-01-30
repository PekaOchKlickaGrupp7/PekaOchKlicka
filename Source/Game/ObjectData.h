#pragma once
#include "..\CommonUtilities\GrowingArray.h"
#include "HitBox.h"
#include "Triangle.h"

class Event;

enum TriggerType
{
	Box,
	Circle,
	Triangles2
};

enum EventTypes
{
	OnClick,
	OnHover,
	OnLeave,
	OnLoad,
	Custom,
	OnUseItem
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

	void AddTriangle(Triangle& aTriangle);

	DX2D::CSprite* mySprite;
	DX2D::CSprite* myOriginalSprite;

	std::string myName;
	float myX;
	float myY;
	float myGlobalX;
	float myGlobalY;
	float myScaleX;
	float myScaleY;
	float myRotation;
	float myPivotX;
	float myPivotY;
	bool myActive;
	DX2D::CColor myColor;

	bool myTriggerEnabled;
	HitBox myHitBox;
	float myRadius;
	TriggerType myTriggerType;

	bool myIsHovering;

	CommonUtilities::GrowingArray<ObjectData*, unsigned int> myChilds;
	CommonUtilities::GrowingArray<Event*, unsigned int> myEvents;
	CommonUtilities::GrowingArray<Triangle, unsigned int> myTriangles;
};

