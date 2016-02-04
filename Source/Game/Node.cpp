#include "stdafx.h"
#include "Node.h"

Node::Node() { }
Node::Node(bool aIsBlocked)
{
	myIsBlocked = aIsBlocked;
}

Node::~Node()
{
}

void Node::SetParent(Node* aNode)
{
	myParent = aNode;
}

void Node::SetG(const float aValue)
{
	myG = aValue;
}

void Node::SetH(const float aValue)
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

float Node::GetG() const
{
	return myG;
}

float Node::GetH() const
{
	return myH;
}
