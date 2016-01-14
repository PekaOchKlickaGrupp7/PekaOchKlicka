#pragma once
#include <string>

class JSON
{
public:
	JSON();
	~JSON();

	bool Load(const std::string& aRootFile);
private:
	const char* ReadFile(const std::string& aFile);
};

