#include "stdafx.h"
#include "MouseManager.h"
#include "ResolutionManager.h"
#include <iostream>
#include "EventVariablesManager.h"

MouseManager* MouseManager::myMouseManager = nullptr;

MouseManager::MouseManager()
{
}

void MouseManager::Initialize(CommonUtilities::GrowingArray<std::string> &aFilePath,
	CU::DirectInput::InputManager* aInputManager)
{
	myHideGameMouse = false;
	myInMenu = false;


	myInputManager = aInputManager;

	mySpriteInteractive.Init(2); // There are six different cursors
	for (unsigned short i = 0; i < aFilePath.Size(); i++)
	{
		mySprite = new DX2D::CSprite(aFilePath[i].c_str());
		mySpriteInteractive.Add(mySprite);
	}

	mySprite = mySpriteInteractive[eMouse(eInteractive::eRegular)];

	mySpriteInteractive[eMouse(eInteractive::eActive)]->SetPivot({ 0.5f, 0.5f });

	myPosition = { 0.5f, 0.5f };
	mySprite->SetPivot({ 0.5f, 0.5f });
	mySprite->SetPosition(myPosition);
	

	myInputManager->SetHideMouse(true);
}

void MouseManager::Update(float aDeltaTime)
{


	//
	//Hold L-CTRL to unlock mouse in debug
#ifdef _DEBUG
	if (myInputManager->KeyDown(DIK_LCONTROL) == false)
	{
		myInputManager->SetAbsoluteMousePos(
			static_cast<int>((((
			ResolutionManager::GetInstance()->GetRenderAreaDimension().x * 0.5f) +
			ResolutionManager::GetInstance()->GetRenderAreaPosition().x)) + mySprite->GetSize().x),
			static_cast<int>((((
			ResolutionManager::GetInstance()->GetRenderAreaDimension().y * 0.5f) +
			ResolutionManager::GetInstance()->GetRenderAreaPosition().y)) + mySprite->GetSize().y));
	}
#endif 

	//Release
#ifdef NDEBUG
	int X = ((((
		ResolutionManager::GetInstance()->GetRenderAreaDimension().x * 0.5f) +
		ResolutionManager::GetInstance()->GetRenderAreaPosition().x)) + mySprite->GetSize().x);
	int Y = ((((
		ResolutionManager::GetInstance()->GetRenderAreaDimension().y * 0.5f) +
		ResolutionManager::GetInstance()->GetRenderAreaPosition().y)) + mySprite->GetSize().y);
	myInputManager->SetAbsoluteMousePos(X, Y);
#endif 


	static float aSpeed = 0.0005f;
	myPosition.x += (myInputManager->GetRelativeMousePos().x) * aSpeed;
	myPosition.y += (myInputManager->GetRelativeMousePos().y) * aSpeed * ResolutionManager::GetInstance()->GetAspectRatio();

	if (myPosition.x <= 0)
	{
		myPosition.x = 0;
	}
	else if ((myPosition.x) >= 1)
	{
		myPosition.x = 1;
	}
	
	if (myPosition.y <= 0)
	{
		myPosition.y = 0;
	}
	else if ((myPosition.y) >= 1)
	{
		myPosition.y = 1;
	}

	//EventVariablesManager::GetInstance()->GetVariable(myInMenu, "InMenu"); // Doing this every frame might not be a great idea. Fuck it. Will fix in the event. /Linus
	//if (myInMenu == false)
	//{
	//	mySprite->SetSize({ (myPosition.y + 0.2f) * 1.5f, (myPosition.y + 0.2f) * 1.5f });
	//	//std::cout << "Size x: " << mySprite->GetSize().x << std::endl;
	//	if (mySprite->GetScale().x <= 0.8f)
	//	{
	//		mySprite->SetSize({ 0.8f, 0.8f });
	//	}
	//}
	//else
	//{
	//	mySprite->SetSize({ 1.0f, 1.0f });
	//}
}

bool MouseManager::ButtonClicked(eMouseButtons aButton)
{
	switch (aButton)
	{
	case eMouseButtons::eLeft:
		return myInputManager->LeftMouseButtonClicked();
		break;
	case eMouseButtons::eRight:
		return myInputManager->RightMouseButtonClicked();
		break;
	case eMouseButtons::eCenter:
		return myInputManager->MiddleMouseButtonClicked();
		break;
	default:
		return false;
		break;
	}
}
void MouseManager::SetInteractiveMode(eInteractive aInteractiveMode)
{
	if (eMouse(aInteractiveMode) >= 0 && eMouse(aInteractiveMode) <= 1)
	{
		mySprite = mySpriteInteractive[eMouse(aInteractiveMode)];
	}
}

void MouseManager::Render(Synchronizer &aSynchronizer)
{
	if (myHideGameMouse != true)
	{
		RenderCommand command;
		command.mySprite = mySprite;
		command.myPosition = myPosition;
		command.myType = eRenderType::eSprite;

		aSynchronizer.AddRenderCommand(command);
	}
}

MouseManager::~MouseManager()
{
}
