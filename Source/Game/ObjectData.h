#include "..\CommonUtilities\GrowingArray.h"
#pragma once

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

	float myX, myY, myScaleX, myScaleY, myRotation, myPivotX, myPivotY;
	bool myActive;

	CommonUtilities::GrowingArray<ObjectData*, unsigned int> myChilds;
};

