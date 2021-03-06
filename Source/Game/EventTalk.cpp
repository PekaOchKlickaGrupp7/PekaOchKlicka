#include "stdafx.h"
#include "EventTalk.h"
#include <tga2d\engine.h>
#include "MouseManager.h"
#include "GameWorld.h"
#include "SoundManager.h"
#include "SoundFileHandler.h"
#include <iostream>

#define SHAKE_CHAR '@'
#define FONTPATH "Text/PassionOne-Regular.ttf_sdf"
#define TARGETSOUND "Sound/SoundFX/Talk.ogg"

bool EventTalk::myIsActive = false;
volatile int EventTalk::myCountActive = 0;

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
	
	mySounds = 0;

	Reset();
	myIsTalking = true;

	myHeight = DX2D::CText::GetHeight(myText, myTextRender->mySize, FONTPATH);
	myWidth = DX2D::CText::GetWidth(myText, myTextRender->mySize, FONTPATH);
	myText.erase(std::remove(myText.begin(), myText.end(), '\r'), myText.end());

	myIdentifier = "EMPTY";
}

bool EventTalk::Update(const float aDeltaTime)
{
	if (myCanBeInterupted == true && myCached == false)
	{
		if (myCountActive > 0)
		{
			return true;
		}
		++myCountActive;
	}

	myCurrentTime += aDeltaTime;
	ObjectData* object = GetGameObject(myTarget);

	if (myCached == false)
	{
		myCached = true;
		myMouseIsInvisible = MouseManager::GetInstance()->GetHideGameMouse();
	}

	if (myIdentifier == "EMPTY")
	{
		if (object->myName == "Chef" || object->myName == "Giraff" || object->myName == "giraffTarget")
		{
			mySoundPath = "Sound/SoundFX/Talk2.ogg";
			myIdentifier = "Chef";
		}
		else if (object->myName == "Antagonist")
		{
			myIdentifier = "Antagonist";
			mySoundPath = "Sound/SoundFX/Talk1.ogg";
		}
		else if (object->myName == "Player")
		{
			myIdentifier = "Player";
			mySoundPath = "Sound/SoundFX/Talk3.ogg";
		}
		else if (object->myName == "RightGuard" || object->myName == "LeftGuard" ||
			object->myName == "talkObjectRightGuard" || object->myName == "talkobject" ||
			object->myName == "GuardL" || object->myName == "Waiter" || object->myName == "CInematicTextPos")
		{
			myIdentifier = "Guard";
			mySoundPath = "Sound/SoundFX/Talk5.ogg";
		}
		else if (object->myName == "SnotKidInBedroom" || object->myName == "SnotKid")
		{
			myIdentifier = "SnotKid";
			mySoundPath = "Sound/SoundFX/Talk7.ogg";
		}
		else if (object->myName == "targetPointLady" || object->myName == "fancyLady" || object->myName == "talkTarget")
		{
			myIdentifier = "FancyLady";
			mySoundPath = "Sound/SoundFX/Talk4.ogg";
		}
	}
	if (myCanBeInterupted == true)
	{
		myGameWorld->SetTalkIsOn();
		MouseManager::GetInstance()->SetHideGameMouse(true);

		if (myFirstFrame == true && myGameWorld->GetOptions()->GetActive() == false && MouseManager::GetInstance()->ButtonClicked(eMouseButtons::eLeft) == true)
		{
			if (myCurrentLetter == myText.size() + 1)
			{
				myGameWorld->SetTalkIsOff();
				MouseManager::GetInstance()->SetHideGameMouse(myMouseIsInvisible);

				--myCountActive;

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

		if (x < 0.03f || (x + myWidth) >= (1.0f - 0.03f))
		{
			if ((x + myWidth) > (1.0f - 0.03f))
			{
				x = (1.0f - myWidth) - 0.03f;
			}
			else
			{
				x = 0.03f;
			}
		}

		if (y < 0.03f || (y + myHeight) > (1.0f - 0.03f))
		{
			if ((y + myHeight) > (1.0f - 0.03f))
			{
				y = (1.0f - myHeight) - 0.03f;
			}
			else
			{
				y = 0.03f;

			}
		}

		myTextRender->myPosition = DX2D::Vector2f(x, y);

		if (myCurrentLetter <= myText.size() && myCurrentTime > myLetterLength * myCurrentLetter)
		{
			return TypeNextLetter(aDeltaTime);
		}
		else if (myCurrentLetter >= myText.size() - 1)
		{
			myShowedTime += aDeltaTime;
			if (myShowedTime > myShowTime + 1.5f)
			{
				if (myCanBeInterupted == true)
				{
					myGameWorld->SetTalkIsOff();
					MouseManager::GetInstance()->SetHideGameMouse(myMouseIsInvisible);
					--myCountActive;
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
			MouseManager::GetInstance()->SetHideGameMouse(myMouseIsInvisible);
			--myCountActive;
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
			SoundFileHandler::GetInstance()->SetupStream(mySoundPath, myIdentifier, false);
			Sound* SoundPtr = SoundFileHandler::GetInstance()->GetSound(myIdentifier);
			
			SoundPtr->Stop();
			SoundPtr->PlaySound();
			++mySounds;

			float random = ((float)rand()) / (float)RAND_MAX;
			float diff = 1.75f - 0.75f;
			float r = random * diff;
			float pitch = 0.75f + r;

			SoundPtr->SetPitch(pitch);
			SoundPtr->SetVolume(0.4f);
		}

		myTextRender->myText = myText.substr(0, myCurrentLetter);
		++myCurrentLetter;
		return false;
	}

	return false;
}

void EventTalk::Reset()
{
	myFirstFrame = false;
	myCached = false;
	myShowedTime = 0.0f;
	myCurrentTime = 0.0f;
	myCurrentLetter = 0;
	myTextRender->myText = " ";
	myIdentifier = "EMPTY";
}