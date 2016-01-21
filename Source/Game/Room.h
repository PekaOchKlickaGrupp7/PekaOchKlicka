#pragma once
#include "..\CommonUtilities\GrowingArray.h"
#include <map>
#include <string>
#include "ObjectData.h"
#include "NavPolygon.h"

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
	CommonUtilities::GrowingArray<NavPolygon> GetNavMeshes();

	CommonUtilities::GrowingArray<ObjectData*, unsigned int>* GetObjectList() { return &myObjects; }
	Item* GetItem(int aIndex) { return myItems[aIndex]; }
	void RemoveItem(int aIndex) { return myItems.RemoveCyclicAtIndex(aIndex); }
	int GetItemListSize(){ return myItems.Size(); }

private:
	CommonUtilities::GrowingArray<ObjectData*, unsigned int> myObjects;
	CommonUtilities::GrowingArray<Item*, unsigned int> myItems;
	CommonUtilities::GrowingArray<NavPolygon, unsigned short> myNavMeshes;
};

