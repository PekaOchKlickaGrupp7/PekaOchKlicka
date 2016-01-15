#include <assert.h>
#pragma once

#define TEMPLATE template<typename ObjectType, typename SizeType = unsigned short>
#define TEMPLATE_ARGUMENTS ObjectType, SizeType


namespace CommonUtilities
{
	TEMPLATE
	class GrowingArray
	{
		public:
			GrowingArray();
			GrowingArray(SizeType aNrOfRecommendedItems, bool aUseSafeModeFlag = true);
			GrowingArray(const GrowingArray& aGrowingArray);
			GrowingArray(GrowingArray&& aGrowingArray);

			~GrowingArray();

			GrowingArray&operator=(const GrowingArray& aGrowingArray);
			GrowingArray&operator=(GrowingArray&& aGrowingArray);

			void Init(SizeType aNrOfRecomendedItems, bool aUseSafeModeFlag = true);
			void ReInit(SizeType aNrOfRecomendedItems, bool aUseSafeModeFlag = true);

			inline ObjectType&operator[](const SizeType& aIndex);
			inline const ObjectType&operator[](const SizeType& aIndex) const;

			inline void Add(const ObjectType& aObject);
			inline void Insert(SizeType aIndex, ObjectType& aObject);
			inline void DeleteCyclic(ObjectType& aObject);
			inline void DeleteCyclicAtIndex(SizeType aItemNumber);
			inline void RemoveCyclic(const ObjectType& aObject);
			inline void RemoveCyclicAtIndex(SizeType aItemNumber);
			inline SizeType Find(const ObjectType& aObject);

			inline ObjectType& GetLast();
			inline const ObjectType& GetLast() const;

			static const SizeType FoundNone = static_cast<SizeType>(-1);

			inline void RemoveAll();
			inline void DeleteAll();

			void Optimize();
			__forceinline SizeType Size() const;
			inline void Resize(SizeType aNewSize);
			inline void Reserve(SizeType aNewSize);

		private:

			ObjectType* myArray;

			SizeType myMaxSize;
			SizeType myNumberOfElements;

			bool myUseSafeMode;
			bool myIsInitiated;
	};



	TEMPLATE
	GrowingArray<TEMPLATE_ARGUMENTS>::GrowingArray()
	{
		myArray = nullptr;
		myNumberOfElements = 0;
		myMaxSize = 0;
		myUseSafeMode = true;
		myIsInitiated = false;
	}



	TEMPLATE
	GrowingArray<TEMPLATE_ARGUMENTS>::GrowingArray(SizeType aNrOfRecommendedItems, bool aUseSafeModeFlag = true)
	{
		myIsInitiated = false;
		Init(aNrOfRecommendedItems, aUseSafeModeFlag);
	}



	TEMPLATE
	GrowingArray<TEMPLATE_ARGUMENTS>::GrowingArray(const GrowingArray& aGrowingArray)
	{
		myArray = nullptr;
		myNumberOfElements = 0;
		myMaxSize = 0;
		myUseSafeMode = true;
		*this = aGrowingArray;
	}



	TEMPLATE
	GrowingArray<TEMPLATE_ARGUMENTS>::GrowingArray(GrowingArray&& aGrowingArray)
	{
		myArray = nullptr;
		myNumberOfElements = 0;
		myMaxSize = 0;
		myUseSafeMode = true;
		*this = aGrowingArray;
	}



	TEMPLATE
	GrowingArray<TEMPLATE_ARGUMENTS>::~GrowingArray()
	{
		delete[] myArray;
		myArray = nullptr;
		myIsInitiated = false;
	}



	TEMPLATE
	GrowingArray<TEMPLATE_ARGUMENTS>& GrowingArray<TEMPLATE_ARGUMENTS>::operator=(const GrowingArray& aGrowingArray)
	{

		delete[] myArray;
		myUseSafeMode = aGrowingArray.myUseSafeMode;
		myIsInitiated = aGrowingArray.myIsInitiated;
		if (aGrowingArray.myUseSafeMode == true)
		{
			myArray = new ObjectType[aGrowingArray.myMaxSize];
			for (SizeType i = 0; i < aGrowingArray.Size(); ++i)
			{
				myArray[i] = aGrowingArray.myArray[i];
			}
			myMaxSize = aGrowingArray.myMaxSize;
			myNumberOfElements = aGrowingArray.myNumberOfElements;
			
		}
		else
		{
			myArray = new ObjectType[aGrowingArray.myMaxSize];
			memcpy(myArray, aGrowingArray.myArray, (sizeof(ObjectType) * aGrowingArray.myNumberOfElements));
			myIsInitiated = aGrowingArray.myIsInitiated;
			myMaxSize = aGrowingArray.myMaxSize;
			myNumberOfElements = aGrowingArray.myNumberOfElements;
			myUseSafeMode = aGrowingArray.myUseSafeMode;
		}

		return *this;
	}



