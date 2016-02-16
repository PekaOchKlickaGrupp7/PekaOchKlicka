#pragma once
#include "../CommonUtilities/Macros.h"
#include "../CommonUtilities/GrowingArray.h"
#include <string>
#include <tga2d/sprite/sprite.h>
#include "ItemList.h"

class Item
{
public:
	Item();
	//Item(const Item* aItem);
	Item(const std::string& aName, const char* aInventorySpritePath, const std::string& aDescription,
		const std::string& aCombinableWith, const std::string& aResultingItem, bool aIsCombinable);
	~Item();

	void Init(const char* aWorldSpritePath, 
		const char* aInventorySpritePath, 
		const std::string& aItemName,
		const std::string& aItemDescription, 
		DX2D::Vector2f& aPosition, 
		bool aCombinableStatus,
		const std::string& aLevelToSpawnIn,
		const std::string& aCombineSoundPath);

	void Destroy();

	void SetPosition(DX2D::Vector2f& aPosition);
	void SetCombinable(bool aCombinableStatus);
	void SetToInventorySprite();
	void SetToWorldSprite();
	void Render(Synchronizer& aSynchronizer);

	inline DX2D::CSprite* GetSprite();
	inline std::string& GetName();
	inline std::string& GetCombinableItem();
	inline std::string& GetDescription();
	inline DX2D::Vector2f& GetPosition();
	inline std::string& GetNameOfResultingItem();

	inline bool IsCombinable();
	inline bool IsClicked();
	bool operator ==(const Item& aItem);

	void PlayCombineSound();

private:

	void InitSprites(const char* aWorldSpritePath, const char* aInventorySpritePath);

	DX2D::CSprite* mySprite;
	DX2D::CSprite* myWorldSprite;
	DX2D::CSprite* myInventorySprite;

	DX2D::Vector2f myPosition;

	std::string myName;
	std::string myDescription;
	std::string myLevelToSpawnIn;

	std::string myCombinableWith;
	std::string myResultingItem; //after combining

	bool myIsCombinable;
	bool myIsClicked;

	std::string myCombineSoundPath;
};

std::string& Item::GetNameOfResultingItem()
{
	return myResultingItem;
}

std::string& Item::GetCombinableItem()
{
	return myCombinableWith;
}
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
//CommonUtilities::GrowingArray<std::string>& Item::GetCombinableWithList()
//{
//	return myCombinableWithList;
//}

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