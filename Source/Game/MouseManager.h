#pragma once
#include "..\DX2DEngine\tga2d\sprite\sprite.h"
#include "..\DX2DEngine\tga2d\engine.h"
#include "..\CommonUtilities\InputManager.h"
#include <Windows.h>

class MouseManager
{
public:
	~MouseManager();

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
		myMouseManager->Destroy();
		delete myMouseManager;
	}

	DX2D::Vector2f &GetPosition() { return myPosition; };

	void Initialize(CU::DirectInput::InputManager* anInputManager) { myInputManager = anInputManager; };

private:
	MouseManager();
	static MouseManager* myMouseManager;

	void Destroy();

	CU::DirectInput::InputManager* myInputManager;
	
	DX2D::Vector2f myPosition;
	DX2D::CSprite* mySprite;
};

