#include <cassert>
#include "Macros.h"

namespace CommonUtilities
{
	template <typename T, int Capacity, typename CountType = unsigned short, bool UseSafeModeFlag = true>
	class VectorOnStack
	{
	public:
		VectorOnStack();
		VectorOnStack(const VectorOnStack& aVectorOnStack);

		~VectorOnStack();

		VectorOnStack & operator=(const VectorOnStack & aVectorOnStack);

		inline const T& operator[](const CountType & aIndex) const;
		inline T& operator[](const CountType & aIndex);

		inline void Add(const T& aObject);
		inline void Insert(CountType aIndex, T& aObject);
		inline void DeleteCyclic(T& aObject);
		inline void DeleteCyclicAtIndex(CountType aItemNumber);
		inline void RemoveCyclic(const T& aObject);
		inline void RemoveCyclicAtIndex(CountType aItemNumber);

		inline void Clear();
		inline void DeleteAll();

		inline CountType Size() const;
	private:
		int mySize = Capacity; // Define the size member variable.
		CountType myNumberOfOccupied;
		CountType myCountType;
		T myArray[Capacity];
	};

	template <typename T, int Capacity, typename CountType, bool UseSafeModeFlag>
	VectorOnStack<T, Capacity, CountType, UseSafeModeFlag>::VectorOnStack()
	{
		myNumberOfOccupied = 0;
    }

	template <typename T, int Capacity, typename CountType, bool UseSafeModeFlag>
	VectorOnStack<T, Capacity, CountType, UseSafeModeFlag>::VectorOnStack(const VectorOnStack& aVectorOnStack)
	{
		*this = aVectorOnStack;
	}

	template <typename T, int Capacity, typename CountType, bool UseSafeModeFlag>
	VectorOnStack<T, Capacity, CountType, UseSafeModeFlag>::~VectorOnStack()
	{
		mySize = 0;
		myNumberOfOccupied = 0;
	}

	template <typename T, int Capacity, typename CountType, bool UseSafeModeFlag>
	VectorOnStack<T, Capacity, CountType, UseSafeModeFlag>&
		VectorOnStack<T, Capacity, CountType, UseSafeModeFlag>::operator=(const VectorOnStack &aVectorOnStack)
	{
		if (UseSafeModeFlag == false)
		{
			memcpy(this, &aVectorOnStack, sizeof(VectorOnStack));
		}
		else
		{
			for (int i = 0; i < aVectorOnStack.mySize; i++)
			{
				myArray[i] = aVectorOnStack.myArray[i];
			}

			mySize = aVectorOnStack.mySize;
			myNumberOfOccupied = aVectorOnStack.myNumberOfOccupied;
		}
		return *this;
	}

	template <typename T, int Capacity, typename CountType, bool UseSafeModeFlag>
	inline T& VectorOnStack<T, Capacity, CountType, UseSafeModeFlag>::operator[](const CountType& aIndex)
	{
		assert(aIndex >= 0 && aIndex < mySize);
		return myArray[aIndex];
	}

	template <typename T, int Capacity, typename CountType, bool UseSafeModeFlag>
	const inline T& VectorOnStack<T, Capacity, CountType, UseSafeModeFlag>::operator[](const CountType& aIndex) const
	{
		assert(aIndex >= 0 && aIndex < myNumberOfOccupied);
		return myArray[aIndex];
	}

	template <typename T, int Capacity, typename CountType, bool UseSafeModeFlag>
	inline void VectorOnStack<T, Capacity, CountType, UseSafeModeFlag>::Add(const T& aObject)
	{
		assert(myNumberOfOccupied < mySize);
		myArray[myNumberOfOccupied] = aObject;
		myNumberOfOccupied += 1;
	}

	template <typename T, int Capacity, typename CountType, bool UseSafeModeFlag>
	inline void VectorOnStack<T, Capacity, CountType, UseSafeModeFlag>::Insert(CountType aIndex, T& aObject)
	{
		assert(myNumberOfOccupied <= mySize);
		assert(aIndex >= 0 && aIndex < myNumberOfOccupied);
		for (int i = mySize - 1; i >= aIndex; i--)
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

	template <typename T, int Capacity, typename CountType, bool UseSafeModeFlag>
	inline void VectorOnStack<T, Capacity, CountType, UseSafeModeFlag>::DeleteCyclic(T& aObject)
	{
		for (int i = 0; i < myNumberOfOccupied; i++) // this function will replace every intance of aObject. Should it only remove one instance of aObject?
		{
			if (myArray[i] == aObject)
			{
				SAFE_DELETE(myArray[i]);
				myArray[i] = myArray[myNumberOfOccupied - 1];
			}
		}
	}

	template <typename T, int Capacity, typename CountType, bool UseSafeModeFlag>
	inline void VectorOnStack<T, Capacity, CountType, UseSafeModeFlag>::DeleteCyclicAtIndex(CountType aItemNumber)
	{
		SAFE_DELETE(myArray[aItemNumber]);
		myArray[aItemNumber] = myArray[myNumberOfOccupied -1];
	}

	template <typename T, int Capacity, typename CountType, bool UseSafeModeFlag>
	inline void VectorOnStack<T, Capacity, CountType, UseSafeModeFlag>::RemoveCyclic(const T& aObject)
	{
		for (int i = 0; i < myNumberOfOccupied; i++)
		{
			if (myArray[i] == aObject)
			{
				myArray[i] = myArray[myNumberOfOccupied -1];
				myNumberOfOccupied -= 1;
			}
		}
	}

	template <typename T, int Capacity, typename CountType, bool UseSafeModeFlag>
	inline void VectorOnStack<T, Capacity, CountType, UseSafeModeFlag>::RemoveCyclicAtIndex(CountType aItemNumber)
	{
		assert(aItemNumber < myNumberOfOccupied - 1);
		myArray[aItemNumber] = myArray[myNumberOfOccupied - 1];
		myNumberOfOccupied -= 1;
	}

	template <typename T, int Capacity, typename CountType, bool UseSafeModeFlag>
	inline void VectorOnStack<T, Capacity, CountType, UseSafeModeFlag>::Clear()
	{
		myNumberOfOccupied = 0;
		mySize = 0;
	}

	template <typename T, int Capacity, typename CountType, bool UseSafeModeFlag>
	__forceinline CountType VectorOnStack<T, Capacity, CountType, UseSafeModeFlag>::Size() const
	{
		CountType newCountType = myNumberOfOccupied;
		return newCountType;
	}

	template <typename T, int Capacity, typename CountType, bool UseSafeModeFlag>
	inline void VectorOnStack<T, Capacity, CountType, UseSafeModeFlag>::DeleteAll()
	{
		for (int i = 0; i < myNumberOfOccupied; i++)
		{
			SAFE_DELETE(myArray[i]);
		}

		Clear();
	}
}
