#pragma once
#include <string>
#include <map>

class EventVariablesManager
{
public:

	static EventVariablesManager* const GetInstance()
	{
		if (myEventVariablesManager == nullptr)
		{
			myEventVariablesManager = new EventVariablesManager();
		}
		return myEventVariablesManager;
	}

	static void DestroyInstance()
	{
		delete myEventVariablesManager;
	}



	void		SetVariable(bool aBool,					const std::string &anIdentifier);
	void		SetVariable(int aInt,					const std::string &anIdentifier);
	void		SetVariable(float aFloat,				const std::string &anIdentifier);
	void		SetVariable(std::string &aString,		const std::string &anIdentifier);

	bool		GetVariable(bool &aBool,				const std::string &anIdentifier);
	bool		GetVariable(int &aInt,					const std::string &anIdentifier);
	bool		GetVariable(float &aFloat,				const std::string &anIdentifier);
	bool		GetVariable(std::string &aString,		const std::string &anIdentifier);


	~EventVariablesManager();
private:
	static EventVariablesManager* myEventVariablesManager;

	EventVariablesManager();

	std::map<std::string, bool>			myBoolVariables;
	std::map<std::string, int>			myIntVariables;
	std::map<std::string, float>		myFloatVariables;
	std::map<std::string, std::string>	myStringVariables;
};

