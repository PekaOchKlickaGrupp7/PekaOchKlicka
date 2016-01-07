#pragma once


#include "vector4.h"
#include <array>
#include <cmath>

// Forward declarations
//
template<typename TYPE>
class Matrix33;


template<typename TYPE>
class Matrix44
{
public:     // Constants to avoid using magic numbers
	//
	static const unsigned int   NumRows = 4,
		NumColumns = 4,
		NumElements = NumRows * NumColumns;

	// Initializes the matrix to the identity matrix
	//
	Matrix44(void);

	// Initializes the matrix with the data from anOther
	//
	Matrix44(const Matrix44 &anOther);

	// Initializes the matrix with the 3x3-part from anOther
	//
	Matrix44(const Matrix33<TYPE> &anOther);

	// Initializes the matrix with the static_cast'ed data from anOther which uses another base type
	//
	template<typename U>
	Matrix44(const Matrix44<U> &anOther);

	// Sets the position part of a 2D transformation matrix 
	// 
	void SetPosition(const Vector3<TYPE> &aPosition);
	void SetPosition(const Vector4<TYPE> &aPosition);

	// Initializes the matrix with the data from an array
	//
	// Explanation:
	// This allows the syntax: Matrix44 identity({1, 0, 0, 0, 1, 0, 0, 0, 1});
	//
	Matrix44(const TYPE(&anArray)[NumElements]);

	// Initializes the matrix with plain values
	//
	Matrix44(
		const TYPE a11, const TYPE a12, const TYPE a13, const TYPE a14,
		const TYPE a21, const TYPE a22, const TYPE a23, const TYPE a24,
		const TYPE a31, const TYPE a32, const TYPE a33, const TYPE a34,
		const TYPE a41, const TYPE a42, const TYPE a43, const TYPE a44);

	// Adds and assigns the values of aRight to this matrix
	//
	Matrix44 &operator +=(const Matrix44 &aRight);

	// Subtract and assigns the values of aRight to this matrix
	//
	Matrix44 &operator -=(const Matrix44 &aRight);

	// Multiplies this matrix with aRight
	//
	Matrix44 &operator *=(const Matrix44 &aRight);

	// Sets the values of this matrix to those of aRight
	//
	Matrix44 &operator =(const Matrix44 &aRight);

	// Transposes the matrix and returns *this
	//
	Matrix44 &Transpose(void);

	/*Vector3<TYPE> GetPosition(void) const;*/
	Vector3<TYPE> Matrix44<TYPE>::GetPosition(void) const;
	Vector3<TYPE> Matrix44<TYPE>::GetForwardVector(void) const;
	Vector3<TYPE> Matrix44<TYPE>::GetRightVector(void) const;
	Vector3<TYPE> Matrix44<TYPE>::GetUpVector(void) const;

	// Creates a transformation matrix for rotating anAngle rad around the x-axis
	//
	static Matrix44 CreateRotateAroundX(const TYPE anAngle);

	// Creates a transformation matrix for rotating anAngle rad around the y-axis
	//
	static Matrix44 CreateRotateAroundY(const TYPE anAngle);

	// Creates a transformation matrix for rotating anAngle rad around the z-axis
	//
	static Matrix44 CreateRotateAroundZ(const TYPE anAngle);

	static Matrix44<TYPE> CreateProjectionMatrixLH(TYPE aNearZ, TYPE aFarZ, TYPE aAspectRatio, TYPE aFovAngle);

	// Returns a transposed copy of aMatrix
	//
	static Matrix44 Transpose(Matrix44 aMatrix);

	// The data of this matrix
	//
#pragma warning( suppress : 4201 ) // NOTE: This works fine in VC2013 and GCC so I'm keeping it. //MAHAM
	union
	{
		struct
		{
			TYPE    m11, m12, m13, m14,
				m21, m22, m23, m24,
				m31, m32, m33, m34,
				m41, m42, m43, m44;
		};

		std::array<TYPE, NumElements> myMatrix;
	};

	// Pre-created matrices
	//
	static const Matrix44   Identity,   // Identity matrix
		Zero;       // Matrix with all 0
};

template<typename TYPE> const Matrix44<TYPE> Matrix44<TYPE>::Identity(
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1);

