#pragma region Constructors
template <typename T>
Vector4<T>::Vector4(void)
{
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}

template <typename T>
Vector4<T>::Vector4(const Vector4 &aVector4)
{
	x = aVector4.x;
	y = aVector4.y;
	z = aVector4.z;
	w = aVector4.w;
}

template <typename T>
inline Vector4<T>::Vector4(const Vector3<T> &anOther)
{
	x = anOther.x;
	y = anOther.y;
	z = anOther.z;
	w = 1;
}

template<typename T>
template<typename U>
inline Vector4<T>::Vector4(const Vector4<U> &anOther)
{
	x = anOther.x;
	y = anOther.y;
	z = anOther.z;
	w = anOther.w;
}

template <typename T>
Vector4<T>::Vector4(const T aX, const T aY, const T aZ, const T aW)
{
	x = aX;
	y = aY;
	z = aZ;
	w = aW;
}

#pragma endregion

#pragma region Operators

template <typename T>
Vector4<T>& Vector4<T>::operator=(const Vector4 &aRight)
{
	x = aRight.x;
	y = aRight.y;
	z = aRight.z;
	w = aRight.w;
	return *this;
}

template <typename T>
Vector4<T>& Vector4<T>::operator=(const Vector3<T> &aRight)
{
	x = aRight.x;
	y = aRight.y;
	z = aRight.z;
	w = 1;
	return *this;
}

template <typename T>
Vector4<T>& Vector4<T>::operator+=(const Vector4 &aRight)
{
	x = x + aRight.x;
	y = y + aRight.y;
	z = z + aRight.z;
	w = w + aRight.w;
	return *this;
}

template <typename T>
Vector4<T>& Vector4<T>::operator-=(const Vector4 &aRight)
{
	x = x - aRight.x;
	y = y - aRight.y;
	z = z - aRight.z;
	w = w - aRight.w;
	return *this;
}

template <typename T>
Vector4<T>& Vector4<T>::operator*=(const T aRight)
{
	x = x * aRight;
	y = y * aRight;
	z = z * aRight;
	w = w * aRight;
	return *this;
}

template <typename T>
Vector4<T>& Vector4<T>::operator/=(const T aRight)
{
	x = x / aRight;
	y = y / aRight;
	z = z / aRight;
	w = w / aRight;
	return *this;
}

#pragma endregion

#pragma region Info

template <typename T>
T Vector4<T>::Length2(void) const
{
	return static_cast<T>((x * x) + (y * y) + (z * z) + (w * w));
}

template <typename T>
T Vector4<T>::Length(void) const
{
	return static_cast<T>(sqrt((x * x) + (y * y) + (z * z) + (w * w)));
}

template <typename T>
Vector4<T> Vector4<T>::GetNormalized(void) const
{
	T tempLength = Length();
	return Vector4<T>(x / tempLength, y / tempLength, z / tempLength, w / tempLength);
}

template <typename T>
T Vector4<T>::Dot(const Vector4<T> &aRight) const
{
	return ((x * aRight.x) + (y * aRight.y) + (z * aRight.z) + (w * aRight.w));
}

template <typename T>
T Vector4<T>::Dot(const Vector4<T> &aLeft, const Vector4<T> &aRight)
{
	return ((aLeft.x * aRight.x) + (aLeft.y * aRight.y) + (aLeft.z * aRight.z) + (aLeft.w * aRight.w));
}

#pragma endregion

#pragma region Manipulation

template <typename T>
Vector4<T>& Vector4<T>::Normalize(void)
{
	T tempLength = Length();
	x /= tempLength;
	y /= tempLength;
	z /= tempLength;
	w /= tempLength;
	return *this;
}

template <typename T>
Vector4<T> Vector4<T>::Normalize(Vector4 aVector)
{
	T tempLength = aVector.Length();
	aVector.x /= tempLength;
	aVector.y /= tempLength;
	aVector.z /= tempLength;
	aVector.w /= tempLength;
	return aVector;
}

#pragma endregion





