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
	myTextRender->myColor = myColor;
	myTextRender->mySize = mySize;
	Reset();
	myIsTalking = true;
}

bool EventTalk::Update(const float aDeltaTime)
{
	myCurrentTime += aDeltaTime;
	ObjectData* object = GetGameObject(myTarget);

	myTextRender->myPosition = DX2D::Vector2f(object->myX, object->myY);
	myTextRender->mySize = 1;

	if (myCurrentTime > myShowTime * myWordCount)
	{
		return NewSubString();
	}

	return false;
}

void EventTalk::Render(Synchronizer &aSynchronizer)
{
	RenderCommand command;
	command.myType = eRenderType::eText;
	command.myPosition = myTextRender->myPosition;
	command.myText = myTextRender;
	command.myText->mySize = mySize;
	aSynchronizer.AddRenderCommand(command);
}

bool EventTalk::NewSubString()
{
	std::string::size_type nextWhiteSpace = myText.find(" ", 0);

	if (myWordCount == 0)
	{
		myTextRender->myText = myText.substr(0, nextWhiteSpace);
		++myWordCount;
		return false;
	}
	else
	{
		nextWhiteSpace = myText.find(" ", myTextRender->myText.length() + 1);
		if (nextWhiteSpace != std::string::npos)
		{
			myTextRender->myText = myText.substr(0, nextWhiteSpace);
			++myWordCount;
			return false;
		}
		else if (myTextRender->myText.length() < myText.length())
		{
			myTextRender->myText = myText;
			++myWordCount;
			return false;
		}
		else
		{
			myTextRender->myText = " ";
			myWordCount = 0;
			myIsTalking = false;
			return true;
		}
	}
}

void EventTalk::Reset()
{
	myCurrentTime = 0.0f;
	myWordCount = 0;
	myTextRender->myText = " ";
}