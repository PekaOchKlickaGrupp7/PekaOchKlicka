#include "stdafx.h"
#include "EventIfGlobalVariable.h"
#include <sstream>
#include "EventVariablesManager.h"

EventIfGlobalVariable::EventIfGlobalVariable()
{
	myVariableType = IfVariableType::eNone;
	myVariable = "";
	myVariableValue = "";
}

EventIfGlobalVariable::~EventIfGlobalVariable()
{
}

void EventIfGlobalVariable::Init(Room* aRoom, CGameWorld* aGameWorld)
{
	myRoom = aRoom;
	myGameWorld = aGameWorld;

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

bool EventIfGlobalVariable::Update(const float)
{
	switch (myVariableType)
	{
	case Bool:
	{
		bool value = false;
		if (EventVariablesManager::GetInstance()->GetVariable(value, myVariable) == true)
		{
			myAutoActivateRecursive = value == myBoolValue;
		}
		else
		{
			myAutoActivateRecursive = false;
		}
		break;
	}
	case String:
	{
		std::string str = "";
		if (EventVariablesManager::GetInstance()->GetVariable(str, myVariable) == true)
		{
			myAutoActivateRecursive = str == myVariableValue;
		}
		else
		{
			myAutoActivateRecursive = false;
		}
		break;
	}
	case Int:
	{
		int value = 0;
		if (EventVariablesManager::GetInstance()->GetVariable(value, myVariable) == true)
		{
			myAutoActivateRecursive = value == myIntValue;
		}
		else
		{
			myAutoActivateRecursive = false;
		}
		break;
	}
	case Float:
	{
		float value = 0;
		if (EventVariablesManager::GetInstance()->GetVariable(value, myVariable) == true)
		{
			myAutoActivateRecursive = value == myFloatValue;
		}
		else
		{
			myAutoActivateRecursive = false;
		}
		break;
	}
	case eNone:
	default:
		myAutoActivateRecursive = false;
		break;
	}
	return true;
}

void EventIfGlobalVariable::Reset()
{
}
