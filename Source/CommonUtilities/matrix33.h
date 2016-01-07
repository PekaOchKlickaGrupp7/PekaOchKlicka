#pragma once

#include "vector2.h"
#include "vector3.h"
#include <array>
#include <cmath>

// Forward declarations
//
template<typename TYPE>
class Matrix44;


template<typename TYPE>
class Matrix33
{
public:     // Constants to avoid using magic numbers
			//
			static const unsigned int   NumRows = 3,
										NumColumns = 3,
										NumElements = NumRows * NumColumns;

			// Initializes the matrix to the identity matrix
			//
			Matrix33( void );

			// Initializes the matrix with the data from anOther
			//
			Matrix33( const Matrix33 &anOther );

			// Initializes the matrix with the 3x3-part from anOther
			//
			Matrix33( const Matrix44<TYPE> &anOther );

			// Initializes the matrix with the static_cast'ed data from anOther which uses another base type
			//
			template<typename U>
			Matrix33( const Matrix33<U> &anOther );

			// Initializes the matrix with the data from an array
			//
			// Explanation:
			// This allows the syntax: Matrix33 identity({1, 0, 0, 0, 1, 0, 0, 0, 1});
			//
			Matrix33( const TYPE( &anArray )[NumElements] );

			// Rotates the upper left 2x2 part of the matrix by anAngle rad around the z-axis
			Matrix33 &Rotate2D(const TYPE anAngle);

			// Sets the upper left 2x2 part of the matrix to the rotation of anAngle rad. Will 
			// create the same values for the rotation-part as the CreateRotateAroundZ but 
			// leaves the other values untouched. 
			Matrix33 &Set2DRotation(const TYPE anAngle);

			// Sets the position part of a 2D transformation matrix 
			// 
			void SetPosition(const Vector2<TYPE> &aPosition);

			// Gets the position part of a 2D transformation matrix 
			// 
			Vector2<TYPE> GetPosition(void) const;

			// Initializes the matrix with plain values
			//
			Matrix33(
				const TYPE a11, const TYPE a12, const TYPE a13,
				const TYPE a21, const TYPE a22, const TYPE a23,
				const TYPE a31, const TYPE a32, const TYPE a33 );

			// Adds and assigns the values of aRight to this matrix
			//
			Matrix33 &operator +=( const Matrix33 &aRight );

			// Subtract and assigns the values of aRight to this matrix
			//
			Matrix33 &operator -=( const Matrix33 &aRight );

			// Multiplies this matrix with aRight
			//
			Matrix33 &operator *=( const Matrix33 &aRight );

			// Sets the values of this matrix to those of aRight
			//
			Matrix33 &operator =( const Matrix33 &aRight );

			// Transposes the matrix and returns *this
			//
			Matrix33 &Transpose( void );

			// Creates a transformation matrix for rotating anAngle rad around the x-axis
			//
			static Matrix33 CreateRotateAroundX( const TYPE anAngle );

			// Creates a transformation matrix for rotating anAngle rad around the y-axis
			//
			static Matrix33 CreateRotateAroundY( const TYPE anAngle );

			// Creates a transformation matrix for rotating anAngle rad around the z-axis
			//
			static Matrix33 CreateRotateAroundZ( const TYPE anAngle );

			// Returns a transposed copy of aMatrix
			//
			static Matrix33 Transpose( Matrix33 aMatrix );

			// The data of this matrix
			//
#pragma warning( suppress : 4201 ) // NOTE: This works fine in VC2013 and GCC so I'm keeping it. //MAHAM
			union
			{
				struct
				{
					TYPE    m11, m12, m13,
							m21, m22, m23,
							m31, m32, m33;
				};

				std::array<TYPE, NumElements> myMatrix;
			};

			// Pre-created matrices
			//
			static const Matrix33   Identity,   // Identity matrix
									Zero;       // Matrix with all 0
};

template<typename TYPE> const Matrix33<TYPE> Matrix33<TYPE>::Identity(1,0,0,
																	  0,1,0,
																	  0,0,1);
