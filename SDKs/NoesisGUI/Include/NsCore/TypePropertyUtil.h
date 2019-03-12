////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_TYPEPROPERTYUTIL_H__
#define __CORE_TYPEPROPERTYUTIL_H__


#include <NsCore/Noesis.h>
#include <NsCore/CompilerTools.h>
#include <NsCore/String.h>
#include <NsCore/Boxing.h>


namespace Noesis
{
namespace Reflection
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Auxiliary function to get a typed pointer using a base pointer and an offset
////////////////////////////////////////////////////////////////////////////////////////////////////
//@{
template<class T>
inline T* PtrOffset(void* ptr, uint32_t offset)
{
    return reinterpret_cast<T*>(static_cast<uint8_t*>(ptr) + offset);
}

template<class T>
inline const T* PtrOffset(const void* ptr, uint32_t offset)
{
    return reinterpret_cast<const T*>(static_cast<const uint8_t*>(ptr) + offset);
}
//@}

typedef Int2Type<0> NonRefCountedType;
typedef Int2Type<1> PtrRefCountedType;
typedef Int2Type<2> PointerRefCountedType;

typedef Int2Type<3> NonStringType;
typedef Int2Type<4> StringType;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Auxiliary functions to get values from a property using boxed values or Ptr objects
////////////////////////////////////////////////////////////////////////////////////////////////////
//@{
template<class T>
Ptr<BaseComponent> GetComponent(const TypeProperty* prop, const void* ptr, const NonStringType&)
{
    return Boxing::Box<T>(prop->Get<T>(ptr));
}

template<class T>
Ptr<BaseComponent> GetComponent(const TypeProperty* prop, const void* ptr, const StringType&)
{
    return Boxing::Box(prop->Get<T>(ptr));
}

template<class T>
Ptr<BaseComponent> GetComponent(const TypeProperty* prop, const void* ptr, NonRefCountedType)
{
    enum { IsStringType = (int)IsSame<T, const char*>::Result + 3 };
    return GetComponent<T>(prop, ptr, Int2Type<IsStringType>());
}

template<class T>
Ptr<BaseComponent> GetComponent(const TypeProperty* prop, const void* ptr, PtrRefCountedType)
{
    return NsDynamicCast<Ptr<BaseComponent> >(prop->Get<T>(ptr));
}

template<class T>
Ptr<BaseComponent> GetComponent(const TypeProperty* prop, const void* ptr, PointerRefCountedType)
{
    return Ptr<BaseComponent>(const_cast<BaseComponent*>(
        NsDynamicCast<const BaseComponent*>(prop->Get<T>(ptr))));
}

template<class T>
Ptr<BaseComponent> GetComponent(const TypeProperty* prop, const void* ptr)
{
    typedef typename IsPointer<T>::PointedType PointedType;
    enum { IsRefCountedType = IsPtr<T>::Result ? 1 : IsRefCounted<PointedType>::Result ? 2 : 0 };
    return GetComponent<T>(prop, ptr, Int2Type<IsRefCountedType>());
}
//@}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Auxiliary functions to set values in a property using boxed values or Ptr objects
////////////////////////////////////////////////////////////////////////////////////////////////////
//@{
template<class T>
void SetComponent(const TypeProperty* prop, void* ptr, BaseComponent* value, const NonStringType&)
{
    prop->Set<T>(ptr, Boxing::Unbox<T>(value));
}

template<class T>
void SetComponent(const TypeProperty* prop, void* ptr, BaseComponent* value, const StringType&)
{
    prop->Set<T>(ptr, Boxing::Unbox<NsString>(value).c_str());
}

template<class T>
void SetComponent(const TypeProperty* prop, void* ptr, BaseComponent* value, NonRefCountedType)
{
    enum { IsStringType = (int)IsSame<T, const char*>::Result + 3 };
    SetComponent<T>(prop, ptr, value, Int2Type<IsStringType>());
}

template<class T>
void SetComponent(const TypeProperty* prop, void* ptr, BaseComponent* value, PtrRefCountedType)
{
    prop->Set<T>(ptr, T(NsDynamicCast<typename T::Type*>(value)));
}

template<class T>
void SetComponent(const TypeProperty* prop, void* ptr, BaseComponent* value, PointerRefCountedType)
{
    prop->Set<T>(ptr, NsDynamicCast<T>(value));
}

template<class T>
void SetComponent(const TypeProperty* prop, void* ptr, BaseComponent* value)
{
    typedef typename IsPointer<T>::PointedType PointedType;
    enum { IsRefCountedType = IsPtr<T>::Result ? 1 : IsRefCounted<PointedType>::Result ? 2 : 0 };
    SetComponent<T>(prop, ptr, value, Int2Type<IsRefCountedType>());
}
//@}

}
}

#endif
