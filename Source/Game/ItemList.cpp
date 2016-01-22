#include "stdafx.h"
#include "ItemList.h"
#include "Item.h"

ItemList::ItemList()
{
	myItemList.Init(4);
}


ItemList::~ItemList()
{
}

void ItemList::Add(Item* aItem)
{	
	myItemList.Add(aItem);
}