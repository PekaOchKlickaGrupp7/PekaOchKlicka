#include "stdafx.h"
#include "EventIsItem.h"
#include "EventVariablesManager.h"

EventIsItem::EventIsItem()
{
}

EventIsItem::~EventIsItem()
{
}

bool EventIsItem::Update(const float)
{
	std::string str = "";
	std::string testWith = "_SELECTED_ITEM";
	if (EventVariablesManager::GetInstance()->GetVariable(str, testWith) == true)
	{
		myAutoActivateRecursive = str == myItemName;
	}
	else
	{
		myAutoActivateRecursive = false;
	}
	return true;
}

void EventIsItem::Reset()
{
}