template<typename TYPE> const Matrix33<TYPE> Matrix33<TYPE>::Zero(0,0,0,
																  0,0,0,
																  0,0,0);

#include "matrix44.h"

template<typename TYPE>
Matrix33<TYPE>::Matrix33(void)
{
	myMatrix = Identity.myMatrix;
}

template<typename TYPE>
Matrix33<TYPE>::Matrix33(const Matrix33 &anOther)
{
	myMatrix = anOther.myMatrix;
}

template<typename TYPE>
Matrix33<TYPE>::Matrix33(const Matrix44<TYPE> &anOther)
{
	m11 = anOther.m11; m12 = anOther.m12; m13 = anOther.m13;
	m21 = anOther.m21; m22 = anOther.m22; m23 = anOther.m23;
	m31 = anOther.m31; m32 = anOther.m32; m33 = anOther.m33;
}

template<typename TYPE>
template<typename U>
Matrix33<TYPE>::Matrix33(const Matrix33<U> &anOther)
{
	m11 = anOther.m11; m12 = anOther.m12; m13 = anOther.m13;
	m21 = anOther.m21; m22 = anOther.m22; m23 = anOther.m23;
	m31 = anOther.m31; m32 = anOther.m32; m33 = anOther.m33;
}

template<typename TYPE>
Matrix33<TYPE>::Matrix33(const TYPE(&anArray)[NumElements])
{
	m11 = anArray[0]; m12 = anArray[1]; m13 = anArray[2];
	m21 = anArray[3]; m22 = anArray[4]; m23 = anArray[5];
	m31 = anArray[6]; m32 = anArray[7]; m33 = anArray[8];
}

template<typename TYPE>
Matrix33<TYPE>::Matrix33(
	const TYPE a11, const TYPE a12, const TYPE a13,
	const TYPE a21, const TYPE a22, const TYPE a23,
	const TYPE a31, const TYPE a32, const TYPE a33)
{
	m11 = a11; m12 = a12; m13 = a13;
	m21 = a21; m22 = a22; m23 = a23;
	m31 = a31; m32 = a32; m33 = a33;
}

template<typename TYPE>
Matrix33<TYPE>& Matrix33<TYPE>::operator +=(const Matrix33 &aRight)
{
	m11 += aRight.m11; m12 += aRight.m12; m13 += aRight.m13;
	m21 += aRight.m21; m22 += aRight.m22; m23 += aRight.m23;
	m31 += aRight.m31; m32 += aRight.m32; m33 += aRight.m33;

	return *this;
}

template<typename TYPE>
Matrix33<TYPE>& Matrix33<TYPE>::operator -=(const Matrix33 &aRight)
{
	m11 -= aRight.m11; m12 -= aRight.m12; m13 -= aRight.m13;
	m21 -= aRight.m21; m22 -= aRight.m22; m23 -= aRight.m23;
	m31 -= aRight.m31; m32 -= aRight.m32; m33 -= aRight.m33;

	return *this;
}

template<typename TYPE>
Matrix33<TYPE>& Matrix33<TYPE>::operator *=(const Matrix33 &aRight)
{
	Matrix33<TYPE> theMatrix(*this);
	m11 = theMatrix.m11 * aRight.m11 + theMatrix.m12 * aRight.m21 + theMatrix.m13 * aRight.m31;
	m12 = theMatrix.m11 * aRight.m12 + theMatrix.m12 * aRight.m22 + theMatrix.m13 * aRight.m32;
	m13 = theMatrix.m11 * aRight.m13 + theMatrix.m12 * aRight.m23 + theMatrix.m13 * aRight.m33;

	m21 = theMatrix.m21 * aRight.m11 + theMatrix.m22 * aRight.m21 + theMatrix.m23 * aRight.m31;
	m22 = theMatrix.m21 * aRight.m12 + theMatrix.m22 * aRight.m22 + theMatrix.m23 * aRight.m32;
	m23 = theMatrix.m21 * aRight.m13 + theMatrix.m22 * aRight.m23 + theMatrix.m23 * aRight.m33;

	m31 = theMatrix.m31 * aRight.m11 + theMatrix.m32 * aRight.m21 + theMatrix.m33 * aRight.m31;
	m32 = theMatrix.m31 * aRight.m12 + theMatrix.m32 * aRight.m22 + theMatrix.m33 * aRight.m32;
	m33 = theMatrix.m31 * aRight.m13 + theMatrix.m32 * aRight.m23 + theMatrix.m33 * aRight.m33;

	return *this;
}

