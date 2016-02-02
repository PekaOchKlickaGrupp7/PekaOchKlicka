#include "stdafx.h"
#include "EventPickupItem.h"
#include "GameWorld.h"

EventPickupItem::EventPickupItem()
{
	myItemVar = nullptr;
}

EventPickupItem::~EventPickupItem()
{
	delete myItemVar;
}

void EventPickupItem::Init(Room* aRoom, CGameWorld* aGameWorld)
{
	Event::Init(aRoom, aGameWorld);

	ItemList* itemList = myGameWorld->GetPlayer()->GetInventory().GetMasterItemList();
	CommonUtilities::GrowingArray<Item>& items = itemList->GetItemList();
	for (unsigned short i = 0; i < items.Size(); ++i)
	{
		if (items[i].GetName() == myItem)
		{
			myItemVar = &items[i];
		}
	}
}

bool EventPickupItem::Update(const float)
{
	if (myItemVar != nullptr)
	{
		myGameWorld->GetPlayer()->AddItemToInventory(myItemVar);
	}
	return true;
}

void EventPickupItem::Reset()
{

}
