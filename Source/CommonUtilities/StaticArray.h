#include <cassert>
#include "Macros.h"

namespace CommonUtilities
{
	template <typename T, int Size>
	class StaticArray
	{
	public:
		StaticArray();
		StaticArray(const StaticArray& aStaticArray);

		~StaticArray();

		StaticArray& operator=(const StaticArray& aStaticArray);

		inline const T& operator[](const int& aIndex) const;
		inline T& operator[](const int& aIndex);


		// Utility functions
		inline void Insert(int aIndex, T& aObject);
		inline void DeleteAll();
	private:
		T myArray[Size];
	};

	template <typename T, int Size>
	StaticArray<T, Size>::StaticArray()
	{

	}

	template <typename T, int Size>
	StaticArray<T, Size>::~StaticArray()
	{

	}

	template <typename T, int Size>
	StaticArray<T, Size>::StaticArray(const StaticArray& aStaticArray)
	{
		for (int i = 0; i < Size; i++)
		{
			myArray[i] = aStaticArray[i];
		}
	}

	template <typename T, int Size>
	StaticArray<T, Size>& StaticArray<T, Size>::operator=(const StaticArray& aStaticArray)
	{
		for (int i = 0; i < Size; i++)
		{
			myArray[i] = aStaticArray[i];
		}
		return *this;
	}

	template <typename T, int Size>
	inline T& StaticArray<T, Size>::operator[](const int& aIndex)
	{
		assert(aIndex >= 0 && aIndex < Size);
		return myArray[aIndex];
	}

	template <typename T, int Size>
	const inline T& StaticArray<T, Size>::operator[](const int& aIndex) const
	{
		assert(aIndex >= 0 && aIndex < Size);
		return myArray[aIndex];
	}

	template <typename T, int Size>
	inline void StaticArray<T, Size>::Insert(int aIndex, T& aObject)
	{
		for (int i = Size-1; i >= aIndex; i--)
		{
			if (i == aIndex)
			{
				myArray[i] = aObject;
			}
			else
			{
				myArray[i] = myArray[i - 1];
			}
		}
	}

	template <typename T, int Size>
	inline void StaticArray<T, Size>::DeleteAll()
	{
		for (int i = 0; i < Size; i++)
		{
			SAFE_DELETE(myArray[i]); // deletes element and sets to nullptr
		}
	}
}