template<typename TYPE> const Matrix44<TYPE> Matrix44<TYPE>::Zero(
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0);

#include "matrix33.h"

template<typename TYPE>
Matrix44<TYPE>::Matrix44(void)
{
	myMatrix = Identity.myMatrix;
}

template<typename TYPE>
Matrix44<TYPE>::Matrix44(const Matrix44 &anOther)
{
	myMatrix = anOther.myMatrix;
}

template<typename TYPE>
Matrix44<TYPE>::Matrix44(const Matrix33<TYPE> &anOther)
{
	m11 = anOther.m11; m12 = anOther.m12; m13 = anOther.m13; m14 = 1;
	m21 = anOther.m21; m22 = anOther.m22; m23 = anOther.m23; m24 = 1;
	m31 = anOther.m31; m32 = anOther.m32; m33 = anOther.m33; m34 = 1;
	m41 = 1; m42 = 1; m43 = 1; m44 = 1;
}

template<typename TYPE>
template<typename U>
Matrix44<TYPE>::Matrix44(const Matrix44<U> &anOther)
{
	m11 = anOther.m11; m12 = anOther.m12; m13 = anOther.m13; m14 = anOther.m14;
	m21 = anOther.m21; m22 = anOther.m22; m23 = anOther.m23; m24 = anOther.m24;
	m31 = anOther.m31; m32 = anOther.m32; m33 = anOther.m33; m34 = anOther.m34;
	m41 = anOther.m41; m42 = anOther.m42; m43 = anOther.m43; m44 = anOther.m44;
}

template<typename TYPE>
Matrix44<TYPE>::Matrix44(const TYPE(&anArray)[NumElements])
{
	m11 = anArray[0]; m12 = anArray[1]; m13 = anArray[2]; m14 = anArray[3];
	m21 = anArray[4]; m22 = anArray[5]; m23 = anArray[6]; m24 = anArray[7];
	m31 = anArray[8]; m32 = anArray[9]; m33 = anArray[10]; m34 = anArray[11];
	m41 = anArray[12]; m42 = anArray[13]; m43 = anArray[14]; m44 = anArray[15];

}
//
//template<typename TYPE>
//Vector3<TYPE> Matrix44<TYPE>::GetPosition() const
//{
//	return Vector3<TYPE>(m41, m42, m43);
//}

template<typename TYPE>
void Matrix44<TYPE>::SetPosition(const Vector3<TYPE> &aPosition)
{
	m41 = aPosition.x;
	m42 = aPosition.y;
	m43 = aPosition.z;
}

template<typename TYPE>
void Matrix44<TYPE>::SetPosition(const Vector4<TYPE> &aPosition)
{
	m41 = aPosition.x;
	m42 = aPosition.y;
	m43 = aPosition.z;
	m44 = aPosition.w;
}

template<typename TYPE>
Matrix44<TYPE>::Matrix44(
	const TYPE a11, const TYPE a12, const TYPE a13, const TYPE a14,
	const TYPE a21, const TYPE a22, const TYPE a23, const TYPE a24,
	const TYPE a31, const TYPE a32, const TYPE a33, const TYPE a34,
	const TYPE a41, const TYPE a42, const TYPE a43, const TYPE a44)
{
	m11 = a11; m12 = a12; m13 = a13; m14 = a14;
	m21 = a21; m22 = a22; m23 = a23; m24 = a24;
	m31 = a31; m32 = a32; m33 = a33; m34 = a34;
	m41 = a41; m42 = a42; m43 = a43; m44 = a44;
}

template<typename TYPE>
Matrix44<TYPE>& Matrix44<TYPE>::operator +=(const Matrix44 &aRight)
{
	m11 += aRight.m11; m12 += aRight.m12; m13 += aRight.m13; m14 += aRight.m14;
	m21 += aRight.m21; m22 += aRight.m22; m23 += aRight.m23; m24 += aRight.m24;
	m31 += aRight.m31; m32 += aRight.m32; m33 += aRight.m33; m34 += aRight.m34;
	m41 += aRight.m41; m42 += aRight.m42; m43 += aRight.m43; m44 += aRight.m44;

	return *this;
}

