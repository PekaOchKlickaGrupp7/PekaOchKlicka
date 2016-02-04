#include "stdafx.h"
#include "Pathfinding.h"
#include "Room.h"

Pathfinding::Pathfinding()
{
	myPath.Init(512);
	myOpenList.Init(512);
	myVisitedList.Init(512);
}

Pathfinding::~Pathfinding()
{
}

bool Pathfinding::FindPath(Room* aRoom, DX2D::Vector2f aFrom, DX2D::Vector2f aTo)
{
	//*TODO: Pathfinding algorithm here*

	CommonUtilities::GrowingArray<Node, int>& nodes = aRoom->GetNavPoints();
	for (int i = 0; i < nodes.Size(); ++i)
	{
		nodes[i].SetPath(false);
	}

	myOpenList.RemoveAll();
	myVisitedList.RemoveAll();

	Node* startNode = aRoom->GetNodeAtPosition(aFrom);
	Node* endNode = aRoom->GetNodeAtPosition(aTo);
	Node* currentNode = nullptr;

	myOpenList.Add(startNode);

	while (myOpenList.Size() > 0)
	{
		currentNode = myOpenList[0];
		for (int i = 1; i < myOpenList.Size(); ++i)
		{
			if (myOpenList[i]->GetF() < currentNode->GetF() ||
				(myOpenList[i]->GetF() == currentNode->GetF() &&
				myOpenList[i]->GetH() < currentNode->GetH()))
			{
				currentNode = myOpenList[i];
			}
		}

		myOpenList.RemoveCyclic(currentNode);
		myVisitedList.Add(currentNode);

		if (currentNode == endNode)
		{
			RetracePath(startNode, endNode);
			return true;
		}

		CommonUtilities::VectorOnStack<Node*, 8> neighbours = aRoom->GetNeighbours(currentNode);
		for (unsigned short i = 0; i < neighbours.Size(); ++i)
		{
			if (neighbours[i]->GetIsBlocked() == true || myVisitedList.Find(neighbours[i]) != myVisitedList.FoundNone)
			{
				continue;
			}

			int newMovementCostToNeighbour = currentNode->GetG() + CalculateDistance(currentNode, neighbours[i]);

			if (newMovementCostToNeighbour < neighbours[i]->GetG() || myOpenList.Find(neighbours[i]) == myOpenList.FoundNone)
			{
				neighbours[i]->SetG(newMovementCostToNeighbour);
				neighbours[i]->SetH(CalculateDistance(neighbours[i], endNode));
				neighbours[i]->SetParent(currentNode);

				if (myOpenList.Find(neighbours[i]) == myOpenList.FoundNone)
				{
					myOpenList.Add(neighbours[i]);
				}
			}
		}
	}

	return false;
}

int Pathfinding::CalculateDistance(Node* aNodeA, Node* aNodeB)
{
	int dstX = abs(aNodeA->GetX() - aNodeB->GetX());
	int dstY = abs(aNodeA->GetY() - aNodeB->GetY());

	if (dstX > dstY)
	{
		return 14 * dstY + 10 * (dstX - dstY);
	}
	return 14 * dstX + 10 * (dstY - dstX);
}

void Pathfinding::RetracePath(Node* aNodeStart, Node* aNodeEnd)
{
	Node* currentNode = aNodeEnd;

	while (currentNode != aNodeStart)
	{
		myPath.Add(currentNode);
		currentNode->SetPath(true);
		currentNode = currentNode->GetParent();
	}

	for (int i = 0; i < myPath.Size() / 2; ++i)
	{
		Node* node = myPath[i];
		myPath[i] = myPath[(myPath.Size() - i) - 1];
		myPath[(myPath.Size() - i) - 1] = node;
	}
}

CommonUtilities::GrowingArray<Node*, int>& Pathfinding::GetPath()
{
	//Returns the resulting path
	return myPath;
}
