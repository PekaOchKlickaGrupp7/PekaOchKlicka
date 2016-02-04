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

CommonUtilities::VectorOnStack<Node*, 8> Room::GetNeighbours(Node* aNode)
{
	CommonUtilities::VectorOnStack<Node*, 8> neighbours;
	for (int y = -1; y <= 1; y++)
	{
		for (int x = -1; x <= 1; x++)
		{
			if (x == 0 && y == 0)
			{
				continue;
			}

			int checkX = aNode->GetX() + x;
			int checkY = aNode->GetY() + y;

			if (checkX >= 0 && checkX < myGridSize && checkY >= 0 && myGridSize)
			{
				neighbours.Add(&myNavPoints[checkX + checkY * static_cast<int>(1920.0f / myGridSize)]);
			}
		}
	}

	return neighbours;
}