template<typename TYPE>
Matrix44<TYPE>& Matrix44<TYPE>::operator -=(const Matrix44 &aRight)
{
	m11 -= aRight.m11; m12 -= aRight.m12; m13 -= aRight.m13; m14 -= aRight.m14;
	m21 -= aRight.m21; m22 -= aRight.m22; m23 -= aRight.m23; m24 -= aRight.m24;
	m31 -= aRight.m31; m32 -= aRight.m32; m33 -= aRight.m33; m34 -= aRight.m34;
	m41 -= aRight.m41; m42 -= aRight.m42; m43 -= aRight.m43; m44 -= aRight.m44;

	return *this;
}

template<typename TYPE>
Matrix44<TYPE>& Matrix44<TYPE>::operator *=(const Matrix44 &aRight)
{
	Matrix44<TYPE> theMatrix(*this);
	m11 = theMatrix.m11 * aRight.m11 + theMatrix.m12 * aRight.m21 + theMatrix.m13 * aRight.m31 + theMatrix.m14 * aRight.m41;
	m12 = theMatrix.m11 * aRight.m12 + theMatrix.m12 * aRight.m22 + theMatrix.m13 * aRight.m32 + theMatrix.m14 * aRight.m42;
	m13 = theMatrix.m11 * aRight.m13 + theMatrix.m12 * aRight.m23 + theMatrix.m13 * aRight.m33 + theMatrix.m14 * aRight.m43;
	m14 = theMatrix.m11 * aRight.m14 + theMatrix.m12 * aRight.m24 + theMatrix.m13 * aRight.m34 + theMatrix.m14 * aRight.m44;

	m21 = theMatrix.m21 * aRight.m11 + theMatrix.m22 * aRight.m21 + theMatrix.m23 * aRight.m31 + theMatrix.m24 * aRight.m41;
	m22 = theMatrix.m21 * aRight.m12 + theMatrix.m22 * aRight.m22 + theMatrix.m23 * aRight.m32 + theMatrix.m24 * aRight.m42;
	m23 = theMatrix.m21 * aRight.m13 + theMatrix.m22 * aRight.m23 + theMatrix.m23 * aRight.m33 + theMatrix.m24 * aRight.m43;
	m24 = theMatrix.m21 * aRight.m14 + theMatrix.m22 * aRight.m24 + theMatrix.m23 * aRight.m34 + theMatrix.m24 * aRight.m44;

	m31 = theMatrix.m31 * aRight.m11 + theMatrix.m32 * aRight.m21 + theMatrix.m33 * aRight.m31 + theMatrix.m34 * aRight.m41;
	m32 = theMatrix.m31 * aRight.m12 + theMatrix.m32 * aRight.m22 + theMatrix.m33 * aRight.m32 + theMatrix.m34 * aRight.m42;
	m33 = theMatrix.m31 * aRight.m13 + theMatrix.m32 * aRight.m23 + theMatrix.m33 * aRight.m33 + theMatrix.m34 * aRight.m43;
	m34 = theMatrix.m31 * aRight.m14 + theMatrix.m32 * aRight.m24 + theMatrix.m33 * aRight.m34 + theMatrix.m34 * aRight.m44;

	m41 = theMatrix.m41 * aRight.m11 + theMatrix.m42 * aRight.m21 + theMatrix.m43 * aRight.m31 + theMatrix.m44 * aRight.m41;
	m42 = theMatrix.m41 * aRight.m12 + theMatrix.m42 * aRight.m22 + theMatrix.m43 * aRight.m32 + theMatrix.m44 * aRight.m42;
	m43 = theMatrix.m41 * aRight.m13 + theMatrix.m42 * aRight.m23 + theMatrix.m43 * aRight.m33 + theMatrix.m44 * aRight.m43;
	m44 = theMatrix.m41 * aRight.m14 + theMatrix.m42 * aRight.m24 + theMatrix.m43 * aRight.m34 + theMatrix.m44 * aRight.m44;

	return *this;
}

template<typename TYPE>
Matrix44<TYPE>& Matrix44<TYPE>::operator =(const Matrix44 &aRight)
{
	m11 = aRight.m11; m12 = aRight.m12; m13 = aRight.m13; m14 = aRight.m14;
	m21 = aRight.m21; m22 = aRight.m22; m23 = aRight.m23; m24 = aRight.m24;
	m31 = aRight.m31; m32 = aRight.m32; m33 = aRight.m33; m34 = aRight.m34;
	m41 = aRight.m41; m42 = aRight.m42; m43 = aRight.m43; m44 = aRight.m44;

	return *this;
}

