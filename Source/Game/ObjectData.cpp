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
	delete mySprite;
	mySprite = nullptr;
	for (unsigned int i = 0; i < myChilds.Size(); ++i)
	{
		delete myChilds[i];
	}
	myChilds.~GrowingArray();
	myEvents.DeleteAll();
}

void ObjectData::AddTriangle(Triangle& aTriangle)
{
	myTriangles.Add(aTriangle);
}
