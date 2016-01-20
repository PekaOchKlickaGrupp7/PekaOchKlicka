#pragma once
#include "Vector.h"

namespace CommonUtilities
{
	namespace Intersection
	{
		class AABB
		{
		public:
			Vector3<float> myCenterPos;
			Vector3<float> myExtents;
			Vector3<float> myMinPos;
			Vector3<float> myMaxPos;
		};
	}
}