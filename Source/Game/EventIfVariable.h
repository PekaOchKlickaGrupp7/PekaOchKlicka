#pragma once
#include "Event.h"

enum IfVariableType
{
	None,
	Bool,
	String,
	Int,
	Float
};

class EventIfVariable :
	public Event
{
public:
	EventIfVariable();
	virtual ~EventIfVariable();

	virtual bool Update(const float aDeltaTime) override;
	virtual void Reset() override;

	std::string myVariable;
	IfVariableType myVariableType;
};

