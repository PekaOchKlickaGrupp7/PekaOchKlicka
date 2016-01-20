#include "stdafx.h"
#include "Room.h"

Room::Room()
{
}

Room::~Room()
{
}

void Room::OnLoad()
{

}

void Room::Init(const CommonUtilities::GrowingArray<Point2f>& aNavPoints)
{
	for (unsigned short i = 0; i < aNavPoints.Size(); i++)
	{
		myNavMesh.AddPoint(aNavPoints[i]);
	}
}
