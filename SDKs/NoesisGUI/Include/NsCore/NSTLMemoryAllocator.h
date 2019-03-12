////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_NSTLMEMORYALLOCATOR_H__
#define __CORE_NSTLMEMORYALLOCATOR_H__


#include <NsCore/Noesis.h>
#include <NsCore/MemoryAllocator.h>


namespace eastl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// nstl allocator based on a MemoryManager::Allocator
////////////////////////////////////////////////////////////////////////////////////////////////////
class MemoryAllocator
{
public:
    inline explicit MemoryAllocator(const char* name = "");
    inline MemoryAllocator(Noesis::MemoryAllocator* allocator);
    inline MemoryAllocator(const MemoryAllocator& other);

    inline MemoryAllocator& operator=(const MemoryAllocator& other);

    inline void* allocate(size_t n, int flags = 0);
    inline void* allocate(size_t n, size_t alignment, size_t offset, int flags = 0);
    inline void deallocate(void* p, size_t n);

    Noesis::MemoryAllocator* allocator;
};

inline bool operator==(const MemoryAllocator& a, const MemoryAllocator& b);
inline bool operator!=(const MemoryAllocator& a, const MemoryAllocator& b);

}

#include "NSTLMemoryAllocator.inl"

#endif
