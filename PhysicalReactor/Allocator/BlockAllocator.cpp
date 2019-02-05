#include "BlockAllocator.h"

using namespace PRE;

BlockAllocator::BlockAllocator(size_t size, void* start, size_t block_size, uint8_t alignment)
	: Allocator(size), _block_size(block_size), _alignment(alignment)
{
	//Implementation is simplified by making these two assumptions
	ASSERT(_alignment >= __alignof(BlockHeader) && "Invalid alignment");
	ASSERT(_block_size % _alignment == 0 && "Invalid alignment");

	ASSERT(size > 0 && start != nullptr);

	_header_size = sizeof(BlockHeader);

	uint8_t rem = sizeof(BlockHeader) % _alignment;

	if (rem != 0)
		_header_size += _alignment - rem;

	_initial_adjustment = alignForwardAdjustment(start, _alignment);

	_free_blocks = (BlockHeader*)add(start, _initial_adjustment);

	ASSERT(isAligned(_free_blocks));

	_free_blocks->size = size - _initial_adjustment - _header_size;
	_free_blocks->next_free_block = nullptr;

	_used_memory += _initial_adjustment + _header_size;

}

BlockAllocator::~BlockAllocator()
{
	_used_memory -= _initial_adjustment + _header_size;
}

void* BlockAllocator::allocate(size_t size, uint8_t alignment)
{
	ASSERT(size % _block_size == 0);
	ASSERT(size > 0);
	ASSERT(alignment == _alignment || alignment == 0);

	BlockHeader* prev_free_block = nullptr;
	BlockHeader* free_block = _free_blocks;

	BlockHeader* best_fit_prev = nullptr;
	BlockHeader* best_fit = nullptr;


	//Find best fit
	while (free_block != nullptr)
	{
		
		if (free_block->size == size)
		{
			best_fit_prev = prev_free_block;
			best_fit = free_block;
		

			break;
		}

		//If its a better fit switch
		if (free_block->size > size && (best_fit == nullptr || free_block->size < best_fit->size))
		{
			best_fit_prev = prev_free_block;
			best_fit = free_block;

		}

		prev_free_block = free_block;
		free_block = free_block->next_free_block;
	}

	if (best_fit == nullptr)
		return nullptr;

	//If allocations in the remaining memory will be impossible
	if (best_fit->size - size < _header_size + _block_size)
	{
		//Increase allocation size instead of creating a new block
		size = best_fit->size;

		if (best_fit_prev != nullptr)
			best_fit_prev->next_free_block = best_fit->next_free_block;
		else
			_free_blocks = best_fit->next_free_block;
	}
	else
	{
		//Else create a new block containing remaining memory
		BlockHeader* new_block = (BlockHeader*)(add(best_fit + 1, size));
		new_block->size = best_fit->size - size - _header_size;
		new_block->next_free_block = best_fit->next_free_block;

		if (best_fit_prev != nullptr)
			best_fit_prev->next_free_block = new_block;
		else
			_free_blocks = new_block;

		_used_memory += _header_size;
	}

	void* block = add(best_fit, _header_size);

	best_fit->size = size;
	best_fit->next_free_block = nullptr;

	_used_memory += size;
	_num_allocations++;

	ASSERT(isAligned(block, _alignment));

	return block;
}

void BlockAllocator::deallocate(void* p)
{

	ASSERT(p != nullptr);
		

	BlockHeader* header = (BlockHeader*)subtract(p, _header_size);

	uintptr_t   block_start = reinterpret_cast<uintptr_t>(header);
	size_t block_size = header->size;
	uintptr_t   block_end = block_start + block_size + _header_size;

	BlockHeader* prev_free_block = nullptr;
	BlockHeader* free_block = _free_blocks;

	while (free_block != nullptr)
	{
		if ((uintptr_t)free_block >= block_end)
			break;

		prev_free_block = free_block;
		free_block = free_block->next_free_block;
	}

	if (prev_free_block == nullptr)
	{
		prev_free_block = (BlockHeader*)block_start;
		prev_free_block->size = block_size;
		prev_free_block->next_free_block = _free_blocks;

		_free_blocks = prev_free_block;

		_used_memory -= block_size;
	}
	else if ((uintptr_t)prev_free_block + _header_size + prev_free_block->size == block_start)
	{
		prev_free_block->size += _header_size + block_size;

		_used_memory -= _header_size + block_size;
	}
	else
	{
		header->next_free_block = prev_free_block->next_free_block;
		prev_free_block->next_free_block = header;

		prev_free_block = header;

		_used_memory -= block_size;
	}

	ASSERT(prev_free_block != nullptr);

	if ((uintptr_t)prev_free_block + _header_size + prev_free_block->size == (uintptr_t)prev_free_block->next_free_block)
	{
		_used_memory -= _header_size;

		prev_free_block->size += _header_size + prev_free_block->next_free_block->size;
		prev_free_block->next_free_block = prev_free_block->next_free_block->next_free_block;
	}

	_num_allocations--;
}

size_t BlockAllocator::getBlockSize() const
{
	return _block_size;
}

uint8_t BlockAllocator::getAlignment() const
{
	return _alignment;
}