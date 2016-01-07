/*
This class handles the creation of the actual window
*/

#pragma once
#include <windows.h>
#include "../math/Vector2.h"

namespace DX2D
{
	class CWindowsWindow
	{
	public:
		CWindowsWindow(void);
		~CWindowsWindow(void);
		bool Init(Vector2<unsigned int> aWindowSize, HWND*& aHwnd, const std::wstring& aAppName, HINSTANCE& aHInstanceToFill);
		HWND GetWindowHandle() const {return myWindowHandle;}
		void SetResolution(DX2D::Vector2<unsigned int> aResolution);
	private:
		 static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		 HWND myWindowHandle;
		 WNDCLASSEX myWindowClass;
	};
}
