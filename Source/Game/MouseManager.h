#pragma once
#include "..\DX2DEngine\tga2d\sprite\sprite.h"
#include "..\DX2DEngine\tga2d\engine.h"
#include "..\CommonUtilities\InputManager.h"
#include "ResolutionManager.h"
#include <Windows.h>
#include "..\CommonUtilities\GrowingArray.h"

enum class eInteractive
{
	eRegular,
	eActive,
};
enum class eMouseButtons
{
	eLeft,
	eRight,
	eCenter
};

// This is actually very handy. I don't have to static_cast all the time.
template <typename T>
int eMouse(T aElementType)
{
	return static_cast<int>(aElementType);
}

class MouseManager
{
public:
	~MouseManager();

	static void CreateInstance()
	{
		if (myMouseManager == nullptr)
		{
			myMouseManager = new MouseManager();
		}
	}

	static MouseManager* const GetInstance()
	{
		if (myMouseManager == nullptr)
		{
			myMouseManager = new MouseManager();
		}
		return myMouseManager;
	}

	static void DestroyInstance()
	{
		if (myMouseManager != nullptr)
		{
			myMouseManager->Destroy();
			delete myMouseManager;
			myMouseManager = nullptr;
		}
	}

	void Initialize(CommonUtilities::GrowingArray<std::string> &aFilePath, CU::DirectInput::InputManager* aInputManager);
	void Render(Synchronizer &aSynchronizer);
	void Update(float aDeltaTime);
	bool ButtonClicked(eMouseButtons aButton);
	bool ButtonPressed(eMouseButtons aButton);

	void SetHideGameMouse(bool aSet) { myHideGameMouse = aSet; };
	bool GetHideGameMouse() { return myHideGameMouse; };

	DX2D::Vector2f &GetPosition()
	{ 
		return myPosition;
	};

	void SetInteractiveMode(eInteractive aInteractiveMode);

private:
	MouseManager();
	static MouseManager* myMouseManager;

	void Destroy();

	CU::DirectInput::InputManager* myInputManager;
	
	DX2D::Vector2f myPosition;
	bool myHideGameMouse;
	bool myInMenu;

	// This is the current sprite
	DX2D::CSprite* mySprite;

	CommonUtilities::GrowingArray<DX2D::CSprite*, int> mySpriteInteractive;
};

