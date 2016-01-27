#pragma once
#include "Event.h"
#include "EventIfGlobalVariable.h"

class EventSetGlobalVariable :
	public Event
{
public:
	EventSetGlobalVariable();
	virtual ~EventSetGlobalVariable();

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

