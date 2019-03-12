////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
// [CR #586]
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifdef NS_COMPILER_MSVC
    #include <intrin.h>
#endif


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> Atomic32<T>::operator T() const
{
    return static_cast<T>(val);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> T Atomic32<T>::FetchAndIncrement()
{
#ifdef NS_COMPILER_MSVC
    return static_cast<T>(_InterlockedIncrement(reinterpret_cast<volatile long*>(&val))) - 1;
#else
    return __sync_fetch_and_add(&val, 1);
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> T Atomic32<T>::FetchAndDecrement()
{
#ifdef NS_COMPILER_MSVC
    return static_cast<T>(_InterlockedDecrement(reinterpret_cast<volatile long*>(&val))) + 1;
#else
    return __sync_fetch_and_add(&val, -1);
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> T Atomic32<T>::FetchAndAdd(T v)
{
#ifdef NS_COMPILER_MSVC
    return static_cast<T>(_InterlockedExchangeAdd(reinterpret_cast<volatile long*>(&val),
        static_cast<long>(v)));
#else
    return __sync_fetch_and_add(&val, v);
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> T Atomic32<T>::FetchAndStore(T v)
{
#ifdef NS_COMPILER_MSVC
    return static_cast<T>(_InterlockedExchange(reinterpret_cast<volatile long*>(&val), 
        static_cast<long>(v)));
#else
    // __sync_lock_test_and_set is not a full barrier
    __sync_synchronize();
    return __sync_lock_test_and_set(&val, v);
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> T Atomic32<T>::CompareAndSwap(T newValue, T comparand)
{
#ifdef NS_COMPILER_MSVC
    return static_cast<T>(_InterlockedCompareExchange(reinterpret_cast<volatile long*>(&val), 
        static_cast<long>(newValue), static_cast<long>(comparand)));
#else
    return __sync_val_compare_and_swap(&val, comparand, newValue);
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> T Atomic32<T>::operator++()
{
    return FetchAndIncrement() + 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> T Atomic32<T>::operator--()
{
    return FetchAndDecrement() - 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> T Atomic32<T>::operator++(int)
{
    return FetchAndIncrement();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> T Atomic32<T>::operator--(int)
{
    return FetchAndDecrement();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> T Atomic32<T>::operator+=(T v)
{
    return FetchAndAdd(v) + v;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> T Atomic32<T>::operator-=(T v)
{
    return operator+=(T(0) - v);
}

}
