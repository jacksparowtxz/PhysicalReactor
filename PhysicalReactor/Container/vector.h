#pragma once
#include "Container/GameType.h"
#include "Allocator/Allocator.h"


#include <xutility>
namespace PRE
{
	template<typename T>
	struct  Vector
	{
		ALLOCATOR_AWARE;

	    Allocator* allocator;
		uint32_t mCapacity;
		uint32_t mSize;
		T* data;
		

		Vector(Allocator& alloc,uint32_t capacity=0):
			allocator(&alloc),
			mCapacity(capacity),
			mSize(0),
			data(capacity>0 ? (T*)alloc.allocate(sizeof(T)*capacity,__alignof(T)) : nullptr)
		{
			
		}

		Vector(const Vector<T>& ohs):
			allocator(ohs.allocator),
			mCapacity(ohs.mCapacity),
			mSize(ohs.mSize),
			,data(ohs.mCapacity > 0 ? (T*)ohs.allocator->allocate(sizeof(T)*ohs._capacity, __alignof(T)) : nullptr)
		{
		     
			std::copy(ohs.data, ohs.data + size, data);
		
		}
		~Vector()
		{
		    for (uint32_t i=0;i<mSize;++i)
		    {
				data[i].~T();
		    }
			if (data!=nullptr)
			{
				allocator->deallocate(data);
			}	
		}
		T& operator[](uint32_t index)
		{
			return data[index];
		}
		const T& operator[](uint32_t index) const
		{
			return data[index];
		}
		const Vector<T>& operator=(const Vector<T> &other)
		{
			uint32_t size = other.Size;
			Resize(size);

			for (uint32_t i=0;i<size;++i)
			{
				new(&data[i]) T(other.data[i]);
			}

			return *this;
		
		}

		bool Empty()
		{
			return mSize == 0;
		}
		uint32_t Size()
		{
			return mSize;
		}
		uint32_t Capacity()
		{
			return mCapacity;
		}

		void Resize(uint32_t size)
		{
		     if (size>mSize)
		     {
				 SetCapacity(size);
		     }
			 mSize = size;
		
		}
		void Reserve(uint32_t capacity)
		{
		    if (capacity>mCapacity)
		    {
				Grow(capacity);
		    }
		}
		void SetCapacity(uint32_t capacity)
		{
		    if (capacity==mCapacity)
		    {
				return;
		    }

			if (capacity<mSize)
			{
				Resize(capacity);
			}

			if (capacity>0)
			{
				T* tmp = data;
				mCapacity = capacity;
				data = (T*)allocator->allocate(capacity * sizeof(T), alignof(T));

				for (uint32_t i = 0; i < mSize; ++i)
				{
					new (data + i)T(tmp[i]);
				}

				for(uint32_t i=0;i<mSize;++i)
				{
					tmp[i].~T();

				}
				if (data!=nullptr)
				{
					allocator->deallocate(data);
				}
				
			}
		
		
		
		}
		void Grow(uint32_t capacity)
		{
			uint32_t new_capacity = mCapacity * 2 + 1;
			if (new_capacity<capacity)
			{
				new_capacity = capacity;
			}
			SetCapacity(new_capacity);
		}
		void Condese()
		{
			Resize(mSize);
		}


		uint32_t Push_Back(const T& item)
		{
		     if (mCapacity==mSize)
		     {
				 Grow(0);
		     }

			 PRE::construct<T>(data + mSize, *allocator, IS_ALLOCATOR_AWARE_TYPE(T)());
			 data[mSize] = item;
			 return mSize++;

		}
		void Pop_Back()
		{
			data[mSize - 1].~T();
			--mSize;
		}
		
		void clear()
		{
		    for (uint32_t i=0;i<mSize;++i)
		    {
				data[i].~T();
		    }

			mSize = 0;
		}
		T* Begin()
		{
			return data;
		}
		T* End()
		{
			return data + size;
		}
		T& front()
		{
			return data[0];
		}
		T& Back() 
		{
			return data[mSize - 1];
		}



	};

}


