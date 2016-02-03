#include "stdafx.h"
#include "EventTalk.h"
#include <tga2d\engine.h>

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
	Event::Init(aRoom, aGameWorld);

	myTextRender = new DX2D::CText(myFontPath.c_str());
	myTextRender->myColor = myColor;
	myTextRender->mySize = mySize;
	Reset();
	myIsTalking = true;


	myWidth = DX2D::CText::GetWidth(myText, myTextRender->mySize, myFontPath.c_str());
	myText.erase(std::remove(myText.begin(), myText.end(), '\r'), myText.end());
}

bool EventTalk::Update(const float aDeltaTime)
{
	myCurrentTime += aDeltaTime;
	ObjectData* object = GetGameObject(myTarget);

	if (object != nullptr)
	{
		float x = object->myX;
		
		myTextRender->myPosition = DX2D::Vector2f(x, object->myY);

		if (myCurrentTime > myWordLength * myWordCount)
		{
			return NewSubString();
		}
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
			if (myCurrentTime >= myShowTime)
			{
				myTextRender->myText = " ";
				myWordCount = 0;
				myIsTalking = false;
				return true;
			}
			return false;
		}
	}
}

void EventTalk::Reset()
{
	myCurrentTime = 0.0f;
	myWordCount = 0;
	myTextRender->myText = " ";
}