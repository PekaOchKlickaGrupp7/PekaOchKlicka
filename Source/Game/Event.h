#pragma once
#include <string>

class Event
{
public:
	Event();
	~Event();

	void Update(const float aDeltaTime);

	void SetName(const std::string& aName);
	const std::string& GetName();
private:
	std::string myName;
};

