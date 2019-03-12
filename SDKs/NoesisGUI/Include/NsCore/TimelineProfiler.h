////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_TIMELINEPROFILER_H__
#define __CORE_TIMELINEPROFILER_H__


#include <NsCore/Noesis.h>
#include <NsCore/KernelApi.h>
#include <NsCore/Symbol.h>
#include <NsCore/NSTLMemoryAllocator.h>
#include <NsCore/Vector.h>


namespace Noesis
{

template<class T> class Delegate;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// The Timeline profiler tracks jobs processed on each thread. For each thread a sorted list of
/// events are given
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_CORE_KERNEL_API TimelineProfilerNode
{
public:
    /// Use this function to create additional timelines that are not associated to OS threads.
    /// The created timeline will be automatically destroyed at the end of the running application
    static void* CreateTimeline(const char* name);

    /// Returns the next node global identifier
    static uint32_t GetNextGID();

    static void Open(NsSymbol sym, uint32_t gid = 0);
    static void Open(void* timeline, uint64_t timeUs, NsSymbol sym, uint32_t gid = 0);
    static void Close(NsSymbol sym, uint32_t gid = 0);
    static void Close(void* timeline, uint64_t timeUs, NsSymbol sym, uint32_t gid = 0);
    static void EnqueueMark(uint32_t gid);
    static void BeginIdle();
    static void EndIdle();

    static void SetThreadName(NsSymbol sym);

    enum Flags
    {
        Flags_Open,
        Flags_Close,
        Flags_Enqueue,
        Flags_BeginIdle,
        Flags_EndIdle
    };
    
    struct CpuNode
    {
        uint32_t firstChildrenIndex;
        uint32_t numChildren;
        
        NsSymbol id;
        uint32_t hits;
        uint32_t memory;
        uint64_t cycles;
    };
    
    struct Event
    {
        // Global Unique Identifier
        uint32_t gid;
        
        NsSymbol id;
        Flags flags;
        
        // Information about where the cpu cycles where spent on this timeline event. This is
        // a snapshot of the current cpu node tree
        typedef eastl::MemoryAllocator Allocator;
        typedef NsVector<CpuNode, Allocator> CpuNodes;
        CpuNodes* cpuNodes;
        
        // Time in microseconds, being 0 the start of the application 
        uint64_t time;
        
        // Time stamp measured in cycles. Only valid when cpuNodes != 0
        uint64_t cyclesTimeStamp;
    };
    
    // Mask must be 2^n - 1
    const static uint32_t Mask = 255;
    typedef Event Events[Mask + 1];
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// NodeOpenClose. RAII class for automatic opening/closing timeline sections
////////////////////////////////////////////////////////////////////////////////////////////////////
class NodeOpenClose
{
public:
    inline NodeOpenClose(NsSymbol sym);
    inline ~NodeOpenClose();

private:
    NodeOpenClose(const NodeOpenClose&);
    NodeOpenClose& operator=(const NodeOpenClose&);

    Symbol mSym;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Macros
////////////////////////////////////////////////////////////////////////////////////////////////////
#define NS_INTERNAL_PROFILE_TIMELINE(sym) \
    static uint32_t profilerTimelineSym; \
    if (profilerTimelineSym == 0) \
    { \
        profilerTimelineSym = NsSymbol(sym); \
    } \
    Noesis::NodeOpenClose timelineSection(static_cast<NsSymbol>(profilerTimelineSym))
    
#define NS_INTERNAL_PROFILE_SYM_TIMELINE(sym)\
    Noesis::NodeOpenClose timelineSection(sym)

#if NS_INSTRUMENTED_ENABLED && !defined(NS_DEBUG_CFG)
    #define NS_PROFILE_TIMELINE(sym) NS_INTERNAL_PROFILE_TIMELINE(sym)
    #define NS_PROFILE_SYM_TIMELINE(sym) NS_INTERNAL_PROFILE_SYM_TIMELINE(sym)
#else
    #define NS_PROFILE_TIMELINE(...) NS_NOOP
    #define NS_PROFILE_SYM_TIMELINE(...) NS_NOOP
#endif

}

#include <NsCore/TimelineProfiler.inl>

#endif
