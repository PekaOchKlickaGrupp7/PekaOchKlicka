#pragma once
#include "..\CommonUtilities\GrowingArray.h"
#include <map>
#include <string>
#include "ObjectData.h"
#include "NavPolygon.h"

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

	CommonUtilities::GrowingArray<ObjectData*, unsigned int>& GetObjectList() { return myObjects; }

private:
	CommonUtilities::GrowingArray<ObjectData*, unsigned int> myObjects;
	CommonUtilities::GrowingArray<NavPolygon, unsigned int> myNavMeshes;
};

