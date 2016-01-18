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

//void Event::SetType(const EventActions aType)
//{
//	myType = aType;
//}
//
//const EventActions Event::GetType() const
//{
//	return myType;
//}

void Event::SetName(const std::string& aName)
{
	myName = aName;
}

const std::string& Event::GetName() const
{
	return myName;
}
