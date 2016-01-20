#pragma once
#include "Vector.h"

namespace CommonUtilities
{
	template <typename T>
	class Plane
	{
	public:
		Plane();
		Plane(Vector3<T> aFirstPoint, Vector3<T> aSecondPoint, Vector3<T> aThirdPoint);
		Plane(Vector3<T> aPoint, Vector3<T> aNormal);
		~Plane();
		bool operator==(const Plane<T>& aPlane);
		bool operator!=(const Plane<T>& aPlane);
		void InitWith3Points(Vector3<T> aFirstPoint, Vector3<T> aSecondPoint, Vector3<T> aThirdPoint);
		void InitWithPointAndNormal(Vector3<T> aPoint, Vector3<T> aDirection);
		bool Inside(Vector3<T> aPosition);
		Vector3<T> GetNormal();

		Vector4<T> myABCD;
	};
}

namespace CommonUtilities
{
	template <typename T>
	Plane<T>::Plane()
	{
	}

	template <typename T>
	Plane<T>::Plane(Vector3<T> aFirstPoint, Vector3<T> aSecondPoint, Vector3<T> aThirdPoint)
	{
		InitWith3Points(aFirstPoint, aSecondPoint, aThirdPoint);
	}

	template <typename T>
	Plane<T>::Plane(Vector3<T> aFirstPoint, Vector3<T> aNormal)
	{
		InitWithPointAndNormal(aFirstPoint, aNormal);
	}

	template <typename T>
	bool Plane<T>::operator==(const Plane<T>& aPlane)
	{
		if (myABCD == aPlane.myABCD)
		{
			return true;
		}
		return false;
	}

	template <typename T>
	bool Plane<T>::operator!=(const Plane<T>& aPlane)
	{
		return !(*this == aPlane);
	}

	template <typename T>
	Plane<T>::~Plane()
	{
	}

	template <typename T>
	void Plane<T>::InitWith3Points(Vector3<T> aFirstPoint, Vector3<T> aSecondPoint, Vector3<T> aThirdPoint)
	{
		Vector3<T> normal = Cross(aSecondPoint - aFirstPoint, aThirdPoint - aSecondPoint);

		myABCD.x = normal.x;
		myABCD.y = normal.y;
		myABCD.z = normal.z;
		myABCD.w = Dot(aFirstPoint, normal);
	}

	template <typename T>
	void Plane<T>::InitWithPointAndNormal(Vector3<T> aPoint, Vector3<T> aDirection)
	{
		myABCD.x = aDirection.x;
		myABCD.y = aDirection.y;
		myABCD.z = aDirection.z;
		myABCD.w = Dot(aPoint, Vector3<T>(myABCD.x, myABCD.y, myABCD.z));
	}

	template <typename T>
	bool Plane<T>::Inside(Vector3<T> aPosition)
	{
		if (Dot(aPosition, Vector3<T>(myABCD.x, myABCD.y, myABCD.z)) < myABCD.w)
		{
			return true;
		}
		return false;
	}

	template <typename T>
	Vector3<T> Plane<T>::GetNormal()
	{
		return Vector3<T>(myABCD.x, myABCD.y, myABCD.z);
	}
}