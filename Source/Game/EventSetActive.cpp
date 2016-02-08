#include "stdafx.h"
#include "EventSetActive.h"
#include "EventManager.h"

EventSetActive::EventSetActive()
{
}

EventSetActive::~EventSetActive()
{
}

bool EventSetActive::Update(const float aDeltaTime)
{
	(aDeltaTime);
	ObjectData* object = GetGameObject(myTarget);
	if (object != nullptr)
	{
		object->myActive = myValue;
		if (myValue == true)
		{
			EventManager::GetInstance()->OnEvent(object, EventTypes::OnActivated);
		}
		else
		{
			EventManager::GetInstance()->OnEvent(object, EventTypes::OnDisabled);
		}
	}
	return true;
}

void EventSetActive::Reset()
{
	
}
