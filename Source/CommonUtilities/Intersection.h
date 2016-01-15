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

	inline bool Intersection::LineVsLine(LineSegment2D aLine1, LineSegment2D aLine2, Vector2<float>& aIntersectionPoint)
	{
		CommonUtilities::Line<float> line1(aLine1.myStartPos, aLine1.myEndPos);
		CommonUtilities::Line<float> line2(aLine2.myStartPos, aLine2.myEndPos);


		float numX = line2.myABC.y * line1.myABC.z - line1.myABC.y * line2.myABC.z;
		float denX = line1.myABC.x * line2.myABC.y - line2.myABC.x * line1.myABC.y;

		float numY = line1.myABC.x * line2.myABC.z - line2.myABC.x * line1.myABC.z;
		float denY = line1.myABC.x * line2.myABC.y - line2.myABC.x * line1.myABC.y;

		if (denX == 0 || denY == 0)
		{
			if (line1.myABC.z != line2.myABC.z)
			{
				return false;
			}

			// check segment overlap

			Vector2<float> dir1 = aLine1.myEndPos - aLine1.myStartPos;
			Vector2<float> dir2 = aLine2.myEndPos - aLine2.myStartPos;

			float lengthDir1 = Vector2<float>::Length(dir1);
			dir1 /= lengthDir1;

			float lengthDir2 = Vector2<float>::Length(dir2);
			dir2 /= lengthDir2;

			if (Vector2<float>::Dot(dir1, aLine2.myEndPos - aLine1.myStartPos) < lengthDir1 &&
				Vector2<float>::Dot(dir1, aLine2.myEndPos - aLine1.myStartPos) > 0)
			{
				aIntersectionPoint = aLine2.myEndPos;
				return true;
			}

			if (Vector2<float>::Dot(dir1, aLine2.myStartPos - aLine1.myStartPos) < lengthDir1 &&
				Vector2<float>::Dot(dir1, aLine2.myStartPos - aLine1.myStartPos) > 0)
			{
				aIntersectionPoint = aLine2.myStartPos;
				return true;
			}

			if (Vector2<float>::Dot(dir2, aLine1.myEndPos - aLine2.myStartPos) < lengthDir2 &&
				Vector2<float>::Dot(dir2, aLine1.myEndPos - aLine2.myStartPos) > 0)
			{
				aIntersectionPoint = aLine1.myEndPos;
				return true;
			}

			if (Vector2<float>::Dot(dir2, aLine1.myStartPos - aLine2.myStartPos) < lengthDir2 &&
				Vector2<float>::Dot(dir2, aLine1.myStartPos - aLine2.myStartPos) > 0)
			{
				aIntersectionPoint = aLine1.myStartPos;
				return true;
			}

			return false;
		}

		aIntersectionPoint = Vector2<float>(numX / denX, numY / denY);


		Vector2<float> v1(aLine1.myEndPos - aLine1.myStartPos);

		Vector2<float> v1intersect(aIntersectionPoint - aLine1.myStartPos);

		if (Vector2<float>::Dot(v1, v1intersect) < 0 || Vector2<float>::Length2(v1intersect) > Vector2<float>::Length2(v1))
		{
			return false;
		}

		Vector2<float> v2(aLine2.myEndPos - aLine2.myStartPos);

		Vector2<float> v2intersect(aIntersectionPoint - aLine2.myStartPos);

		if (Vector2<float>::Dot(v2, v2intersect) < 0 || Vector2<float>::Length2(v2intersect) > Vector2<float>::Length2(v2))
		{
			return false;
		}

		return true;
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