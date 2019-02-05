#pragma once
#include <cstdint>


class Number {
public:
	static inline constexpr bool IsPowerOf2(uint64_t n) { return n != 0 && (n & (n - 1)) == 0; }
	static uint64_t RoundUp(uint64_t numToRound, uint64_t multipleOf);
	static uint64_t RoundUpToPowerOf2(uint64_t numToRound, uint64_t powerOf2);
	static uint64_t MaxWithNBit(uint64_t numBits);
	static uint64_t NextPowerOf2(uint64_t n);
};

