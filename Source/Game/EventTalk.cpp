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

	delete myTextOutline;
	myTextOutline = nullptr;
}

void EventTalk::Init(Room* aRoom, CGameWorld* aGameWorld)
{
	Event::Init(aRoom, aGameWorld);

	myTextRender = new DX2D::CText(myFontPath.c_str());
	myTextRender->myColor = myColor;
	myTextRender->mySize = mySize;

	myTextOutline = new DX2D::CText(myFontPath.c_str());
	myTextOutline->myColor = { 0, 0, 0, 1 }; // Black
	myTextOutline->mySize = mySize;

	Reset();
	myIsTalking = true;


//	myWidth = DX2D::CText::GetWidth(myText, myTextRender->mySize, myFontPath.c_str());
	myWidth = 0;
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
	float onePixel = 0.002f;

	RenderCommand command;
	DX2D::Vector2f outlinePos = myTextRender->myPosition;

	command.myType = eRenderType::eText;
	command.myText = myTextOutline;
	//outline left
	command.myPosition = { outlinePos.x - onePixel, outlinePos.y };
	aSynchronizer.AddRenderCommand(command);

	//outline right
	command.myPosition = { outlinePos.x + onePixel, outlinePos.y };
	aSynchronizer.AddRenderCommand(command);

	//outline top
	command.myPosition = { outlinePos.x, outlinePos.y - onePixel };
	aSynchronizer.AddRenderCommand(command);

	//outline bottom
	command.myPosition = { outlinePos.x, outlinePos.y + onePixel };
	aSynchronizer.AddRenderCommand(command);

	command.myText = myTextRender;
	command.myPosition = myTextRender->myPosition;
	aSynchronizer.AddRenderCommand(command);
}

bool EventTalk::NewSubString()
{
	std::string::size_type nextWhiteSpace = myText.find(" ", 0);

	if (myWordCount == 0)
	{
		myTextRender->myText = myText.substr(0, nextWhiteSpace);
		myTextOutline->myText = myTextRender->myText;

		++myWordCount;
		return false;
	}
	else
	{
		nextWhiteSpace = myText.find(" ", myTextRender->myText.length() + 1);
		if (nextWhiteSpace != std::string::npos)
		{
			myTextRender->myText = myText.substr(0, nextWhiteSpace);
			myTextOutline->myText = myTextRender->myText;

			++myWordCount;
			return false;
		}
		else if (myTextRender->myText.length() < myText.length())
		{
			myTextRender->myText = myText;
			myTextOutline->myText = myTextRender->myText;

			++myWordCount;
			return false;
		}
		else
		{
			if (myCurrentTime >= myShowTime)
			{
				myTextRender->myText = " ";
				myTextOutline->myText = myTextRender->myText;

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