#pragma once
#include <string>

enum EventActions
{
	None,
	SetActive
};

enum TriggerType
{
	Box,
	Circle
};

enum EventTypes
{
	OnClick,
	OnHover,
	OnLoad,
	Custom
};

class Event
{
public:
	Event();
	~Event();

	void Update(const float aDeltaTime);

	//void Event::SetType(const EventActions aType);
	//const EventActions GetType() const;

	void SetName(const std::string& aName);
	const std::string& GetName() const;

	const char* myTarget;
	EventTypes myType;
	EventActions myAction;
private:
	std::string myName;
};

