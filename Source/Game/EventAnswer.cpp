#include "stdafx.h"
#include "EventAnswer.h"
#include "MouseManager.h"

//Work in progress

EventAnswer::EventAnswer() : myBackgroundSprite(nullptr)
{
	myTextIndex = 0;
}

EventAnswer::~EventAnswer()
{
	SAFE_DELETE(myBackgroundSprite);
	SAFE_DELETE(myTextRender);
}

void EventAnswer::Init(Room* aRoom, CGameWorld* aGameWorld)
{
	Event::Init(aRoom, aGameWorld);
	myTextRender = new DX2D::CText("Text/courier.ttf_sdf");
	myTextRender->myText = myText;

	
	myBackgroundSprite = new DX2D::CSprite("Sprites/TalkBackground.dds");
	myBackgroundSprite->SetSize({ 1.0f, myTextRender->GetHeight() });
}

bool EventAnswer::Update(const float)
{
	MouseManager::GetInstance()->GetPosition();
	return true;
}

void EventAnswer::Render(Synchronizer& aSynchronzier)
{
	float y = myTextIndex * myBackgroundSprite->GetSize().y;

	RenderCommand command;
	command.myType = eRenderType::eSprite;
	command.mySprite = myBackgroundSprite;
	command.myPosition = { 0, y };
	aSynchronzier.AddRenderCommand(command);

	command.myType = eRenderType::eText;
	command.myText = myTextRender;
	command.myPosition = myTextRender->myPosition;
	aSynchronzier.AddRenderCommand(command);
}

void EventAnswer::Reset()
{

}
