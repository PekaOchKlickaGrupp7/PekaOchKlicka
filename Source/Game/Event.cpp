#include "stdafx.h"
#include "Event.h"

Event::Event()
{
}

Event::~Event()
{
}

void Event::Update(const float aDeltaTime)
{

}

void Event::SetName(const std::string& aName)
{
	myName = aName;
}

const std::string& Event::GetName()
{
	return myName;
}
