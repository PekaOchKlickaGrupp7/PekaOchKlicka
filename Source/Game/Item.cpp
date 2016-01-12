#include "stdafx.h"
#include "Item.h"


Item::Item()
{
	mySprite = nullptr;

	myName = "";
	myDescription = "";

	myIsCombinable = false;
	myPickupStatus = false;

	myPosition = DX2D::Vector2f(0.0f, 0.0f);
}

Item::Item(DX2D::CSprite* aSprite, std::string& aItemName, std::string& aItemDescription,
	DX2D::Vector2f& aPosition, bool aCombinableStatus, bool aCanBePickedUpStatus)
{
	mySprite = aSprite;
	myName = aItemName;
	myDescription = aItemDescription;
	myPosition = aPosition;
	myIsCombinable = aCombinableStatus;
	myPickupStatus = aCanBePickedUpStatus;
}

Item::~Item()
{
	SAFE_DELETE(mySprite);
}

void Item::SetSprite(DX2D::CSprite* aSprite)
{
	mySprite = aSprite;
}

void Item::SetName(std::string& aName)
{
	myName = aName;
}

void Item::SetDescription(std::string& aDescription)
{
	myDescription = aDescription;
}

void Item::SetPosition(DX2D::Vector2f& aPosition)
{
	myPosition = aPosition;
}

void Item::SetCombinable(bool aCombinableStatus)
{
	myIsCombinable = aCombinableStatus;
}

void Item::SetPickupStatus(bool aPickupStatus)
{
	myPickupStatus = aPickupStatus;
}