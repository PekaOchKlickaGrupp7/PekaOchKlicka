#pragma once
#include "../DX2DEngine/tga2d/math/vector2.h"
#include <tga2d/primitives/custom_shape.h>
#include <tga2d/text/text.h>

enum class eRenderType
{
	eSprite,
	eText,
	eCustomShape
};
namespace DX2D
{
	class CSprite;
}

struct RenderCommand
{
	DX2D::Vector2f myPosition;
	DX2D::CSprite *mySprite;
	DX2D::CCustomShape *myCustomShape;
	DX2D::CText *myText;
	eRenderType myType;
};