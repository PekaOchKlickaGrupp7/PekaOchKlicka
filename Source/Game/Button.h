#pragma once
#include "tga2d\sprite\sprite.h"
#include "MouseManager.h"

// Simplifies argument sending and initializing. They only need different positions. // Linus
struct ButtonSpriteArguments
{
	const char* myButtonUnCheckedPath = "Sprites/Options_InG/Fullscreen_Unchecked2.dds";
	const char* myButtonCheckedPath = "Sprites/Options_InG/Fullscreen_Checked2.dds";
	const char* myButtonHoveredPath = "Sprites/Options_InG/Fullscreen_Unchecked2.dds";
};
struct ButtonPositionArguments
{
	DX2D::Vector2f myButtonPosition;
	DX2D::Vector2f myButtonPivot;
};
struct ButtonSetOfArguments
{
	ButtonSpriteArguments mySpriteArguments;
	ButtonPositionArguments myPositionArguments;
};

class Button
{
public:
	Button();

	virtual void InitValue() = 0;
	void InitSprites(ButtonSpriteArguments &aSpriteArguments);
	void InitPositions(ButtonPositionArguments &aPositionsArguments);

	void Update(float aDeltaTime);
	void Render(Synchronizer& aSynchronizer);

	DX2D::Vector2f GetPosition(){ return mySprite->GetPosition(); };
	DX2D::Vector2f GetSize(){ return mySprite->GetSize(); };

	virtual ~Button();
protected:
	DX2D::CSprite* mySprite;
	DX2D::CSprite* myButtonUnChecked;
	DX2D::CSprite* myButtonChecked;
	DX2D::CSprite* myButtonHovered;

	void UpdateImage();
	
	bool myIsChecked;
	bool myIsHovering;
	bool myShouldHover;

	bool OnMouseClick();
	bool OnMouseHover();

	virtual void ButtonSpecificUpdate() = 0;

	virtual void ProgressSet() = 0;
	virtual void ProgressGet() = 0;
};

