#include "Game.h"
#include "VersionNo.h"
#include <string>
#include <stringapiset.h>

void Init(const char** argv, const int argc);

#ifdef _DEBUG
#pragma comment(lib,"Game_Debug.lib")
#endif // DEBUG
#ifdef NDEBUG
#pragma comment(lib,"Game_Release.lib")
#endif // DEBUG


#define USE_CONSOLE_COMMAND
// Comment out below define to disable command line
#pragma region WindowsInit
#ifdef USE_CONSOLE_COMMAND
	#pragma comment(linker, "/SUBSYSTEM:console")
#else
#pragma comment(linker, "/SUBSYSTEM:windows")
#endif

#ifdef USE_CONSOLE_COMMAND
int main(const int argc, const char** argv)
{
	Init(argv, argc);
	return 0;
}
#else
int WinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, char*, int /*nShowCmd*/)
{	
	Init();
	return 0;
}
#endif
#pragma endregion MainFunction

void Init(const char** argv, const int argc) 
{ 
	CGame myGame;
	    
	int version[] = { PRODUCTVER }; 
	std::string versionNumber;   
	 
	int count = sizeof(version) / sizeof(version[0]);  
	for (int i = 0; i < count; i++)
	{
		versionNumber += std::to_string(version[i]); 
		versionNumber += ",";
	} 
	versionNumber.pop_back();  

	

	myGame.Init(argv, argc); 
	 

}


