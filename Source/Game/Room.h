#pragma once
#include "..\CommonUtilities\GrowingArray.h"
#include <map>
#include <string>
#include "ObjectData.h"

class Room
{
public:
	Room();
	~Room();

	void OnLoad();
	void OnClick();
	void OnHover();

	void LoadLevel();

	CommonUtilities::GrowingArray<ObjectData*, unsigned int>& GetObjectList() { return myObjects; }

private:
	CommonUtilities::GrowingArray<ObjectData*, unsigned int> myObjects;
};

