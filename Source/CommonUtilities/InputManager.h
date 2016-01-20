#pragma once
#define DIRECTINPUT_VERSION 0x0800
//Linking
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
//Includes
#include <dinput.h>
#include "Windows.h"

namespace CU
{
	namespace DirectInput
	{
		class InputManager
		{
		public:
			InputManager();
			~InputManager();

			void Initialize(HINSTANCE aHINSTANCE, HWND aHWND, int aScreenWidth, int aScreenHeight);
			void ReleaseDevices();

			void ReadKeyboard();
			void ReadMouse();
			void Update();
			void HandleMouseInput();
			
			bool LeftMouseButtonDown() const;
			bool MiddleMouseButtonDown() const;
			bool RightMouseButtonDown() const;

			bool LeftMouseButtonClicked() const;
			bool MiddleMouseButtonClicked() const;
			bool RightMouseButtonClicked() const;

			bool LeftMouseButtonReleased() const;
			bool MiddleMouseButtonReleased() const;
			bool RightMouseButtonReleased() const;

			inline LONG GetRelativeScrollWheel() const;
			inline LONG GetAbsoluteScrollWheel() const;

			bool KeyDown(unsigned char aKey) const;
			bool KeyPressed(unsigned char aKey) const;
			bool KeyUp(unsigned char aKey) const;
			bool IsPressed(BYTE aValue) const;

			inline bool CheckIfInitialized();
			inline POINT GetRelativeMousePos();
			inline POINT GetAbsoluteMousePos();
			inline void SetAbsoluteMousePos(int aX, int aY);

		private:
			IDirectInput8* myDirectInput;
			IDirectInputDevice8* myKeyboard;
			IDirectInputDevice8* myMouse;

			unsigned char myKeyboardState[256];
			unsigned char myPreviousKeyboardState[256];
			DIMOUSESTATE2 myMouseState;
			DIMOUSESTATE2 myPreviousMouseState;

			POINT myMousePos;
			long myScrollWheelPos;

			int myScreenWidth;
			int myScreenHeight;
			int myMouseX;
			int myMouseY;

			bool myInitialized;
		};

		bool InputManager::CheckIfInitialized()
		{
			if (myInitialized == true)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		//Returns the change in scroll wheel position since the last frame
		LONG InputManager::GetRelativeScrollWheel() const
		{
			return myMouseState.lZ;
		}

		//Returns the total scroll wheel displacement since program start
		LONG InputManager::GetAbsoluteScrollWheel() const
		{
			return myScrollWheelPos;
		}

		POINT InputManager::GetRelativeMousePos()
		{
			POINT pos;
			pos.x = myMouseX;
			pos.y = myMouseY;
			return pos;
		}

		POINT InputManager::GetAbsoluteMousePos()
		{
			GetCursorPos(&myMousePos);
			return myMousePos;
		}

		void InputManager::SetAbsoluteMousePos(int aX, int aY)
		{
			SetCursorPos(aX, aY);
		}
	}
}



//KEYS

//DIK_ESCAPE
//DIK_1             On main keyboard
//DIK_2             On main keyboard
//DIK_3             On main keyboard
//DIK_4             On main keyboard
//DIK_5             On main keyboard
//DIK_6             On main keyboard
//DIK_7             On main keyboard
//DIK_8             On main keyboard
//DIK_9             On main keyboard
//DIK_0             On main keyboard
//DIK_MINUS         On main keyboard
//DIK_EQUALS        On main keyboard
//DIK_BACK          The backspace key
//DIK_TAB
//DIK_Q
//DIK_W
//DIK_E
//DIK_R
//DIK_T
//DIK_Y
//DIK_U
//DIK_I
//DIK_O
//DIK_P
//DIK_LBRACKET      The[key
//DIK_RBRACKET      The] key
//DIK_RETURN        enter key on main keyboard
//DIK_LCONTROL      Left ctrl key
//DIK_A
//DIK_S
//DIK_D
//DIK_F
//DIK_G
//DIK_H
//DIK_J
//DIK_K
//DIK_L
//DIK_SEMICOLON
//DIK_APOSTROPHE
//DIK_GRAVE          Grave accent(`) key
//DIK_LSHIFT         Left shift key
//DIK_BACKSLASH
//DIK_Z
//DIK_X
//DIK_C
//DIK_V
//DIK_B
//DIK_N
//DIK_M
//DIK_COMMA
//DIK_PERIOD         On main keyboard
//DIK_SLASH          Forward slash on main keyboard
//DIK_RSHIFT         Right shift key
//DIK_MULTIPLY       The * key on numeric keypad
//DIK_LMENU          Left alt key
//DIK_SPACE          spacebar
//DIK_CAPITAL        caps lock key
//DIK_F1
//DIK_F2
//DIK_F3
//DIK_F4
//DIK_F5
//DIK_F6
//DIK_F7
//DIK_F8
//DIK_F9
//DIK_F10
//DIK_NUMLOCK
//DIK_SCROLL         scroll lock
//DIK_NUMPAD7
//DIK_NUMPAD8
//DIK_NUMPAD9
//DIK_SUBTRACT       minus sign on numeric keypad
//DIK_NUMPAD4
//DIK_NUMPAD5
//DIK_NUMPAD6
//DIK_ADD            plus sign on numeric keypad
//DIK_NUMPAD1
//DIK_NUMPAD2
//DIK_NUMPAD3
//DIK_NUMPAD0
//DIK_DECIMAL        period(decimal point) on numeric keypad
//DIK_F11
//DIK_F12
//DIK_F13
//DIK_F14
//DIK_F15
//DIK_KANA           On Japanese keyboard
//DIK_CONVERT        On Japanese keyboard
//DIK_NOCONVERT      On Japanese keyboard
//DIK_YEN            On Japanese keyboard
//DIK_NUMPADEQUALS   On numeric keypad(NEC PC98)
//DIK_CIRCUMFLEX     On Japanese keyboard
//DIK_AT             On Japanese keyboard
//DIK_COLON          On Japanese keyboard
//DIK_UNDERLINE      On Japanese keyboard
//DIK_KANJI          On Japanese keyboard
//DIK_STOP           On Japanese keyboard
//DIK_AX             On Japanese keyboard
//DIK_UNLABELED      On Japanese keyboard
//DIK_NUMPADENTER
//DIK_RCONTROL       Right ctrl key
//DIK_NUMPADCOMMA    comma on NEC PC98 numeric keypad
//DIK_DIVIDE         Forward slash on numeric keypad
//DIK_SYSRQ
//DIK_RMENU          Right alt key
//DIK_HOME
//DIK_UP             up arrow
//DIK_PRIOR          page up
//DIK_LEFT           left arrow
//DIK_RIGHT          right arrow
//DIK_END
//DIK_DOWN           down arrow
//DIK_NEXT           page down
//DIK_INSERT
//DIK_DELETE
//DIK_LWIN           Left Windows key
//DIK_RWIN           Right Windows key
//DIK_APPS           Application key
//DIK_PAUSE