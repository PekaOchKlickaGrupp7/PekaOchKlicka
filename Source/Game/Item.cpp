#include "stdafx.h"
#include "Item.h"


Item::Item()
{
	mySprite = nullptr;
	myCombinableWithList.Init(4);

	myName = "";
	myDescription = "";
	myLevelToSpawnIn = "";

	myIsCombinable = false;
	myPickupStatus = false;
	myIsClicked = false;

	myPosition = DX2D::Vector2f(0.0f, 0.0f);
}

//Constructor that sets all of the items values except the sprite
Item::Item(std::string& aItemName, std::string& aItemDescription,
	DX2D::Vector2f& aPosition, bool aCombinableStatus, bool aCanBePickedUpStatus,
	std::string& aLevelToSpawnIn)
{
	myCombinableWithList.Init(4);

	myName = aItemName;
	myDescription = aItemDescription;
	myLevelToSpawnIn = aLevelToSpawnIn;

	myPosition = aPosition;
	
	myIsCombinable = aCombinableStatus;
	myPickupStatus = aCanBePickedUpStatus;
	myIsClicked = false;
}

//Constructor that sets all of the items values
Item::Item(DX2D::CSprite* aSprite, std::string& aItemName, std::string& aItemDescription,
	DX2D::Vector2f& aPosition, bool aCombinableStatus, bool aCanBePickedUpStatus,
	std::string& aLevelToSpawnIn)
{
	myCombinableWithList.Init(4);

	mySprite = aSprite;

	myName = aItemName;
	myDescription = aItemDescription;
	myLevelToSpawnIn = aLevelToSpawnIn;

	myPosition = aPosition;

	myIsCombinable = aCombinableStatus;
	myPickupStatus = aCanBePickedUpStatus;
	myIsClicked = false;
}

Item::~Item()
{
	SAFE_DELETE(mySprite);
}

//Sets the items position on the screen
void Item::SetPosition(DX2D::Vector2f& aPosition)
{
	myPosition = aPosition;
}

//Sets if the item can be combined with something else (true / false)
void Item::SetCombinable(bool aCombinableStatus)
{
	myIsCombinable = aCombinableStatus;
}

//Sets if the item can be picked up (true / false)
void Item::SetPickupStatus(bool aPickupStatus)
{
	myPickupStatus = aPickupStatus;
}

//Adds the items sprite to the rendering buffer
void Item::Draw()
{

}

//Gets the list of item names that this item can be combined with
CommonUtilities::GrowingArray<std::string>& Item::GetList()
{
	return myCombinableWithList;
}

bool Item::operator ==(const Item& aItem)
{
	return myName == aItem.myName;
}