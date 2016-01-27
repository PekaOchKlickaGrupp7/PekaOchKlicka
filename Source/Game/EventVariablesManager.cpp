#include "stdafx.h"
#include "EventVariablesManager.h"
#include <iostream>

EventVariablesManager* EventVariablesManager::myEventVariablesManager = nullptr;


EventVariablesManager::EventVariablesManager()
{
}

void EventVariablesManager::Destroy()
{
	delete myEventVariablesManager;
}

EventVariablesManager::~EventVariablesManager()
{
	myBoolVariables.clear();
	myIntVariables.clear();
	myFloatVariables.clear();
	myStringVariables.clear();
}

#pragma region SetVariables

void EventVariablesManager::SetVariable(bool aBool, std::string &anIdentifier)
{
	myBoolVariables[anIdentifier] = aBool;
}

void EventVariablesManager::SetVariable(int aInt, std::string &anIdentifier)
{
	myIntVariables[anIdentifier] = aInt;
}

void EventVariablesManager::SetVariable(float aFloat, std::string &anIdentifier)
{
	myFloatVariables[anIdentifier] = aFloat;
}

void EventVariablesManager::SetVariable(std::string &aString, std::string &anIdentifier)
{
	myStringVariables[anIdentifier] = aString;
}

#pragma endregion

#pragma region GetVariables

bool EventVariablesManager::GetVariable(bool &aBool, std::string &anIdentifier)
{
	if (myBoolVariables.find(anIdentifier) == myBoolVariables.end())
	{
		return false;
	}
	else
	{
		aBool = myBoolVariables[anIdentifier];
	}
	return true;
}

bool EventVariablesManager::GetVariable(int &aInt, std::string &anIdentifier)
{
	if (myIntVariables.find(anIdentifier) == myIntVariables.end())
	{
		return false;
	}
	else
	{
		aInt = myIntVariables[anIdentifier];
	}
	return true;
}

bool EventVariablesManager::GetVariable(float &aFloat, std::string &anIdentifier)
{
	if (myFloatVariables.find(anIdentifier) == myFloatVariables.end())
	{
		return false;
	}
	else
	{
		aFloat = myFloatVariables[anIdentifier];
	}
	return true;
}

bool EventVariablesManager::GetVariable(std::string &aString, std::string &anIdentifier)
{
	if (myStringVariables.find(anIdentifier) == myStringVariables.end())
	{
		return false;
	}
	else
	{
		aString = myStringVariables[anIdentifier];
	}
	return true;
}

#pragma endregion
