#pragma once
#include <cmath>

namespace CommonUtilities
{
	//The algorithm works by ensuring there is no gap between any of the 4 sides of the rectangles. Any gap means a collision does not exist.
	inline static bool AxisAlignedBoundingBox(float aPosX1, float aPosY1, float aWidth1, float aHeight1,
		float aPosX2, float aPosY2, float aWitdh2, float aHeight2)
	{
		if (aPosX1 < aPosX2 + aWitdh2 &&
			aPosX1 + aWidth1 > aPosX2 &&
			aPosY1 < aPosY2 + aHeight2 &&
			aHeight1 + aPosY1 > aPosY2)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//This algorithm works by taking the centre points of the two circles and ensuring the distance between the centre points are less than the two radii added together.
	inline static bool CircleCollision(float aRadius1, float aPosX1, float aPosY1,
									   float aRadius2, float aPosX2, float aPosY2)
	{
		float diffX = aPosX1 - aPosX2;
		float diffY = aPosY1 - aPosY2;
		float distance = sqrtf(diffX * diffX + diffY * diffY);

		if (distance < aRadius1 + aRadius2)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

}