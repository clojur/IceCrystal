////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
// [CR #488]
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_TYPEPROPERTYIMPL_H__
#define __CORE_TYPEPROPERTYIMPL_H__


#include <NsCore/Noesis.h>
#include <NsCore/TypeProperty.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// TypePropertyOffset. Represents a property defined by an offset from the instance pointer.
////////////////////////////////////////////////////////////////////////////////////////////////////
//@{
template<class T>
class TypePropertyOffset: public TypeProperty
{
public:
    /// Constructor
    TypePropertyOffset(NsSymbol name, uint32_t offset);

    /// From TypeProperty
    //@{
    void* GetContent(void* ptr) const;
    const void* GetContent(const void* ptr) const;
    Ptr<BaseComponent> GetComponent(const void* ptr) const;
    void SetComponent(void* ptr, BaseComponent* value) const;
    //@}

protected:
    /// From TypeProperty
    //@{
    const void* InternalGet(const void* ptr) const;
    void InternalGet(const void* ptr, void* storage) const;
    void InternalSet(void* ptr, const void* value) const;
    bool InternalIsReadOnly() const;
    //@}

private:
    typedef typename IsConst<T>::NonConstType TT;
    uint32_t mOffset;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// TypePropertyArray. Represents a static array property in a class.
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T, uint32_t N>
class TypePropertyArray: public TypeProperty
{
public:
    /// Constructor
    TypePropertyArray(NsSymbol name, uint32_t offset);

    /// From TypeProperty
    //@{
    void* GetContent(void* ptr) const;
    const void* GetContent(const void* ptr) const;
    Ptr<BaseComponent> GetComponent(const void* ptr) const;
    void SetComponent(void* ptr, BaseComponent* value) const;
    //@}

protected:
    /// From TypeProperty
    //@{
    const void* InternalGet(const void* ptr) const;
    void InternalGet(const void* ptr, void* storage) const;
    void InternalSet(void* ptr, const void* value) const;
    bool InternalIsReadOnly() const;
    //@}

private:
    uint32_t mOffset;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// TypePropertyFunction. Represents a property defined by Getter and Setter functions of a class.
////////////////////////////////////////////////////////////////////////////////////////////////////
//@{
template<class C, class T>
class TypePropertyFunction: public TypeProperty
{
private:
    struct ByRef
    {
        typedef const T& (C::*GetterFn)() const;
        typedef void (C::*SetterFn)(const T&);

        static bool CanGetRef();
        static bool CanGetCopy();

        static const void* GetRef(GetterFn getter, const void* ptr);
        static void GetCopy(GetterFn getter, const void* ptr, void* storage);
    };

    struct ByCopy
    {
        typedef T (C::*GetterFn)() const;
        typedef void (C::*SetterFn)(T);

        static bool CanGetRef();
        static bool CanGetCopy();

        static const void* GetRef(GetterFn getter, const void* ptr);
        static void GetCopy(GetterFn getter, const void* ptr, void* storage);
    };

public:
    typedef typename If<IsBestByCopy<T>::Result, ByCopy, ByRef>::Result Helper;
    typedef typename Helper::GetterFn GetterFn;
    typedef typename Helper::SetterFn SetterFn;

    /// Constructor
    TypePropertyFunction(NsSymbol name, GetterFn getter, SetterFn setter);

    /// From TypeProperty
    //@{
    void* GetContent(void* ptr) const;
    const void* GetContent(const void* ptr) const;
    Ptr<BaseComponent> GetComponent(const void* ptr) const;
    void SetComponent(void* ptr, BaseComponent* value) const;
    //@}

protected:
    /// From TypeProperty
    //@{
    const void* InternalGet(const void* ptr) const;
    void InternalGet(const void* ptr, void* storage) const;
    void InternalSet(void* ptr, const void* value) const;
    bool InternalIsReadOnly() const;
    //@}

private:
    GetterFn mGetter;
    SetterFn mSetter;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// A property holding a member delegate
////////////////////////////////////////////////////////////////////////////////////////////////////
class TypePropertyOffsetEvent: public TypeProperty
{
public:
    /// Constructor
    inline TypePropertyOffsetEvent(NsSymbol name, uint32_t offset);

    /// From TypeProperty
    //@{
    inline void* GetContent(void* ptr) const;
    inline const void* GetContent(const void* ptr) const;
    inline Ptr<BaseComponent> GetComponent(const void* ptr) const;
    inline void SetComponent(void* ptr, BaseComponent* value) const;
    //@}

protected:
    /// From TypeProperty
    //@{
    inline const void* InternalGet(const void* ptr) const;
    inline void InternalGet(const void* ptr, void* storage) const;
    inline void InternalSet(void* ptr, const void* value) const;
    inline bool InternalIsReadOnly() const;
    //@}

private:
    uint32_t mOffset;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// A property holding a getter delegate
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class C>
class TypePropertyFunctionEvent: public TypeProperty
{
public:
    /// Constructor
    typedef void* (C::*GetterFn)();
    inline TypePropertyFunctionEvent(NsSymbol name, GetterFn getter);

    /// From TypeProperty
    //@{
    inline void* GetContent(void* ptr) const;
    inline const void* GetContent(const void* ptr) const;
    inline Ptr<BaseComponent> GetComponent(const void* ptr) const;
    inline void SetComponent(void* ptr, BaseComponent* value) const;
    //@}

protected:
    /// From TypeProperty
    //@{
    inline const void* InternalGet(const void* ptr) const;
    inline void InternalGet(const void* ptr, void* storage) const;
    inline void InternalSet(void* ptr, const void* value) const;
    inline bool InternalIsReadOnly() const;
    //@}

private:
    GetterFn mGetter;
};

}

// Inline Include
#include <NsCore/TypePropertyImpl.inl>

#endif
