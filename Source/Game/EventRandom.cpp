#include "stdafx.h"
#include "EventRandom.h"

EventRandom::EventRandom()
{
	myIndex = 0;
	myRandomFactor = 100;
}

EventRandom::~EventRandom()
{
}

bool EventRandom::Update(const float)
{
	myIndex = (rand() % myRandomFactor) % static_cast<int>(myChilds.Size());
}

void EventRandom::Reset()
{

}
