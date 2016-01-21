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

	myPosition.x += (myInputManager->GetRelativeMousePos().x * aDeltaTime) * 0.00005f;
	myPosition.y += (myInputManager->GetRelativeMousePos().y * aDeltaTime) * 0.00005f;
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
