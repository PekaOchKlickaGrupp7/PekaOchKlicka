#include "stdafx.h"
#include "ItemList.h"
#include "Item.h"

ItemList::ItemList()
{
	myItemList.Init(4);
}


ItemList::~ItemList()
{
	for (unsigned short i = 0; i < myItemList.Size(); ++i)
	{
		myItemList[i].Destroy();
	}
	myItemList.RemoveAll();
}

void ItemList::Add(Item* aItem)
{	
	myItemList.Add(*aItem);
}