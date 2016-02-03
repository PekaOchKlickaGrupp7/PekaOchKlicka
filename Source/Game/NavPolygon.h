#pragma once
#include "..\CommonUtilities\GrowingArray.h"
#include "..\CommonUtilities\Vector2.h"

class NavPolygon
{
public:
	NavPolygon();
	~NavPolygon();

	void AddPoint(Point2f aPoint);

	bool PointInsideCheck(const Point2f& aPointToCheck);

	CommonUtilities::GrowingArray<Point2f> GetPoints();

private:
	CommonUtilities::GrowingArray<Point2f> myPoints;
};

