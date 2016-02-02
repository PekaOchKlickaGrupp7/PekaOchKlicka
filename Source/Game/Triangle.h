#pragma once
#include "..\CommonUtilities\Vector2.h"

class ObjectData;

class Triangle
{
public:
	Triangle();
	~Triangle();

	float Sign(Vector2f p1, Vector2f p2, Vector2f p3);
	bool PointInside(Vector2f aPoint);

	Vector2f myV1;
	Vector2f myV2;
	Vector2f myV3;

	ObjectData* myObject;
};

