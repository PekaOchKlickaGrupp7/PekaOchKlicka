#include "stdafx.h"
#include "Room.h"
#include "Item.h"
#include <iostream>

Room::Room()
{
	myNavMeshes.Init(12);

	/*
	myItems.Add(new Item());
	myItems[0]->Init("Sprites/inventoryItem.png", "Sprites/inventoryItem.png",
		"Shovel", "A Shovel", DX2D::Vector2f(0.2f, 0.7f), false, "Test Level");
		*/
}

Room::~Room()
{
	myObjects.DeleteAll();
}

void Room::OnLoad()
{

}

std::string& Room::GetName()
{
	return myName;
}

void Room::SetName(std::string& aName)
{
	myName = aName;
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

	int gridX = static_cast<int>(roundf(1920.0f / myGridSize));
	int gridY = static_cast<int>(roundf(1080.0f / myGridSize));
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

			if (checkX >= 0 && checkX < gridX && checkY >= 0 && checkY < gridY)
			{
				neighbours.Add(&myNavPoints[checkX + checkY * gridX]);
			}
		}
	}

	return neighbours;
}
