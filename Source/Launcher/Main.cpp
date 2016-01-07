#include "Game.h"

void Init();

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
int main()
{
	Init();
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


void Init()
{
	CGame myGame;
	myGame.Init();


}


