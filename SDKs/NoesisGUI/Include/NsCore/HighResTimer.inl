////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsCore/Error.h>

#if defined(NS_PLATFORM_WINDOWS)
    #include <windows.h>
#elif defined(NS_PLATFORM_APPLE)
    #include <mach/mach_time.h>
#else
    #include <sys/time.h>
#endif


namespace Noesis
{
namespace HighResTimer
{

////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint64_t Ticks()
{
#if defined(NS_PLATFORM_WINDOWS)
    LARGE_INTEGER count;
    QueryPerformanceCounter(&count); // will always succeed
    return count.QuadPart;

#elif defined(NS_PLATFORM_APPLE)
    return mach_absolute_time();

#else
    timeval tv;
    int error = gettimeofday(&tv, 0);
    NS_ASSERT(error == 0);
    return (uint64_t)tv.tv_usec + ((uint64_t)tv.tv_sec * 1000000);
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline double Seconds(uint64_t ticks)
{
#if defined(NS_PLATFORM_WINDOWS)
    static uint64_t frequency;
    if (frequency == 0)
    {
        LARGE_INTEGER f;
        QueryPerformanceFrequency(&f); // will always succeed
        frequency = f.QuadPart;
    }

    return (double)ticks / frequency;

#elif defined(NS_PLATFORM_APPLE)
    static mach_timebase_info_data_t tbi;
    if (tbi.denom == 0)
    {
        (void)mach_timebase_info(&tbi);
    }

    return (double)ticks * tbi.numer / (tbi.denom * 1e9);

#else
    return double(ticks) / 1000000.0;

#endif
}

}
}
