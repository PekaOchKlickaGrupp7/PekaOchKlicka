#include "stdafx.h"
#include "MouseManager.h"
#include "ResolutionManager.h"
#include <iostream>

MouseManager* MouseManager::myMouseManager = nullptr;

MouseManager::MouseManager()
{
}

void MouseManager::Initialize(CommonUtilities::GrowingArray<std::string> &aFilePath,
	CU::DirectInput::InputManager* aInputManager)
{
	myInputManager = aInputManager;

	mySpriteInteractive.Init(6); // There are six different cursors
	for (size_t i = 0; i < aFilePath.Size(); i++)
	{
		mySprite = new DX2D::CSprite(aFilePath[i].c_str());
		mySpriteInteractive.Add(mySprite);
	}

	mySprite = mySpriteInteractive[eMouse(eInteractive::eRegular)];

	myPosition = { 0.5f - (mySprite->GetSize().x*0.5f), 0.5f - (mySprite->GetSize().y*0.5f) };
	mySprite->SetPosition(myPosition);

	myInputManager->SetHideMouse(true);
}

void MouseManager::Update(float aDeltaTime)
{
	myInputManager->SetAbsoluteMousePos(
		(ResolutionManager::GetInstance()->GetRenderAreaDimension().x + ResolutionManager::GetInstance()->GetRenderAreaPosition().x) * 0.5f,  
		(ResolutionManager::GetInstance()->GetRenderAreaDimension().y + ResolutionManager::GetInstance()->GetRenderAreaPosition().y) * 0.5f);

	float aSpeed = 0.0005f;

	
	

	myPosition.x += (myInputManager->GetRelativeMousePos().x) * aSpeed;
	myPosition.y += (myInputManager->GetRelativeMousePos().y) * aSpeed;

	if (myPosition.x <= 0)
	{
		myPosition.x = 0;
	}
	else if ((myPosition.x + mySprite->GetSize().x) >= 1)
	{
		myPosition.x = 1 - mySprite->GetSize().x;
	}
	
	if (myPosition.y <= 0)
	{
		myPosition.y = 0;
	}
	else if ((myPosition.y + mySprite->GetSize().y) >= 1)
	{
		myPosition.y = 1 - mySprite->GetSize().y;
	}

	//std::cout << myPosition.x << " " << myPosition.y << std::endl;
}

void MouseManager::SetInteractiveMode(eInteractive aInteractiveMode)
{
	mySprite = mySpriteInteractive[eMouse(aInteractiveMode)];
}

void MouseManager::Render(Synchronizer &aSynchronizer)
{
	RenderCommand command;
	command.mySprite = mySprite;
	command.myPosition = myPosition;
	command.myType = eRenderType::eSprite;

	aSynchronizer.AddRenderCommand(command);
}

MouseManager::~MouseManager()
{
}
