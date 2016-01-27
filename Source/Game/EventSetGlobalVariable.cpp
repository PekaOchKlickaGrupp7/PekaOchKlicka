#include "stdafx.h"
#include "EventSetGlobalVariable.h"
#include <sstream>

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
		myAutoActivateRecursive = myBoolValue;
		break;
	case String:
		myAutoActivateRecursive = myVariableValue == "Test";
		break;
	case Int:
		myAutoActivateRecursive = myIntValue == 1;
		break;
	case Float:
		myAutoActivateRecursive = myFloatValue == 1.0f;
		break;
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
