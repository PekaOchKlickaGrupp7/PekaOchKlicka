#pragma once
#include "fmod.hpp"
#include "fmod_errors.h"
#include <string>
class ChannelObject
{
public:
	ChannelObject();
	ChannelObject(std::string &aName, FMOD::Channel* aChannel);
	~ChannelObject();
	std::string &GetName() { return myName; };
	FMOD::Channel* &GetFMODChannel() { return myChannel; };
private:
	std::string myName;
	FMOD::Channel* myChannel;
};

