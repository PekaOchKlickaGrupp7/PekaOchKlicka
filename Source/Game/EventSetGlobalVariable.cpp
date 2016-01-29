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
		EventVariablesManager::GetInstance()->SetVariable(myBoolValue, myVariable);
		break;
	}
	case String:
	{
		EventVariablesManager::GetInstance()->SetVariable(myVariableValue, myVariable);
		break;
	}
	case Int:
	{
		EventVariablesManager::GetInstance()->SetVariable(myIntValue, myVariable);
		break;
	}
	case Float:
	{
		EventVariablesManager::GetInstance()->SetVariable(myFloatValue, myVariable);
		break;
	}
	case eNone:
	default:
		break;
	}
	return true;
}

void EventSetGlobalVariable::Reset()
{
}
