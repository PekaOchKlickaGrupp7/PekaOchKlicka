#pragma once


// Forward declarations
//
template<typename T>
class Vector4;


//
// Vector3
//

template <typename T>
class Vector3
{
public:
	// Construction
	//
	Vector3(void);
	Vector3(const Vector3 &aVector3);
	explicit Vector3(const Vector4<T> &aVector4);
	template<typename U> explicit Vector3(const Vector3<U> &aVector3);
	Vector3(const T anX, const T aY, const T aZ);

	// Arithmetic
	//
	friend Vector3 operator +(Vector3 aLeft, const Vector3 &aRight)
	{
		return aLeft += aRight;
	}


	friend Vector3 operator -(Vector3 aLeft, const Vector3 &aRight)
	{
		return aLeft -= aRight;
	}


	friend Vector3 operator *(Vector3 aLeft, const T aRight)
	{
		return aLeft *= aRight;
	}


	friend Vector3 operator /(Vector3 aLeft, const T aRight)
	{
		return aLeft /= aRight;
	}

	friend Vector3 operator -(Vector3 aRight)
	{
		return aRight *= -1;
	}

	// Comparison
	//
	friend bool operator ==(const Vector3 &aLeft, const Vector3 &aRight)
	{
		if (aLeft.x == aRight.x)
		{
			if (aLeft.y == aRight.y)
			{
				if (aLeft.z == aRight.z)
				{
					return true;
				}
			}
		}
		return false;
	}


	friend bool operator !=(const Vector3 &aLeft, const Vector3 &aRight)
	{
		if (aLeft.x == aRight.x && aLeft.y == aRight.y && aLeft.z == aRight.z)
		{
			return false;
		}
		return true;
	}

	// Assignment
	//
	Vector3 &operator =(const Vector3 &aRight);
	Vector3 &operator =(const Vector4<T> &aRight);

	// Compound assignment
	//
	Vector3 &operator +=(const Vector3 &aRight);
	Vector3 &operator -=(const Vector3 &aRight);
	Vector3 &operator *=(const T aRight);
	Vector3 &operator /=(const T aRight);

	// Info
	//
	T Length(void) const;
	T Length2(void) const;
	static T Length2(const Vector3<T>& aVector);
	T Dot(const Vector3<T> &aRight) const;
	Vector3 Cross(const Vector3 &aRight) const;
	Vector3 GetNormalized(void) const;

	// Manipulation
	//
	Vector3 &Normalize(void);

	union
	{
		T   x,
			u,
			r;
	};

	union
	{
		T   y,
			v,
			g;
	};

	union
	{
		T   z,
			w,
			b;
	};

	static T Dot(const Vector3 &aFirstVector, const Vector3 &aSecondVector);
	static Vector3 Cross(const Vector3 &aLeft, const Vector3 &aRight);
	static Vector3 Normalize(Vector3 aVector);

	static const Vector3	Zero,
							UnitX,
							UnitY,
							UnitZ,
							One;
};

using Vector3c = Vector3<char>;
using Vector3i = Vector3<int>;
using Vector3ui = Vector3<unsigned int>;
using Vector3f = Vector3<float>;
using Vector3d = Vector3<double>;

using Point3c = Vector3<char>;
using Point3i = Vector3<int>;
using Point3ui = Vector3<unsigned int>;
using Point3f = Vector3<float>;
using Point3d = Vector3<double>;

template<typename TYPE> const Vector3<TYPE> Vector3<TYPE>::Zero(0, 0, 0);
template<typename TYPE> const Vector3<TYPE> Vector3<TYPE>::UnitX(1, 0, 0);
template<typename TYPE> const Vector3<TYPE> Vector3<TYPE>::UnitY(0, 1, 0);
template<typename TYPE> const Vector3<TYPE> Vector3<TYPE>::UnitZ(0, 0, 1);
template<typename TYPE> const Vector3<TYPE> Vector3<TYPE>::One(1, 1, 1);

#include "Vector3.inl"