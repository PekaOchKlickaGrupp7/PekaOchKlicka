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
	(aDeltaTime);
	
}

void Event::SetType(const EventTypes aType)
{
	myType = aType;
}

const EventTypes Event::GetType() const
{
	return myType;
}

void Event::SetName(const std::string& aName)
{
	myName = aName;
}

const std::string& Event::GetName() const
{
	return myName;
}
