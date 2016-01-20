#include "Game.h"

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
	const char* value = "";
	Init(&value, 0);
	return 0;
}
#endif
#pragma endregion MainFunction


void Init(const char** argv, const int argc)
{
	CGame myGame;
	myGame.Init(argv, argc);


}


