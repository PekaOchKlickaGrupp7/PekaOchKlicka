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
	myHideGameMouse = false;

	myInputManager = aInputManager;

	mySpriteInteractive.Init(6); // There are six different cursors
	for (unsigned short i = 0; i < aFilePath.Size(); i++)
	{
		mySprite = new DX2D::CSprite(aFilePath[i].c_str());
		mySpriteInteractive.Add(mySprite);
	}

	mySprite = mySpriteInteractive[eMouse(eInteractive::eRegular)];

	mySpriteInteractive[eMouse(eInteractive::eTalkTo)]->SetPivot({ 0.5f, 0.0f });
	mySpriteInteractive[eMouse(eInteractive::eExamine)]->SetPivot({ 0.5f, 0.5f });
	mySpriteInteractive[eMouse(eInteractive::eChooseOption)]->SetPivot({ 0.6f, 0.0f });
	mySpriteInteractive[eMouse(eInteractive::ePickUp)]->SetPivot({ 0.5f, 0.5f });
	mySpriteInteractive[eMouse(eInteractive::eGrabAndDrag)]->SetPivot({ 0.5f, 0.0f });

	myPosition = { 0.5f, 0.5f };
	mySprite->SetPivot({ 0.5f, 0.5f });
	mySprite->SetPosition(myPosition);
	

	myInputManager->SetHideMouse(true);
}

void MouseManager::Update(float)
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
	myInputManager->SetAbsoluteMousePos(
		static_cast<int>((((
		ResolutionManager::GetInstance()->GetRenderAreaDimension().x * 0.5f) +
		ResolutionManager::GetInstance()->GetRenderAreaPosition().x)) + mySprite->GetSize().x),
		static_cast<int>((((
		ResolutionManager::GetInstance()->GetRenderAreaDimension().y * 0.5f) +
		ResolutionManager::GetInstance()->GetRenderAreaPosition().y)) + mySprite->GetSize().y));
#endif 


	static float aSpeed = 0.0005f;
	myPosition.x += (myInputManager->GetRelativeMousePos().x) * aSpeed;
	myPosition.y += (myInputManager->GetRelativeMousePos().y) * aSpeed;

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

	mySprite->SetSize({(myPosition.y + 0.2f) * 1.5f,(myPosition.y + 0.2f) * 1.5f});
	/*if (mySprite->GetSize <= 0.2f)
	{
		mySprite->SetSize({ 0.2f, 0.2f });
	}*/
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
	mySprite = mySpriteInteractive[eMouse(aInteractiveMode)];
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
