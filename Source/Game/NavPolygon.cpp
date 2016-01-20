#include "stdafx.h"
#include "NavPolygon.h"

NavPolygon::NavPolygon()
{
	myPoints.Init(12);
}

NavPolygon::~NavPolygon()
{
}

void NavPolygon::AddPoint(Point2f aPoint)
{
	myPoints.Add(aPoint);
}

bool NavPolygon::PointInsideCheck(const Point2f& aPointToCheck)
{
	int j = myPoints.Size() - 1;
	bool oddNumberOfNodes = false;

	for (int i = 0; i < myPoints.Size(); i++)
	{
		if (myPoints[i].y < aPointToCheck.y
			&& myPoints[j].y >= aPointToCheck.y
			|| myPoints[j].y < aPointToCheck.y
			&& myPoints[i].y >= aPointToCheck.y)
		{
			if (myPoints[i].x + (aPointToCheck.y - myPoints[i].y)
				/ (myPoints[j].y - myPoints[i].y)
				* (myPoints[j].x - myPoints[i].x) < aPointToCheck.x)
			{
				oddNumberOfNodes = !oddNumberOfNodes;
			}
		}
		j = i;
	}

	return oddNumberOfNodes;
}