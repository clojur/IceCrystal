////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
// [CR #586]
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_ATOMIC_H__
#define __CORE_ATOMIC_H__


#include <NsCore/Noesis.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Template class for atomic operations
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> struct Atomic;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// 32 bits atomic implementation. Using compiler generated constructors to have trivial
/// constructors that behave correctly with static instances.
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> struct Atomic32
{
    inline operator T() const;

    /// All these atomic operations return the original value before applying the new value
    //@{
    inline T FetchAndIncrement();
    inline T FetchAndDecrement();
    inline T FetchAndAdd(T v);
    inline T FetchAndStore(T v);
    inline T CompareAndSwap(T newValue, T comparand);
    //@}

    /// Standard Operators
    //@{
    inline T operator++();
    inline T operator--();
    inline T operator++(int);
    inline T operator--(int);
    inline T operator+=(T v);
    inline T operator-=(T v);
    //@}

protected:
    volatile T val;
};

#define DEFINE_ATOMIC32(T) \
template<> struct Atomic<T>: public Atomic32<T>\
{\
    inline T operator=(T v)\
    {\
        FetchAndStore(v);\
        return v;\
    }\
};

DEFINE_ATOMIC32(uint32_t)
DEFINE_ATOMIC32(int32_t)

}

#include <NsCore/Atomic.inl>


#endif