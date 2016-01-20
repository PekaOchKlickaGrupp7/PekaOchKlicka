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

Item::Item(const Item& aItemToCopy)
{
	this->myCombinableWithList = aItemToCopy.myCombinableWithList;
	this->mySprite = aItemToCopy.mySprite;
	this->myWorldSprite = aItemToCopy.myWorldSprite;
	this->myInventorySprite = aItemToCopy.myInventorySprite;
	this->myPosition = aItemToCopy.myPosition;
	this->myName = aItemToCopy.myName;
	this->myDescription = aItemToCopy.myDescription;
	this->myLevelToSpawnIn = aItemToCopy.myLevelToSpawnIn;
	this->myIsCombinable = aItemToCopy.myIsCombinable;
	this->myIsClicked = aItemToCopy.myIsClicked;
}

Item::~Item()
{
	SAFE_DELETE(myInventorySprite);
	SAFE_DELETE(myWorldSprite);
}

//Initialize the item
void Item::Init(const char* aWorldSpritePath, const char* aInventorySpritePath, const std::string& aItemName,
	const std::string& aItemDescription, DX2D::Vector2f& aPosition, bool aCombinableStatus,
	const std::string& aLevelToSpawnIn)
{
	InitSprites(aWorldSpritePath, aInventorySpritePath);

	myName = aItemName;
	myDescription = aItemDescription;
	myLevelToSpawnIn = aLevelToSpawnIn;

	myPosition = aPosition;

	myIsCombinable = aCombinableStatus;
	myIsClicked = false;
}

//Initialize sprites
void Item::InitSprites(const char* aWorldSpritePath, const char* aInventorySpritePath)
{
	myWorldSprite = new DX2D::CSprite(aWorldSpritePath);
	myInventorySprite = new DX2D::CSprite(aInventorySpritePath);

	//Set standard sprite
	mySprite = myWorldSprite;
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