#include <cmath>
#pragma region Constructors
template <typename T>
Vector2<T>::Vector2(void)
{
	x = 0;
	y = 0;
}

template <typename T>
Vector2<T>::Vector2(const Vector2 &aVector2)
{
	x = aVector2.x;
	y = aVector2.y;
}

template<typename T> 
template<typename U>
inline Vector2<T>::Vector2(const Vector2<U> &aVector2)
{
	x = aVector2.x;
	y = aVector2.y;
}

template <typename T>
Vector2<T>::Vector2(const T aX, const T aY)
{
	x = aX;
	y = aY;
}

#pragma endregion

#pragma region Operators

template <typename T>
Vector2<T>& Vector2<T>::operator=(const Vector2 &aRight)
{
	x = aRight.x;
	y = aRight.y;

	return *this;
}

template <typename T>
Vector2<T>& Vector2<T>::operator+=(const Vector2 &aRight)
{
	x = x + aRight.x;
	y = y + aRight.y;

	return *this;
}

template <typename T>
Vector2<T>& Vector2<T>::operator-=(const Vector2 &aRight)
{
	x = x - aRight.x;
	y = y - aRight.y;

	return *this;
}

template <typename T>
Vector2<T>& Vector2<T>::operator*=(const T aRight)
{
	x = x * aRight;
	y = y * aRight;

	return *this;
}

template <typename T>
Vector2<T>& Vector2<T>::operator/=(const T aRight)
{
	x = x / aRight;
	y = y / aRight;

	return *this;
}

template <typename T>
Vector2<T> operator+(const Vector2<T>& aVector, T aScalar)
{
	Vector2<T> result(aVector);
	result += aScalar;
	return result;
}
template <typename T>
Vector2<T> operator+(T aScalar, const Vector2<T>& aVector)
{
	return aVector + aScalar;
}
template <typename T>
Vector2<T> operator+=(Vector2<T>& aVector, T aScalar)
{
	aVector.x += aScalar;
	aVector.y += aScalar;
	return aVector;
}

template <typename T>
Vector2<T> operator-(const Vector2<T>& aVector, T aScalar)
{
	Vector2<T> result(aVector);
	result -= aScalar;
	return result;
}

template <typename T>
Vector2<T> operator-=(Vector2<T>& aVector, T aScalar)
{
	aVector.x -= aScalar;
	aVector.y -= aScalar;
	return aVector;
}

template <typename T>
Vector2<T> operator*(const Vector2<T>& aVector, T aScalar)
{
	Vector2<T> result(aVector);
	result *= aScalar;
	return result;
}
template <typename T>
Vector2<T> operator*(T aScalar, const Vector2<T>& aVector)
{
	return aVector * aScalar;
}
template <typename T>
Vector2<T> operator*=(Vector2<T>& aVector, T aScalar)
{
	aVector.x *= aScalar;
	aVector.y *= aScalar;
	return aVector;
}

template <typename T>
Vector2<T> operator/(const Vector2<T>& aVector, T aScalar)
{
	Vector2<T> result(aVector);
	result /= aScalar;
	return result;
}
template <typename T>
Vector2<T> operator/=(Vector2<T>& aVector, T aScalar)
{
	assert(aScalar != 0 && "Division by zero.");
	aVector.x /= aScalar;
	aVector.y /= aScalar;
	return aVector;
}
#pragma endregion

#pragma region Info

template <typename T>
T Vector2<T>::Length2(void) const
{
	return static_cast<T>((x * x) + (y * y));
}

template <typename T>
T Vector2<T>::Length(void) const
{
	return static_cast<T>(sqrt((x * x) + (y * y)));
}

template <typename T>
Vector2<T> Vector2<T>::GetNormalized(void) const
{
	T tempLength = Length();
	return Vector2<T>(x / tempLength, y / tempLength);
}

template <typename T>
T Vector2<T>::Dot(const Vector2<T> &aRight) const
{
	return ((x * aRight.x) + (y * aRight.y));
}

template <typename T>
T Vector2<T>::Dot(const Vector2<T> &aLeft, const Vector2<T> &aRight)
{
	return ((aLeft.x * aRight.x) + (aLeft.y * aRight.y));
}

#pragma endregion

#pragma region Manipulation

template <typename T>
Vector2<T>& Vector2<T>::Normalize(void)
{
	T tempLength = Length();
	x /= tempLength;
	y /= tempLength;
	return *this;
}

template <typename T>
Vector2<T> Vector2<T>::Normalize(Vector2 aVector)
{
	T tempLength = aVector.Length();
	aVector.x /= tempLength;
	aVector.y /= tempLength;
	return aVector;
}

template <typename T>
T Length2(const Vector2<T>& aVector)
{
	return Dot(aVector, aVector);
}
#pragma endregion





