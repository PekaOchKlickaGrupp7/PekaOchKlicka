#include "stdafx.h"
#include "Item.h"
#include "JSON.h"

Item::Item()
{
	mySprite = nullptr;
	myWorldSprite = nullptr;
	myInventorySprite = nullptr;

	myName = "";
	myDescription = "";
	myLevelToSpawnIn = "";

	myIsCombinable = false;
	myIsClicked = false;

	myPosition = DX2D::Vector2f(0.0f, 0.0f);
}

Item::Item(const Item* aItem)
{
	mySprite = aItem->mySprite;
	myWorldSprite = aItem->myWorldSprite;
	myInventorySprite = aItem->myInventorySprite;

	myName = aItem->myName;
	myDescription = aItem->myDescription;
	myLevelToSpawnIn = aItem->myLevelToSpawnIn;

	myIsCombinable = aItem->myIsCombinable;
	myIsClicked = aItem->myIsClicked;

	myPosition = aItem->myPosition;
}

//only what is needed for the inventory-part of items.
Item::Item(const std::string& aName, const char* aInventorySpritePath, const std::string& aDescription,
	const std::string& aCombinableWith, const std::string& aResultingItem, bool aIsCombinable)
{
	myName = aName;
	InitSprites(nullptr, aInventorySpritePath);
	myDescription = aDescription;
	myIsCombinable = aIsCombinable;
	myCombinableWith = aCombinableWith;
	myResultingItem = aResultingItem;
	myLevelToSpawnIn = "Test Level";

	myPosition = DX2D::Vector2f(0.0f, 0.0f);
	mySprite = new DX2D::CSprite("Sprites/TestItems/GraveShovel_inventory.png");
	myWorldSprite = new DX2D::CSprite("Sprites/TestItems/GraveShovel_inventory.png");
	myInventorySprite = new DX2D::CSprite("Sprites/TestItems/GraveShovel_inventory.png");
}

Item::~Item()
{
	SAFE_DELETE(myWorldSprite);
	SAFE_DELETE(myInventorySprite);
}

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

void Item::InitSprites(const char* aWorldSpritePath, const char* aInventorySpritePath)
{
	myWorldSprite = new DX2D::CSprite(aWorldSpritePath);
	myInventorySprite = new DX2D::CSprite(aInventorySpritePath);

	//Set standard sprite
	mySprite = myWorldSprite;
}

void Item::SetPosition(DX2D::Vector2f& aPosition)
{
	myPosition = aPosition;
}

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

void Item::Render(Synchronizer& aSynchronizer)
{
	RenderCommand command;
	command.mySprite = mySprite;
	command.myPosition = myPosition;
	command.myType = eRenderType::eSprite;

	aSynchronizer.AddRenderCommand(command);
}

//Gets the list of item names that this item can be combined with
//CommonUtilities::GrowingArray<std::string>& Item::GetList()
//{
//	return myCombinableWithList;
//}

bool Item::operator ==(const Item& aItem)
{
	return myName == aItem.myName;
}