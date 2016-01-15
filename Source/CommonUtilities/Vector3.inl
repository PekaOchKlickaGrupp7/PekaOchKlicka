#pragma region Constructors
template <typename T>
Vector3<T>::Vector3(void)
{
	x = 0;
	y = 0;
	z = 0;
}

template <typename T>
Vector3<T>::Vector3(const Vector3 &aVector3)
{
	x = aVector3.x;
	y = aVector3.y;
	z = aVector3.z;
}

template<typename T>
inline Vector3<T>::Vector3(const Vector4<T> &aVector4)
{
	x = aVector4.x;
	y = aVector4.y;
	z = aVector4.z;
}

template<typename T>
template<typename U>
inline Vector3<T>::Vector3(const Vector3<U> &aVector3)
{
	x = aVector3.x;
	y = aVector3.y;
	z = aVector3.z;
}

template <typename T>
Vector3<T>::Vector3(const T aX, const T aY, const T aZ)
{
	x = aX;
	y = aY;
	z = aZ;
}

#pragma endregion

#pragma region Operators

template <typename T>
Vector3<T>& Vector3<T>::operator=(const Vector3 &aRight)
{
	x = aRight.x;
	y = aRight.y;
	z = aRight.z;

	return *this;
}

template <typename T>
Vector3<T>& Vector3<T>::operator=(const Vector4<T> &aRight)
{
	x = aRight.x;
	y = aRight.y;
	z = aRight.z;

	return *this;
}

template <typename T>
Vector3<T>& Vector3<T>::operator+=(const Vector3 &aRight)
{
	x = x + aRight.x;
	y = y + aRight.y;
	z = z + aRight.z;

	return *this;
}

template <typename T>
Vector3<T>& Vector3<T>::operator-=(const Vector3 &aRight)
{
	x = x - aRight.x;
	y = y - aRight.y;
	z = z - aRight.z;
	return *this;
}

template <typename T>
Vector3<T>& Vector3<T>::operator*=(const T aRight)
{
	x = x * aRight;
	y = y * aRight;
	z = z * aRight;
	return *this;
}

template <typename T>
Vector3<T>& Vector3<T>::operator/=(const T aRight)
{
	x = x / aRight;
	y = y / aRight;
	z = z / aRight;
	return *this;
}

#pragma endregion

#pragma region Info

template <typename T>
T Vector3<T>::Length2(void) const
{
	return static_cast<T>((x * x) + (y * y) + (z * z));
}

template <typename T>
T Vector3<T>::Length(void) const
{
	return static_cast<T>(sqrt((x * x) + (y * y) + (z * z)));
}

template <typename T>
Vector3<T> Vector3<T>::GetNormalized(void) const
{
	T tempLength = Length();
	return Vector3<T>(x / tempLength, y / tempLength, z / tempLength);
}

template <typename T>
T Vector3<T>::Dot(const Vector3<T> &aRight) const
{
	return ((x * aRight.x) + (y * aRight.y) + (z * aRight.z));
}

template <typename T>
T Vector3<T>::Dot(const Vector3<T> &aLeft, const Vector3<T> &aRight)
{
	return ((aLeft.x * aRight.x) + (aLeft.y * aRight.y) + (aLeft.z * aRight.z));
}

template <typename T>
T Length2(const Vector3<T>& aVector)
{
	return Dot(aVector, aVector);
}

template <typename T>
Vector3<T> Vector3<T>::Cross(const Vector3 &aRight) const
{
	Vector3 newVector;
	newVector.x = y * aRight.z - z * aRight.y;
	newVector.y = z * aRight.x - x * aRight.z;
	newVector.z = x * aRight.y - y * aRight.x;
	return newVector;
}

template <typename T>
Vector3<T> Vector3<T>::Cross(const Vector3 &aLeft, const Vector3 &aRight)
{
	Vector3 newVector;
	newVector.x = aLeft.y * aRight.z - aLeft.z * aRight.y;
	newVector.y = aLeft.z * aRight.x - aLeft.x * aRight.z;
	newVector.z = aLeft.x * aRight.y - aLeft.y * aRight.x;
	return newVector;
}

#pragma endregion

#pragma region Manipulation

template <typename T>
Vector3<T>& Vector3<T>::Normalize(void)
{
	T tempLength = Length();
	x /= tempLength;
	y /= tempLength;
	z /= tempLength;
	return *this;
}

template <typename T>
Vector3<T> Vector3<T>::Normalize(Vector3 aVector)
{
	T tempLength = aVector.Length();
	aVector.x /= tempLength;
	aVector.y /= tempLength;
	aVector.z /= tempLength;
	return aVector;
}

// operator with scalar

template <typename T>
Vector3<T> operator+(const Vector3<T>& aVector, T aScalar)
{
	Vector3<T> result(aVector);
	result += aScalar;
	return result;
}
template <typename T>
Vector3<T> operator+(T aScalar, const Vector3<T>& aVector)
{
	return aVector + aScalar;
}
template <typename T>
Vector3<T> operator+=(Vector3<T>& aVector, T aScalar)
{
	aVector.x += aScalar;
	aVector.y += aScalar;
	aVector.z += aScalar;
	return aVector;
}

template <typename T>
Vector3<T> operator-(const Vector3<T>& aVector, T aScalar)
{
	Vector3<T> result(aVector);
	result -= aScalar;
	return result;
}
template <typename T>
Vector3<T> operator-=(Vector3<T>& aVector, T aScalar)
{
	aVector.x -= aScalar;
	aVector.y -= aScalar;
	aVector.z -= aScalar;
	return aVector;
}

template <typename T>
Vector3<T> operator*(const Vector3<T>& aVector, T aScalar)
{
	Vector3<T> result(aVector);
	result *= aScalar;
	return result;
}
template <typename T>
Vector3<T> operator*(T aScalar, const Vector3<T>& aVector)
{
	return aVector * aScalar;
}
template <typename T>
Vector3<T> operator*=(Vector3<T>& aVector, T aScalar)
{
	aVector.x *= aScalar;
	aVector.y *= aScalar;
	aVector.z *= aScalar;
	return aVector;
}

template <typename T>
Vector3<T> operator/(const Vector3<T>& aVector, T aScalar)
{
	Vector3<T> result(aVector);
	result /= aScalar;
	return result;
}
template <typename T>
Vector3<T> operator/=(Vector3<T>& aVector, T aScalar)
{
	assert(aScalar != 0 && "Division by zero.");
	aVector.x /= aScalar;
	aVector.y /= aScalar;
	aVector.z /= aScalar;
	return aVector;
}

#pragma endregion