template<typename TYPE>
Matrix44<TYPE>& Matrix44<TYPE>::Transpose(void)
{
	Matrix44<TYPE> TheMatrix(*this);

	m11 = TheMatrix.m11; m12 = TheMatrix.m21; m13 = TheMatrix.m31; m14 = TheMatrix.m41;
	m21 = TheMatrix.m12; m22 = TheMatrix.m22; m23 = TheMatrix.m32; m24 = TheMatrix.m42;
	m31 = TheMatrix.m13; m32 = TheMatrix.m23; m33 = TheMatrix.m33; m34 = TheMatrix.m43;
	m41 = TheMatrix.m14; m42 = TheMatrix.m24; m43 = TheMatrix.m34; m44 = TheMatrix.m44;

	return *this;
}

template<typename TYPE>
Matrix44<TYPE> Matrix44<TYPE>::CreateRotateAroundX(const TYPE anAngle)
{
	Matrix44<TYPE> TheMatrix;

	TheMatrix.m22 = cos(anAngle);
	TheMatrix.m23 = sin(anAngle);

	TheMatrix.m32 = -sin(anAngle);
	TheMatrix.m33 = cos(anAngle);


	return TheMatrix;
}

template<typename TYPE>
Matrix44<TYPE> Matrix44<TYPE>::CreateRotateAroundY(const TYPE anAngle)
{
	Matrix44<TYPE> TheMatrix;

	TheMatrix.m11 = cos(anAngle);
	TheMatrix.m13 = -sin(anAngle);

	TheMatrix.m31 = sin(anAngle);
	TheMatrix.m33 = cos(anAngle);

	return TheMatrix;
}

// Creates a transformation matrix for rotating anAngle rad around the z-axis
//
template<typename TYPE>
Matrix44<TYPE> Matrix44<TYPE>::CreateRotateAroundZ(const TYPE anAngle)
{
	Matrix44<TYPE> TheMatrix;

	TheMatrix.m11 = cos(anAngle);
	TheMatrix.m12 = sin(anAngle);

	TheMatrix.m21 = -sin(anAngle);
	TheMatrix.m22 = cos(anAngle);

	return TheMatrix;
}

// Returns a transposed copy of aMatrix
//
template<typename TYPE>
Matrix44<TYPE> Matrix44<TYPE>::Transpose(Matrix44 aMatrix)
{
	//aMatrix.m11 = m11; aMatrix.m12 = m21; aMatrix.m13 = m31;
	//aMatrix.m21 = m12; aMatrix.m22 = m22; aMatrix.m23 = m32;
	//aMatrix.m31 = m13; aMatrix.m32 = m23; aMatrix.m33 = m33;

	return aMatrix.Transpose();
}


//----------------------------------------------------



template<typename TYPE>
Vector3<TYPE> Matrix44<TYPE>::GetPosition(void) const
{
	return Vector3<TYPE>(m41, m42, m43);
}
template<typename TYPE>
Vector3<TYPE> Matrix44<TYPE>::GetForwardVector(void) const
{
	return Vector3<TYPE>(m31, m32, m33);
}
template<typename TYPE>
Vector3<TYPE> Matrix44<TYPE>::GetRightVector(void) const
{
	return Vector3<TYPE>(m11, m12, m13);
}
template<typename TYPE>
Vector3<TYPE> Matrix44<TYPE>::GetUpVector(void) const
{
	return Vector3<TYPE>(m21, m22, m23);
}



// Alias for Matrix44<float>. Add more if needed.
//
using Matrix44f = Matrix44<float>;


// Returns a new matrix which is the sum of aLeft and aRight
//
template<typename TYPE>
Matrix44<TYPE> operator +(Matrix44<TYPE> aLeft, const Matrix44<TYPE> &aRight)
{
	return aLeft += aRight;
}


// Returns a new matrix wich is the difference of aLeft and aRight
//
template<typename TYPE>
Matrix44<TYPE> operator -(Matrix44<TYPE> aLeft, const Matrix44<TYPE> &aRight)
{
	return aLeft -= aRight;
}


