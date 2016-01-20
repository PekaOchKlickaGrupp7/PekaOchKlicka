#pragma once
#include "../CommonUtilities/Macros.h"
#include "../CommonUtilities/GrowingArray.h"
#include <string>
#include <tga2d/sprite/sprite.h>

class Item
{
public:
	Item();

	//Constructor that sets all of the items values
	Item(DX2D::CSprite* aWorldSprite, DX2D::CSprite* aInventorySprite, std::string& aItemName, std::string& aItemDescription,
		DX2D::Vector2f& aPosition, bool aCombinableStatus, std::string& aLevelToSpawnIn);
	~Item();

	//Sets the items position on the screen
	void SetPosition(DX2D::Vector2f& aPosition);

	//Sets if the item can be combined with something else (true / false)
	void SetCombinable(bool aCombinableStatus);

	//Set the standard sprite to be the sprite made for the inventory
	void SetToInventorySprite();

	//Set the standard sprite to be the sprite made for the world
	void SetToWorldSprite();

	//Adds the items sprite to the rendering buffer
	void Render(Synchronizer& aSynchronizer);

	//Gets the list of item names that this item can be combined with
	CommonUtilities::GrowingArray<std::string>& GetList();

	//Gets the items sprite
	inline DX2D::CSprite* GetSprite();

	//Gets the items name
	inline std::string& GetName();

	//Gets the items Description
	inline std::string& GetDescription();

	//Gets the items position
	inline DX2D::Vector2f& GetPosition();

	//Gets a list of what this item can be combined with
	inline CommonUtilities::GrowingArray<std::string>& GetCombinableWithList();

	//Checks if the item can be combined with something else (true / false)
	inline bool IsCombinable();

	//Checks if the item has been clicked (true / false)
	inline bool IsClicked();

	bool operator ==(const Item& aItem);

private:
	CommonUtilities::GrowingArray<std::string> myCombinableWithList;
	DX2D::CSprite* mySprite;
	DX2D::CSprite* myWorldSprite;
	DX2D::CSprite* myInventorySprite;

	DX2D::Vector2f myPosition;

	std::string myName;
	std::string myDescription;
	std::string myLevelToSpawnIn;

	bool myIsCombinable;
	bool myIsClicked;
};

//Gets the items sprite
DX2D::CSprite* Item::GetSprite()
{
	return mySprite;
}

//Gets the items name
std::string& Item::GetName()
{
	return myName;
}

//Gets the items Description
std::string& Item::GetDescription()
{
	return myDescription;
}

//Gets the items position
DX2D::Vector2f& Item::GetPosition()
{
	return myPosition;
}

//Gets a list of what this item can be combined with
CommonUtilities::GrowingArray<std::string>& Item::GetCombinableWithList()
{
	return myCombinableWithList;
}

//Checks if the item can be combined with something else (true / false)
bool Item::IsCombinable()
{
	return myIsCombinable;
}

//Checks if the item has been clicked (true / false)
bool Item::IsClicked()
{
	return myIsClicked;
}