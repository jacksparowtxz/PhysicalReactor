#pragma once
#include "LinearAllocator.h"



namespace PRE
{
	class FixedLinearAllocator : public LinearAllocator
	{
	public:
		FixedLinearAllocator(size_t size, void* start);
		FixedLinearAllocator();
		virtual ~FixedLinearAllocator();

		virtual void* allocate(size_t size, uint8_t alignment = ADEFAULT_ALIGNMENT) override;

		virtual void  rewind(void* mark) override;

		void		  clear() override final;
	//	FixedLinearAllocator(const FixedLinearAllocator&);
		FixedLinearAllocator& operator=(const FixedLinearAllocator&);
	
		
	};

	namespace allocatorFC
	{
		inline FixedLinearAllocator* newFixedLinearAllocator(size_t size, Allocator& allocator)
		{
			void* p = allocator.allocate(size + sizeof(FixedLinearAllocator), alignof(FixedLinearAllocator));
			return new (p)FixedLinearAllocator(size,add(p, sizeof(FixedLinearAllocator)));
		}
		
		inline FixedLinearAllocator* new2FixedLinearAllocator(size_t size,Allocator& allocator,void* start)
		{
			void* p = allocator.allocate(size + sizeof(FixedLinearAllocator), alignof(FixedLinearAllocator));
			return new (p)FixedLinearAllocator(size, start);
		}

		inline void deleteFixedLinearAllocator(Allocator& allocator, FixedLinearAllocator& linear_allocator)
		{
			linear_allocator.~FixedLinearAllocator();

			allocator.deallocate(&linear_allocator);
		}
	};
};