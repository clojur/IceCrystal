////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_COMPILERSETTINGS_H__
#define __CORE_COMPILERSETTINGS_H__


// Macro for generating unique variable names inside a cpp file.
#define NS_UNIQUE_NAME(prefix) NS_UNIQUE_NAME_INTERNAL0(prefix, __COUNTER__)
#define NS_UNIQUE_NAME_INTERNAL0(a, b) NS_UNIQUE_NAME_INTERNAL1(a, b)
#define NS_UNIQUE_NAME_INTERNAL1(a, b) a##b

#define NS_STRINGIFY(x) #x
#define NS_TOSTRING(x) NS_STRINGIFY(x)
#define NS_CONFIG_NAME NS_TOSTRING(NS_CONFIG)

#define NS_DISABLE_COPY(Class) \
    Class(const Class &) = delete; \
    Class &operator=(const Class &) = delete;

/// Paranoid macro to activate extra level of debugging
/// Level0 is the default for debug and release. No extra checks are done
/// Level1 activates extra checkings decreasing a bit the program execution speed
/// Level2 activates more extra checking that drastically alters the program execution speed 
/// Level3 activates all the paranoids. Every things is slow here. Your last chance for that bug
#ifndef NS_PARANOID_LEVEL
    #define NS_PARANOID_LEVEL 0
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Compiler dependant settings. Unifies the differences between compilers
////////////////////////////////////////////////////////////////////////////////////////////////////

// Microsoft VC++
#if defined(_MSC_VER) && _MSC_VER >= 1400
    #define NS_COMPILER_MSVC
    #define NS_COMPILER_VERSION _MSC_VER
    #define NS_COMPILER_NAME "Microsoft Visual C++"

    #define NS_MACRO_BEGIN \
        __pragma(warning(push)) \
        __pragma(warning(disable:4127)) \
        do {
    #define NS_MACRO_END \
        } while(false) \
        __pragma(warning(pop))

    #ifdef NS_STATIC_LIBRARY
        #define NS_DLL_EXPORT
        #define NS_DLL_IMPORT
    #else
        #define NS_DLL_EXPORT __declspec(dllexport)
        #define NS_DLL_IMPORT __declspec(dllimport)
    #endif

    #define NS_UNUSED(...) (void)(true ? (void)0 : ((void)(__VA_ARGS__)))

    #define NS_NOOP __noop
    #define NS_DEBUG_BREAK __debugbreak()
    #define NS_FORCEINLINE __forceinline
    #define NS_NOINLINE __declspec(noinline)
    #define NS_UNUSED_FUNC
    #define NS_WARNING_PUSH __pragma(warning(push))
    #define NS_MSVC_WARNING_DISABLE(n) __pragma(warning(disable:n))
    #define NS_MSVC_WARNING_SUPPRESS(n) __pragma(warning(suppress:n))
    #define NS_GCC_WARNING_DISABLE(x)
    #define NS_CLANG_HAS_WARNING(x) 0
    #define NS_CLANG_WARNING_DISABLE(x)
    #define NS_WARNING_POP __pragma(warning(pop))
    #define NS_ALIGN(_x) __declspec(align(_x))
    #define NS_INTERFACE struct __declspec(novtable)
    #define NS_ASSUME(cond) __assume(cond)
    #define NS_UNREACHABLE __assume(false)
    #define NS_COUNTOF(_x) _countof(_x)
    #define NS_LIKELY(x) (x)
    #define NS_UNLIKELY(x) (x)
    #define NS_FORMAT_PRINTF_ATTR(n)

    #ifdef _PREFAST_
        #include <sal.h>
        #define NS_FORMAT_PRINTF _Printf_format_string_
    #else
        #define NS_FORMAT_PRINTF
    #endif

