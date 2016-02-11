#include "stdafx.h"
#include "EventItemIsNot.h"
#include "EventVariablesManager.h"

EventItemIsNot::EventItemIsNot()
{
}

EventItemIsNot::~EventItemIsNot()
{
}

bool EventItemIsNot::Update(const float)
{
	std::string str = "";
	std::string testWith = "_SELECTED_ITEM";
	if (EventVariablesManager::GetInstance()->GetVariable(str, testWith) == true)
	{
		if (myItemName == "None")
		{
			myAutoActivateRecursive = str != "";
			return true;
		}
		else
		{
			myAutoActivateRecursive = str != myItemName;
		}
	}
	else
	{
		myAutoActivateRecursive = false;
	}
	return true;
}

void EventItemIsNot::Reset()
{
	
}
