////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_COMPILERTOOLS_H__
#define __CORE_COMPILERTOOLS_H__


#include <NsCore/Noesis.h>


namespace Noesis
{

class Symbol;
class BaseRefCounted;
template<class T> class Ptr;
NS_INTERFACE Interface;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used as a placeholder for "no type here"
////////////////////////////////////////////////////////////////////////////////////////////////////
class NullType {};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Converts each integral constant into a unique type
////////////////////////////////////////////////////////////////////////////////////////////////////
template<int N> struct Int2Type
{
    enum { Result = N };
};

////////////////////////////////////////////////////////////////////////////////
/// Converts each type into a unique, insipid type
////////////////////////////////////////////////////////////////////////////////
template<class T> struct Type2Type
{
    typedef T OriginalType;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// If-Then-Else statements
////////////////////////////////////////////////////////////////////////////////////////////////////
template<bool Condition, class Then, class Else> struct If;

template<class Then, class Else> struct If<true, Then, Else>
{
    typedef Then Result;
};

template<class Then, class Else> struct If<false, Then, Else>
{
    typedef Else Result;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Checks if a type is const-qualified 
///
///   Example:
///     IsConst<MyClass>::Result             -> false
///     IsConst<const MyClass>::Result       -> true
///     IsConst<const MyClass>::RemoveConst  -> MyClass
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> class IsConst
{
    template<class U> struct ConstTraits
    {
        enum { Result = false };
        typedef U Type;
    };
    template<class U> struct ConstTraits<const U>
    {
        enum { Result = true };
        typedef U Type;
    };
    template<class U> struct ConstTraits<const U&>
    {
        enum { Result = true };
        typedef U& Type;
    };

public:
    enum { Result = ConstTraits<T>::Result };
    typedef typename ConstTraits<T>::Type NonConstType;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Checks if a type is volatile-qualified 
///
///   Example:
///     IsVolatile<MyClass>::Result                   -> false
///     IsVolatile<volatile MyClass>::Result          -> true
///     IsVolatile<volatile MyClass>::RemoveVolatile  -> MyClass
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> class IsVolatile
{
    template<class U> struct VolatileTraits
    {
        enum { Result = false };
        typedef U Type;
    };
    template<class U> struct VolatileTraits<volatile U>
    {
        enum { Result = true };
        typedef U Type;
    };
    template<class U> struct VolatileTraits<volatile U&>
    {
        enum { Result = true };
        typedef U& Type;
    };

public:
    enum { Result = VolatileTraits<T>::Result };
    typedef typename VolatileTraits<T>::Type NonVolatileType;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Removes the const and/or volatile qualifiers from type
///
///   Example:
///     RemoveCV<MyClass>::Result                -> MyClass
///     RemoveCV<const MyClass>::Result          -> MyClass
///     RemoveCV<volatile MyClass>::Result       -> MyClass
///     RemoveCV<const volatile MyClass>::Result -> MyClass
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> struct RemoveCV
{
    typedef typename IsVolatile<typename IsConst<T>::NonConstType>::NonVolatileType Type;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Checks if a type is derived from the other type
///
///   Example:
///     class Father { };
///     class Child: public Father { };
///     IsDerived<Child, Father>::Result -> true;
///     IsDerived<Father, Child>::Result -> false;
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Derived, class Base> struct IsDerived
{
    enum { Result = __is_base_of(Base, Derived) };
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Checks if two types are the same
///
///   Example:
///     class Father { };
///     class Child: public Father { };
///     IsSame<Child, Father>::Result -> false
///     IsSame<Child, Child>::Result  -> true
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T, class U> struct IsSame
{
    enum { Result = false };
};

template<class T> struct IsSame<T, T>
{
    enum { Result = true };
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Checks if a type is void
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> struct IsVoid
{
    enum { Result = IsSame<typename RemoveCV<T>::Type, void>::Result };
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Checks if a type is a pointer type
///
///   Example:
///     IsPointer<MyClass>::Result       -> false
///     IsPointer<MyClass*>::Result      -> true
///     IsPointer<MyClass*>::PointedType -> MyClass
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> class IsPointer
{
    template<class U> struct PointerTraits
    {
        enum { Result = false };
        typedef NullType Type;
    };
    template<class U> struct PointerTraits<U*>
    {
        enum { Result = true };
        typedef U Type;
    };

public:
    enum { Result = PointerTraits<typename RemoveCV<T>::Type>::Result };
    typedef typename PointerTraits<typename RemoveCV<T>::Type>::Type PointedType;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Checks if a type is ref counted (derived from Interface or BaseRefCounted).
/// If Result is true, the type T can be stored inside a Ptr<>
///
///   Example:
///     IsRefCounted<int>::Result               -> false
///     IsRefCounted<ICommand>::Result          -> true
///     IsRefCounted<const ICommand>::Result    -> true
///     IsRefCounted<Button>::Result            -> true
///     IsRefCounted<const Button>::Result      -> true
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> class IsRefCounted
{
    typedef typename If<IsDerived<T, BaseRefCounted>::Result, BaseRefCounted, T>::Result TT;

public:
    enum {  Result = IsDerived<TT, BaseRefCounted>::Result || IsDerived<TT, Interface>::Result };
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Checks if a type is an interface
///
///   Example:
///     IsInterface<int>::Result                -> false
///     IsInterface<Button>::Result             -> false
///     IsInterface<ICommand>::Result           -> true
///     IsInterface<const ICommand>::Result     -> true
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> class IsInterface
{
    typedef typename If<IsDerived<T, BaseRefCounted>::Result, BaseRefCounted, T>::Result TT;

public:
    enum { Result = !IsDerived<TT, BaseRefCounted>::Result && IsDerived<TT, Interface>::Result };
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Checks if a type is a Ptr<> and permits getting pointed type
///
///   Example:
///     IsPtr<MyClass>::Result            -> false
///     IsPtr<Ptr<MyClass> >::Result      -> true
///     IsPtr<Ptr<MyClass> >::PointedType -> MyClass
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> class IsPtr
{
    template<class U> struct PtrTraits
    {
        enum { Result = false };
        typedef U Type;
    };
    template<class U> struct PtrTraits<Ptr<U> >
    {
        enum { Result = true };
        typedef U Type;
    };

public:
    enum { Result = PtrTraits<typename RemoveCV<T>::Type>::Result };
    typedef typename PtrTraits<typename RemoveCV<T>::Type>::Type PointedType;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Checks if a type is either lvalue reference or rvalue reference
///
///   Example:
///     IsReference<MyClass>::Result            -> false
///     IsReference<MyClass&>::Result           -> true
///     IsReference<MyClass&>::ReferredType     -> MyClass
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> class IsReference
{
    template<class U> struct ReferenceTraits
    {
        enum { Result = false };
        typedef U Type;
    };
    template<class U> struct ReferenceTraits<U&>
    {
        enum { Result = true };
        typedef U Type;
    };

public:
    enum { Result = ReferenceTraits<T>::Result };
    typedef typename ReferenceTraits<T>::Type NonReferenceType;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Checks if a type is an array
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> class IsArray
{
    typedef char Small;
    struct Big { char dummy[2]; };

    template<class R> static R (*Check1(Type2Type<R>))(Type2Type<R>);
    template<class R> static char Check1(...);

    template<class R> static Big Check2(R(*)(Type2Type<R>));
    template<class R> static Small Check2(...);

public:
    enum { Result = sizeof(Small) == sizeof(Check2<T>(Check1<T>(Type2Type<T>()))) };
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Checks if a type is a non-union class type
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> struct IsClass
{
    enum { Result = __is_class(T) };
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Checks if type is an enum
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> struct IsEnum
{
    enum { Result = __is_enum(T) };
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Checks if a type is integral type 
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> class IsIntegral
{
    template<class TT>
    struct IsIntegralTraits
    {
        enum
        {
            Result =
                IsSame<TT, bool>::Result ||
                IsSame<TT, char>::Result ||
                IsSame<TT, signed char>::Result ||
                IsSame<TT, unsigned char>::Result ||
                IsSame<TT, signed short>::Result ||
                IsSame<TT, unsigned short>::Result ||
                IsSame<TT, signed int>::Result ||
                IsSame<TT, unsigned int>::Result ||
                IsSame<TT, signed long>::Result ||
                IsSame<TT, unsigned long>::Result ||
                IsSame<TT, signed long long>::Result ||
                IsSame<TT, unsigned long long>::Result ||
                IsSame<TT, Symbol>::Result
        };
     };

public:
     enum { Result = IsIntegralTraits<typename RemoveCV<T>::Type>::Result };
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Checks if type is float
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> class IsFloat
{
    template<class TT>
    struct IsFloatTraits
    {
        enum
        {
            Result =
                IsSame<TT, float>::Result ||
                IsSame<TT, double>::Result ||
                IsSame<TT, long double>::Result
        };
    };
    
public:
    enum { Result = IsFloatTraits<typename RemoveCV<T>::Type>::Result };
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Checks if a type is arithmetic type 
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> struct IsArithmetic
{
    enum
    {
        Result = IsIntegral<T>::Result || IsFloat<T>::Result
    };
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Checks if it is best to pass a type as copy or as reference
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> struct IsBestByCopy
{
    enum
    {
        Result = IsArithmetic<T>::Result || IsEnum<T>::Result || IsPointer<T>::Result
    };
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Calculates the type that must be used to efficiently pass T as a parameter to a function
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> struct Param
{
    typedef typename If<IsBestByCopy<T>::Result, T, const T&>::Result Type;
};

template<class T> struct Param<T&>
{
    typedef T& Type;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// This metafunction is a convenient way to leverage SFINAE to conditionally remove functions
/// from overload resolution based on type traits and to provide separate function overloads and
/// specializations for different type traits. 
////////////////////////////////////////////////////////////////////////////////////////////////////
template<bool B, class T = void> struct EnableIf {};

template<class T> struct EnableIf<true, T>
{
    typedef T Type;
};

template<bool B, class T = void> using EnableIfT = typename EnableIf<B,T>::Type;

}

#endif
