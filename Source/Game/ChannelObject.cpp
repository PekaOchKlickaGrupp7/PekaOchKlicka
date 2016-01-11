#include "stdafx.h"
#include "ChannelObject.h"


ChannelObject::ChannelObject()
{
}

ChannelObject::ChannelObject(std::string &aName, FMOD::Channel* aChannel)
{
	myName = aName;
	myChannel = aChannel;
}


ChannelObject::~ChannelObject() // may be risky, if crash - change so that it does not delete
{
	delete myChannel;
	myChannel = nullptr;
}
