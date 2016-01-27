#include "stdafx.h"
#include "EventSetGlobalVariable.h"
#include <sstream>
#include "EventVariablesManager.h"

EventSetGlobalVariable::EventSetGlobalVariable()
{
	myVariableType = IfVariableType::eNone;
	myVariable = "";
	myVariableValue = "";
}

EventSetGlobalVariable::~EventSetGlobalVariable()
{
}

void EventSetGlobalVariable::Init(Room* aRoom, CGameWorld* aGameWorld)
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

bool EventSetGlobalVariable::Update(const float)
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

void EventSetGlobalVariable::Reset()
{
}
