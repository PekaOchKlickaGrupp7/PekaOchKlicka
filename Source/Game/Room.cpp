#include "stdafx.h"
#include "Room.h"
#include "Item.h"

Room::Room()
{
	myNavMeshes.Init(12);
	myItems.Init(10);

	/*
	myItems.Add(new Item());
	myItems[0]->Init("Sprites/inventoryItem.png", "Sprites/inventoryItem.png",
		"Shovel", "A Shovel", DX2D::Vector2f(0.2f, 0.7f), false, "Test Level");
		*/
}

Room::~Room()
{
	myItems.DeleteAll();
	myObjects.DeleteAll();
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

void Room::SetGridSize(float aGridSize)
{
	myGridSize = static_cast<int>(aGridSize);
}

void Room::SetNavPoints(CommonUtilities::GrowingArray<bool, int>& aNodes)
{
	myNavPoints = aNodes;
}

void Room::AddNavPolygon(NavPolygon poly)
{
	myNavMeshes.Add(poly);
}

CommonUtilities::GrowingArray<NavPolygon>& Room::GetNavMeshes()
{
	return myNavMeshes;
}