#include "stdafx.h"
#include "TextTalk.h"


TextTalk::TextTalk()
{

}

TextTalk::~TextTalk()
{
	SAFE_DELETE(myText);
}

void TextTalk::Init(const char* aPath, DX2D::CColor aColor)
{
	myText = new DX2D::CText(aPath);
	myText->myText = " ";
	myIsTalking = false;
	myWords = 0;
	myColor = aColor;
	myText->myColor = myColor;
}

void TextTalk::Update(const DX2D::Vector2f &aPosition)
{
	myTimerManager.UpdateTimers();
	if (myIsTalking == false)
	{
		return;
	}

	if (myTimerManager.GetTimer(myTimerHandle).GetTime().GetMiliseconds() > (mySpeed * myWords))
	{
		NewSubString();
	}
	myText->myPosition = aPosition;

}

void TextTalk::Render(Synchronizer& aSynchronizer, DX2D::Vector2f aPos)
{
	if (myIsTalking == true)
	{
		RenderCommand command;
		command.myType = eRenderType::eText;
		command.myPosition = myText->myPosition;
		command.myText = myText;
		aSynchronizer.AddRenderCommand(command);
	}
}

DX2D::CText* TextTalk::GetCText() const
{
	return myText;
}

void TextTalk::NewSubString()
{
	std::string::size_type nextWhiteSpace = myString.find(" ", 0);

	if (myWords == 0)
	{
		myText->myText = myString.substr(0, nextWhiteSpace);
		++myWords;
	}
	else
	{
		nextWhiteSpace = myString.find(" ", myText->myText.length() + 1);
		if (nextWhiteSpace != std::string::npos)
		{
			myText->myText = myString.substr(0, nextWhiteSpace);
			++myWords;
		}
		else if (myText->myText.length() < myString.length())
		{
			myText->myText = myString;
			++myWords;
		}
		else
		{
			myString = " ";
			myText->myText = " ";
			myWords = 0;
			myIsTalking = false;
			myTimerManager.StopTimers(myTimerHandle);
			myTimerManager.ResetTimer(myTimerHandle);
		}
	}
}

void TextTalk::StartTalk(const std::string &aString, const float &aTextSize, const unsigned int &aTextSpeed)
{
	myIsTalking = true;
	myString = aString;
	myText->mySize = aTextSize;
	myTimerHandle = myTimerManager.CreateTimer();
	myTimerManager.StartTimers(myTimerHandle);
	mySpeed = aTextSpeed;
}