template<typename TYPE>
Matrix33<TYPE>& Matrix33<TYPE>::operator =(const Matrix33 &aRight)
{
	m11 = aRight.m11; m12 = aRight.m12; m13 = aRight.m13;
	m21 = aRight.m21; m22 = aRight.m22; m23 = aRight.m23;
	m31 = aRight.m31; m32 = aRight.m32; m33 = aRight.m33;

	return *this;
}

template<typename TYPE>
Matrix33<TYPE>& Matrix33<TYPE>::Transpose(void)
{
	Matrix33<TYPE> TheMatrix(*this);

	m11 = TheMatrix.m11; m12 = TheMatrix.m21; m13 = TheMatrix.m31;
	m21 = TheMatrix.m12; m22 = TheMatrix.m22; m23 = TheMatrix.m32;
	m31 = TheMatrix.m13; m32 = TheMatrix.m23; m33 = TheMatrix.m33;

	return *this;
}

template<typename TYPE>
Matrix33<TYPE> Matrix33<TYPE>::CreateRotateAroundX(const TYPE anAngle)
{
	Matrix33<TYPE> TheMatrix;

	TheMatrix.m11 = 1;
	TheMatrix.m12 = 0;
	TheMatrix.m13 = 0;
	
	TheMatrix.m21 = 0;
	TheMatrix.m22 = cos(anAngle);
	TheMatrix.m23 = sin(anAngle);
	
	TheMatrix.m31 = 0;
	TheMatrix.m32 = -sin(anAngle);
	TheMatrix.m33 = cos(anAngle);
	

	return TheMatrix;
}

template<typename TYPE>
Matrix33<TYPE> Matrix33<TYPE>::CreateRotateAroundY(const TYPE anAngle)
{
	Matrix33<TYPE> TheMatrix;

	TheMatrix.m11 = cos(anAngle);
	TheMatrix.m12 = 0;
	TheMatrix.m13 = -sin(anAngle);

	TheMatrix.m21 = 0;
	TheMatrix.m22 = 1;
	TheMatrix.m23 = 0;

	TheMatrix.m31 = sin(anAngle);
	TheMatrix.m32 = 0;
	TheMatrix.m33 = cos(anAngle);

	return TheMatrix;
}

// Creates a transformation matrix for rotating anAngle rad around the z-axis
//
template<typename TYPE>
Matrix33<TYPE> Matrix33<TYPE>::CreateRotateAroundZ(const TYPE anAngle)
{
	Matrix33<TYPE> TheMatrix;

	TheMatrix.m11 = cos(anAngle);
	TheMatrix.m12 = sin(anAngle);
	TheMatrix.m13 = 0;

	TheMatrix.m21 = -sin(anAngle);
	TheMatrix.m22 = cos(anAngle);
	TheMatrix.m23 = 0;

	TheMatrix.m31 = 0;
	TheMatrix.m32 = 0;
	TheMatrix.m33 = 1;

	return TheMatrix;
}

// Returns a transposed copy of aMatrix
//
template<typename TYPE>
Matrix33<TYPE> Matrix33<TYPE>::Transpose(Matrix33 aMatrix)
{
	//aMatrix.m11 = m11; aMatrix.m12 = m21; aMatrix.m13 = m31;
	//aMatrix.m21 = m12; aMatrix.m22 = m22; aMatrix.m23 = m32;
	//aMatrix.m31 = m13; aMatrix.m32 = m23; aMatrix.m33 = m33;

	return aMatrix.Transpose();
}


//----------------------------------------------------


// Alias for Matrix33<float>. Add more if needed.
//
using Matrix33f = Matrix33<float>;


