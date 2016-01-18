#pragma once

#define DIRECTINPUT_VERSION 0x0800

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#include <dinput.h>
#include <vector>

namespace CU
{
	namespace DirectInput 
	{
		class InputWrapper
		{
		public:
			InputWrapper();
			~InputWrapper();

			bool Initialize(HINSTANCE, HWND);

			void Update();

			inline int GetMouseScroll();
			void GetMouseLocation(int& aX, int& aY); // Relative
			int GetMouseLocationX();
			int GetMouseLocationY();
			int GetMouseLocationXInPixels();
			int GetMouseLocationYInPixels();
			void GetMouseAbsoluteLocation(int& aX, int& aY);
			void SetMouseLocation(int aX, int aY);


			inline bool GetKeyDown(int aKey);
			inline bool GetKeyWasPressed(int aKey);
			inline bool GetKeyWasReleased(int aKey);

			inline bool GetMouseDown(int aButton);
			inline bool GetMouseWasDown(int aButton);
			inline bool GetMouseWasReleased(int aButton);
		private:
			IDirectInput8* myDirectInput;
			IDirectInputDevice8* myKeyboard;
			IDirectInputDevice8* myMouse;
			IDirectInputDevice8* myJoystick;

			std::vector<unsigned char> myKeyboardState;
			std::vector<unsigned char> myPrevKeyboardState;
			DIMOUSESTATE myMouseState;
			DIMOUSESTATE myPrevMouseState;

			DIJOYSTATE myJoystickState;
			DIJOYSTATE myPrevJoystickState;

			HWND myWindowHandler;

			bool ReadKeyboard();
			bool ReadMouse();
			bool ReadJoystick();

			bool mySuccededKeyboard;
			bool mySuccededMouse;
			bool mySuccededJoystick;

			void ProcessInput();
			int myScreenWidth;
			int myScreenHeight;
			int myMouseX;
			int myMouseY;
			int myMouseZ;
			
		};

		bool InputWrapper::GetKeyDown(int aKey)
		{
			if (myKeyboardState[aKey] > 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		bool InputWrapper::GetKeyWasPressed(int aKey)
		{
			if ((myKeyboardState[aKey] > 0)
				&& ((myPrevKeyboardState[aKey] == 0)))
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		bool InputWrapper::GetKeyWasReleased(int aKey)
		{
			if ((myKeyboardState[aKey] == 0)
				&& ((myPrevKeyboardState[aKey] > 0)))
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		bool InputWrapper::GetMouseDown(int aButton)
		{
			if (myMouseState.rgbButtons[aButton] > 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		bool InputWrapper::GetMouseWasDown(int aButton)
		{
			if (myMouseState.rgbButtons[aButton] > 0
				&& ((myPrevMouseState.rgbButtons[aButton]) == 0))
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		bool InputWrapper::GetMouseWasReleased(int aButton)
		{
			if (myMouseState.rgbButtons[aButton] == 0
				&& ((myPrevMouseState.rgbButtons[aButton]) > 0))
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		int InputWrapper::GetMouseScroll()
		{
			return myMouseZ;
		}
	}
}