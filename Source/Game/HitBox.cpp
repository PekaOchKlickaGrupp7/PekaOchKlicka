#include "stdafx.h"
#include "HitBox.h"
#include <sstream>
#include <iostream>
#include "..\CommonUtilities\Collision.h"
#include "..\CommonUtilities\DL_Debug.h"
#include "ObjectData.h"

HitBox::HitBox() : myObject(nullptr)
{
}

HitBox::~HitBox()
{
}

void HitBox::Init(ObjectData* aObject)
{
	myObject = aObject;
}

bool HitBox::IsMouseColliding(const float aX, const float aY)
{
	if (myObject == nullptr)
	{
		DL_ASSERT("DataObject is nullptr");
	}
	return CommonUtilities::AxisAlignedBoundingBox(aX, aY, 0, 0, myObject->myX + myX, myObject->myY + myY, myWidth, myHeight);
}

void HitBox::Parse(const std::string& aString)
{
	int lastIndex = 0;
	int index = 0;
	for (unsigned int i = 0; i < aString.size(); ++i)
	{
		if (aString.substr(i, 2) == ", " || i + 1 >= aString.length())
		{
			int value = 0;
			bool lastStrIndex = (i + 1) >= aString.length();
			std::string str = "";
			if (lastStrIndex == true)
			{
				str = aString.substr(lastIndex, (i - lastIndex) + 1);
			}
			else
			{
				str = aString.substr(lastIndex, i - lastIndex);
			}
			std::cout << str << std::endl;
			std::stringstream(str) >> value;
			lastIndex = i + 2;
			switch (++index)
			{
			case 1: //X
				myX = value / 1280.0f;
				break;
			case 2: //Y
				myY = value / 720.0f;
				break;
			case 3: //Width
				myWidth = value / 1280.0f;
				break;
			case 4: //height
				myHeight = value / 720.0f;
				break;
			}
		}
	}
}
