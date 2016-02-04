#pragma once
#include "Vector.h"
#include "Sphere.h"
#include "AABB.h"
#include "LineSegment2D.h"
#include "LineSegment3D.h"
#include "Plane.h"
#include "Line.h"
#include <math.h>


namespace CommonUtilities
{
	namespace Intersection
	{
		//2D
		bool LineVsLine(LineSegment2D aLine1, LineSegment2D aLine2,Vector2<float>& aIntersectionPoint);
		bool PointVsRect(Vector2<float> aPoint, Vector2<float> aRectTopLeft, Vector2<float> aRectBottomRight);
		bool CircleVsCircle(Vector2<float> aCenter1, float aRadius1, Vector2<float> aCenter2, float aRadius2);
		bool CircleVsRect(Vector2<float> aCenter, float aRadius, Vector2<float> aRectTopLeft, Vector2<float> aRectBottomRight);

		//3D
		bool PointInsideSphere(Sphere aSphere, Vector3<float> aPoint);
		bool PointInsideAABB(AABB aAABB, Vector3<float> aPoint);
		bool LineVsSphere(LineSegment3D aLineSegment3D, Sphere aSphere, Vector3<float>& aIntersectionPoint);
		int PlaneVsSphere(CommonUtilities::Plane<float> aPlane, Sphere aSphere);
		bool SwepthSphereVsSphere(LineSegment3D aLineSegment3D, float aRadius, Sphere aSphere);
		bool SwepthSphereVsAABB(LineSegment3D aLineSegment3D, float aRadius, AABB aAABB);
		bool LineVsAABB(LineSegment3D aLineSegment3D, AABB aAABB, Vector3<float>& aIntersectoinPoint);
		double perpDot(const Vector2<float>& a, const Vector2<float>& b);
	}


	inline bool Intersection::CircleVsCircle(Vector2<float> aCenter1, float aRadius1, Vector2<float> aCenter2, float aRadius2)
	{
		if (Vector2<float>::Length2(aCenter2 - aCenter1) <= (aRadius1 + aRadius2) * (aRadius1 + aRadius2))
		{
			return true;
		}
		return false;
	}

	inline bool Intersection::PointVsRect(Vector2<float> aPoint, Vector2<float> aRectTopLeft, Vector2<float> aRectBottomRight)
	{
		if (aPoint.x < aRectTopLeft.x) return false;
		if (aPoint.y < aRectTopLeft.y) return false;
		if (aPoint.x > aRectBottomRight.x) return false;
		if (aPoint.y > aRectBottomRight.y) return false;

		return true;
	}

	inline bool Intersection::CircleVsRect(Vector2<float> aCenter, float aRadius, Vector2<float> aRectTopLeft, Vector2<float> aRectBottomRight)
	{
		return PointVsRect(aCenter, Vector2<float>(aRectTopLeft.x - aRadius, aRectTopLeft.y - aRadius), Vector2<float>(aRectBottomRight.x + aRadius, aRectBottomRight.y + aRadius));
	}

