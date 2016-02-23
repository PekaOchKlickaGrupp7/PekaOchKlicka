#include "stdafx.h"
#include "EventsFactory.h"
#include "..\CommonUtilities\DL_Debug.h"

//Events
#include "EventNone.h"
#include "EventSetActive.h"
#include "EventChangeLevel.h"
#include "EventTalk.h"
#include "EventChangeCursor.h"
#include "EventPlaySound.h"
#include "EventChangeImage.h"
#include "EventDelay.h"
#include "EventChangeToOriginalImage.h"
#include "EventStopSound.h"
#include "EventChangeSoundPosition.h"
#include "EventQuit.h"
#include "EventIfGlobalVariable.h"
#include "EventFadeColor.h"
#include "EventSetColor.h"
#include "EventSetPosition.h"
#include "EventFadePosition.h"
#include "EventToggleFullscreen.h"
#include "EventSetGlobalVariable.h"
#include "EventWalkTo.h"
#include "EventHideMouse.h"
#include "EventSetCinematic.h"
#include "EventPickupItem.h"
#include "EventIsItem.h"
#include "EventChangePlayerDirection.h"
#include "EventRemoveSelectedItem.h"
#include "EventDeselect.h"
#include "EventItemIsNot.h"
#include "EventResetGame.h"
#include "EventRandom.h"
#include "EventAnswer.h"
#include "EventIfNotGlobalVariable.h"
#include "EventCreateAnimation.h"
#include "EventUpdateAnimation.h"
#include "EventParticleSystem.h"

using namespace rapidjson;

EventsFactory::EventsFactory()
{
}

EventsFactory::~EventsFactory()
{
}

