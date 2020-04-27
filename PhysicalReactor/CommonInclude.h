#pragma once

#define PREDEBUG 

// Platform specific:
#include <SDKDDKVer.h>
#include <windows.h>

#ifdef WINSTORE_SUPPORT
#include <Windows.UI.Core.h>
#endif // WINSTORE_SUPPORT



/*#if __has_include("vulkan/vulkan.h")
#define PRE_BUILD_VULKAN
#endif // HAS VULKAN*/



#include <cassert>
#include <ctime>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>


/*#include "Archive.h"*/
// Platform agnostic:
#define _XM_NO_INTRINSICS_
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
using namespace DirectX;
using namespace DirectX::PackedVector;

class Archive;

#define ALIGN_16 void* operator new(size_t i){return _mm_malloc(i, 16);} void operator delete(void* p){_mm_free(p);}

#define SAFE_INITIL(a) (a) = nullptr;

#define SAFE_RELEASE(a) if((a)!=nullptr){(a)->Release();(a)=nullptr;}

#define SAFE_DELETE(a) if((a)!=nullptr){delete (a);(a)=nullptr;}

#define SAFE_DELETE_ARRAY(a) if((a)!=nullptr){delete[](a);(a)=nullptr;}

#define GFX_STRUCT struct alignas(16)

#define GFX_CLASS class alignas(16)

#define COMMAND_PACKET_ALIGNED 1

#define DEBUG_TAG_COMMANDS 0

#if DEBUG_TAG_COMMANDS
#define DEBUG_COMMANDS_PRINT 0
#endif

#define FORCE_INLINE inline __forceinline


template <typename T>
inline void SwapPtr(T*& a, T*& b)
{
	T* swap = a;
	a = b;
	b = swap;
}

template<typename T>
inline void RECREATE(T*& myObject)
{
	SAFE_DELETE(myObject);
	myObject = new T;
}



