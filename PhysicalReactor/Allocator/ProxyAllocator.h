#pragma once
#include "Allocator.h"



namespace PRE
{
	class ProxyAllocator : public Allocator
	{
	public:
		ProxyAllocator(Allocator& allocator);
		~ProxyAllocator();

		void* allocate(size_t size, uint8_t alignment=8) override;

		void deallocate(void* p) override;

	private:
		ProxyAllocator(const ProxyAllocator&);
		ProxyAllocator& operator=(const ProxyAllocator&);

		Allocator& _allocator;
	};
};