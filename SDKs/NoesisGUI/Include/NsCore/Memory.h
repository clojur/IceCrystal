////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
// [CR #715]
// [CR #750]
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_MEMORY_H__
#define __CORE_MEMORY_H__


#include <stdint.h>
#include <NsCore/KernelApi.h>


////////////////////////////////////////////////////////////////////////////////////////////////////
/// NoesisEngine memory management functions. These functions are shortcuts to the equivalents in 
/// the memory manager. For example:
///
///     void* ptr = NsGetKernel()->GetMemoryManager()->Alloc(128);
///
/// is equivalent to
///
///     void* ptr = NsAlloc(128);
///
////////////////////////////////////////////////////////////////////////////////////////////////////
//@{
NS_CORE_KERNEL_API void* NsAlloc(size_t size);
NS_CORE_KERNEL_API void* NsRealloc(void* ptr, size_t size);
NS_CORE_KERNEL_API void NsDealloc(void* ptr);
//@}

/// These functions are normally redirected to (NsAlloc/NsDealloc) but if the kernel has not been
/// initialized they are redirected back to standard ANSI functions (AnsiMalloc/AnsiFree)
//@{ 
NS_CORE_KERNEL_API void* NsSafeAlloc(size_t size);
NS_CORE_KERNEL_API void NsSafeDealloc(void* ptr);
//@}

namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Standard C memory functions
////////////////////////////////////////////////////////////////////////////////////////////////////
//@{
NS_CORE_KERNEL_API void* AnsiMalloc(size_t size);
NS_CORE_KERNEL_API void* AnsiCalloc(size_t numElements, size_t elementSize);
NS_CORE_KERNEL_API void* AnsiRealloc(void* ptr, size_t size);
NS_CORE_KERNEL_API void AnsiFree(void* ptr);
//@}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Utility class to align size to the specified type boundaries
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> struct Alignment
{
    enum { Mask = sizeof(T) - 1 };
    static uint32_t Align(uint32_t size) { return (size + Mask) & ~Mask; }
    static bool IsAligned(uint32_t size) { return (size & Mask) == 0; }
    static bool IsAlignedPtr(const void* ptr) { return (uintptr_t(ptr) & Mask) == 0; }
};

}

#endif
