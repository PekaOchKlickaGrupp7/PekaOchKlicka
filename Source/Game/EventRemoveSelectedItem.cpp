#include "stdafx.h"
#include "EventRemoveSelectedItem.h"
#include "GameWorld.h"

EventRemoveSelectedItem::EventRemoveSelectedItem()
{
}

EventRemoveSelectedItem::~EventRemoveSelectedItem()
{
}

bool EventRemoveSelectedItem::Update(const float)
{
	myGameWorld->GetPlayer()->GetInventory().RemoveSelectedItem();
	return true;
}

void EventRemoveSelectedItem::Reset()
{

}
