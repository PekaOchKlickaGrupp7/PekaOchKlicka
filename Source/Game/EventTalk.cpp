#include "stdafx.h"
#include "EventTalk.h"


EventTalk::EventTalk() : myTextRender(nullptr)
{

}


EventTalk::~EventTalk()
{
	delete myTextRender;
	myTextRender = nullptr;
}

void EventTalk::Init(Room* aRoom, CGameWorld* aGameWorld)
{
	myTextRender = new DX2D::CText(myFontPath.c_str());
}

bool EventTalk::Update(const float aDeltaTime)
{
	myCurrentTime += aDeltaTime;

	myTextRender->myText = myText;
	myTextRender->myColor = myColor;
	ObjectData* object = GetGameObject(myTarget);

	myTextRender->myPosition = DX2D::Vector2f(object->myX, object->myY);
	myTextRender->mySize = 1;

	if (myCurrentTime >= myShowTime)
	{
		return true;
	}

	return false;
}

void EventTalk::Render(Synchronizer &aSynchronizer)
{
	RenderCommand command;
	command.myType = eRenderType::eText;
	command.myPosition = myTextRender->myPosition;
	command.myText = myTextRender;
	aSynchronizer.AddRenderCommand(command);
}

void EventTalk::Reset()
{
	myCurrentTime = 0.0f;
	myWordCount = 0;
}