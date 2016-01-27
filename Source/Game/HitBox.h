#pragma once
#include <string>

class ObjectData;

class HitBox
{
public:
	HitBox();
	~HitBox();

	void Init(ObjectData* aEvent);
	void Parse(const std::string& aString);
	bool IsMouseColliding(const float aX, const float aY, const float aRelativeX, const float aRelativeY);

	ObjectData* myObject;
	float myX, myY, myWidth, myHeight;
};

