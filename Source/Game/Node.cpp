#include "stdafx.h"
#include "Node.h"

Node::Node()
{
	myIsBlocked = false;
	myG = 0;
	myH = 0;
}
Node::Node(int aX, int aY, bool aIsBlocked)
{
	myIsBlocked = aIsBlocked;
	myG = 0;
	myH = 0;
	myX = aX;
	myY = aY;
}

Node::~Node()
{
}

void Node::SetParent(Node* aNode)
{
	myParent = aNode;
}

void Node::SetG(const int aValue)
{
	myG = aValue;
}

void Node::SetH(const int aValue)
{
	myH = aValue;
}

Node* Node::GetParent()
{
	return myParent;
}

void Node::SetX(const int aValue)
{
	myX = aValue;
}

void Node::SetY(const int aValue)
{
	myY = aValue;
}

int Node::GetX() const
{
	return myX;
}

int Node::GetY() const
{
	return myY;
}

int Node::GetG() const
{
	return myG;
}

int Node::GetH() const
{
	return myH;
}

void Node::SetPath(bool aIsPath)
{
	myIsPath = aIsPath;
}

