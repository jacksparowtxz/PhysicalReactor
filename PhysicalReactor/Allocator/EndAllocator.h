#pragma once
#include "FixedLinearAllocator.h"



namespace PRE
{
	class EndAllocator : public FixedLinearAllocator
	{
	public:
		EndAllocator(size_t size, void* start);
		~EndAllocator();

		void* allocate(size_t size, uint8_t alignment) override;

		void rewind(void* p) override;

	private:
		EndAllocator(const EndAllocator&);
		EndAllocator& operator=(const EndAllocator&);
	};
};