	TEMPLATE
	GrowingArray<TEMPLATE_ARGUMENTS>& GrowingArray<TEMPLATE_ARGUMENTS>::operator=(GrowingArray&& aGrowingArray)
	{
		

		delete[] myArray;
		myArray = aGrowingArray.myArray;
		aGrowingArray.myArray = nullptr;
		myIsInitiated = aGrowingArray.myIsInitiated;
		myMaxSize = aGrowingArray.myMaxSize;
		myNumberOfElements = aGrowingArray.myNumberOfElements;
		myUseSafeMode = aGrowingArray.myUseSafeMode;

		return (*this);
	}



	TEMPLATE
	void GrowingArray<TEMPLATE_ARGUMENTS>::Init(SizeType aNrOfRecomendedItems, bool aUseSafeModeFlag = true)
	{
		assert(myIsInitiated == false && " GROWING ARRAY ERROR  -  Trying to initiate a growing array twice! Use ReInit instead of Init.");
		myNumberOfElements = 0;
		myIsInitiated = true;
		myMaxSize = aNrOfRecomendedItems;
		myArray = new ObjectType[myMaxSize];
		myUseSafeMode = aUseSafeModeFlag;

	}



	TEMPLATE
	void GrowingArray<TEMPLATE_ARGUMENTS>::ReInit(SizeType aNrOfRecomendedItems, bool aUseSafeModeFlag = true)
	{
		assert(myIsInitiated && " GROWING ARRAY ERROR  -  Trying to use ReInit before Init! Initiate your array before reinitiating it!");
		myNumberOfElements = 0;
		delete[] myArray;
		myArray = nullptr;
		myIsInitiated = true;
		Init(aNrOfRecomendedItems, aUseSafeModeFlag);
	}



	TEMPLATE
	ObjectType& GrowingArray<TEMPLATE_ARGUMENTS>::operator[](const SizeType& aIndex)
	{
		assert(myIsInitiated && " GROWING ARRAY ERROR  -  Trying to do stuff with a GrowingArray without initiating it!");
		assert(aIndex >= 0 && aIndex < myNumberOfElements && "GrowingArray out of bounds.");

		return myArray[aIndex];
	}



	TEMPLATE
	const ObjectType& GrowingArray<TEMPLATE_ARGUMENTS>::operator[](const SizeType& aIndex) const
	{
		assert(myIsInitiated && " GROWING ARRAY ERROR  -  Trying to do stuff with a GrowingArray without initiating it!");
		assert(aIndex >= 0 && aIndex < myNumberOfElements && "GrowingArray out of bounds.");

		return myArray[aIndex];
	}



	TEMPLATE
	void GrowingArray<TEMPLATE_ARGUMENTS>::Add(const ObjectType& aObject)
	{
		assert(myIsInitiated && " GROWING ARRAY ERROR  -  Trying to do stuff with a GrowingArray without initiating it!");
		Resize(myNumberOfElements + 1);
		myArray[myNumberOfElements - 1] = aObject;
	}

	TEMPLATE
	void GrowingArray<TEMPLATE_ARGUMENTS>::Insert(SizeType aIndex, ObjectType& aObject)
	{
		assert(myIsInitiated && " GROWING ARRAY ERROR  -  Trying to do stuff with a GrowingArray without initiating it!");
		assert(aIndex >= 0 && aIndex < myNumberOfElements && "GrowingArray out of bounds.");
		

		for(int i = myNumberOfElements -1; i >= aIndex; i--)
		{
			if (i == aIndex)
			{
				myArray[i] = aObject;
				break;
			}
			else
			{
				myArray[i] = myArray[i - 1];
			}
		}

		Resize(myNumberOfElements + 1);
	}

	TEMPLATE
	void GrowingArray<TEMPLATE_ARGUMENTS>::DeleteCyclic(ObjectType& aObject)
	{
		assert(myIsInitiated && " GROWING ARRAY ERROR  -  Trying to do stuff with a GrowingArray without initiating it!");
		SizeType index = Find(aObject);
		if (index != FoundNone)
		{
			DeleteCyclicAtIndex(index);
		}

		/*for(int i = 0; i < myNumberOfElements; ++i)
		{
			if (myArray[i] == aObject)
			{
				delete myArray[i];
				myArray[i] = nullptr;

				if (i < myNumberOfElements - 1)
				{
					myArray[i] = myArray[myNumberOfElements - 1];
				}
			}
		}

		Resize(myNumberOfElements - 1);*/
	}

	TEMPLATE
	void GrowingArray<TEMPLATE_ARGUMENTS>::DeleteCyclicAtIndex(SizeType aItemNumber)
	{
		assert(myIsInitiated && " GROWING ARRAY ERROR  -  Trying to do stuff with a GrowingArray without initiating it!");
		assert(aItemNumber >= 0 && aItemNumber < myNumberOfElements);

		delete myArray[aItemNumber];
		myArray[aItemNumber] = nullptr;

		RemoveCyclicAtIndex(aItemNumber);
		/*if (aItemNumber < myNumberOfElements - 1)
		{
			myArray[aItemNumber] = myArray[myNumberOfElements - 1];
		}
		Resize(myNumberOfElements - 1);*/
	}

