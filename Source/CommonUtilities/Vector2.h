#pragma once
template <typename T>
class Vector2
{
public:
	// Construction
	//
	Vector2(void);
	Vector2(const Vector2 &aVector2);
	template<typename U> explicit Vector2(const Vector2<U> &aVector2);
	Vector2(const T anX, const T aY);

	// Arithmetic
	//
	friend Vector2 operator +(Vector2 aLeft, const Vector2 &aRight)
	{
		return aLeft += aRight;
	}


	friend Vector2 operator -(Vector2 aLeft, const Vector2 &aRight)
	{
		return aLeft -= aRight;
	}


	friend Vector2 operator *(Vector2 aLeft, const T aRight)
	{
		return aLeft *= aRight;
	}


	friend Vector2 operator /(Vector2 aLeft, const T aRight)
	{
		return aLeft /= aRight;
	}

	friend Vector2 operator -(Vector2 aRight)
	{
		return aRight *= -1;
	}

	// Comparison
	//
	friend bool operator ==(const Vector2 &aLeft, const Vector2 &aRight)
	{
		if (aLeft.x == aRight.x)
		{
			if (aLeft.y == aRight.y)
			{
				return true;
			}
		}
		return false;
	}


	friend bool operator !=(const Vector2 &aLeft, const Vector2 &aRight)
	{
		if (aLeft.x == aRight.x && aLeft.y == aRight.y)
		{
			return false;
		}
		return true;
	}

	// Assignment
	//
	Vector2 &operator =(const Vector2 &aRight);

	// Compound assignment
	//
	Vector2 &operator +=(const Vector2 &aRight);
	Vector2 &operator -=(const Vector2 &aRight);
	Vector2 &operator *=(const T aRight);
	Vector2 &operator /=(const T aRight);

	// Info
	//
	T Length(void) const;
	T Length2(void) const;
	T Dot(const Vector2<T> &aRight) const;
	static T Length2(const Vector2<T>& aVector);
	static T Length(const Vector2<T>& aVector);
	Vector2 GetNormalized(void) const;
	T Cross(const Vector2<T>& aRightValue) const;


	// Manipulation
	//
	Vector2 &Normalize(void);

	union
	{
		T   x,
			u;
	};

	union
	{
		T   y,
			v;
	};

	static T Dot(const Vector2 &aFirstVector, const Vector2 &aSecondVector);
	static Vector2 Normalize(Vector2 aVector);

	static const Vector2	Zero,
		UnitX,
		UnitY,
		One;
};

using Vector2c = Vector2<char>;
using Vector2i = Vector2<int>;
using Vector2ui = Vector2<unsigned int>;
using Vector2f = Vector2<float>;
using Vector2d = Vector2<double>;

using Point2c = Vector2<char>;
using Point2i = Vector2<int>;
using Point2ui = Vector2<unsigned int>;
using Point2f = Vector2<float>;
using Point2d = Vector2<double>;

template<typename T> const Vector2<T> Vector2<T>::Zero(0, 0);
template<typename T> const Vector2<T> Vector2<T>::UnitX(1, 0);
template<typename T> const Vector2<T> Vector2<T>::UnitY(0, 1);
template<typename T> const Vector2<T> Vector2<T>::One(1, 1);

#include "Vector2.inl"