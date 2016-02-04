#pragma once
#include "..\CommonUtilities\GrowingArray.h"
#include <map>
#include <string>
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
	CommonUtilities::GrowingArray<NavPolygon>& GetNavMeshes();

	inline bool GetGridAt(float aX, float aY) const
	{
		return myNavPoints[
			static_cast<int>((aX * 1920.0f) / myGridSize) + 
			static_cast<int>((aY * 1080.0f) / myGridSize) * 
			static_cast<int>(1920.0f / myGridSize)].GetIsBlocked();
	}

	CommonUtilities::GrowingArray<ObjectData*, unsigned int>* GetObjectList() { return &myObjects; }
	Item* GetItem(int aIndex) { return myItems[aIndex]; }
	void RemoveItem(int aIndex) { return myItems.RemoveCyclicAtIndex(aIndex); }
	int GetItemListSize(){ return myItems.Size(); }

	void SetGridSize(float aGridSize);
	void SetNavPoints(CommonUtilities::GrowingArray<Node, int>& aNodes);

	CommonUtilities::GrowingArray<Node, int>& GetNavPoints() { return myNavPoints; }

	float GetGridSize() { return myGridSize; }

	Node* GetNodeAtPosition(DX2D::Vector2f aPosition);

private:
	float myGridSize;

	CommonUtilities::GrowingArray<ObjectData*, unsigned int> myObjects;
	CommonUtilities::GrowingArray<Node, int> myNavPoints;
	CommonUtilities::GrowingArray<Item*, unsigned int> myItems;
	CommonUtilities::GrowingArray<NavPolygon, unsigned short> myNavMeshes;
};

