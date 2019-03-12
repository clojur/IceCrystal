////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifdef NS_COMPILER_MSVC
    #include <math.h>
    #include <intrin.h>
#endif


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
void CpuProfilerNode::Enter()
{
#if defined(NS_COMPILER_MSVC) && (defined(NS_PROCESSOR_X86_64) || defined(NS_PROCESSOR_X86))
    // rdtsc because the QueryPerformanceCounter() is too inefficient for a profiler
    lastEnterTime = __rdtsc();
    hits++;
#else
    lastEnterTime = 0;
    hits++;
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CpuProfilerNode::Leave()
{
#if defined(NS_COMPILER_MSVC) && (defined(NS_PROCESSOR_X86_64) || defined(NS_PROCESSOR_X86))
    cycles = cycles + __rdtsc() - lastEnterTime;
#endif
}

}
