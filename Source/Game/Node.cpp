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
