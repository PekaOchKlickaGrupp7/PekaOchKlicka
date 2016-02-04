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
	CommonUtilities::GrowingArray<Node*, int>& GetPath();

private:
	void RetracePath(Node* aNodeStart, Node* aNodeEnd);
	int CalculateDistance(Node* aNode1, Node* aNode2);

	CommonUtilities::GrowingArray<Node*, int> myPath;

	CommonUtilities::GrowingArray<Node*, int> myOpenList;
	CommonUtilities::GrowingArray<Node*, int> myVisitedList;
};

