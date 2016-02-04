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
		myRelMouseX = 0;
		myRelMouseY = 0;

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
			myRelMouseX = 0;
			myRelMouseY = 0;

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
		tagPOINT cursorPoint;
		GetCursorPos(&cursorPoint);
		ScreenToClient(GetActiveWindow(), &cursorPoint);
		myMouseX = static_cast<int>(cursorPoint.x);
		myMouseY = static_cast<int>(cursorPoint.y);

		myMouseX += myMouseState.lX;
		myMouseY += myMouseState.lY;
		myRelMouseX += myMouseState.lX;
		myRelMouseY += myMouseState.lY;

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
		bool InputManager::RightMouseButtonClicked() const
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


		/*InputManager::InputManager()
		{

		}

		void InputManager::Init(HWND aHwnd, HINSTANCE aHInstance, DWORD aKeyCoopFlags, DWORD aMouseCoopFlags)
		{

			DirectInput8Create(aHInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<void**>(&myDirectInput), 0);

			myDirectInput->CreateDevice(GUID_SysKeyboard, &myKeyboardDevice, nullptr);
			myDirectInput->CreateDevice(GUID_SysMouse, &myMouseDevice, nullptr);

			myKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
			myMouseDevice->SetDataFormat(&c_dfDIMouse);

			myKeyboardDevice->SetCooperativeLevel(aHwnd, aKeyCoopFlags);
			myMouseDevice->SetCooperativeLevel(aHwnd, aMouseCoopFlags);

			myKeyboardDevice->Acquire();
			myMouseDevice->Acquire();

			myWindowHandler = aHwnd;
		}



		InputManager::~InputManager()
		{
			myKeyboardDevice->Unacquire();
			myMouseDevice->Unacquire();
		}

		void InputManager::Update()
		{
			CapturePreviousState();
			HRESULT hr = myKeyboardDevice->GetDeviceState(sizeof(myKeyState), reinterpret_cast<void**>(&myKeyState));

			if (FAILED(hr))
			{
				ZeroMemory(myKeyState, sizeof(myKeyState));

				myKeyboardDevice->Acquire();
			}

			hr = myMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), reinterpret_cast<void**>(&myMouseState));
			if (FAILED(hr))
			{
				ZeroMemory(&myMouseState, sizeof(myMouseState));

				myMouseDevice->Acquire();
			}

			tagPOINT cursorPoint;
			GetCursorPos(&cursorPoint);
			ScreenToClient(myWindowHandler, &cursorPoint);
			myMousePos.x = static_cast<float>(cursorPoint.x);
			myMousePos.y = static_cast<float>(cursorPoint.y);

			//myMousePos.x += myMouseState.lX;
			//myMousePos.y += myMouseState.lY;
			//myMousePos.z += myMouseState.lZ;
		}

		const Vector2<float>& InputManager::GetMousePosition() const
		{
			return myMousePos;
		}

		void InputManager::CapturePreviousState()
		{
			memcpy_s(myPreviousKeyState, sizeof(myPreviousKeyState), myKeyState, sizeof(myKeyState));
			memcpy_s(&myPreviousMouseState, sizeof(myPreviousMouseState), &myMouseState, sizeof(myMouseState));
		}

		bool InputManager::KeyIsPressed(unsigned int aKey) const
		{
			UCHAR key = static_cast<UCHAR>(aKey);
			return (myKeyState[key] & 0x80) != 0;
		}

		bool InputManager::KeyDown(unsigned int aKey) const
		{
			UCHAR key = static_cast<UCHAR>(aKey);
			return ((myKeyState[key] & 0x80) != 0 && (myPreviousKeyState[key] & 0x80) == 0);
		}

		bool InputManager::KeyUp(unsigned int aKey) const
		{
			UCHAR key = static_cast<UCHAR>(aKey);
			return ((myKeyState[key] & 0x80) == 0 && (myPreviousKeyState[key] & 0x80) != 0);
		}

		bool InputManager::MouseIsPressed(int aButton) const
		{
			return (myMouseState.rgbButtons[aButton] & 0x80) != 0;
		}

		bool InputManager::MouseDown(int aButton) const
		{
			return ((myMouseState.rgbButtons[aButton] & 0x80) != 0 && (myPreviousMouseState.rgbButtons[aButton] & 0x80) == 0);
		}

		bool InputManager::MouseUp(int aButton) const
		{
			return ((myMouseState.rgbButtons[aButton] & 0x80) == 0 && (myPreviousMouseState.rgbButtons[aButton] & 0x80) != 0);
		}

		double InputManager::GetMouseDX() const
		{
			return static_cast<double>(myMouseState.lX);
		}
		double InputManager::GetMouseDY() const
		{
			return static_cast<double>(myMouseState.lY);
		}

		double InputManager::GetMouseDZ() const
		{
			return static_cast<double>(myMouseState.lZ);
		}*/
	}
}
