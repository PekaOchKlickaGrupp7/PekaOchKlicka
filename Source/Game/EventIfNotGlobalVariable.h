#pragma once
#include "EventIfGlobalVariable.h"

class EventIfNotGlobalVariable :
	public Event
{
public:
	EventIfNotGlobalVariable();
	virtual ~EventIfNotGlobalVariable();
	
	virtual void Init(Room* aRoom, CGameWorld* aGameWorld) override;
	virtual bool Update(const float aDeltaTime) override;
	virtual void Reset() override;

	std::string myVariable;
	std::string myVariableValue;
	IfVariableType myVariableType;
protected:
	bool myBoolValue;
	int myIntValue;
	float myFloatValue;
};

