#include "stdafx.h"
#include "EventIfNotGlobalVariable.h"
#include "EventVariablesManager.h"
#include <sstream>

EventIfNotGlobalVariable::EventIfNotGlobalVariable()
{
	myVariableType = IfVariableType::eNone;
	myVariable = "";
	myVariableValue = "";
}

EventIfNotGlobalVariable::~EventIfNotGlobalVariable()
{
}

void EventIfNotGlobalVariable::Init(Room* aRoom, CGameWorld* aGameWorld)
{
	Event::Init(aRoom, aGameWorld);

	switch (myVariableType)
	{
	case eNone:
		break;
	case Bool:
		myBoolValue = myVariableValue == "True";
		break;
	case String:
		break;
	case Int:
		std::stringstream(myVariableValue) >> myIntValue;
		break;
	case Float:
		std::stringstream(myVariableValue) >> myFloatValue;
		break;
	}
}

bool EventIfNotGlobalVariable::Update(const float)
{
	switch (myVariableType)
	{
	case Bool:
	{
		bool value = false;
		if (EventVariablesManager::GetInstance()->GetVariable(value, myVariable) == true)
		{
			myAutoActivateRecursive = value != myBoolValue;
		}
		else
		{
			myAutoActivateRecursive = true;
		}
		break;
	}
	case String:
	{
		std::string str = "";
		if (EventVariablesManager::GetInstance()->GetVariable(str, myVariable) == true)
		{
			myAutoActivateRecursive = str != myVariableValue;
		}
		else
		{
			myAutoActivateRecursive = true;
		}
		break;
	}
	case Int:
	{
		int value = 0;
		if (EventVariablesManager::GetInstance()->GetVariable(value, myVariable) == true)
		{
			myAutoActivateRecursive = value != myIntValue;
		}
		else
		{
			myAutoActivateRecursive = true;
		}
		break;
	}
	case Float:
	{
		float value = 0;
		if (EventVariablesManager::GetInstance()->GetVariable(value, myVariable) == true)
		{
			myAutoActivateRecursive = value != myFloatValue;
		}
		else
		{
			myAutoActivateRecursive = true;
		}
		break;
	}
	case eNone:
	default:
		myAutoActivateRecursive = true;
		break;
	}
	return true;
}

void EventIfNotGlobalVariable::Reset()
{
}
