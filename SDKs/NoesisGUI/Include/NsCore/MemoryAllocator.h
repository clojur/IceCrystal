////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
// [CR #715]
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_MEMORYALLOCATOR_H__
#define __CORE_MEMORYALLOCATOR_H__


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Base class for memory allocators. All implementations must be thread-safe.
////////////////////////////////////////////////////////////////////////////////////////////////////
class MemoryAllocator
{
public:
    virtual ~MemoryAllocator() = 0;

    /// Allocates a block of memory of specified size
    virtual void* Alloc(size_t size) = 0;

    /// Reallocates a block of memory
    virtual void* Realloc(void* ptr, size_t size) = 0;

    /// Free previously allocated memory
    virtual void Dealloc(void* ptr) = 0;
};

inline MemoryAllocator::~MemoryAllocator() {}

}

#endif
