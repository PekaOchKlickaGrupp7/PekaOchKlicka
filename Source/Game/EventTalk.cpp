#include "stdafx.h"
#include "EventTalk.h"
#include <tga2d\engine.h>
#include "MouseManager.h"
#include "GameWorld.h"
#include "SoundManager.h"
#include "SoundFileHandler.h"

#define SHAKE_CHAR '@'
#define FONTPATH "Text/PassionOne-Regular.ttf_sdf"
#define TARGETSOUND "Sound/SoundFX/Talk.ogg"

bool EventTalk::myIsActive = false;

EventTalk::EventTalk() : myTextRender(nullptr)
{
	myIsActive = false;
	myLetterLength = 0.1f;
	myCanBeInterupted = true;
	myTextRender = nullptr;
	myTextOutline = nullptr;
}

EventTalk::~EventTalk()
{
	delete myTextRender;
	myTextRender = nullptr;

	delete myTextOutline;
	myTextOutline = nullptr;

	//
	//myTextRenders.DeleteAll();
}

void EventTalk::Init(Room* aRoom, CGameWorld* aGameWorld)
{
	Event::Init(aRoom, aGameWorld);

	myTextRender = new DX2D::CText(FONTPATH);
	myTextRender->myColor = myColor;
	myTextRender->mySize = mySize;

	myTextOutline = new DX2D::CText(FONTPATH);
	myTextOutline->myColor = { 0, 0, 0, 1 }; // Black
	myTextOutline->mySize = mySize;

	myCurrentLetter = 0;
	
	// Work in progrss
	//
	//myShakeStart = 0;
	//myShakeStop = 0;
	//myCurrentIndex = 0;
	//myTextRenders.Init(5);

	Reset();
	myIsTalking = true;

	myHeight = DX2D::CText::GetHeight(myText, myTextRender->mySize, FONTPATH);
	myWidth = DX2D::CText::GetWidth(myText, myTextRender->mySize, FONTPATH);
	myText.erase(std::remove(myText.begin(), myText.end(), '\r'), myText.end());


}

