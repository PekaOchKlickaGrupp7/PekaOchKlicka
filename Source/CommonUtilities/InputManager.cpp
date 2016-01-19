#include "stdafx.h"
#include "InputManager.h"
#include <assert.h>

namespace CU
{
	namespace DirectInput
	{
		enum class eMouseButtons
		{
			LeftButton,
			RightButton,
			MiddleButton
		};

		InputManager::InputManager()
		{
			myDirectInput = 0;
			myKeyboard = 0;
			myMouse = 0;

			//ZeroMemory: Set all bits starting at address (arg1) to zero, for a length of (arg2)
			ZeroMemory(&myKeyboardState, sizeof(BYTE) * 256);
			ZeroMemory(&myPreviousKeyboardState, sizeof(BYTE) * 256);
			ZeroMemory(&myMouseState, sizeof(myMouseState));
		}

		InputManager::~InputManager()
		{
			ReleaseDevices();
			myInitialized = false;
		}

		void InputManager::Initialize(HINSTANCE aHINSTANCE, HWND aHWND, int aScreenWidth, int aScreenHeight)
		{
			//Initialize DirectInput object
			DirectInput8Create(aHINSTANCE, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&myDirectInput, NULL);

			//Initialize keyboard
			myDirectInput->CreateDevice(GUID_SysKeyboard, &myKeyboard, NULL);
			myKeyboard->SetDataFormat(&c_dfDIKeyboard);
			myKeyboard->SetCooperativeLevel(aHWND, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
			myKeyboard->Acquire();

			//Initialize mouse
			myDirectInput->CreateDevice(GUID_SysMouse, &myMouse, NULL);
			myMouse->SetDataFormat(&c_dfDIMouse2);
			myMouse->SetCooperativeLevel(aHWND, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
			myMouse->Acquire();

			myMouseX = 0;
			myMouseY = 0;

			myScreenWidth = aScreenWidth;
			myScreenHeight = aScreenHeight;

			myInitialized = true;
		}

		void InputManager::ReleaseDevices()
		{
			if (myMouse)
			{
				myMouse->Unacquire();
				myMouse->Release();
				myMouse = 0;
			}

			// Release the keyboard.
			if (myKeyboard)
			{
				myKeyboard->Unacquire();
				myKeyboard->Release();
				myKeyboard = 0;
			}

			// Release the main interface to direct input.
			if (myDirectInput)
			{
				myDirectInput->Release();
				myDirectInput = 0;
			}
		}

		void InputManager::ReadKeyboard()
		{
			//HRESULT is a data type for catching errors
			HRESULT result;
			result = myKeyboard->GetDeviceState(sizeof(myKeyboardState), (LPVOID)&myKeyboardState);
			//Reacquire keyboard if lost
			if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
			{
				myKeyboard->Acquire();
			}
		}

		void InputManager::ReadMouse()
		{
			//HRESULT is a data type for catching errors
			HRESULT result;
			result = myMouse->GetDeviceState(sizeof(DIMOUSESTATE2), (LPVOID)&myMouseState);
			//Reacquire mouse if lost
			if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
			{
				myMouse->Acquire();
			}
		}

		void InputManager::Update()
		{
			myPreviousMouseState = myMouseState;
			for (unsigned int i = 0; i < 256; i++)
			{
				myPreviousKeyboardState[i] = myKeyboardState[i];
			}
			ReadKeyboard();
			ReadMouse();
			HandleMouseInput();
			myScrollWheelPos += GetRelativeScrollWheel();
		}

		void InputManager::HandleMouseInput()
		{
			//Update position
			myMouseX += myMouseState.lX;
			myMouseY += myMouseState.lY;

			//Restrict mouse to window
			if (myMouseX < 0)
			{
				myMouseX = 0;
			}
			if (myMouseY < 0)
			{
				myMouseY = 0;
			}
			if (myMouseX > myScreenWidth)
			{
				myMouseX = myScreenWidth;
			}
			if (myMouseY > myScreenHeight)
			{
				myMouseY = myScreenHeight;
			}

			//Check mouse buttons' status
		}
		
		//(BUTTON & 0x80) != 0 means the button is down
		//(BUTTON & 0x80) == 0 means the button is up
		bool InputManager::LeftMouseButtonDown() const
		{
			if (IsPressed(myMouseState.rgbButtons[static_cast<int> (eMouseButtons::LeftButton)]) == true)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		bool InputManager::MiddleMouseButtonDown() const
		{
			if (IsPressed(myMouseState.rgbButtons[static_cast<int> (eMouseButtons::RightButton)]) == true)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		bool InputManager::RightMouseButtonDown() const
		{
			if (IsPressed(myMouseState.rgbButtons[static_cast<int> (eMouseButtons::MiddleButton)]) == true)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		bool InputManager::LeftMouseButtonClicked() const
		{
			if (IsPressed(myMouseState.rgbButtons[static_cast<int> (eMouseButtons::LeftButton)]) == true
				&& IsPressed(myPreviousMouseState.rgbButtons[static_cast<int> (eMouseButtons::LeftButton)]) == false)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		bool InputManager::MiddleMouseButtonClicked() const
		{
			if (IsPressed(myMouseState.rgbButtons[static_cast<int> (eMouseButtons::RightButton)]) == true
				&& IsPressed(myPreviousMouseState.rgbButtons[static_cast<int> (eMouseButtons::RightButton)]) == false)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		bool InputManager::RightMouseButtonClicked() const
		{
			if (IsPressed(myMouseState.rgbButtons[static_cast<int> (eMouseButtons::MiddleButton)]) == true
				&& IsPressed(myPreviousMouseState.rgbButtons[static_cast<int> (eMouseButtons::MiddleButton)]) == false)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		bool InputManager::LeftMouseButtonReleased() const
		{
			if (IsPressed(myMouseState.rgbButtons[static_cast<int> (eMouseButtons::LeftButton)]) == false)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		bool InputManager::MiddleMouseButtonReleased() const
		{
			if (IsPressed(myMouseState.rgbButtons[static_cast<int> (eMouseButtons::RightButton)]) == false)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		bool InputManager::RightMouseButtonReleased() const
		{
			if (IsPressed(myMouseState.rgbButtons[static_cast<int> (eMouseButtons::MiddleButton)]) == false)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		bool InputManager::KeyDown(unsigned char aKey) const
		{
			assert(aKey < 256 && "Key not recognized!");
			return IsPressed(myKeyboardState[aKey]) == true;
		}
		bool InputManager::KeyPressed(unsigned char aKey) const
		{
			assert(aKey < 256 && "Key not recognized!");
			return (IsPressed(myKeyboardState[aKey]) == true
				&& IsPressed(myPreviousKeyboardState[aKey]) == false);
		}
		bool InputManager::KeyUp(unsigned char aKey) const
		{
			assert(aKey < 256 && "Key not recognized!");
			return IsPressed(myKeyboardState[aKey]) == false;
		}
		bool InputManager::IsPressed(BYTE aValue) const
		{
			return ((aValue & 0x80) > 0);
		}
	}
}
