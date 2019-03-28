#pragma once
#include <stdint.h>






namespace PRE
{


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



