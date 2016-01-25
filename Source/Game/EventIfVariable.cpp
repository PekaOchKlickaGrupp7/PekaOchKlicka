#include "stdafx.h"
#include "EventIfVariable.h"

EventIfVariable::EventIfVariable()
{
	myVariableType = IfVariableType::None;
}

EventIfVariable::~EventIfVariable()
{
}

bool EventIfVariable::Update(const float aDeltaTime)
{
	return true;
}

void EventIfVariable::Reset()
{

}