// Returns a new matrix which is the sum of aLeft and aRight
//
template<typename TYPE>
Matrix33<TYPE> operator +(Matrix33<TYPE> aLeft, const Matrix33<TYPE> &aRight)
{
	return aLeft += aRight;
}


// Returns a new matrix wich is the difference of aLeft and aRight
//
template<typename TYPE>
Matrix33<TYPE> operator -(Matrix33<TYPE> aLeft, const Matrix33<TYPE> &aRight)
{
	return aLeft -=  aRight;
}


// Returns a new matrix which is the product of aLeft and aRight
//
template<typename TYPE>
Matrix33<TYPE> operator *(Matrix33<TYPE> aLeft, const Matrix33<TYPE> &aRight)
{
	return aLeft *= aRight;
}


// Returns a new vector3 which is the product of the vector aLeft and the matrix aRight
//
template<typename TYPE>
Vector3<TYPE> operator *(Vector3<TYPE> aLeft, const Matrix33<TYPE> &aRight)
{
	Vector3<TYPE> TheVector;

	TheVector.x = aLeft.x * aRight.m11 + aLeft.y * aRight.m21 + aLeft.z * aRight.m31;
	TheVector.y = aLeft.x * aRight.m12 + aLeft.y * aRight.m22 + aLeft.z * aRight.m32;
	TheVector.z = aLeft.x * aRight.m13 + aLeft.y * aRight.m23 + aLeft.z * aRight.m33;

	return TheVector;
}


// Compares aLeft and aRight componentwise
//
template<typename TYPE>
bool operator ==(const Matrix33<TYPE> &aLeft, const Matrix33<TYPE> &aRight)
{
	return aLeft.m11 == aRight.m11 && aLeft.m12 == aRight.m12 && aLeft.m13 == aRight.m13 &&
			aLeft.m21 == aRight.m21 && aLeft.m22 == aRight.m22 && aLeft.m23 == aRight.m23 &&
			aLeft.m31 == aRight.m31 && aLeft.m32 == aRight.m32 && aLeft.m33 == aRight.m33;
}


// Returns the negated result of aLeft == aRight
//
template<typename TYPE>
bool operator !=(const Matrix33<TYPE> &aLeft, const Matrix33<TYPE> &aRight)
{
	return !(aLeft == aRight);
}

// Matrix uppgift 2

// Returns a new vector which is the product of the vector aLeft and the matrix 
// aRight. Make sure you convert aLeft to a Vector3 and set it's .z-value to 1 
///
template<typename TYPE>
Vector2<TYPE> operator *(Vector2<TYPE> aLeft, const Matrix33<TYPE> &aRight)
{
	Vector3<TYPE> theVector(aLeft);
	Vector2<TYPE> returnVector(theVector * aRight);
	return returnVector;
}

// Rotates the upper left 2x2 part of the matrix by anAngle rad around the z-axis
template<typename TYPE>
Matrix33<TYPE> &Matrix33<TYPE>::Rotate2D(const TYPE anAngle)
{
	/*m11 = cos(anAngle); m12 = sin(anAngle); m13 = 0;
	m21 = -sin(anAngle); m22 = cos(anAngle); m23 = 0;
	m31 = m31; m32 = m32; m33 = 1;*/

	(*this) *= (CreateRotateAroundZ(anAngle));
	return *this;
}

template<typename TYPE>
void Matrix33<TYPE>::SetPosition(const Vector2<TYPE> &aPosition)
{
	m31 = aPosition.x;
	m32 = aPosition.y;
}

template<typename TYPE>
Vector2<TYPE> Matrix33<TYPE>::GetPosition() const
{
	return Vector2<TYPE>(m31, m32);
}

// Sets the upper left 2x2 part of the matrix to the rotation of anAngle rad. Will 
// create the same values for the rotation-part as the CreateRotateAroundZ but 
// leaves the other values untouched. 
template<typename TYPE>
Matrix33<TYPE> &Matrix33<TYPE>::Set2DRotation(const TYPE anAngle)
{
	m11 = cos(anAngle); m12 = sin(anAngle);
	m21 = -sin(anAngle); m22 = cos(anAngle);

	return *this;
}