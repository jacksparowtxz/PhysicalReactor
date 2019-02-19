#pragma once
#include <stdint.h>
#include "Allocator/Allocator.h"

#define ALLOCATOR_AWARE typedef int allocator_aware



namespace PRE
{


template <int v>
struct Int2Type { enum { value = v }; };
#define IS_ALLOCATOR_AWARE(T) is_allocator_aware<T>::value
#define IS_ALLOCATOR_AWARE_TYPE(T) Int2Type< IS_ALLOCATOR_AWARE(T) >
template <typename T>
inline T &construct(void *p, Allocator& a, Int2Type<true>)
{
	new (p) T(a); 
    return *(T *)p;
}

template <typename T> 
inline T &construct(void *p, Allocator& /*a*/, Int2Type<false>)
{ 
	new (p) T;
    return *(T *)p;
}

template <typename T> 
inline T &construct(void *p, Allocator& a)
{ 
	return construct<T>(p, a, IS_ALLOCATOR_AWARE_TYPE(T)() );
}



template<typename T>
struct is_allocator_aware {
	template<typename C>
	static char testfun(typename C::allocator_aware*);

	template<typename C>
	static int testfun(...);

	enum 
	{
		value = (sizeof(testfun<T>(0)) == sizeof(char))
	};

};

#define IS_ALLOCATOR_AWARE(T) is_allocator_aware<T>::value


template<typename T>
struct equal_to
{
	bool operator()(const T& a, const T& b) const
	{
		return a == b;
	};
};


template<typename T>
struct not_equal_to
{
	bool operator()(const T&a, const T& b) const
	{
		return a != b;
	};

};


template <typename T>
struct greater
{
	bool operator()(const T& a, const T& b) const
	{
		return a > b;
	};
};

template<typename T>
struct less
{
	bool operator()(const T& a, const T& b) const
	{
		return a < b;
	};
};

template<typename T>
struct greater_equal
{
	bool operator()(const T& a, const T& b) const
	{
		return a >= b;
	};
};

template<typename T>
struct less_equal
{
	bool operator()(const T& a, const T& b) const
	{
		return a <= b;
	};
};
template<typename T>
struct hash;

template<>
struct hash<bool>
{
	uint32_t operator()(const bool val) const
	{
		return (uint32_t)val;
	}
};

template<>
struct hash<int8_t>
{
	uint32_t operator()(const int8_t val) const
	{
		return (uint32_t)val;
	}
};

template<>
struct hash<uint8_t>
{

	uint32_t operator()(const uint8_t val) const {
		return (uint32_t)val;
	}

};


template<>
struct hash<int16_t>
{

	uint32_t operator()(const int16_t val) const
	{
		return (uint32_t)val;
	}

};


template<>
struct hash<uint16_t>
{
	uint32_t operator()(const uint16_t val) const
	{
		return (uint32_t)val;
	}

};

template<>
struct hash<int32_t>
{
	uint32_t operator()(const int32_t val) const
	{
		return (uint32_t)val;
	}
};

template<>
struct hash<uint32_t>
{
	uint32_t operator()(const uint32_t val) const
	{
		return (uint32_t)val;
	}
};


template<>
struct hash<int64_t>
{
	uint32_t operator()(const int64_t val) const
	{
		return (uint32_t)val;
	}
};

template<>
struct hash<uint64_t>
{
	uint32_t operator()(const uint64_t val) const
	{
		return (uint32_t)val;
	}
};

/*
template<>
struct hash<float>
{
	uint32_t operator()(const float val) const
	{
		return val == 0.0f ? 0 : murmur(&val, sizeof(val), 0);
	}
};

template<>
struct hash<double>
{
	uint32_t operator()(const double val) const
	{
		return val == 0.0 ? 0 : murmur(&val, sizeof(val), 0);
	}
};*/



}



