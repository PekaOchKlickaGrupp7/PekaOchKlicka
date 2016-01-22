#pragma once

class Item;

class ItemList
{
public:
	ItemList();
	~ItemList();
	void Add(Item* aItem);
	inline CommonUtilities::GrowingArray<Item*>& GetItemList();

private:
	CommonUtilities::GrowingArray<Item*> myItemList;
};



inline CommonUtilities::GrowingArray<Item*>& ItemList::GetItemList()
{
	return myItemList;
}