	TEMPLATE
	void GrowingArray<TEMPLATE_ARGUMENTS>::RemoveCyclic(const ObjectType& aObject)
	{
		assert(myIsInitiated && " GROWING ARRAY ERROR  -  Trying to do stuff with a GrowingArray without initiating it!");
		SizeType index = Find(aObject);
		if (index != FoundNone)
		{
			RemoveCyclicAtIndex(index);
		}

		/*for(int i = 0; i < myNumberOfElements; ++i)
		{
			if (myArray[i] == aObject)
			{
				if (i < myNumberOfElements - 1)
				{
					myArray[i] = myArray[myNumberOfElements - 1];
				}
			}
		}
		Resize(myNumberOfElements - 1);*/
	}

	TEMPLATE
	void GrowingArray<TEMPLATE_ARGUMENTS>::RemoveCyclicAtIndex(SizeType aItemNumber)
	{
		assert(myIsInitiated && " GROWING ARRAY ERROR  -  Trying to do stuff with a GrowingArray without initiating it!");
		assert(aItemNumber >= 0 && aItemNumber < myNumberOfElements && "Removing Index out of the boundry of growingArray");

		myArray[aItemNumber] = myArray[myNumberOfElements - 1];

		Resize(myNumberOfElements - 1);
	}

	TEMPLATE
	SizeType GrowingArray<TEMPLATE_ARGUMENTS>::Find(const ObjectType& aObject)
	{
		assert(myIsInitiated && " GROWING ARRAY ERROR  -  Trying to do stuff with a GrowingArray without initiating it!");
		for (SizeType i = 0; i < myNumberOfElements; ++i)
		{
			if (myArray[i] == aObject)
			{
				return i;
			}
		}

		return FoundNone;
	}

	TEMPLATE
	void GrowingArray<TEMPLATE_ARGUMENTS>::RemoveAll()
	{
		assert(myIsInitiated && " GROWING ARRAY ERROR  -  Trying to do stuff with a GrowingArray without initiating it!");
		myNumberOfElements = 0;
	}

	TEMPLATE
	void GrowingArray<TEMPLATE_ARGUMENTS>::DeleteAll()
	{
		assert(myIsInitiated && " GROWING ARRAY ERROR  -  Trying to do stuff with a GrowingArray without initiating it!");
		for (SizeType i = 0; i < myNumberOfElements; ++i)
		{
			delete myArray[i];
			myArray[i] = nullptr;
		}

		RemoveAll();
	}


	TEMPLATE
	void GrowingArray<TEMPLATE_ARGUMENTS>::Resize(SizeType aNewSize)
	{
		assert(myIsInitiated && " GROWING ARRAY ERROR  -  Trying to do stuff with a GrowingArray without initiating it!");
		assert(myMaxSize != 0 && " GROWING ARRAY ERROR  -  myMaxSize cannot be 0!");
		//assert(myMaxSize != -myMaxSize && " GROWING ARRAY ERROR  -  myMaxSize cannot be 0 or negativ!");
		
		while (aNewSize>myMaxSize)
		{
			Reserve(myMaxSize * 2);
		}
		myNumberOfElements = aNewSize;
	}

	TEMPLATE
	void GrowingArray<TEMPLATE_ARGUMENTS>::Reserve(SizeType aNewSize)
	{
		assert(myIsInitiated && " GROWING ARRAY ERROR  -  Trying to do stuff with a GrowingArray without initiating it!");
		assert(aNewSize >= myMaxSize && "GrowingArray; reserve lower than size.");

		ObjectType* newArray = new ObjectType[aNewSize];

		for (SizeType i = 0; i < myNumberOfElements; ++i)
		{
			newArray[i] = myArray[i];
		}
		
		myMaxSize = aNewSize;
		delete[] myArray;
		//myArray = nullptr;

		myArray = newArray;
		newArray = nullptr;
	}

	TEMPLATE
	void GrowingArray<TEMPLATE_ARGUMENTS>::Optimize()
	{
		assert(myIsInitiated && " GROWING ARRAY ERROR  -  Trying to do stuff with a GrowingArray without initiating it!");
		Reserve(myNumberOfElements);
	}

	TEMPLATE
	SizeType GrowingArray<TEMPLATE_ARGUMENTS>::Size() const
	{
		assert(myIsInitiated && " GROWING ARRAY ERROR  -  Trying to do stuff with a GrowingArray without initiating it!");
		return myNumberOfElements;
	}

	TEMPLATE
	ObjectType& GrowingArray<TEMPLATE_ARGUMENTS>::GetLast()
	{
		assert(myIsInitiated && " GROWING ARRAY ERROR  -  Trying to do stuff with a GrowingArray without initiating it!");
		assert(myNumberOfElements > 0 && "GrowingArray out of bounds.");
		return myArray[myNumberOfElements - 1];
	}

	TEMPLATE
	const ObjectType& GrowingArray<TEMPLATE_ARGUMENTS>::GetLast() const
	{
		assert(myIsInitiated && " GROWING ARRAY ERROR  -  Trying to do stuff with a GrowingArray without initiating it!");
		assert(myNumberOfElements > 0 && "GrowingArray out of bounds.");
		return myArray[myNumberOfElements - 1];
	}
};
