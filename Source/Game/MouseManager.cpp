#include "stdafx.h"
#include "MouseManager.h"
#include "ResolutionManager.h"

MouseManager* MouseManager::myMouseManager = nullptr;

MouseManager::MouseManager()
{
}

void MouseManager::Initialize(const std::string &aFilePath, CU::DirectInput::InputManager* aInputManager)
{
	myInputManager = aInputManager;
	mySprite = new DX2D::CSprite(aFilePath.c_str());
	myPosition = {0.5f-(mySprite->GetSize().x*0.5f),0.5f-(mySprite->GetSize().y*0.5f)};
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