// GNU Gcc C++ compiler
#elif defined(__GNUG__) && __GNUG__ >= 4
    #define NS_COMPILER_GCC
    #define NS_COMPILER_VERSION (__GNUC__ * 1000 + __GNUC_MINOR__)
    #define NS_COMPILER_NAME "GCC"

    #define NS_MACRO_BEGIN do {
    #define NS_MACRO_END } while (false)

    // http://gcc.gnu.org/wiki/Visibility
    #define NS_DLL_EXPORT __attribute__ ((visibility("default")))
    #define NS_DLL_IMPORT __attribute__ ((visibility("default")))

    // https://stackoverflow.com/questions/23235910/variadic-unused-function-macro
    #define NS_UNUSED_1(x1) (void)(true ? (void)0 : ((void)(x1)))
    #define NS_UNUSED_2(x1, x2) NS_UNUSED_1(x1); NS_UNUSED_1(x2);
    #define NS_UNUSED_3(x1, x2, x3) NS_UNUSED_2(x1, x2); NS_UNUSED_1(x3);
    #define NS_UNUSED_4(x1, x2, x3, x4) NS_UNUSED_3(x1, x2, x3); NS_UNUSED_1(x4);
    #define NS_UNUSED_5(x1, x2, x3, x4, x5) NS_UNUSED_4(x1, x2, x3, x4); NS_UNUSED_1(x5);
    #define NS_UNUSED_6(x1, x2, x3, x4, x5, x6) NS_UNUSED_5(x1, x2, x3, x4, x5); NS_UNUSED_1(x6);
    #define NS_UNUSED_7(x1, x2, x3, x4, x5, x6, x7) NS_UNUSED_6(x1, x2, x3, x4, x5, x6); NS_UNUSED_1(x7);

    #define VA_NUM_ARGS_IMPL(_1,_2,_3,_4,_5, _6, _7, N, ...) N
    #define VA_NUM_ARGS(...) VA_NUM_ARGS_IMPL(__VA_ARGS__, 7, 6, 5, 4, 3, 2, 1)

    #define NS_UNUSED_IMPL_(nargs) NS_UNUSED_ ## nargs
    #define NS_UNUSED_IMPL(nargs) NS_UNUSED_IMPL_(nargs)
    #define NS_UNUSED(...) NS_UNUSED_IMPL(VA_NUM_ARGS(__VA_ARGS__))(__VA_ARGS__ )

    #define NS_NOOP (void)(sizeof(0))

    #if defined(__ORBIS__)
        #define NS_DEBUG_BREAK __asm volatile ("int $0x41")
    #elif defined(__i386__) || defined(__x86_64__)
        #define NS_DEBUG_BREAK __asm__("int $0x03")
    #elif defined(__arm__)
        #include <unistd.h>
        #include <signal.h>
        #define NS_DEBUG_BREAK \
            __asm__ __volatile__ ( \
                "mov r0, %0\n" \
                "mov r1, %1\n" \
                "mov r12, #37\n" \
                "swi 128\n" \
                : : "r" (getpid()), "r" (SIGINT) : "r12", "r0", "r1", "cc" \
            )
    #else
        #define NS_DEBUG_BREAK __builtin_trap()
    #endif

    #define NS_FORCEINLINE __attribute__ ((always_inline)) inline
    #define NS_NOINLINE __attribute__ ((noinline))
    #define NS_UNUSED_FUNC __attribute__ ((unused))
    #define NS_WARNING_PUSH _Pragma("GCC diagnostic push")
    #define NS_MSVC_WARNING_DISABLE(n)
    #define NS_MSVC_WARNING_SUPPRESS(n)
    #define NS_GCC_WARNING_DISABLE(x) _Pragma(NS_STRINGIFY(GCC diagnostic ignored x))

    #ifdef __clang__
        #define NS_CLANG_HAS_WARNING(x) __has_warning(x)
        #define NS_CLANG_WARNING_DISABLE(x) _Pragma(NS_STRINGIFY(clang diagnostic ignored x))
    #else
        #define NS_CLANG_HAS_WARNING(x) 0
        #define NS_CLANG_WARNING_DISABLE(x)
    #endif

    #define NS_WARNING_POP _Pragma("GCC diagnostic pop")
    #define NS_ALIGN(_x) __attribute__((aligned(_x)))
    #define NS_INTERFACE struct
    #define NS_ASSUME(cond) do { if (!(cond)) __builtin_unreachable(); } while (0)
    #define NS_UNREACHABLE __builtin_unreachable()
    
    // http://cnicholson.net/2011/01/stupid-c-tricks-a-better-sizeof_array/
    #include <cstddef>
    template<typename T, size_t N> char (&NS_COUNTOF_REQUIRES_ARRAY_ARGUMENT(const T(&)[N]))[N];
    #define NS_COUNTOF(_x) sizeof(NS_COUNTOF_REQUIRES_ARRAY_ARGUMENT(_x))

    #define NS_LIKELY(x) __builtin_expect(!!(x), true)
    #define NS_UNLIKELY(x) __builtin_expect(!!(x), false)
    #define NS_FORMAT_PRINTF_ATTR(n) __attribute__ ((format (printf, n, n + 1)))
    #define NS_FORMAT_PRINTF

#else
    #error Compiler not supported
#endif

// Only bindings are exported in Unity/NET API
#if defined(NS_UNITY3D) || defined(NS_NETSDK)
    #undef NS_DLL_EXPORT
    #define NS_DLL_EXPORT
    #undef NS_DLL_IMPORT
    #define NS_DLL_IMPORT
#endif

#if defined(__GXX_ABI_VERSION) && defined(__GXX_RTTI)
    #define NS_COMPILER_RTTI
#elif defined(_MSC_VER) && defined(_CPPRTTI)
    #define NS_COMPILER_RTTI
#endif

#ifndef NS_INSTRUMENTED_ENABLED
    #define NS_INSTRUMENTED_ENABLED 0
#endif

#ifndef PATH_MAX
    #define PATH_MAX 260
#endif

#endif
