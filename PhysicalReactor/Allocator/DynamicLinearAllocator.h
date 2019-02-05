#pragma once
#include "LinearAllocator.h"
#include "MemoryUtil.h"

/*
	TODO: Remove getStart() from LinearAllocator
		because it doesn't make sense in DynamicLinearAllocators and there's no real need for it
			
*/

namespace PRE
{
	class Allocator;

	class DynamicLinearAllocator : public LinearAllocator
	{
	public:
		DynamicLinearAllocator(Allocator& backing_allocator, size_t block_size, uint8_t block_alignment);
		~DynamicLinearAllocator() override final;

		void* allocate(size_t size, uint8_t alignment = ADEFAULT_ALIGNMENT) override final;

		void rewind(void* mark) override final;

		void clear() override final;

	private:
		DynamicLinearAllocator(const DynamicLinearAllocator&);
		DynamicLinearAllocator& operator=(const DynamicLinearAllocator&);

		bool allocateNewBlock(size_t size, uint8_t alignment);

		struct BlockDesc;

		Allocator& _backing_allocator;

		BlockDesc* _current_block;

		size_t _block_size;
		uint8_t    _block_alignment;
	};
};