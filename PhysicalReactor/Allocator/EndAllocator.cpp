#include "EndAllocator.h"
#include "MemoryUtil.h"

using namespace PRE;

EndAllocator::EndAllocator(size_t size, void* start)
	: FixedLinearAllocator(size, start)
{
}

EndAllocator::~EndAllocator()
{}

void* EndAllocator::allocate(size_t size, uint8_t alignment)
{
	ASSERT(size != 0 && alignment != 0);

	void* address =subtract(_current_pos, size);

	uint8_t adjustment = alignBackwardAdjustment(address, alignment);

	if(_used_memory + adjustment + size > _size)
		return nullptr;

	void* aligned_address = subtract(address, adjustment);

	_current_pos = aligned_address;

	_used_memory = (uintptr_t)_start - (uintptr_t)_current_pos;

	return aligned_address;
}

void EndAllocator::rewind(void* p)
{
	ASSERT(_current_pos <= p && _start >= p);

	_current_pos = p;
	_used_memory = (uintptr_t)_start - (uintptr_t)_current_pos;
}