Event* EventsFactory::CreateEventData(ObjectData* aData, Value& aParent, Room* aRoom, CGameWorld* aGameWorld) const
{
	EventActions action = static_cast<EventActions>(aParent["action"].GetInt());
	Event* event = nullptr;
	Value& extra = aParent["extra"];
	switch (action)
	{
	case EventActions::CreateParticleSystem:
	{

		break;
	}
	case EventActions::SetActive:
	{
		EventSetActive* setActive = new EventSetActive();
		setActive->Init(aRoom, aGameWorld);

		if (extra.HasMember("value") == true)
		{
			Value& myValue = extra["value"];
			if (myValue.IsNull() == true)
			{
				DL_ASSERT("Event SetActive Value is null");
			}
			setActive->myValue = myValue.GetBool();
		}

		event = setActive;
		break;
	}
	case EventActions::SetPosition:
	{
		EventSetPosition* var = new EventSetPosition();
		var->Init(aRoom, aGameWorld);

		if (extra.HasMember("TargetPositionX") == true && extra.HasMember("TargetPositionY") == true)
		{
			var->myTargetOffset = { extra["TargetPositionX"].GetFloat() / 1920.0f, extra["TargetPositionY"].GetFloat() / 1080.0f };
		}

		event = var;
		break;
	}
	case EventActions::ChangeLevel:
	{
		EventChangeLevel* changeLevel = new EventChangeLevel();
		changeLevel->Init(aRoom, aGameWorld);

		if (extra.HasMember("TargetScene") == true)
		{
			Value& myValue = extra["TargetScene"];
			if (myValue.IsNull() == true)
			{
				DL_ASSERT("Event Change Level Value is null");
			}
			changeLevel->myTargetLevelName = myValue.GetString();
			changeLevel->myTargetPosition = DX2D::Vector2f(static_cast<float>(extra["x"].GetDouble()) / 1920.0f, static_cast<float>(extra["y"].GetDouble()) / 1080.0f);
		}
		if (extra.HasMember("UseFading") == true)
		{
			changeLevel->myUseFading = extra["UseFading"].GetBool();
		}
		if (extra.HasMember("NextTheme") == true)
		{
			changeLevel->myNextTheme = extra["NextTheme"].GetString();
		}
		if (extra.HasMember("PlayerDirection") == true)
		{
			changeLevel->myPlayerDirection = extra["PlayerDirection"].GetInt() - 1;
		}

		event = changeLevel;
		break;
	}
	case EventActions::ChangePlayerDirection:
	{
		EventChangePlayerDirection* changePlayerDirection = new EventChangePlayerDirection();

		if (extra.HasMember("PlayerDirection") == true)
		{
			changePlayerDirection->myPlayerDirection = extra["PlayerDirection"].GetInt() - 1;
		}

		changePlayerDirection->Init(aRoom, aGameWorld);

		event = changePlayerDirection;
		break;
	}
	case EventActions::Talk:
	{
		EventTalk* talk = new EventTalk();

		if (extra.HasMember("CanBeInterupted") == true)
		{
			talk->myCanBeInterupted = extra["CanBeInterupted"].GetBool();
		}
		if (extra.HasMember("text") == true)
		{
			talk->myText = extra["text"].GetString();
		}
		if (extra.HasMember("length") == true)
		{
			talk->myShowTime = static_cast<float>(extra["length"].GetDouble());
		}
		if (extra.HasMember("wordLength") == true)
		{
			talk->myLetterLength = static_cast<float>(extra["wordLength"].GetDouble());
		}
		if (extra.HasMember("color") == true)
		{
			Value &colorVal = extra["color"];
			talk->myColor = DX2D::CColor(static_cast<float>(colorVal["r"].GetDouble()),
				static_cast<float>(colorVal["g"].GetDouble()),
				static_cast<float>(colorVal["b"].GetDouble()),
				static_cast<float>(colorVal["a"].GetDouble()));
		}
		if (extra.HasMember("size") == true)
		{
			talk->mySize = static_cast<float>(extra["size"].GetDouble());
		}

		talk->Init(aRoom, aGameWorld);

		event = talk;
		break;
	}
	case EventActions::ChangeCursor:
	{
		EventChangeCursor* changeCursor = new EventChangeCursor();
		if (extra.HasMember("cursor") == true)
		{
			changeCursor->myTargetCursor = extra["cursor"].GetInt();
		}

		changeCursor->Init(aRoom, aGameWorld);

		event = changeCursor;
		break;
	}
	case EventActions::PlaySoundFile:
	{
		EventPlaySound* sound = new EventPlaySound();
		if (extra.HasMember("path") == true)
		{
			sound->myTargetSound = extra["path"].GetString();
		}
		if (extra.HasMember("SoundName") == true)
		{
			sound->myIdentifier = extra["SoundName"].GetString();
		}
		if (extra.HasMember("volume") == true)
		{
			sound->myVolume = static_cast<float>(extra["volume"].GetDouble());
		}
		if (extra.HasMember("looping") == true)
		{
			sound->myIsLooping = extra["looping"].GetBool();
		}
		if (extra.HasMember("is3D") == true)
		{
			sound->myIs3D = extra["is3D"].GetBool();
		}
		if (extra.HasMember("positionX") == true && extra.HasMember("positionY") == true)
		{
			sound->myPosition = DX2D::Vector2f(static_cast<float>(extra["positionX"].GetDouble()) / 1920.0f, static_cast<float>(extra["positionY"].GetDouble()) / 1080.0f);
		}

		sound->Init(aRoom, aGameWorld);

		event = sound;
		break;
	}
	case EventActions::ChangeImage:
	{
		EventChangeImage* changeImage = new EventChangeImage();
		if (extra.HasMember("image") == true)
		{
			changeImage->myImagePath = extra["image"].GetString();
		}

		changeImage->Init(aRoom, aGameWorld);

		event = changeImage;
		break;
	}
	case EventActions::Delay:
	{
		EventDelay* delay = new EventDelay();
		if (extra.HasMember("delay") == true)
		{
			delay->myDelay = static_cast<float>(extra["delay"].GetDouble());
		}
		delay->Init(aRoom, aGameWorld);

		event = delay;
		break;
	}
	case EventActions::ChangeToOriginalImage:
	{
		EventChangeToOriginalImage* changeToOriginalImage = new EventChangeToOriginalImage();

		changeToOriginalImage->Init(aRoom, aGameWorld);

		event = changeToOriginalImage;
		break;
	}
	case EventActions::StopSound:
	{
		EventStopSound* stopSoundEvent = new EventStopSound();

		if (extra.HasMember("SoundName") == true)
		{
			stopSoundEvent->myTargetSound = extra["SoundName"].GetString();
		}

		stopSoundEvent->Init(aRoom, aGameWorld);
		event = stopSoundEvent;
		break;
	}
	case EventActions::ChangeSoundPosition:
	{
		EventChangeSoundPosition* changePositionEvent = new EventChangeSoundPosition();

		if (extra.HasMember("offsetPositionX") == true)
		{
			changePositionEvent->myPosition.x = static_cast<float>(extra["offsetPositionX"].GetDouble());
		}
		if (extra.HasMember("offsetPositionY") == true)
		{
			changePositionEvent->myPosition.y = static_cast<float>(extra["offsetPositionY"].GetDouble());
		}

		changePositionEvent->Init(aRoom, aGameWorld);
		event = changePositionEvent;
		break;
	}
	case EventActions::Quit:
	{
		EventQuit* quitEvent = new EventQuit();

		quitEvent->Init(aRoom, aGameWorld);

		event = quitEvent;
		break;
	}
	case EventActions::SetGlobalVariable:
	{
		EventSetGlobalVariable* var = new EventSetGlobalVariable();

		if (extra.HasMember("Type") == true && extra.HasMember("VariableName") == true && extra.HasMember("VariableValue") == true)
		{
			var->myVariableType = static_cast<IfVariableType>(extra["Type"].GetInt());
			var->myVariable = extra["VariableName"].GetString();
			var->myVariableValue = extra["VariableValue"].GetString();
		}

		var->Init(aRoom, aGameWorld);

		event = var;
		break;
	}
	case EventActions::IfGlobalVariable:
	{
		EventIfGlobalVariable* var = new EventIfGlobalVariable();

		if (extra.HasMember("Type") == true && extra.HasMember("VariableName") == true && extra.HasMember("VariableValue") == true)
		{
			var->myVariableType = static_cast<IfVariableType>(extra["Type"].GetInt());
			var->myVariable = extra["VariableName"].GetString();
			var->myVariableValue = extra["VariableValue"].GetString();
		}

		var->Init(aRoom, aGameWorld);

		event = var;
		break;
	}
	case EventActions::IfNotGlobalVariable:
	{
		EventIfNotGlobalVariable* var = new EventIfNotGlobalVariable();

		if (extra.HasMember("Type") == true && extra.HasMember("VariableName") == true && extra.HasMember("VariableValue") == true)
		{
			var->myVariableType = static_cast<IfVariableType>(extra["Type"].GetInt());
			var->myVariable = extra["VariableName"].GetString();
			var->myVariableValue = extra["VariableValue"].GetString();
		}

		var->Init(aRoom, aGameWorld);

		event = var;
		break;
	}
	case EventActions::FadeColor:
	{
		EventFadeColor* var = new EventFadeColor();

		if (extra.HasMember("Duration") == true && extra.HasMember("TargetColor") == true)
		{
			var->myFadeTime = static_cast<float>(extra["Duration"].GetDouble());
			Value& value = extra["TargetColor"];
			var->myTargetColor.myR = static_cast<float>(value["r"].GetDouble());
			var->myTargetColor.myG = static_cast<float>(value["g"].GetDouble());
			var->myTargetColor.myB = static_cast<float>(value["b"].GetDouble());
			var->myTargetColor.myA = static_cast<float>(value["a"].GetDouble());
		}
		var->Init(aRoom, aGameWorld);

		event = var;
		break;
	}
	case EventActions::SetColor:
	{
		EventSetColor* var = new EventSetColor();

		if (extra.HasMember("TargetColor") == true)
		{
			Value& value = extra["TargetColor"];
			var->myTargetColor.myR = static_cast<float>(value["r"].GetDouble());
			var->myTargetColor.myG = static_cast<float>(value["g"].GetDouble());
			var->myTargetColor.myB = static_cast<float>(value["b"].GetDouble());
			var->myTargetColor.myA = static_cast<float>(value["a"].GetDouble());
		}
		var->Init(aRoom, aGameWorld);

		event = var;
		break;
	}
		/*case EventActions::SetVariable:
		{
		EventSetVariable* var = new EventSetVariable();

		var->Init(aRoom, aGameWorld);

		event = var;
		break;
		}*/
	case EventActions::FadePosition:
	{
		EventFadePosition* var = new EventFadePosition();

		if (extra.HasMember("Duration") == true && extra.HasMember("TargetPositionX") == true && extra.HasMember("TargetPositionY") == true)
		{
			var->myDuration = static_cast<float>(extra["Duration"].GetDouble());
			var->myTargetOffset = { static_cast<float>(extra["TargetPositionX"].GetDouble()) / 1920.0f, static_cast<float>(extra["TargetPositionY"].GetDouble()) / 1080.0f };
		}

		var->Init(aRoom, aGameWorld);

		event = var;
		break;
	}
	case EventActions::ToggleFullscreen:
	{
		EventToggleFullscreen* var = new EventToggleFullscreen();

		var->Init(aRoom, aGameWorld);

		event = var;
		break;
	}
	case EventActions::WalkTo:
	{
		EventWalkTo* var = new EventWalkTo();

		var->Init(aRoom, aGameWorld);

		event = var;
		break;
	}
	case EventActions::HideMouse:
	{
		EventHideMouse* var = new EventHideMouse();

		if (extra.HasMember("hide") == true)
		{
			var->myHideGameMouse = extra["hide"].GetBool();
		}

		var->Init(aRoom, aGameWorld);

		event = var;
		break;
	}
	case EventActions::SetCinematic:
	{
		EventSetCinematic* var = new EventSetCinematic();

		if (extra.HasMember("on") == true)
		{
			var->mySetOn = extra["on"].GetBool();
		}

		var->Init(aRoom, aGameWorld);

		event = var;
		break;
	}
	case EventActions::PickupItem:
	{
		EventPickupItem* var = new EventPickupItem();

		if (extra.HasMember("name") == true)
		{
			var->myItem = extra["name"].GetString();
		}

		var->Init(aRoom, aGameWorld);

		event = var;
		break;
	}
	case EventActions::IsItem:
	{
		EventIsItem* var = new EventIsItem();

		if (extra.HasMember("item") == true)
		{
			var->myItemName = extra["item"].GetString();
		}

		var->Init(aRoom, aGameWorld);

		event = var;
		break;
	}
	case EventActions::ResetGame:
	{
		EventResetGame* var = new EventResetGame();

		if (extra.HasMember("TargetScene") == true)
		{
			var->myTargetScene = extra["TargetScene"].GetString();
		}

		var->Init(aRoom, aGameWorld);

		event = var;
		break;
	}
	case EventActions::RemoveSelectedItem:
	{
		EventRemoveSelectedItem* var = new EventRemoveSelectedItem();
		var->Init(aRoom, aGameWorld);

		event = var;
		break;
	}
	case EventActions::Deselect:
	{
		EventDeselect* var = new EventDeselect();
		var->Init(aRoom, aGameWorld);

		event = var;
		break;
	}
	case EventActions::ItemIsNot:
	{
		EventItemIsNot* var = new EventItemIsNot();

		if (extra.HasMember("item") == true)
		{
			var->myItemName = extra["item"].GetString();
		}

		var->Init(aRoom, aGameWorld);

		event = var;
		break;
	}
	case EventActions::Random:
	{
		EventRandom* var = new EventRandom();

		if (extra.HasMember("Factor") == true)
		{
			var->myRandomFactor = extra["Factor"].GetInt();
		}

		var->Init(aRoom, aGameWorld);

		event = var;
		break;
	}
	case EventActions::Answer:
	{
		EventAnswer* var = new EventAnswer();

		if (extra.HasMember("Index") == true)
		{
			var->myTextIndex = extra["Index"].GetInt() - 1;
		}
		if (extra.HasMember("Text") == true)
		{
			var->myText = extra["Text"].GetString();
		}
		if (extra.HasMember("Color") == true)
		{
			var->myColor = {
				extra["Color"]["r"].GetFloat(),
				extra["Color"]["g"].GetFloat(),
				extra["Color"]["b"].GetFloat(),
				extra["Color"]["a"].GetFloat()
			};
		}

		var->Init(aRoom, aGameWorld);

		event = var;
		break;
	}
	case EventActions::CreateAnimation:
	{
		EventCreateAnimation* var = new EventCreateAnimation();

		if (extra.HasMember("FilePath") == true)
		{
			var->myFilePath = extra["FilePath"].GetString();
		}
		if (extra.HasMember("FrameDuration") == true)
		{
			var->myFrameDuration = extra["FrameDuration"].GetFloat();
		}
		if (extra.HasMember("FramesPerRow") == true)
		{
			var->myFramesPerRow = extra["FramesPerRow"].GetInt();
		}
		if (extra.HasMember("NumberOfFrames") == true)
		{
			var->myNumberOfFrames = extra["NumberOfFrames"].GetInt();
		}
		if (extra.HasMember("Scale") == true)
		{
			var->myScale = extra["Scale"].GetFloat();
		}
		if (extra.HasMember("Flip") == true)
		{
			var->myFlip = extra["Flip"].GetBool();
		}

		var->myFrameDuration = max(0.0001f, var->myFrameDuration);
		var->myFramesPerRow = max(1, var->myFramesPerRow);
		var->myNumberOfFrames = max(1, var->myNumberOfFrames);

		var->myObjectData = aData;
		var->Init(aRoom, aGameWorld);

		event = var;
		break;
	}
	case EventActions::UpdateAnimation:
	{
		EventUpdateAnimation* var = new EventUpdateAnimation();

		if (extra.HasMember("AnimationIndex") == true)
		{
			var->myAnimationIndex = extra["AnimationIndex"].GetInt() - 1;
		}

		var->Init(aRoom, aGameWorld);

		event = var;
		break;
	}
	default:
		event = new EventNone();
		event->Init(aRoom, aGameWorld);
		break;
	}
	event->myType = static_cast<EventTypes>(aParent["type"].GetInt());
	event->myTarget = std::string(aParent["target"].GetString());
	event->myObjectData = aData;
	event->myAction = action;

	return event;
}
