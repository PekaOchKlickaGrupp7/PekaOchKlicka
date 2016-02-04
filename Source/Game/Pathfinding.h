#pragma once
#include "tga2d\math\vector2.h"

class Node;
class Room;

class Pathfinding
{
public:
	Pathfinding();
	~Pathfinding();

	bool FindPath(Room* aRoom, DX2D::Vector2f aFrom, DX2D::Vector2f aTo);
	CommonUtilities::GrowingArray<Node*, int> GetPath();

private:
	void CalculatePath(Node* aNodeStart, Node* aNodeEnd);
	CommonUtilities::GrowingArray<Node*, int> myPath;
};

