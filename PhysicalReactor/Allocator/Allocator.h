#pragma once
#include "MemoryUtil.h"


#include <new>
#include <functional>

namespace PRE
{
	static const uint8_t ADEFAULT_ALIGNMENT = 8;

	class Allocator
	{
	public:
		Allocator(size_t size);
		virtual ~Allocator();

		virtual void* allocate(size_t size, uint8_t alignment = ADEFAULT_ALIGNMENT) = 0;

		virtual void deallocate(void* p) = 0;

		//virtual void check(void* p) = 0;

		size_t getSize() const;
		size_t getUsedMemory() const;
		size_t getNumAllocations() const;

	protected:
		Allocator(const Allocator&);
		Allocator& operator=(Allocator&);

		size_t        _size;

		size_t        _used_memory;
		size_t        _num_allocations;
	};

	namespace allocatorFC
	{
		template <class T, class... Args>
		T* allocateNew(Allocator& allocator, Args&&... args)
		{
			return new (allocator.allocate(sizeof(T), __alignof(T))) T(std::forward<Args>(args)...);
		}

		template <class T>
		void deallocateDelete(Allocator& allocator, T* object)
		{
			object->~T();
			allocator.deallocate(object);
		}

		template <class T>
		T* allocateArray(Allocator& allocator, size_t length)
		{
			ASSERT(length != 0);

			uint8_t header_size = sizeof(size_t) / sizeof(T);

			if (sizeof(size_t) % sizeof(T) > 0)
				header_size += 1;

			//Allocate extra space to store array length in the bytes before the array
			T* p = ((T*)allocator.allocate(sizeof(T)*(length + header_size), __alignof(T))) + header_size;

			*(((size_t*)p) - 1) = length;

			for (size_t i = 0; i < length; i++)
				new (&p[i]) T;

			return p;
		}

		template <class T>
		T* allocateArrayNoConstruct(Allocator& allocator, size_t length)
		{
			ASSERT(length != 0);

			uint8_t header_size = sizeof(size_t) / sizeof(T);

			if (sizeof(size_t) % sizeof(T) > 0)
				header_size += 1;

			//Allocate extra space to store array length in the bytes before the array
			T* p = ((T*)allocator.allocate(sizeof(T)*(length + header_size), __alignof(T))) + header_size;

			*(((size_t*)p) - 1) = length;

			return p;
		}

		template <class T>
		void deallocateArray(Allocator& allocator, T* array)
		{
			ASSERT(array != nullptr);

			size_t length = *(((size_t*)array) - 1);

			//should be reverse order
			for (size_t i = 0; i < length; i++)
				array[i].~T();

			//Calculate how much extra memory was allocated to store the length before the array
			uint8_t header_size = sizeof(size_t) / sizeof(T);

			if (sizeof(size_t) % sizeof(T) > 0)
				header_size += 1;

			allocator.deallocate(array - header_size);
		}

		template <class T>
		void deallocateArrayNoDestruct(Allocator& allocator, T* array)
		{
			ASSERT(array != nullptr);

			//Calculate how much extra memory was allocated to store the length before the array
			uint8_t header_size = sizeof(size_t) / sizeof(T);

			if (sizeof(size_t) % sizeof(T) > 0)
				header_size += 1;

			allocator.deallocate(array - header_size);
		}
	};


};