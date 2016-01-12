#pragma once
#include "../CommonUtilities/Macros.h"
#include <string>
#include <tga2d/sprite/sprite.h>

class Item
{
public:
	Item();
	Item(DX2D::CSprite* aSprite, std::string& aItemName, std::string& aItemDescription,
		DX2D::Vector2f& aPosition, bool aCombinableStatus, bool aCanBePickedUpStatus);
	~Item();

	void SetSprite(DX2D::CSprite* aSprite);
	void SetName(std::string& aName);
	void SetDescription(std::string& aDescription);
	void SetPosition(DX2D::Vector2f& aPosition);
	void SetCombinable(bool aCombinableStatus);
	void SetPickupStatus(bool aPickupStatus);

	inline DX2D::CSprite* GetSprite();
	inline std::string& GetName();
	inline std::string& GetDescription();
	inline DX2D::Vector2f& GetPosition();
	inline bool GetCombinable();
	inline bool GetPickupStatus();

private:
	DX2D::CSprite* mySprite;

	DX2D::Vector2f myPosition;

	std::string myName;
	std::string myDescription;

	bool myIsCombinable;
	bool myPickupStatus;
};

DX2D::CSprite* Item::GetSprite()
{
	return mySprite;
}

std::string& Item::GetName()
{
	return myName;
}

std::string& Item::GetDescription()
{
	return myDescription;
}

DX2D::Vector2f& Item::GetPosition()
{
	return myPosition;
}

bool Item::GetCombinable()
{
	return myIsCombinable;
}

bool Item::GetPickupStatus()
{
	return myPickupStatus;
}