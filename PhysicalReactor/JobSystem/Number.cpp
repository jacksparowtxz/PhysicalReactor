#include "Number.h"
#include <limits>


uint64_t Number::RoundUp(uint64_t numberToRound, uint64_t multipleOf) {
	if (multipleOf == 0) return numberToRound;

	uint64_t remainder = numberToRound % multipleOf;
	if (remainder == 0) return numberToRound;
	return numberToRound + multipleOf - remainder;
}

uint64_t Number::RoundUpToPowerOf2(uint64_t numToRound, uint64_t powerOf2) {
	//ASSERT(IsPowerOf2(powerOf2), "RoundUpToMultipleOf2: powerOf2 isn't a power of 2");
	return (numToRound + powerOf2 - 1) & ~(powerOf2 - 1);
}

uint64_t Number::MaxWithNBit(uint64_t numBits) {
	static const uint64_t MAX = std::numeric_limits<uint64_t>::max();
	return (MAX >> (sizeof(uint64_t) * 8 - numBits));
}

uint64_t Number::NextPowerOf2(uint64_t n)
{
	if (n < 1) return 0;
	--n;
	n |= n >> 1;
	n |= n >> 2;
	n |= n >> 4;
	n |= n >> 8;
	n |= n >> 16;
	n |= n >> 32;
	return n + 1;
}

