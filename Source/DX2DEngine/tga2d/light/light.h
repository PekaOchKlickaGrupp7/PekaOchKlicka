#pragma once
#include "tga2d/math/Vector2.h"
#include "tga2d/math/Vector4.h"

namespace DX2D
{
	class CLight
	{
	public:
		CLight();
		~CLight();
		void Render();
		Vector2f myPosition;
		Vector4f myColor;
		float myIntensity;
		float myFallOff;
	};
}