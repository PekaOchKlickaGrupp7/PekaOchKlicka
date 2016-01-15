#include "stdafx.h"
#include "InputWrapper.h"
#include <dinput.h>
#include "DL_Debug.h"

namespace CU
{
	namespace DirectInput
	{
		InputWrapper::InputWrapper()
		{
			myDirectInput = 0;
			myKeyboard = 0;
			myMouse = 0;
			myJoystick = 0;
			myKeyboardState.resize(256);
			myPrevKeyboardState.resize(256);
		}


		InputWrapper::~InputWrapper()
		{

		}

		bool InputWrapper::Initialize(HINSTANCE aHinstance, HWND aHwnd)
		{
			HRESULT result;

			myMouseX = 0;
			myMouseY = 0;

			result = DirectInput8Create(aHinstance, DIRECTINPUT_VERSION,
				IID_IDirectInput8, (void**)&myDirectInput, NULL);
			if (myDirectInput == nullptr)
			{
				mySuccededKeyboard = false;
				DL_PRINT("A keyboard device could not be found.");
				//DL_DEBUG("Joystick was not found.");
			}
			else
			{
				result = myDirectInput->CreateDevice(GUID_SysKeyboard, &myKeyboard, NULL);
				result = myKeyboard->SetDataFormat(&c_dfDIKeyboard);
				result = myKeyboard->SetCooperativeLevel(aHwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
				result = myKeyboard->Acquire();
				mySuccededKeyboard = true;
				DL_PRINT("A Keyboard device was found and acquired.");
			}

			result = myDirectInput->CreateDevice(GUID_SysMouse, &myMouse, NULL);
			if (myMouse == nullptr)
			{
				mySuccededMouse = false;
				DL_PRINT("A mouse device could not be found.");
				//DL_DEBUG("Joystick was not found.");
			}
			else
			{
				result = myMouse->SetDataFormat(&c_dfDIMouse);
				result = myMouse->SetCooperativeLevel(aHwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
				result = myMouse->Acquire();

				mySuccededMouse = true;

				DL_PRINT("A Mouse device was found and acquired.");
			}

			result = myDirectInput->CreateDevice(GUID_Joystick, &myJoystick, NULL);
			if (myJoystick == nullptr)
			{
				mySuccededJoystick = false;
				DL_PRINT("A Joystick device could not be found.");
				//DL_DEBUG("Joystick was not found.");
			}
			else
			{
				result = myJoystick->SetDataFormat(&c_dfDIJoystick);
				result = myJoystick->SetCooperativeLevel(aHwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
				result = myJoystick->Acquire();
				mySuccededJoystick = true;
				DL_PRINT("A Joystick device was found and acquired.");
			}

			myWindowHandler = aHwnd;
			return true;
		}

		void InputWrapper::ProcessInput()
		{
			tagPOINT cursorPoint;
			GetCursorPos(&cursorPoint);
			ScreenToClient(myWindowHandler, &cursorPoint);
			myMouseX = static_cast<float>(cursorPoint.x);
			myMouseY = static_cast<float>(cursorPoint.y);
			myMouseZ += myMouseState.lZ;
		}

		// send as reference so I can change the 2 values instead of return them
		void InputWrapper::GetMouseLocation(int& aMouseX, int& aMouseY)
		{
			aMouseX = myMouseX;
			aMouseY = myMouseY;
			return;
		}

		int InputWrapper::GetMouseLocationX()
		{
			return myMouseX;
		}

		int InputWrapper::GetMouseLocationY()
		{
			return myMouseY;
		}

		void InputWrapper::SetMouseLocation(int aX, int aY)
		{
			SetCursorPos(aX, aY);
		}

		bool InputWrapper::ReadKeyboard()
		{
			if (mySuccededKeyboard == true)
			{
				HRESULT result;

				result = myKeyboard->GetDeviceState(sizeof(char) * myKeyboardState.size(), (LPVOID)&myKeyboardState[0]);

				if (FAILED(result))
				{
					if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
					{
						myKeyboard->Acquire();
					}
					else
					{
						return false;
					}
				}
				return true;
			}
			else
			{
				return false;
			}
		}

		bool InputWrapper::ReadMouse()
		{
			if (mySuccededMouse == true)
			{
				HRESULT result;

				result = myMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&myMouseState);
				if (FAILED(result))
				{
					if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
					{
						myMouse->Acquire();
					}
					else
					{
						return false;
					}
				}
				return true;
			}
			else
			{
				return false;
			}
		}

		bool InputWrapper::ReadJoystick()
		{
			if (mySuccededJoystick == true)
			{
				HRESULT result;

				result = myJoystick->GetDeviceState(sizeof(DIJOYSTATE), (LPVOID)&myJoystick);
				if (FAILED(result))
				{
					if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
					{
						myJoystick->Acquire();
					}
					else
					{
						return false;
					}
				}
				return true;
			}
			else
			{
				return false;
			}
		}

		void InputWrapper::Update()
		{
			bool result;
			myPrevKeyboardState = myKeyboardState;

			result = ReadKeyboard();
			result = ReadMouse();
			result = ReadJoystick();

			ProcessInput();

			return;
		}
	}
}