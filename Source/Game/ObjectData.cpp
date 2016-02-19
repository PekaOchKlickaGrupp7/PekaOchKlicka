#include "stdafx.h"
#include "ObjectData.h"
#include "tga2d\sprite\sprite.h"
#include "Event.h"
#include "Animation.h"

ObjectData::ObjectData()
{
	myIsHovering = false; 
	myColor = { 1, 1, 1, 1 };
	myTriangles.Init(2);
	myAmountActiveEvents = 0;

	myIsAnimation = false;
	myCurrentAnimation = 0;
	myAnimations.Init(12);

	mySprite = nullptr;
	myOriginalSprite = nullptr;

	myX = 0;
	myY = 0;
	myGlobalX = 0;
	myGlobalY = 0;
	myScaleX = 0;
	myScaleY = 0;
	myRotation = 0;
	myPivotX = 0;
	myPivotY = 0; 
	myActive = false;
	
	myOriginalData = nullptr;
}

ObjectData::~ObjectData()
{
	if (myChilds.GetIsInitialized() == true)
	{
		for (unsigned int i = 0; i < myChilds.Size(); ++i)
		{
			delete myChilds[i];
		}
	}
	myEvents.DeleteAll();
	delete myOriginalSprite;
	myOriginalSprite = nullptr;

	myAnimations.DeleteAll();
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
	myOriginalData->myActive = myActive;
	myOriginalData->myColor = myColor;

	/*for (unsigned int i = 0; i < myChilds.Size(); ++i)
	{
		myChilds[i]->CreateOriginalData();
	}*/
}

void ObjectData::ResetToOriginalData()
{
	if (myOriginalData != nullptr)
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
		myColor = myOriginalData->myColor;

		myAmountActiveEvents = 0;
		myIsHovering = false;

		if (myChilds.GetIsInitialized() == true)
		{
			for (unsigned int i = 0; i < myChilds.Size(); ++i)
			{
				myChilds[i]->ResetToOriginalData();
			}
		}
	}
	else
	{
		return;
	}
}
