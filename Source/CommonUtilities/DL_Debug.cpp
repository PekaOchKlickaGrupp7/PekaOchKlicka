#include "stdafx.h"

#include "ReleaseAssert.h"
#include "Macros.h"
#include "DL_Debug.h"
#include <stdarg.h>
#include <stdio.h>
#include <ctime>
#include "DL_StackWalker.h"
#include <Windows.h>
#include "..\Launcher\VersionNo.h"

namespace DL_Debug
{
	Debug* Debug::ourInstance = nullptr;

	Debug::Debug()
	{

	}

	Debug::~Debug()
	{
		Destroy();
	}

	bool Debug::Create(std::string aFile)
	{
		assert(ourInstance == nullptr && "Debugobject already created");

		ourInstance = new Debug;

		if (ourInstance == nullptr)
		{
			return false;
		}

		time_t t = time(0); // get current system time;
		struct tm timeinfo;
		localtime_s(&timeinfo, &t);

		int version[] = { PRODUCTVER };
		std::string versionNumber;

		int count = sizeof(version) / sizeof(version[0]);
		for (int i = 0; i < count; i++)
		{
			versionNumber += std::to_string(version[i]);
			versionNumber += ".";
		}
		versionNumber.pop_back();

		if (CreateDirectory(L"Logs", 0) || ERROR_ALREADY_EXISTS == GetLastError())
		{
			std::string tempString = ".\\Logs\\Log " + std::to_string(timeinfo.tm_year + 1900) + "." + std::to_string(timeinfo.tm_mon + 1) + "." + std::to_string(timeinfo.tm_mday) +
				"  " + std::to_string(timeinfo.tm_hour) + "h" + std::to_string(timeinfo.tm_min) + "m" + std::to_string(timeinfo.tm_sec) + "s" + " v" + versionNumber + ".txt";
			const char* fileNameToGive = "uninitialized.txt";
			fileNameToGive = tempString.c_str();
			ourInstance->myDebugFile.open(fileNameToGive, std::fstream::out); // std::ios::out
		}
		else
		{
			// failed to create the folder
		}

		

		//ourInstance->myDebugFile.open(aFile, std::fstream::app);



		assert(ourInstance->myDebugFile.fail() != true && "Failed to open the debug file.");

		return true;
	}
	bool Debug::Destroy()
	{
		ourInstance->myDebugFile.close();
		SAFE_DELETE(ourInstance);
		return true;
	}

	Debug* Debug::GetInstance()
	{
		return ourInstance;
	}

	void Debug::AssertMessage(const char* aFileName, int aLine, const char* aFunctionName, const char* aString)
	{
		time_t t = time(0); // get current system time;
		struct tm timeinfo;
		localtime_s(&timeinfo, &t);
		myDebugFile << timeinfo.tm_hour << ":" << timeinfo.tm_min << ":" << timeinfo.tm_sec << " error" << std::endl;
		myDebugFile << "	Error: " << aString << " in function " << aFunctionName << " line " << aLine << " in file " << aFileName << std::endl;
		myDebugFile.flush();


		StackWalker aStackWalker;
		aStackWalker.ShowCallstack();

		size_t* a(NULL);

		wchar_t tmpFileName[256];
		mbstowcs_s(a, tmpFileName, aFileName, 256);

		wchar_t tmpString[256];
		mbstowcs_s(a, tmpString, aString, 256);

		_wassert(tmpString, tmpFileName, aLine);
	}

	void Debug::PrintMessage(const char* aString)
	{
		time_t t = time(0); // get current system time;
		struct tm timeinfo;
		localtime_s(&timeinfo, &t);
		myDebugFile << timeinfo.tm_hour << ":" << timeinfo.tm_min << ":" << timeinfo.tm_sec << " print " << std::endl;
		myDebugFile << "	" << aString << std::endl;
		myDebugFile.flush();
	}

	void Debug::DebugMessage(const int aLine, const char* aFileName, const char* aFormattedString, ...)
	{
		time_t t = time(0); // get current system time;
		struct tm timeinfo;
		localtime_s(&timeinfo, &t);
		myDebugFile << timeinfo.tm_hour << ":" << timeinfo.tm_min << ":" << timeinfo.tm_sec << " debug " << std::endl;
		char buffer[256];
		va_list args;
		va_start(args, aFormattedString);
		vsprintf_s(buffer, aFormattedString, args);
		va_end(args);
		myDebugFile << "	Debug: " << buffer << " at line " << aLine << " in file " << aFileName << std::endl;
		myDebugFile.flush();
	}
}
