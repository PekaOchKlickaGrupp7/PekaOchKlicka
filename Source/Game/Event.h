#pragma once
#include <string>

enum EventTypes
{
	None,
	SetActive
};

class Event
{
public:
	Event();
	~Event();

	void Update(const float aDeltaTime);

	void Event::SetType(const EventTypes aType);
	const EventTypes GetType() const;

	void SetName(const std::string& aName);
	const std::string& GetName() const;
private:
	std::string myName;
	EventTypes myType;
};

