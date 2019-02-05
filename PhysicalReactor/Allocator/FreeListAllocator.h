#pragma once
#include "Allocator.h"
#define  Halo_DEBUG 0
#define  Halo_DEVELOPMENT 0



namespace PRE
{
	class FreeListAllocator : public Allocator
	{
	public:
		FreeListAllocator(size_t size, void* start);
		~FreeListAllocator();

		void* allocate(size_t size, uint8_t alignment) override;

		void deallocate(void* p) override;

#if Halo_DEBUG || Halo_DEVELOPMENT
		void checkFreeBlockList();
#endif

	private:

		struct AllocationHeader
		{
			#if Halo_DEBUG || Halo_DEVELOPMENT
			size_t identifier;
			uint32_t	   magic_number;
			#endif
			size_t size;
			uint8_t     adjustment;
		};

		struct FreeBlock
		{
			#if Halo_DEBUG || Halo_DEVELOPMENT
			uint32_t magic_number;
			#endif
			size_t     size;
			FreeBlock* next;
		};

		static_assert(sizeof(AllocationHeader) >= sizeof(FreeBlock), "sizeof(AllocationHeader) < sizeof(FreeBlock)");

		FreeListAllocator(const FreeListAllocator&);
		FreeListAllocator& operator=(const FreeListAllocator&);

		FreeBlock* _free_blocks;

		#if Halo_DEBUG || Halo_DEVELOPMENT
		size_t _next_identifier;
		#endif
	};
};