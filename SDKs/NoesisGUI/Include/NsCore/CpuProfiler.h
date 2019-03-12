////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_CPUPROFILER_H__
#define __CORE_CPUPROFILER_H__


#include <NsCore/Noesis.h>
#include <NsCore/Profiler.h>
#include <NsCore/KernelApi.h>


namespace Noesis
{

#if NS_INSTRUMENTED_ENABLED

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Profiler that tracks CPU cycles usage. Each node holds the time spent on it and the times it
/// has been called. Memory allocations on each node are also stored
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_CORE_KERNEL_API CpuProfilerNode: public ProfilerNode<CpuProfilerNode>
{
public:
    CpuProfilerNode(NsSymbol sym = NsSymbol(uint32_t(0)), MemoryAllocator* allocator = 0);

    /// \return the root node
    static CpuProfilerNode* GetRootNode();
    
    /// \return the current active node
    static CpuProfilerNode* GetCurrentNode();
    
    /// Sets the current active node
    static void SetCurrentNode(CpuProfilerNode* node);

    NS_FORCEINLINE void Enter();
    NS_FORCEINLINE void Leave();
    
    static void AcquireMutexForAddingChild();
    static void ReleaseMutexForAddingChild();
    static void AcquireMutexForGettingChildren();
    static void ReleaseMutexForGettingChildren();
    
public:
    // Written always by the same thread. Read by the rest of threads
    uint32_t hits;
    uint32_t memory;
    uint64_t cycles;

private:
    uint64_t lastEnterTime;
};

NS_WARNING_POP

#endif

#if NS_INSTRUMENTED_ENABLED && !defined(NS_DEBUG_CFG)
    #define NS_PROFILE_CPU(sym) NS_INTERNAL_PROFILE((sym), CpuProfilerNode, NS_UNIQUE_NAME(cpu))
    #define NS_PROFILE_SYM_CPU(sym) NS_INTERNAL_PROFILE_SYM((sym), CpuProfilerNode)
#else
    #define NS_PROFILE_CPU(...) NS_NOOP
    #define NS_PROFILE_SYM_CPU(...) NS_NOOP
#endif

}

#if NS_INSTRUMENTED_ENABLED
#include <NsCore/CpuProfiler.inl>
#endif

#endif
