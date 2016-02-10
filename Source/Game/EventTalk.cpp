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
	myLetterLenght = 0.1f;
	myCurrentLetter = 0;

	Reset();
	myIsTalking = true;


	myWidth = DX2D::CText::GetWidth(myText, myTextRender->mySize, myFontPath.c_str());
	myWidth = 0;
	myText.erase(std::remove(myText.begin(), myText.end(), '\r'), myText.end());
}

bool EventTalk::Update(const float aDeltaTime)
{
	myCurrentTime += aDeltaTime;
	ObjectData* object = GetGameObject(myTarget);

	if (object != nullptr)
	{
		float x = object->myX - myWidth / 2;

		if (x < 0.0f || (x + myWidth) >= 1.0f)
		{
			if ((x + myWidth) > 1.0f)
			{
				x = (1.0f - myWidth) - 0.01f;
			}
			else
			{
				x = 0.01f;
			}
		}

		myTextRender->myPosition = DX2D::Vector2f(x, object->myY);

		if (myCurrentTime > myLetterLenght * myCurrentLetter)// * aDeltaTime)
		{
			return TypeNextLetter();
			
		}
	}
	return false;
}

void EventTalk::Render(Synchronizer &aSynchronizer)
{
	float offset = 1.0f / 1080.0f;

	RenderCommand command;
	DX2D::Vector2f outlinePos = myTextRender->myPosition;

	command.myType = eRenderType::eText;
	command.myText = myTextOutline;
	//outline left
	command.myPosition = { outlinePos.x - offset, outlinePos.y };
	aSynchronizer.AddRenderCommand(command);

	//outline right
	command.myPosition = { outlinePos.x + offset, outlinePos.y };
	aSynchronizer.AddRenderCommand(command);

	//outline top
	command.myPosition = { outlinePos.x, outlinePos.y - offset };
	aSynchronizer.AddRenderCommand(command);

	//outline bottom
	command.myPosition = { outlinePos.x, outlinePos.y + offset };
	aSynchronizer.AddRenderCommand(command);

	command.myText = myTextRender;
	command.myPosition = myTextRender->myPosition;
	aSynchronizer.AddRenderCommand(command);
}

bool EventTalk::TypeNextLetter()
{
	if (myCurrentLetter <= myText.size())
	{
		myTextRender->myText = myText.substr(0, myCurrentLetter);
		++myCurrentLetter;
		return false;
	}
	else
	{
		if (myCurrentTime > myShowTime)
		{
			return true;
		}
	}
	return true;
}

void EventTalk::Reset()
{
	myCurrentTime = 0.0f;
	myWordCount = 0;
	myTextRender->myText = " ";
}