	inline bool Intersection::LineVsLine(LineSegment2D aFirstLine, LineSegment2D aSecondLine, Vector2<float>& aIntersectionPoint)
	{
		Vector2f r = aFirstLine.myEndPos - aFirstLine.myStartPos;
		Vector2f s = aSecondLine.myEndPos - aSecondLine.myStartPos;
		float rCrossS = r.Cross(s);
		float qPcrossR = (aSecondLine.myStartPos - aFirstLine.myStartPos).Cross(r);

		//Check if lines are collinear (i.e. if rCrossS = 0 and (qPcrossR = 0) )
		if (abs(rCrossS) <= 1e-10	// Check against 1 * 10^(-10) because
			&&						// of float precision
			abs(qPcrossR) <= 1e-10)
		{
			// Check if line segments are overlapping
			if ((0 <= (aSecondLine.myStartPos - aFirstLine.myStartPos).Dot(r)
				&& (aSecondLine.myStartPos - aFirstLine.myStartPos).Dot(r) <= r.Dot(r))
				||
				(0 <= (aFirstLine.myStartPos - aSecondLine.myStartPos).Dot(s)
				&& (aFirstLine.myStartPos - aSecondLine.myStartPos).Dot(s) <= s.Dot(s)))
			{
				return true;
			}

			//Line segments are collinear but not together
			return false;
		}

		//Check if lines are parallel (i.e. if rCrossS = 0 and qPcrossR != 0)
		if (abs(rCrossS) <= 1e-10	// Check against 1 * 10^(-10) because
			&&						// of float precision
			!(abs(qPcrossR) <= 1e-10))
		{
			return false;
		}

		//Strange math magics here
		float t = (aSecondLine.myStartPos - aFirstLine.myStartPos).Cross(s) / rCrossS;
		float u = qPcrossR / rCrossS;

		//Finding the intersection point if there is one
		if (!(abs(rCrossS) <= 1e-10)
			&& (0 <= t && t <= 1)
			&& (0 <= u && u <= 1))
		{
			//Calculating the intersection point
			Vector2f tempVector = aFirstLine.myStartPos + t * r;
			aIntersectionPoint.x = tempVector.x;
			aIntersectionPoint.y = tempVector.y;
			return true;
		}

		//Line segments are not parallel but do not intersect
		return false;
	}

	inline bool Intersection::PointInsideSphere(Sphere aSphere, Vector3<float> aPoint)
	{
		if (Vector3<float>::Length2(aPoint - aSphere.myCenterPosition) < aSphere.myRadiusSquared)
		{
			return true;
		}
		return false;
	}

	inline bool Intersection::PointInsideAABB(AABB aAABB, Vector3<float> aPoint)
	{
		if (aPoint.x > aAABB.myMaxPos.x) return false;
		if (aPoint.x < aAABB.myMinPos.x) return false;
		if (aPoint.y > aAABB.myMaxPos.y) return false;
		if (aPoint.y < aAABB.myMinPos.y) return false;
		if (aPoint.z > aAABB.myMaxPos.z) return false;
		if (aPoint.z < aAABB.myMinPos.z) return false;

		return true;
	}

	inline bool Intersection::LineVsSphere(LineSegment3D aLineSegment3D, Sphere aSphere, Vector3<float>& aIntersectionPoint)
	{
		Vector3<float> d = aLineSegment3D.myEndPos - aLineSegment3D.myStartPos;
		Vector3<float>Normalize(d);
		Vector3<float> e = aSphere.myCenterPosition - aLineSegment3D.myStartPos;
		float a = Vector3<float>::Dot(e, d);

		float fSquared = aSphere.myRadiusSquared - Vector3<float>::Dot(e, e) + a * a;
		if (fSquared < 0)
		{
			return false;
		}

		float t = a - sqrt(fSquared);
		aIntersectionPoint = aLineSegment3D.myStartPos + d * t;

		if (t*t > Vector3<float>::Length2(aLineSegment3D.myEndPos - aLineSegment3D.myStartPos))
		{
			return false;
		}

		return true;
	}

	inline int Intersection::PlaneVsSphere(CommonUtilities::Plane<float> aPlane, Sphere aSphere)
	{
		float d = Vector3<float>::Dot(aPlane.GetNormal(), aSphere.myCenterPosition) - aPlane.myABCD.w;

		if (d >= aSphere.myRadius)
		{
			return 1;
		}
		if (d <= -aSphere.myRadius)
		{
			return -1;
		}

		return 0;
	}

	inline bool Intersection::SwepthSphereVsSphere(LineSegment3D aLineSegment3D, float aRadius, Sphere aSphere)
	{
		aSphere.myRadius += aRadius;
		aSphere.myRadiusSquared = aSphere.myRadius * aSphere.myRadius;
		Vector3<float> intersectionPoint;
		return LineVsSphere(aLineSegment3D, aSphere, intersectionPoint);
	}

	inline bool Intersection::SwepthSphereVsAABB(LineSegment3D aLineSegment3D, float aRadius, AABB aAABB)
	{
		aAABB.myMinPos -= aRadius;
		aAABB.myMaxPos += aRadius;
		Vector3<float> intersectionPoint;
		return LineVsAABB(aLineSegment3D, aAABB, intersectionPoint);
	}

