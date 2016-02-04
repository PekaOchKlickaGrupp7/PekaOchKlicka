#include "stdafx.h"
#include "Pathfinding.h"
#include "Room.h"

Pathfinding::Pathfinding()
{
	myPath.Init(512);
}

Pathfinding::~Pathfinding()
{
}

bool Pathfinding::FindPath(Room* aRoom, DX2D::Vector2f aFrom, DX2D::Vector2f aTo)
{
	//*TODO: Pathfinding algorithm here*
	return false;
}

void Pathfinding::CalculatePath(Node* aNodeStart, Node* aNodeEnd)
{

}

CommonUtilities::GrowingArray<Node*, int> Pathfinding::GetPath()
{
	//Returns the resulting path
	return myPath;
}
