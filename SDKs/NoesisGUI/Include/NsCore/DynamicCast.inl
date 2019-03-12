////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
// [CR #620]
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsCore/CompilerTools.h>
#include <NsCore/TypeClass.h>
#include <NsCore/Ptr.h>


namespace Noesis
{

class BaseObject;
NS_INTERFACE Interface;

namespace Reflection
{

struct StaticCast
{
    template<class CastT, class SourceT>
    static CastT* Apply(SourceT* ptr)
    {
        return static_cast<CastT*>(ptr);
    }
};

typedef Int2Type<0> T_NoInterface;
typedef Int2Type<1> T_Interface;

template<class SourceT> BaseObject* GetBaseObject(SourceT* ptr, T_Interface)
{
    return (BaseObject*)(ptr == 0 ? 0 : ptr->GetBaseObject()); 
}

template<class SourceT> BaseObject* GetBaseObject(SourceT* ptr, T_NoInterface)
{
    return (BaseObject*)ptr;
}

NS_CORE_KERNEL_API void* Cast(const TypeClass* destType, BaseObject* source);

struct DynamicCast
{
    template<class CastT, class SourceT>
    static CastT* Apply(SourceT* ptr)
    {
        typedef typename IsConst<CastT>::NonConstType CastT_;
        const TypeClass* castType = CastT::StaticGetClassType((T2T<CastT_>*)0);
        BaseObject* source = GetBaseObject(ptr, Int2Type<IsInterface<SourceT>::Result>());
        return reinterpret_cast<CastT*>(Cast(castType, source));
    }
};

/// Up-casting to a derived class is optimized with a static cast
template<class CastT, class SourceT>
CastT* InternalDynamicCast(SourceT* ptr, T_NoInterface)
{
    return If<IsDerived<SourceT, CastT>::Result,
        StaticCast,
        DynamicCast>::Result::template Apply<CastT, SourceT>(ptr);
}

/// When casting to Interface dynamic cast must be done always, because static cast can generate an
/// ambiguity if class implements two or more interfaces
template<class CastT, class SourceT>
CastT* InternalDynamicCast(SourceT* ptr, T_Interface)
{
    return DynamicCast::Apply<CastT, SourceT>(ptr);
}

}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class CastT, class SourceT>
CastT NsDynamicCast(SourceT ptr)
{
    typedef typename Noesis::IsPointer<CastT>::PointedType CastTT;
    typedef typename Noesis::IsPointer<SourceT>::PointedType SourceTT;

    static_assert(
        !(Noesis::IsConst<SourceTT>::Result && !Noesis::IsConst<CastTT>::Result),
        "Cannot convert from const to non-const pointer");

    enum
    {
        CastToInterface =
            Noesis::IsSame<CastTT, Noesis::Interface>::Result ||
            Noesis::IsSame<CastTT, const Noesis::Interface>::Result
    };

    return Noesis::Reflection::InternalDynamicCast<CastTT, SourceTT>(ptr,
        Noesis::Int2Type<CastToInterface>());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class CastT, class SourceT>
CastT NsStaticCast(SourceT ptr)
{
    NS_ASSERT(!ptr || NsDynamicCast<CastT>(ptr) != 0);
    return static_cast<CastT>(ptr);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class CastT, class SourceT>
CastT NsDynamicCast(const Noesis::Ptr<SourceT>& ptr)
{
    static_assert(Noesis::IsPtr<CastT>::Result, "Ptr can only be casted to Ptr");
    return CastT(NsDynamicCast<typename CastT::Type*>(ptr.GetPtr()));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class CastT, class SourceT>
CastT NsStaticCast(const Noesis::Ptr<SourceT>& ptr)
{
    static_assert(Noesis::IsPtr<CastT>::Result, "Ptr can only be casted to Ptr");
    return CastT(NsStaticCast<typename CastT::Type*>(ptr.GetPtr()));
}
