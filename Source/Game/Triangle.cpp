#include "stdafx.h"
#include "Triangle.h"
#include "ObjectData.h"

Triangle::Triangle()
{
}

Triangle::~Triangle()
{
}

float Triangle::Sign(Vector2f p1, Vector2f p2, Vector2f p3)
{
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool Triangle::PointInside(Vector2f aPoint)
{
	bool b1, b2, b3;

	Vector2f V1 = myV1 + Vector2f(myObject->myGlobalX, myObject->myGlobalY);
	Vector2f V2 = myV2 + Vector2f(myObject->myGlobalX, myObject->myGlobalY);
	Vector2f V3 = myV3 + Vector2f(myObject->myGlobalX, myObject->myGlobalY);

	b1 = Sign(aPoint, V1, V2) < 0.0f;
	b2 = Sign(aPoint, V2, V3) < 0.0f;
	b3 = Sign(aPoint, V3, V1) < 0.0f;

	return ((b1 == b2) && (b2 == b3));
}
