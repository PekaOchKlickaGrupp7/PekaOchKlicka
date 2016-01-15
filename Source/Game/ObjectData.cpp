#include "stdafx.h"
#include "ObjectData.h"


ObjectData::ObjectData()
{
}


ObjectData::~ObjectData()
{
	delete mySprite;
	myChilds.DeleteAll();
}
