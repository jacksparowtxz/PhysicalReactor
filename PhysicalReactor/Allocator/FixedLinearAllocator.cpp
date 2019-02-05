#include "FixedLinearAllocator.h"
#include "MemoryUtil.h"

using namespace PRE;

FixedLinearAllocator::FixedLinearAllocator(size_t size, void* start) : LinearAllocator(size, start)
{

}

PRE::FixedLinearAllocator::FixedLinearAllocator():LinearAllocator(0,NULL)
{

}



PRE::FixedLinearAllocator& PRE::FixedLinearAllocator::operator=(const FixedLinearAllocator& other)
{
	this->_current_pos = other._current_pos;
	this->_size = other._size;
	this->_start = other._start;
	this->_used_memory = this->_used_memory;
	return *this;
}

FixedLinearAllocator::~FixedLinearAllocator()
{
}

void* FixedLinearAllocator::allocate(size_t size, uint8_t alignment)
{
	ASSERT(size != 0 && alignment != 0);

	uint8_t adjustment = alignForwardAdjustment(_current_pos, alignment);

	if(_used_memory + adjustment + size > _size)
		return nullptr;

	uintptr_t aligned_address = (uintptr_t)_current_pos + adjustment;

	_current_pos = (void*)(aligned_address + size);

	_used_memory = (uintptr_t)_current_pos - (uintptr_t)_start;

	return (void*)aligned_address;
}

void FixedLinearAllocator::rewind(void* p)
{
	ASSERT(_current_pos > p && _start < p);

	_current_pos = p;
	_used_memory = (uintptr_t)_current_pos - (uintptr_t)_start;
}

void FixedLinearAllocator::clear()
{
	_num_allocations = 0;
	_used_memory     = 0;

	_current_pos = _start;
}