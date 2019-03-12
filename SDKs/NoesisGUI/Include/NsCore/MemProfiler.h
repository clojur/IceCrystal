////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_MEMPROFILER_H__
#define __CORE_MEMPROFILER_H__


#include <NsCore/Noesis.h>
#include <NsCore/Profiler.h>
#include <NsCore/KernelApi.h>
#include <NsCore/Atomic.h>


namespace Noesis
{

#if NS_INSTRUMENTED_ENABLED

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
// Profiler for memory usage
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_CORE_KERNEL_API MemProfilerNode: public ProfilerNode<MemProfilerNode>
{
public:
    MemProfilerNode(NsSymbol sym = NsSymbol(uint32_t(0)), MemoryAllocator* allocator = 0);

    /// This class is created before the memory manager. So malloc() and free() are used to create
    /// and destroy the instance
    //@{
    static void *operator new(size_t size);
    static void *operator new(size_t size, void* placementPtr);
    static void operator delete(void* ptr);
    static void operator delete(void* ptr, void* placementPtr);
    //@}

    /// \return the root node
    static MemProfilerNode* GetRootNode();
    
    /// \return the current active node
    static MemProfilerNode* GetCurrentNode();
    
    /// Sets the current active node
    static void SetCurrentNode(MemProfilerNode* node);

    NS_FORCEINLINE void Enter();
    NS_FORCEINLINE void Leave();
    
    static void AcquireMutexForAddingChild();
    static void ReleaseMutexForAddingChild();
    static void AcquireMutexForGettingChildren();
    static void ReleaseMutexForGettingChildren();

public:
    // Atomic because they may be incremented / decremented by different threads (a deallocation
    // may occur on a different thread that the allocation)
    // The information given for each node does not include children
    Atomic<uint32_t> memory;
    Atomic<uint32_t> allocs;
};

NS_WARNING_POP

#endif

#if NS_INSTRUMENTED_ENABLED && !defined(NS_DEBUG_CFG)
    #define NS_PROFILE_MEM(sym) NS_INTERNAL_PROFILE((sym), MemProfilerNode, NS_UNIQUE_NAME(mem))
    #define NS_PROFILE_SYM_MEM(sym) NS_INTERNAL_PROFILE_SYM((sym), MemProfilerNode)
#else
    #define NS_PROFILE_MEM(...) NS_NOOP
    #define NS_PROFILE_SYM_MEM(...) NS_NOOP
#endif

}

#if NS_INSTRUMENTED_ENABLED
#include <NsCore/MemProfiler.inl>
#endif

#endif
