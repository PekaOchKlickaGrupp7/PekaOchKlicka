#include "stdafx.h"
#include "ObjectData.h"
#include "tga2d\sprite\sprite.h"

ObjectData::ObjectData()
{
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
	myEvents.~GrowingArray();
}

void ObjectData::Update(float aDeltaTime)
{
	(aDeltaTime);
}
