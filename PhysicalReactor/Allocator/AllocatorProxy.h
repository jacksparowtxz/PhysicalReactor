#pragma once

#if !_DEBUG
	#define BlockAllocatorProxy BlockAllocator
	//#define ProxyAllocator Allocator
#else
	namespace PRE
	{
		class BlockAllocatorProxy;
		class ProxyAllocator;
	};
#endif