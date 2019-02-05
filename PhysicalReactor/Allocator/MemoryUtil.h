#pragma once
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cassert>

#define ASSERT1(cond) assert(cond)
#define ASSERT2(cond, message) assert(message && cond)

#define GET_ASSERT_MACRO(_1,_2,NAME,...) NAME
#define ASSERT(...) GET_ASSERT_MACRO(__VA_ARGS__, ASSERT2, ASSERT1)(__VA_ARGS__)
namespace PRE
{


	inline void* add(void* p, size_t x)
	{
		return (void*)(reinterpret_cast<uintptr_t>(p) + x);
	}

	inline const void* add(const void* p, size_t x)
	{
		return (const void*)(reinterpret_cast<uintptr_t>(p) + x);
	}


	template<typename T, unsigned N>
	unsigned int size(const T(&)[N])
	{
		return N;
	}

	template<typename T ,unsigned int N, unsigned int M>
	unsigned int size(const T(&)[N][M])
	{
		return N*M;
	}

	template<typename T, unsigned int N>
	void fill(T(&buffer)[N], const T& value)
	{
		std::fill(buffer, buffer + N, value);
	}

	template<typename T, typename U>
	unsigned int offsetOf(U T::*member)
	{
		const long offset = (char*)&((T*)NULL->f*member) - (char*)NULL;
			assert(offset >= 0);
		return (unsigned int)offset;
	}

	template<typename T>
	bool isAligned(const void*p)
	{
		const unsigned int alignmnet = alignof(T);
		const unsigned int address = reinterpret_cast<unsigned int>(p);
		return 0 == (address%alignmnet);
	}

	inline bool isBigEndian()
	{
		union
		{
			uint32_t i;
			char c[4];
		}bint = { 0x01020304 };
		return bint.c[0] = 1;
	}

	inline bool isLittleEndian()
	{
		return !isBigEndian();
	}

	inline uint8_t* alignForward(uint8_t* address, uint32_t alignment)
	{
		const uint32_t a = alignment - 1;
		const uint32_t na = ~a;
		return (uint8_t*)((reinterpret_cast<uintptr_t>(address)&static_cast<uintptr_t>(alignment - 1)));
	}

	inline uint32_t alignForwardPadding(const uint8_t* address, uint32_t alignment)
	{
		unsigned int adjustment = alignment - (reinterpret_cast<uintptr_t>(address) & static_cast<uintptr_t>(alignment - 1));

		if (adjustment == alignment)
		{
			return 0;
		}
		return static_cast<uint32_t>(adjustment);
	}

	inline uint8_t alignForwardAdjustment(const void* address, uint8_t alignment)
	{
		uint8_t adjustment = alignment - (reinterpret_cast<uintptr_t>(address)& static_cast<uintptr_t>(alignment - 1));

		if (adjustment == alignment)
			return 0; //already aligned

		return adjustment;
	}

		inline bool isAligned(const void* address, uint8_t alignment)
		{
			return alignForwardAdjustment(address, alignment) == 0;
		}

		template<class T>
		inline bool isAligned(const T* address)
		{
			return alignForwardAdjustment(address, __alignof(T)) == 0;
		}

		inline void* alignForward(void* address, uint8_t alignment)
		{
			return (void*)((reinterpret_cast<uintptr_t>(address) + static_cast<uintptr_t>(alignment - 1)) & static_cast<uintptr_t>(~(alignment - 1)));
		}

		inline const void* alignForward(const void* address, uint8_t alignment)
		{
			return (void*)((reinterpret_cast<uintptr_t>(address) + static_cast<uintptr_t>(alignment - 1)) & static_cast<uintptr_t>(~(alignment - 1)));
		}

		inline void* alignBackward(void* address, uint8_t alignment)
		{
			return (void*)(reinterpret_cast<uintptr_t>(address)& static_cast<uintptr_t>(~(alignment - 1)));
		}

		inline const void* alignBackward(const void* address, uint8_t alignment)
		{
			return (void*)(reinterpret_cast<uintptr_t>(address)& static_cast<uintptr_t>(~(alignment - 1)));
		}

	
		template<typename T>
		inline uint8_t alignForwardAdjustmentWithHeader(const void* address, uint8_t alignment)
		{
			if (__alignof(T) > alignment)
				alignment = __alignof(T);

			uint8_t adjustment = sizeof(T) + alignForwardAdjustment(add(address, sizeof(T)), alignment);

			return adjustment;
		}

		inline uint8_t alignBackwardAdjustment(const void* address, uint8_t alignment)
		{
			uint8_t adjustment = reinterpret_cast<uintptr_t>(address)& static_cast<uintptr_t>(alignment - 1);

			if (adjustment == alignment)
				return 0; //already aligned

			return adjustment;
		}

	

		inline void* subtract(void* p, size_t x)
		{
			return (void*)(reinterpret_cast<uintptr_t>(p) - x);
		}

		inline const void* subtract(const void* p, size_t x)
		{
			return (const void*)(reinterpret_cast<uintptr_t>(p) - x);
		}

		inline void* align_top(void* p, unsigned int align)
		{
			assert(align >= 1);
			assert(align % 2 == 0 || align == 1);

			uintptr_t ptr = (uintptr_t)p;
			const unsigned int mod = ptr%align;

			if (mod)
			{
				ptr += align - mod;
			}

			return (void*)ptr;
		}

		template<typename T>
		inline void exchange(T& a, T& b)
		{
			T c = a;
			a = b;
			b = c;
		}
}