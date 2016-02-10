#pragma once
#include "tga2d\sprite\sprite.h"
#include "MouseManager.h"

// Simplifies argument sending and initializing. They only need different positions. // Linus
struct SliderSpriteArguments
{
	const char* mySliderPath = "Sprites/Options_InG/Slider.dds";
	const char* myKnobPath = "Sprites/Options_InG/Knob.dds";
	const char* myKnobPressedPath = "Sprites/Options_InG/KnobPressed.dds";
	const char* myKnobHoveredPath = "Sprites/Options_InG/KnobHovered.dds";
};
struct SliderPositionArguments
{
	DX2D::Vector2f mySliderPosition;
	DX2D::Vector2f myKnobPosition;
	DX2D::Vector2f myKnobPivot;
};
struct SliderSetOfArguments
{
	SliderSpriteArguments mySpriteArguments;
	SliderPositionArguments myPositionArguments;
};


class Slider
{
public:
	Slider();

	virtual void InitValue() = 0;
	void InitSprites(SliderSpriteArguments &aSpriteArguments);
	void InitPositions(SliderPositionArguments &aPositionsArguments);

	void Update(float aDeltaTime);
	void Render(Synchronizer& aSynchronizer);

	DX2D::Vector2f GetSliderPosition(){ return mySlider->GetPosition(); };
	DX2D::Vector2f GetKnobPosition(){ return myKnob->GetPosition(); };

	DX2D::Vector2f GetSliderSize(){ return mySlider->GetSize(); };	
	DX2D::Vector2f GetKnobSize(){ return myKnob->GetSize(); };
	virtual ~Slider();
protected:
	DX2D::CSprite* mySlider;
	DX2D::CSprite* myKnob;
	DX2D::CSprite* myKnobRegular;
	DX2D::CSprite* myKnobPressed;
	DX2D::CSprite* myKnobHovered;

	DX2D::Vector2f myValueConvertedToPosition;
	float myValue;

	bool myIsHoveringKnob;
	bool myIsDraggingKnob;

	bool OnMouseClick();
	bool OnMouseHover();

	float ConvertValueToPosition();
	float ConvertPositionToValue(float aPos);
	void SetAllKnobs(float aPos);
	virtual void SliderSpecificUpdate() = 0;

	virtual void ProgressSet() = 0;
	virtual void ProgressGet() = 0;
};

