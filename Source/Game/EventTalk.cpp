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


	myLetterLength = 0.1f;
	myCurrentLetter = 0;

	Reset();
	myIsTalking = true;

	myHeight = DX2D::CText::GetHeight(myText, myTextRender->mySize, myFontPath.c_str());
	myWidth = DX2D::CText::GetWidth(myText, myTextRender->mySize, myFontPath.c_str());
	myText.erase(std::remove(myText.begin(), myText.end(), '\r'), myText.end());
}

bool EventTalk::Update(const float aDeltaTime)
{
	myCurrentTime += aDeltaTime;
	ObjectData* object = GetGameObject(myTarget);

	if (object != nullptr)
	{
		float x = object->myGlobalX - myWidth / 2;
		float y = object->myGlobalY - myHeight;

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

		myTextRender->myPosition = DX2D::Vector2f(x, y);

		if (myCurrentTime > myLetterLength * myCurrentLetter)
		{
			return TypeNextLetter();
		}
	}
	return false;
}

void EventTalk::Render(Synchronizer &aSynchronizer)
{
	float offsetX = 1.0f / 1920.0f;
	float offsetY = 1.0f / 1080.0f;

	RenderCommand command;
	DX2D::Vector2f outlinePos = myTextRender->myPosition;

	myTextOutline->myText = myTextRender->myText;

	command.myType = eRenderType::eText;
	command.myText = myTextOutline;
	//outline left
	command.myPosition = { outlinePos.x - offsetX, outlinePos.y };
	aSynchronizer.AddRenderCommand(command);

	//outline right
	command.myPosition = { outlinePos.x + offsetX, outlinePos.y };
	aSynchronizer.AddRenderCommand(command);

	//outline top
	command.myPosition = { outlinePos.x, outlinePos.y - offsetY };
	aSynchronizer.AddRenderCommand(command);

	//outline bottom
	command.myPosition = { outlinePos.x, outlinePos.y + offsetY };
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

	if (myCurrentTime > myShowTime)
	{
		return true;
	}
	return false;
}

void EventTalk::Reset()
{
	myCurrentTime = 0.0f;
	myCurrentLetter = 0;
	myTextRender->myText = " ";
}