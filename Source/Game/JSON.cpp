#include "stdafx.h"
#include "JSON.h"
#include "rapidjson\document.h"
#include "..\CommonUtilities\DL_Debug.h"

using namespace rapidjson;

JSON::JSON() { }
JSON::~JSON() { }

bool JSON::Load(const std::string& aRootFile)
{
	const char* data = ReadFile(aRootFile);

	Document root;
	root.Parse(data);
	if (root.HasParseError() == true)
	{
		DL_DEBUG("Failed to load root.json.");
		return false;
	}

	return true;
}

const char* JSON::ReadFile(const std::string& aFile)
{
	std::ifstream input(aFile);

	std::string str((std::istreambuf_iterator<char>(input)),
		std::istreambuf_iterator<char>());

	char* data = new char[str.length()];
	int a = 0;
	for (unsigned int i = 0; i < str.length(); ++i)
	{
		if (str[i] != 0)
		{
			data[a] = str[i];
			a++;
		}
	}
	data[a] = '\0';
	input.close();
	
	return data;
}
