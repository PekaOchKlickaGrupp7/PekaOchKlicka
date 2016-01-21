#pragma once
#include "..\DX2DEngine\tga2d\sprite\sprite.h"
#include "..\DX2DEngine\tga2d\engine.h"
#include "..\CommonUtilities\InputManager.h"
#include "ResolutionManager.h"
#include <Windows.h>

class MouseManager
{
public:
	~MouseManager();

	static void CreateInstance()
	{
		if (myMouseManager = nullptr)
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

	void Initialize(const std::string &aFilePath, CU::DirectInput::InputManager* aInputManager);
	void Render(Synchronizer &aSynchronizer);
	void Update(float aDeltaTime);

	DX2D::Vector2f &GetPosition()
	{ 
		myResolutionIndependentPosition = myPosition;

		myResolutionIndependentPosition.x /=
			(ResolutionManager::GetInstance()->GetRenderAreaDimension().x +
			ResolutionManager::GetInstance()->GetRenderAreaPosition().x);
		myResolutionIndependentPosition.y /=
			(ResolutionManager::GetInstance()->GetRenderAreaDimension().y +
			ResolutionManager::GetInstance()->GetRenderAreaPosition().y);

		return myResolutionIndependentPosition;
	};

	void Initialize(CU::DirectInput::InputManager* anInputManager) { myInputManager = anInputManager; };

private:
	MouseManager();
	static MouseManager* myMouseManager;

	void Destroy();

	CU::DirectInput::InputManager* myInputManager;
	
	DX2D::Vector2f myPosition;
	DX2D::Vector2f myResolutionIndependentPosition;
	DX2D::CSprite* mySprite;
};

