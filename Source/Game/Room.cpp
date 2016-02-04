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
	myGridSize = aGridSize;
}

void Room::SetNavPoints(CommonUtilities::GrowingArray<Node, int>& aNodes)
{
	myNavPoints = aNodes;
}

void Room::AddNavPolygon(NavPolygon aPoly)
{
	myNavMeshes.Add(aPoly);
}

CommonUtilities::GrowingArray<NavPolygon>& Room::GetNavMeshes()
{
	return myNavMeshes;
}

Node* Room::GetNodeAtPosition(DX2D::Vector2f aPosition)
{
	int x = static_cast<int>(roundf((aPosition.x * 1920.0f) / myGridSize));
	int y = static_cast<int>(roundf((aPosition.y * 1080.0f) / myGridSize));
	return &myNavPoints[x + y * static_cast<int>(roundf(1920.0f / myGridSize))];
}