bool EventTalk::Update(const float aDeltaTime)
{
	myCurrentTime += aDeltaTime;
	ObjectData* object = GetGameObject(myTarget);


	if (object->myName == "Player")
	{
		mySoundPath = "Sound/SoundFX/TalkPlayer.ogg";
	}
	else if (object->myName == "Chef")
	{
		mySoundPath = "Sound/SoundFX/TalkChef.ogg";
	}
	

	if (myCanBeInterupted == true)
	{
		myGameWorld->SetTalkIsOn();
		MouseManager::GetInstance()->SetInteractiveMode(eInteractive::eActive);

		if (myFirstFrame == true && myGameWorld->GetOptions()->GetActive() == false && MouseManager::GetInstance()->ButtonClicked(eMouseButtons::eLeft) == true)
		{
			if (myCurrentLetter == myText.size() + 1)
			{
				myGameWorld->SetTalkIsOff();
				MouseManager::GetInstance()->SetInteractiveMode(eInteractive::eRegular);

				return true;
			}
			else
			{
				myCurrentLetter = myText.size() + 1;
				myTextRender->myText = myText;
				myShowedTime = -1;
			}
		}
		myFirstFrame = true;
	}


	if (object != nullptr)
	{
		if (myObjectData != nullptr)
		{
			if (myObjectData->myActive == false || object->myActive == false)
			{
				return true;
			}
		}
		else if (object->myActive == false)
		{
			return true;
		}
		float x = object->myGlobalX - myWidth / 2;
		float y = object->myGlobalY - (myHeight);

		if (object->mySprite != nullptr)
		{
			y = object->myGlobalY - (myHeight + (object->mySprite->GetSize().y / 2));
		}
		else if (object->myName == "Player")
		{
			y = object->myGlobalY - myHeight;
		}

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

		if (y < 0.0f || (y + myHeight) > 1.0f)
		{
			if ((y + myHeight) > 1.0f)
			{
				y = (1.0f - myHeight) - 0.01f;
			}
			else
			{
				y = 0.01f;
			}
		}

		myTextRender->myPosition = DX2D::Vector2f(x, y);
		
		//work in progress
		//
		//myTextRenders[0]->myPosition = DX2D::Vector2f(x, y);
		//for (unsigned int i = 1; i < myTextRenders.Size(); ++i)
		//{
		//	myTextRenders[i]->myPosition = { myTextRenders[i]->myPosition.x + myTextRenders[i]->GetWidth(), y };
		//}



		if (myCurrentLetter <= myText.size() && myCurrentTime > myLetterLength * myCurrentLetter)
		{
			return TypeNextLetter(aDeltaTime);
		}
		else if (myCurrentLetter >= myText.size() - 1)
		{
			myShowedTime += aDeltaTime;
			if (myShowedTime > myShowTime)
			{
				if (myCanBeInterupted == true)
				{
					myGameWorld->SetTalkIsOff();
					MouseManager::GetInstance()->SetInteractiveMode(eInteractive::eRegular);
				}
				return true;
			}
		}
	}
	else
	{
		if (myCanBeInterupted == true)
		{
			myGameWorld->SetTalkIsOff();
			MouseManager::GetInstance()->SetInteractiveMode(eInteractive::eRegular);
		}
		return true;
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

bool EventTalk::TypeNextLetter(float)
{
	if (myCurrentLetter <= myText.size())
	{


		if (myCurrentLetter % 3 == 0 && myText[myCurrentLetter] != ' ')
		{
			if (myCurrentIndex != 0)
			{
				std::string pastIdentifier = "text" + std::to_string(myCurrentLetter - 1);
				SoundFileHandler::GetInstance()->SetupStream(mySoundPath, pastIdentifier, false);
				Sound* SoundPtr = SoundFileHandler::GetInstance()->GetSound(pastIdentifier);
				SoundPtr->Stop();
			}
			
			std::string identifier = "text" + std::to_string(myCurrentLetter);
			SoundFileHandler::GetInstance()->SetupStream(mySoundPath, identifier, false);
			Sound* SoundPtr = SoundFileHandler::GetInstance()->GetSound(identifier);

			SoundPtr->PlaySound();

			float random = ((float)rand()) / (float)RAND_MAX;
			float diff = 1.25f - 0.75f;
			float r = random * diff;
			float pitch = 0.75f + r;

			SoundPtr->SetPitch(pitch);
			SoundPtr->SetVolume(0.5f);
		}

		myTextRender->myText = myText.substr(0, myCurrentLetter);
		++myCurrentLetter;
		return false;
	}

	return false;
}

//void EventTalk::CutUpString()
//{
//	// work in progress
//
//
//
//
//	std::size_t start = 0;
//	std::size_t found = myText.find('@', start);
//	std::size_t stop = found;
//
//	if (found != std::string::npos)
//	{
//		std::string subString = myText.substr(start, found);
//
//		DX2D::CText* textRender = new DX2D::CText(FONTPATH);
//		textRender->myColor = myColor;
//		textRender->mySize = mySize;
//		textRender->myText = subString;
//
//		myTextRenders.Add(textRender);
//
//		start = found + 1;
//		stop = myText.find('@', start);
//
//
//
//		++myShakeStart;
//		myShakeStop = myShakeStart;
//	
//		subString = myText.substr(start, stop);
//
//		for (std::size_t i = 0; i < subString.size(); ++i)
//		{
//			++myShakeStop;
//
//			DX2D::CText* textRender = new DX2D::CText(FONTPATH);
//			textRender->myColor = myColor;
//			textRender->mySize = mySize;
//			textRender->myText = subString[i];
//
//			myTextRenders.Add(textRender);
//		}
//
//		start = stop;
//
//		subString = myText.substr(start, myText.size());
//
//		DX2D::CText* textRender = new DX2D::CText(FONTPATH);
//		textRender->myColor = myColor;
//		textRender->mySize = mySize;
//		textRender->myText = subString;
//
//		myTextRenders.Add(textRender);
//
//		myText.erase(std::remove(myText.begin(), myText.end(), '@'), myText.end());
//	}
//
//	DX2D::CText* textRender = new DX2D::CText(FONTPATH);
//	textRender->myColor = myColor;
//	textRender->mySize = mySize;
//	textRender->myText = myText;
//
//	myTextRenders.Add(textRender);
//
//}

void EventTalk::Reset()
{
	myFirstFrame = false;
	myShowedTime = 0.0f;
	myCurrentTime = 0.0f;
	myCurrentLetter = 0;
	myTextRender->myText = " ";
}