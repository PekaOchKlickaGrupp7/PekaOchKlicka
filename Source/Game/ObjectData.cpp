#include "stdafx.h"
#include "ObjectData.h"
#include "tga2d\sprite\sprite.h"
#include "Event.h"

ObjectData::ObjectData()
{
	myIsHovering = false; 
	myColor = { 1, 1, 1, 1 };
	myTriangles.Init(2);
}

ObjectData::~ObjectData()
{
	for (unsigned int i = 0; i < myChilds.Size(); ++i)
	{
		delete myChilds[i];
	}
	myChilds.~GrowingArray();
	myEvents.DeleteAll();
	delete myOriginalSprite;
	myOriginalSprite = nullptr;
}

void ObjectData::AddTriangle(Triangle& aTriangle)
{
	myTriangles.Add(aTriangle);
}

void ObjectData::CreateOriginalData()
{
	myOriginalData = new ObjectData();
	myOriginalData->myActive = myActive;
	myOriginalData->mySprite = mySprite;
	myOriginalData->myX = myX;
	myOriginalData->myY = myY;
	myOriginalData->myGlobalX = myGlobalX;
	myOriginalData->myGlobalY = myGlobalY;
	myOriginalData->myScaleX = myScaleX;
	myOriginalData->myScaleY = myScaleY;
	myOriginalData->myName = myName;
	myOriginalData->myPivotX = myPivotX;
	myOriginalData->myPivotY = myPivotY;
	myOriginalData->myPivotX = myPivotX;
	myOriginalData->myActive = myActive;
	myOriginalData->myColor = myColor;
	myOriginalData->myEvents = CommonUtilities::GrowingArray<Event*, unsigned int>(myEvents);

	for (unsigned int i = 0; i < myChilds.Size(); ++i)
	{
		myChilds[i]->CreateOriginalData();
	}
}
void ObjectData::ResetToOriginalData()
{
	myActive = myOriginalData->myActive;
	mySprite = myOriginalData->mySprite;
	myX = myOriginalData->myX;
	myY = myOriginalData->myY;
	myGlobalX = myOriginalData->myGlobalX;
	myGlobalY = myOriginalData->myGlobalY;
	myScaleX = myOriginalData->myScaleX;
	myScaleY = myOriginalData->myScaleY;
	myName = myOriginalData->myName;
	myPivotX = myOriginalData->myPivotX;
	myPivotY = myOriginalData->myPivotY;
	myPivotX = myOriginalData->myPivotX;
	myActive = myOriginalData->myActive;
	myColor = myOriginalData->myColor;
	myEvents = CommonUtilities::GrowingArray<Event*, unsigned int>(myOriginalData->myEvents);

	myIsHovering = false;

	for (unsigned int i = 0; i < myChilds.Size(); ++i)
	{
		myChilds[i]->ResetToOriginalData();
	}
}
