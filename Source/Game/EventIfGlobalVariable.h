#pragma once
#include "Event.h"

enum IfVariableType
{
	eNone,
	Bool,
	String,
	Int,
	Float
};

class EventIfGlobalVariable :
	public Event
{
public:
	EventIfGlobalVariable();
	virtual ~EventIfGlobalVariable();

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

