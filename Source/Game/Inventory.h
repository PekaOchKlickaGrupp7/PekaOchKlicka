#pragma once
#include "../CommonUtilities/GrowingArray.h"
#include "../CommonUtilities/Macros.h"
#include "Item.h"
#include <tga2d/sprite/sprite.h>
#include "ItemList.h"

class Synchronizer;

class Inventory
{
public:
	Inventory();
	~Inventory();

	void Init(const char* aFilePath, DX2D::Vector2f aPosition);
	void Add(Item* aItemToAdd);
	void Remove(Item* aItemToRemove);
	void Combine(Item* aItemToCombine, Item* aItemToCombineWith);
	void Render(Synchronizer& aSynchronizer);
	void Open();
	void Close();
	inline ItemList* GetMasterItemList();

private:
	CommonUtilities::GrowingArray<Item*, unsigned int> myContents;
	DX2D::CSprite* myBackground;
	DX2D::Vector2f myPosition;

	bool myIsOpen;
	ItemList *myMasterItemList;
};


inline ItemList* Inventory::GetMasterItemList()
{
	return myMasterItemList;
}