	inline bool Intersection::LineVsAABB(LineSegment3D aLineSegment3D, AABB aAABB, Vector3<float>& aIntersectoinPoint)
	{
		Vector3<float> rayOrg = aLineSegment3D.myStartPos;
		Vector3<float> rayDelta = aLineSegment3D.myEndPos - aLineSegment3D.myStartPos;

		bool inside = true;

		float xt;
		float xn;

		if (rayOrg.x < aAABB.myMinPos.x)
		{
			xt = aAABB.myMinPos.x - rayOrg.x;
			if (xt > rayDelta.x)
			{
				return false;
			}
			xt /= rayDelta.x;
			inside = false;
			xn = -1.f;
		}
		else if (rayOrg.x > aAABB.myMaxPos.x)
		{
			xt = aAABB.myMaxPos.x - rayOrg.x;
			if (xt < rayDelta.x)
			{
				return false;
			}
			xt /= rayDelta.x;
			inside = false;
			xn = 1.f;
		}
		else
		{
			xt = -1.f;
		}

		float yt;
		float yn;

		if (rayOrg.y < aAABB.myMinPos.y)
		{
			yt = aAABB.myMinPos.y - rayOrg.y;
			if (yt > rayDelta.y)
			{
				return false;
			}
			yt /= rayDelta.y;
			inside = false;
			yn = -1.f;
		}
		else if (rayOrg.y > aAABB.myMaxPos.y)
		{
			yt = aAABB.myMaxPos.y - rayOrg.y;
			if (yt < rayDelta.y)
			{
				return false;
			}
			yt /= rayDelta.y;
			inside = false;
			yn = 1.f;
		}
		else
		{
			yt = -1.f;
		}

		float zt;
		float zn;

		if (rayOrg.z < aAABB.myMinPos.z)
		{
			zt = aAABB.myMinPos.z - rayOrg.z;
			if (zt > rayDelta.z)
			{
				return false;
			}
			zt /= rayDelta.z;
			inside = false;
			zn = -1.f;
		}
		else if (rayOrg.z > aAABB.myMaxPos.z)
		{
			zt = aAABB.myMaxPos.z - rayOrg.z;
			if (zt < rayDelta.z)
			{
				return false;
			}
			zt /= rayDelta.z;
			inside = false;
			zn = 1.f;
		}
		else
		{
			zt = -1.f;
		}

		if (inside == true)
		{
			aIntersectoinPoint = aLineSegment3D.myStartPos;
			return true;
		}

		int which = 0;

		float t = xt;

		if (yt > t)
		{
			which = 1;
			t = yt;
		}

		if (zt > t)
		{
			which = 2;
			t = zt;
		}

		switch (which)
		{
		case 0:
		{
			float y = rayOrg.y + rayDelta.y * t;
			if (y < aAABB.myMinPos.y || y > aAABB.myMaxPos.y)
			{
				return false;
			}
			float z = rayOrg.z + rayDelta.z * t;
			if (z < aAABB.myMinPos.z || z > aAABB.myMaxPos.z)
			{
				return false;
			}
			break;
		}
		case 1:
		{
			float x = rayOrg.x + rayDelta.x * t;
			if (x < aAABB.myMinPos.x || x > aAABB.myMaxPos.x)
			{
				return false;
			}
			float z = rayOrg.z + rayDelta.z * t;
			if (z < aAABB.myMinPos.z || z > aAABB.myMaxPos.z)
			{
				return false;
			}
			break;
		}
		case 2:
		{
			float x = rayOrg.x + rayDelta.x * t;
			if (x < aAABB.myMinPos.x || x > aAABB.myMaxPos.x)
			{
				return false;
			}
			float y = rayOrg.y + rayDelta.y * t;
			if (y < aAABB.myMinPos.y || y > aAABB.myMaxPos.y)
			{
				return false;
			}
			break;
		}
		}

		aIntersectoinPoint = aLineSegment3D.myStartPos + rayDelta * t;
		return true;
	}

	inline double Intersection::perpDot(const Vector2<float>& a, const Vector2<float>& b)
	{
		return (a.y*b.x) - (a.x*b.y);
	}
}