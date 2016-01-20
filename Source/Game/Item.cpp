#include "stdafx.h"
#include "Item.h"


Item::Item()
{
	mySprite = nullptr;
	myWorldSprite = nullptr;
	myInventorySprite = nullptr;

	myCombinableWithList.Init(4);

	myName = "";
	myDescription = "";
	myLevelToSpawnIn = "";

	myIsCombinable = false;
	myIsClicked = false;

	myPosition = DX2D::Vector2f(0.0f, 0.0f);
}

//Constructor that sets all of the items values
Item::Item(DX2D::CSprite* aWorldSprite, DX2D::CSprite* aInventorySprite, std::string& aItemName, std::string& aItemDescription,
	DX2D::Vector2f& aPosition, bool aCombinableStatus, std::string& aLevelToSpawnIn)
{
	myCombinableWithList.Init(4);

	myWorldSprite = aWorldSprite;
	myInventorySprite = aInventorySprite;

	//Set standard sprite
	mySprite = aWorldSprite;

	myName = aItemName;
	myDescription = aItemDescription;
	myLevelToSpawnIn = aLevelToSpawnIn;

	myPosition = aPosition;

	myIsCombinable = aCombinableStatus;
	myIsClicked = false;
}

Item::~Item()
{
	SAFE_DELETE(mySprite);
	SAFE_DELETE(myWorldSprite);
	SAFE_DELETE(myInventorySprite);
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

//Change from world sprite to inventory sprite
void Item::SetToInventorySprite()
{
	mySprite = myInventorySprite;
}

//Change from inventory sprite to world sprite
void Item::SetToWorldSprite()
{
	mySprite = myWorldSprite;
}

//Adds the items sprite to the rendering buffer
void Item::Render(Synchronizer& aSynchronizer)
{
	RenderCommand command;
	command.mySprite = mySprite;
	command.myPosition = myPosition;
	command.myType = eRenderType::eSprite;

	aSynchronizer.AddRenderCommand(command);
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