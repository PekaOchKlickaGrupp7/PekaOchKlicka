#pragma once
#include "..\CommonUtilities\GrowingArray.h"
#include "HitBox.h"
#include "Triangle.h"

class Event;
class Animation;

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
	OnFirstLoad,
	OnActivated,
	OnDisabled
};

namespace DX2D
{
	class CSprite;
}

class ObjectData
{
public:
	ObjectData();
	ObjectData(const std::string& aName);
	~ObjectData();

	void AddTriangle(Triangle& aTriangle);

	inline float GetX() const;
	inline float GetY() const;
	inline float GetGlobalX() const;
	inline float GetGlobalY() const;
	inline float GetScaleX() const;
	inline float GetScaleY() const;
	inline float GetRotation() const;
	inline float GetPivotX() const;
	inline float GetPivotY() const;

	inline bool GetActive() const;

	inline DX2D::CColor& GetColor() const;
	inline std::string& GetName() const;

	DX2D::CSprite* mySprite;
	DX2D::CSprite* myOriginalSprite;

	std::string myName;

	int myCurrentAnimation;
	bool myIsAnimation;
	CommonUtilities::GrowingArray<Animation*, unsigned int> myAnimations;

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
	int myAmountActiveEvents;

	DX2D::CColor myColor;

	bool myTriggerEnabled;
	HitBox myHitBox;
	float myRadius;
	TriggerType myTriggerType;

	bool myIsHovering;

	void CreateOriginalData();
	void ResetToOriginalData();

	CommonUtilities::GrowingArray<ObjectData*, unsigned int> myChilds;
	CommonUtilities::GrowingArray<Event*, unsigned int> myEvents;
	CommonUtilities::GrowingArray<Triangle, unsigned int> myTriangles;
protected:
	//Reset Game Data
	ObjectData* myOriginalData;
};

