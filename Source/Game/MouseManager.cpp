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
	mySpriteInteractive[eMouse(eInteractive::eActive)]->SetColor({ 0,0,0,0 });

	myPosition = { 0.5f, 0.5f };
	mySprite->SetPivot({ 0.5f, 0.5f });
	mySprite->SetPosition(myPosition);
	

	myInputManager->SetHideMouse(true);

	myScale = 1.0f;
	myGoUp = true;
	myTotalLines = 8;

	myLineSprites.Init(myTotalLines);
	for (int i = 0; i < myTotalLines; ++i)
	{
		myLineSprites.Add(new DX2D::CSprite("Sprites/Cursor/Line.dds"));
		myLineSprites[myLineSprites.Size() - 1]->SetPivot({ 0.0f, 0.5f });
	}
}

void MouseManager::DrawLine(Synchronizer& aSynchronizer, int aBuffer, DX2D::Vector2f aPos, float aRotation, float aScale)
{
	float rot = aRotation + 20;
	float scale = 0.01f * aScale;

	float ratio = DX2D::CEngine::GetInstance()->GetWindowRatioInversed();
	DX2D::Vector2f pos = { aPos.x + (cos(rot * (3.14f / 180.0f)) * scale * ratio), aPos.y + sin(rot * (3.14f / 180.0f)) * scale };

	myLineSprites[aBuffer]->SetRotation(rot * (3.14f / 180.0f));
	//myLineSprites[aBuffer]->SetSize({ 0.5f, 0.5f });

	RenderCommand command;
	command.mySprite = myLineSprites[aBuffer];
	command.myPosition = pos;
	command.myType = eRenderType::eSprite;

	aSynchronizer.AddRenderCommand(command);

}

void MouseManager::Update(float aDeltaTime)
{
	float scaleSpeed = 1.2f;
	if (myGoUp == true)
	{
		myScale += scaleSpeed * aDeltaTime;
		if (myScale >= 1.2f)
		{
			myGoUp = false;
		}
	}
	else
	{
		myScale -= scaleSpeed * aDeltaTime;
		if (myScale <= 0.8f)
		{
			myGoUp = true;
		}
	}


	/*if (mySprite == mySpriteInteractive[eMouse(eInteractive::eActive)])
	{
		float aRad = mySprite->GetRotation() + (0.001f * aDeltaTime);
		mySprite->SetRotation(aRad);
	}
	else
	{
		mySprite->SetRotation(0.0f);
	}*/



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
	float X = ((((
		ResolutionManager::GetInstance()->GetRenderAreaDimension().x * 0.5f) +
		ResolutionManager::GetInstance()->GetRenderAreaPosition().x)) + mySprite->GetSize().x);
	float Y = ((((
		(ResolutionManager::GetInstance()->GetRenderAreaDimension().y) * 0.5f) +
		(ResolutionManager::GetInstance()->GetRenderAreaPosition().y))) + mySprite->GetSize().y);
	myInputManager->SetAbsoluteMousePos(static_cast<int>(X), static_cast<int>(Y));
#endif 


	static float aSpeed = 0.0005f;
	if (myHideGameMouse != true)
	{
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
bool MouseManager::ButtonPressed(eMouseButtons aButton)
{
	switch (aButton)
	{
	case eMouseButtons::eLeft:
		return myInputManager->LeftMouseButtonDown();
		break;
	case eMouseButtons::eRight:
		return myInputManager->RightMouseButtonDown();
		break;
	case eMouseButtons::eCenter:
		return myInputManager->MiddleMouseButtonDown();
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


		if (mySprite == mySpriteInteractive[eMouse(eInteractive::eActive)])
		{
			for (float i = 0; i < myTotalLines; ++i)
			{
				DrawLine(aSynchronizer, static_cast<int>(i), myPosition, (i / myTotalLines) * 360.0f, myScale);
			}
		}
	}
}

MouseManager::~MouseManager()
{
}
