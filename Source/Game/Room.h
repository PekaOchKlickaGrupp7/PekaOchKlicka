#pragma once
#include "..\CommonUtilities\GrowingArray.h"
#include <map>
#include <string>
#include "..\CommonUtilities\VectorOnStack.h"
#include "ObjectData.h"
#include "NavPolygon.h"
#include "Node.h"

class Item;

class Room
{
public:
	Room();
	~Room();
	void Init();
	void Init(const CommonUtilities::GrowingArray<Point2f>& aNavPoints);

	void OnLoad();

	void LoadLevel();
	void AddNavPolygon(NavPolygon poly);

	std::string& GetName();
	void SetName(std::string& aName);

	CommonUtilities::GrowingArray<NavPolygon>& GetNavMeshes();
	CommonUtilities::GrowingArray<ObjectData*, unsigned int>* GetObjectList() { return &myObjects; }

	Item* GetItem(int aIndex) { return myItems[aIndex]; }
	void RemoveItem(int aIndex) { return myItems.RemoveCyclicAtIndex(aIndex); }
	int GetItemListSize(){ return myItems.Size(); }

	void SetGridSize(float aGridSize);
	void SetNavPoints(CommonUtilities::GrowingArray<Node, int>& aNodes);
	CommonUtilities::GrowingArray<Node, int>& GetNavPoints() { return myNavPoints; }
	float GetGridSize() { return myGridSize; }
	Node* GetNodeAtPosition(DX2D::Vector2f aPosition);
	CommonUtilities::VectorOnStack<Node*, 8> GetNeighbours(Node* aNode);
private:
	float myGridSize;
	std::string myName;

	CommonUtilities::GrowingArray<ObjectData*, unsigned int> myObjects;
	CommonUtilities::GrowingArray<Node, int> myNavPoints;
	CommonUtilities::GrowingArray<Item*, unsigned int> myItems;
	CommonUtilities::GrowingArray<NavPolygon, unsigned short> myNavMeshes;
};