// Returns a new matrix which is the product of aLeft and aRight
//
template<typename TYPE>
Matrix44<TYPE> operator *(Matrix44<TYPE> aLeft, const Matrix44<TYPE> &aRight)
{
	return aLeft *= aRight;
}


template <typename T>
Vector3<T> operator*(const Vector3<T>& aVector, const Matrix44<T>& aMatrix)
{
	return Vector3<T>(
		aVector.x * aMatrix.myMatrix[0] + aVector.y * aMatrix.myMatrix[4] + aVector.z * aMatrix.myMatrix[8],
		aVector.x * aMatrix.myMatrix[1] + aVector.y * aMatrix.myMatrix[5] + aVector.z * aMatrix.myMatrix[9],
		aVector.x * aMatrix.myMatrix[2] + aVector.y * aMatrix.myMatrix[6] + aVector.z * aMatrix.myMatrix[10]);
}

// Returns a new vector3 which is the product of the vector aLeft and the matrix aRight
//
template<typename TYPE>
Vector4<TYPE> operator *(Vector4<TYPE> aLeft, const Matrix44<TYPE> &aRight)
{
	return Vector4<TYPE>(
		aLeft.x * aRight.m11 + aLeft.y * aRight.m21 + aLeft.z * aRight.m31 + aLeft.w * aRight.m41,
		aLeft.x * aRight.m12 + aLeft.y * aRight.m22 + aLeft.z * aRight.m32 + aLeft.w * aRight.m42,
		aLeft.x * aRight.m13 + aLeft.y * aRight.m23 + aLeft.z * aRight.m33 + aLeft.w * aRight.m43,
		aLeft.x * aRight.m14 + aLeft.y * aRight.m24 + aLeft.z * aRight.m34 + aLeft.w * aRight.m44);
}


// Compares aLeft and aRight componentwise
//
template<typename TYPE>
bool operator ==(const Matrix44<TYPE> &aLeft, const Matrix44<TYPE> &aRight)
{
	return
		aLeft.m11 == aRight.m11 && aLeft.m12 == aRight.m12 && aLeft.m13 == aRight.m13 && aLeft.m14 == aRight.m14 &&
		aLeft.m21 == aRight.m21 && aLeft.m22 == aRight.m22 && aLeft.m23 == aRight.m23 && aLeft.m24 == aRight.m24 &&
		aLeft.m31 == aRight.m31 && aLeft.m32 == aRight.m32 && aLeft.m33 == aRight.m33 && aLeft.m34 == aRight.m34 &&
		aLeft.m41 == aRight.m41 && aLeft.m42 == aRight.m42 && aLeft.m43 == aRight.m43 && aLeft.m44 == aRight.m44;
}


// Returns the negated result of aLeft == aRight
//
template<typename TYPE>
bool operator !=(const Matrix44<TYPE> &aLeft, const Matrix44<TYPE> &aRight)
{
	return !(aLeft == aRight);
}


template<typename TYPE>
Matrix44<TYPE> Matrix44<TYPE>::CreateProjectionMatrixLH(TYPE aNearZ, TYPE aFarZ, TYPE aAspectRatio, TYPE aFovAngle)
{
	Matrix44 temp;
	TYPE    SinFov;
	TYPE    CosFov;
	TYPE    Height;
	TYPE    Width;

	SinFov = sin(0.5f * aFovAngle);
	CosFov = cos(0.5f * aFovAngle);

	Height = CosFov / SinFov;
	Width = Height / aAspectRatio;

	TYPE scaling = aFarZ / (aFarZ - aNearZ);

	//temp.myVectorRows[0].Set(Width, 0.0f, 0.0f, 0.0f);
	//temp.myVectorRows[1].Set(0.0f, Height, 0.0f, 0.0f);
	//temp.myVectorRows[2].Set(0.0f, 0.0f, scaling, -scaling * aNearZ);
	//temp.myVectorRows[3].Set(0.0f, 0.0f, 1.0f, 0.0f);

	temp.myMatrix[0] = Width;
	temp.myMatrix[5] = Height;
	temp.myMatrix[10] = scaling;
	temp.myMatrix[11] = 1.f;
	temp.myMatrix[14] = -scaling * aNearZ;
	temp.myMatrix[15] = 0;

	return temp;
}