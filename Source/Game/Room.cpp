#include "stdafx.h"
#include "Room.h"

Room::Room()
{
	myNavMeshes.Init(12);
}

Room::~Room()
{
}

void Room::OnLoad()
{

}

void Room::Init(const CommonUtilities::GrowingArray<Point2f>& aNavPoints)
{
	NavPolygon poly;
	for (unsigned short i = 0; i < aNavPoints.Size(); i++)
	{
		poly.AddPoint(aNavPoints[i]);
	}

	myNavMeshes.Add(poly);
}

void Room::Init()
{

}

void Room::AddNavPolygon(NavPolygon poly)
{
	myNavMeshes.Add(poly);
}

CommonUtilities::GrowingArray<NavPolygon> Room::GetNavMeshes()
{
	return myNavMeshes;
}