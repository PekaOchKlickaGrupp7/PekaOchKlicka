#include "stdafx.h"
#include "Pathfinding.h"

Pathfinding::Pathfinding()
{
}

Pathfinding::~Pathfinding()
{
}

bool Pathfinding::FindPath(DX2D::Vector2f aFrom, DX2D::Vector2f aTo)
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
